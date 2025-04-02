
#ifndef STATE_MANAGER_H
#define STATE_MANAGER_H

/**
 * @brief  Initialize the state manager.
 */
void state_manager_init();

/**
 * @brief  Reads all sensor data and updates the system state.
 */
void state_manager_update_sensors();

/**
 * @brief  Updates next FSM state based on sensor readings.
 */
void state_manager_update_mode();

/**
 * @brief  Applies updates to the hardware based on the current system state.
 * @note   This function is not responsible for checking whether the current state
 *         is accurate, but only for reflecting this current state through hardware updates.
 */
void state_manager_apply_hardware_updates();

#endif /* STATE_MANAGER_H */
