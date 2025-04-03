#include "state_manager.h"
#include "battery.h"
#include "charging.h"
#include "gpio.h"
#include "mode.h"
#include "debug.h"
#include "supplying.h"
#include "encoder.h"
#include "menu.h"

void state_manager_init()
{
    mode_set(MODE_DUAL);
}

void state_manager_update_sensors()
{
    battery_update_state();
    charging_update_state();
}

void state_manager_update_mode()
{
    battery_state_t battery_state = battery_get_state();
    charging_state_t charging_state = charging_get_state();

    // Initialize next mode to current mode
    mode_e current_mode = mode_get();
    mode_e next_mode = current_mode;
    
    switch (current_mode) {
        case MODE_RECEIVING:
            if (!battery_in_charge_temp_range()) {
                next_mode = MODE_BATTERY_OVERTEMP;
                break;
            }
            if (battery_balancing_needed()) {
                next_mode = MODE_BALANCING;
                break;
            }
            if (!charging_current_within_limits()) {
                next_mode = MODE_CHARGING_FAULT;
                break;
            }
            if (battery_is_fully_charged(charging_state.battery_metrics)) {
                next_mode = MODE_MONITORING;
                break;
            }
            // If no crucial tasks need to be taken, listen for user input
            if (encoder_get_event() == ENCODER_EVENT_BUTTON_PRESS) {
              // Button press indicates the user would to be presented the menu
              next_mode = MODE_MENU;
              break;
            }
            break;
        case MODE_SUPPLYING:
            if (!battery_in_discharge_temp_range()) {
                next_mode = MODE_BATTERY_OVERTEMP;
                break;
            }
            if (battery_is_depleted(charging_state.battery_metrics)) {
                next_mode = MODE_BATTERY_UNDER_MIN;
                break;
            }
            if (battery_balancing_needed()) {
                next_mode = MODE_BALANCING;
                break;
            }
            // If no crucial tasks need to be taken, listen for user input
            if (encoder_get_event() == ENCODER_EVENT_BUTTON_PRESS) {
                // Button press indicates the user would to be presented the menu
                next_mode = MODE_MENU;
                break;
            }
            break;
        case MODE_MONITORING: {
            if (battery_balancing_needed()) {
                next_mode = MODE_BALANCING;
                break;
            }
            // If no crucial tasks need to be taken, listen for user input
            if (encoder_get_event() == ENCODER_EVENT_BUTTON_PRESS) {
                // Button press indicates the user would to be presented the menu
                next_mode = MODE_MENU;
                break;
            }
            break;
        }
        case MODE_BALANCING:
            if (!battery_in_discharge_temp_range()) {
                next_mode = MODE_BATTERY_OVERTEMP;
                break;
            }
            if (battery_is_depleted(charging_state.battery_metrics)) {
                next_mode = MODE_BATTERY_UNDER_MIN;
                break;
            }
            if (!battery_balancing_needed()) {
                next_mode = MODE_MONITORING;
                break;
            }
            break;
        // Very imporant to declare new scope here
        case MODE_MENU: {
            encoder_event_e event = encoder_get_event();
            // If button press, we want to move to the currently selected state
            if (event == ENCODER_EVENT_BUTTON_PRESS) {
                next_mode = menu_get_selected_state();
            } else {
                // If not a button press, update the menu state.
                menu_update_state(event);
                break;
            }
            break;
        }
        case MODE_CHARGING_FAULT:
        case MODE_BATTERY_OVERTEMP:
        case MODE_BATTERY_UNDER_MIN:
            /*
             * In the event of battery under voltage, require manual intervention.
             */
            if (encoder_get_event() == ENCODER_EVENT_BUTTON_PRESS) {
                // Button press indicates the user would to be presented the menu
                next_mode = MODE_MENU;
                break;
            }
            break;
        case MODE_DUAL:
            /*
             * Charging conditions
             */
            if (!battery_in_charge_temp_range()) {
                next_mode = MODE_BATTERY_OVERTEMP;
                break;
            }
            if (!charging_current_within_limits()) {
                next_mode = MODE_CHARGING_FAULT;
                break;
            }
            if (battery_is_fully_charged(charging_state.battery_metrics)) {
                next_mode = MODE_MONITORING;
                break;
            }

            /*
             * Discharging conditions
             */
            if (!battery_in_discharge_temp_range()) {
                next_mode = MODE_BATTERY_OVERTEMP;
                break;
            }
            if (battery_is_depleted(charging_state.battery_metrics)) {
                next_mode = MODE_BATTERY_UNDER_MIN;
                break;
            }

            /*
             * Common conditions
             */
            if (battery_balancing_needed()) {
                next_mode = MODE_BALANCING;
                break;
            }

            // If no crucial tasks need to be taken, listen for user input
            if (encoder_get_event() == ENCODER_EVENT_BUTTON_PRESS) {
              // Button press indicates the user would to be presented the menu
              next_mode = MODE_MENU;
              break;
            }
            break;
        default:
            next_mode = MODE_MONITORING;
            break;
    }

    mode_set(next_mode);   
}

void state_manager_apply_hardware_updates()
{   
    battery_state_t battery_state = battery_get_state();
    charging_state_t charging_state = charging_get_state();
    mode_e current_mode = mode_get();

    switch (current_mode) {
        case MODE_BALANCING: 
            // Don't charge while balancing
            charging_disable();
            supplying_disable();
            // Determine which cell to balance
            if (battery_state.upper_cell_voltage_v - battery_state.lower_cell_voltage_v > 0) {
                battery_set_upper_discharge(true);
                battery_set_lower_discharge(false);
            } else {
                battery_set_upper_discharge(false);
                battery_set_lower_discharge(true);
            }
            break;
        case MODE_RECEIVING:
            supplying_disable();
            charging_enable();
            battery_set_upper_discharge(false);
            battery_set_lower_discharge(false);
            
            charging_set_duty_cycle(charging_calculate_duty_cycle());
            break;
        case MODE_DUAL:
            supplying_enable();
            charging_enable();
            battery_set_upper_discharge(false);
            battery_set_lower_discharge(false);
            
            charging_set_duty_cycle(charging_calculate_duty_cycle());
            break;
        case MODE_SUPPLYING:
            charging_disable();
            supplying_enable();
            break;
        case MODE_CHARGING_FAULT:
        case MODE_MONITORING:
        case MODE_BATTERY_OVERTEMP:
        case MODE_MENU:
        case MODE_BATTERY_UNDER_MIN:
            charging_disable();
            supplying_disable();
            battery_set_upper_discharge(false);
            battery_set_lower_discharge(false);
            break;
        default:
            current_mode = MODE_MONITORING;
            break;
    }
    return;
}
