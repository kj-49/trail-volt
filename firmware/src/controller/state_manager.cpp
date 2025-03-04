
#include "state_manager.h"
#include "battery.h"
#include "charging.h"
#include "gpio.h"
#include "mode.h"

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

void state_manager_update_application_mode() {
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
            }
            break;
        case MODE_SUPPLYING:
            if (needs_balancing) {
                next_mode = MODE_BALANCING;
            }
            break;
        case MODE_MONITORING:
            if (needs_balancing) {
                next_mode = MODE_BALANCING;
            }
            break;
        case MODE_BALANCING:
            if (!needs_balancing) {
                next_mode = MODE_MONITORING;
            }
        default:
            current_mode = MODE_MONITORING;
    }

    set_mode(next_mode);
}

/**
 * @brief  Applies updates to the hardware based on the current system state.
 */
void state_manager_apply_hardware_updates() {
    battery_state_t battery_state = battery_get_state();
    charging_state_t charging_state = charging_get_state();
    mode_e current_mode = get_mode();

    /*
     * At this point in the code, we are not concerned about what
     * the state SHOULD be, we are only concerned about what the
     * state IS, and what needs to happen for the hardware to
     * reflect this current state.
     */

    switch (current_mode) {
        case MODE_RECEIVING:
            /*
             * Follow PWM charging procedures.
             */
            // Balanncing should be disabled in this mode
            battery_set_upper_discharge(false);
            battery_set_lower_discharge(false);
            break;
        case MODE_SUPPLYING:
            /*
             * Supply currect to device etc.
             */
            // Balanncing should be disabled in this mode
            battery_set_upper_discharge(false);
            battery_set_lower_discharge(false);
            break;
        case MODE_MONITORING:
            // Balanncing should be disabled in this mode
            battery_set_upper_discharge(false);
            battery_set_lower_discharge(false);
            break;
        case MODE_BALANCING:
            /*
             * Set discharge GPIOs according to battery state. If we are in this mode, balancing
             * is assumed to be needed, not our responsiblity to check again.
             */
            float upper_overshoot = battery_state.upper_cell_voltage_v - battery_state.lower_cell_voltage_v;

            if (upper_overshoot > 0) {
                battery_set_upper_discharge(true);
                battery_set_lower_discharge(false);
            } else {
                battery_set_upper_discharge(false);
                battery_set_lower_discharge(true);
            }
            break;

        default:
            current_mode = MODE_MONITORING;
    }
}

// void state_manager_update_sensor_readings(const Adafruit_INA260 *ina260) {
//     // Update battery status
//     update_battery_readings(&current_state.battery_state);
    
//     // Update power metrics
//     update_power_metrics(ina260, &current_state.charging_state.power_metrics);
// }

// system_command_t state_manager_process_state() {
//     system_command_t cmd = {0};
    
//     // Set the next mode based on current state
//     cmd.next_mode = current_state.mode;
    
//     // Determine if balancing is needed
//     cmd.battery_cmd.should_balance = is_balancing_needed(&current_state.battery_state);
    
//     // Logic to determine cell discharge commands
//     if (cmd.battery_cmd.should_balance) {
//         if (current_state.battery_state.upper_cell_voltage_v > 
//             current_state.battery_state.lower_cell_voltage_v) {
//             cmd.battery_cmd.should_discharge_upper = true;
//             cmd.battery_cmd.should_discharge_lower = false;
//         } else {
//             cmd.battery_cmd.should_discharge_upper = false;
//             cmd.battery_cmd.should_discharge_lower = true;
//         }
//     }
    
//     // Determine charging commands
//     if (current_state.mode == MODE_RECEIVING) {
//         cmd.charging_cmd.target_duty_cycle = calculate_duty_cycle(
//             &current_state.charging_state);
//     } else if (current_state.mode == MODE_BALANCING) {
//         cmd.charging_cmd.should_stop_charging = true;
//     }
    
//     // State transition logic
//     switch (current_state.mode) {
//         case MODE_MONITORING:
//             if (cmd.battery_cmd.should_balance) {
//                 cmd.next_mode = MODE_BALANCING;
//             } else if (is_receiving_charge()) {
//                 cmd.next_mode = MODE_RECEIVING;
//             }
//             break;
            
//         case MODE_RECEIVING:
//             if (cmd.battery_cmd.should_balance) {
//                 cmd.next_mode = MODE_BALANCING;
//             } else if (!is_receiving_charge()) {
//                 cmd.next_mode = MODE_MONITORING;
//             }
//             break;
            
//         case MODE_BALANCING:
//             if (!cmd.battery_cmd.should_balance) {
//                 cmd.next_mode = MODE_MONITORING;
//             }
//             break;
//         default:
//             cmd.next_mode = MODE_MONITORING;
//             break;
//     }
    
//     return cmd;
// }

// void state_manager_apply_command(const system_command_t* cmd) {

//     /**
//      * Here we execute the commands that our application has determined necessary.
//      *
//      * We map the the should_[command] to their physical meanings.
//      * 
//      * Ex. The should_balance command maps to the toggling of our cell balancing GPIO pins)
//      */

//     // Apply mode change
//     current_state.mode = cmd->next_mode;

//     // Apply battery commands
//     if (cmd->battery_cmd.should_balance) {
//         // Update the flags in our system state
//         current_state.battery_state.upper_discharging = 
//             cmd->battery_cmd.should_discharge_upper;
//         current_state.battery_state.lower_discharging = 
//             cmd->battery_cmd.should_discharge_lower;
        
//         // Set GPIO pins according to this new state
//         set_upper_discharge(cmd->battery_cmd.should_discharge_upper);
//         set_lower_discharge(cmd->battery_cmd.should_discharge_lower);
//     } else {
//         // No balancing needed, ensure both GPIOs set low
//         current_state.battery_state.upper_discharging = false;
//         current_state.battery_state.lower_discharging = false;
//         set_upper_discharge(false);
//         set_lower_discharge(false);
//     }
    
//     // Apply charging commands
//     if (cmd->charging_cmd.should_stop_charging) {
//         stop_charging();
//         current_state.charging_state.duty_cycle_uint8 = 0;
//     } else {
//         current_state.charging_state.duty_cycle_uint8 = 
//             cmd->charging_cmd.target_duty_cycle;
//         set_charging_duty_cycle(cmd->charging_cmd.target_duty_cycle);
//     }
// }
