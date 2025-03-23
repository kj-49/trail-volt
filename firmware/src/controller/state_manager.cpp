#include "state_manager.h"
#include "battery.h"
#include "charging.h"
#include "gpio.h"
#include "mode.h"
#include "debug.h"
#include "supplying.h"
#include "encoder.h"
#include "menu.h"

void state_manager_init() {
    set_mode(MODE_MONITORING);
}

/**
 * @brief  Reads all sensor data and updates the system state.
 */
void state_manager_update_sensors() {
    battery_update_state();
    charging_update_state();
}

void state_manager_update_mode() {
    /* Modify application mode based on new state */
    battery_state_t battery_state = battery_get_state();
    charging_state_t charging_state = charging_get_state();

    bool needs_balancing = battery_balancing_needed();
    
    // Initialize next mode to current mode
    mode_e current_mode = get_mode();
    mode_e next_mode = current_mode;

    switch (current_mode) {
        case MODE_RECEIVING:
            if (needs_balancing) {
                next_mode = MODE_BALANCING;
                break;
            }
            if (charging_state.is_over_current) {
                next_mode = MODE_CHARGING_FAULT;
                break;
            }
            // If no crucial tasks need to be taken, listen for user input
            if (encoder_get_event() == ENCODER_EVENT_BUTTON_PRESS) {
              // Button press indicates the user would to be presented the menu
              next_mode = MODE_MENU;
            }
            break;
        case MODE_CHARGING_FAULT:
            if (!charging_state.is_over_current) {
                next_mode = MODE_MONITORING;
            }
            break;
        case MODE_SUPPLYING:
            if (needs_balancing) {
                next_mode = MODE_BALANCING;
                break;
            }
            // If no crucial tasks need to be taken, listen for user input
            if (encoder_get_event() == ENCODER_EVENT_BUTTON_PRESS) {
              // Button press indicates the user would to be presented the menu
              next_mode = MODE_MENU;
            }
            break;
        case MODE_MONITORING: {
            if (needs_balancing) {
                next_mode = MODE_BALANCING;
                break;
            }
            // If no crucial tasks need to be taken, listen for user input
            if (encoder_get_event() == ENCODER_EVENT_BUTTON_PRESS) {
              // Button press indicates the user would to be presented the menu
              next_mode = MODE_MENU;
            }
            break;
        }
        case MODE_BALANCING:
            if (!needs_balancing) {
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
            }
            break;
        }
        default:
            next_mode = MODE_MONITORING;
            break;
    }

    set_mode(next_mode);   
}

/**
 * @brief  Applies updates to the hardware based on the current system state.
 */
void state_manager_apply_hardware_updates() {
    /*
     * This function is not responsible for checking whether the current state
     * is accurate, but only for reflecting this current state through hardware updates.
     */
    
    battery_state_t battery_state = battery_get_state();
    charging_state_t charging_state = charging_get_state();
    mode_e current_mode = get_mode();

    switch (current_mode) {
        case MODE_BALANCING: 
            // Don't charge while balancing
            charging_stop();
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
        case MODE_CHARGING_FAULT:
            // Stop charging
            charging_stop(); 
            // Stop supplying (shouldn't be anyways)
            supplying_disable();
            // Stop discharging (shouldn't be anyways)
            battery_set_upper_discharge(false);
            battery_set_lower_discharge(false);
            break;
        case MODE_RECEIVING:
            supplying_disable();
            battery_set_upper_discharge(false);
            battery_set_lower_discharge(false);
            
            charging_set_duty_cycle(charging_calculate_duty_cycle());
            break;
        case MODE_SUPPLYING:
            charging_stop();
            supplying_enable();
            battery_set_upper_discharge(false);
            battery_set_lower_discharge(false);
            break;
        case MODE_MONITORING:
            charging_stop();
            supplying_disable();
            battery_set_upper_discharge(false);
            battery_set_lower_discharge(false);
            break;
        case MODE_MENU:
            charging_stop();
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
