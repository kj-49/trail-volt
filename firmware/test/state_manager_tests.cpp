#include "unity/unity.h"
#include "fff.h"
#include "../src/controller/battery.h"
#include "../src/controller/hal.h"
#include "../src/controller/state_manager.h"
#include "../src/controller/encoder.h"
#include "../src/controller/supplying.h"
#include "../src/controller/charging.h"

// Fakes for dependencies
DEFINE_FFF_GLOBALS;
FAKE_VALUE_FUNC(mode_e, mode_get);
FAKE_VOID_FUNC(mode_set, mode_e);
FAKE_VALUE_FUNC(battery_state_t, battery_get_state);
FAKE_VALUE_FUNC(charging_state_t, charging_get_state);
FAKE_VOID_FUNC(battery_update_state);
FAKE_VOID_FUNC(charging_update_state);
FAKE_VALUE_FUNC(bool, battery_in_charge_temp_range);
FAKE_VALUE_FUNC(bool, battery_in_discharge_temp_range);
FAKE_VALUE_FUNC(bool, battery_balancing_needed);
FAKE_VALUE_FUNC(bool, battery_balancing_done);
FAKE_VALUE_FUNC(bool, charging_current_within_limits);
FAKE_VALUE_FUNC(bool, battery_is_fully_charged, battery_metrics_t);
FAKE_VALUE_FUNC(bool, battery_is_depleted, battery_metrics_t);
FAKE_VOID_FUNC(charging_disable);
FAKE_VOID_FUNC(charging_enable);
FAKE_VOID_FUNC(supplying_disable);
FAKE_VOID_FUNC(supplying_enable);
FAKE_VOID_FUNC(battery_set_upper_discharge, bool);
FAKE_VOID_FUNC(battery_set_lower_discharge, bool);
FAKE_VALUE_FUNC(uint8_t, charging_calculate_duty_cycle);
FAKE_VOID_FUNC(charging_set_duty_cycle, uint8_t);
FAKE_VALUE_FUNC(encoder_event_e, encoder_get_event);
FAKE_VALUE_FUNC(mode_e, menu_get_selected_state);
FAKE_VOID_FUNC(menu_update_state, encoder_event_e);


void setUp(void) {
    RESET_FAKE(mode_get);
    RESET_FAKE(mode_set);
    RESET_FAKE(battery_get_state);
    RESET_FAKE(charging_get_state);
    RESET_FAKE(battery_update_state);
    RESET_FAKE(charging_update_state);
    RESET_FAKE(battery_in_charge_temp_range);
    RESET_FAKE(battery_in_discharge_temp_range);
    RESET_FAKE(battery_balancing_needed);
    RESET_FAKE(battery_balancing_done);
    RESET_FAKE(charging_current_within_limits);
    RESET_FAKE(battery_is_fully_charged);
    RESET_FAKE(battery_is_depleted);
    RESET_FAKE(charging_disable);
    RESET_FAKE(charging_enable);
    RESET_FAKE(supplying_disable);
    RESET_FAKE(supplying_enable);
    RESET_FAKE(battery_set_upper_discharge);
    RESET_FAKE(battery_set_lower_discharge);
    RESET_FAKE(charging_calculate_duty_cycle);
    RESET_FAKE(charging_set_duty_cycle);
    RESET_FAKE(encoder_get_event);
    RESET_FAKE(menu_get_selected_state);
    RESET_FAKE(menu_update_state);
    
    // Default
    battery_in_charge_temp_range_fake.return_val = true;
    battery_in_discharge_temp_range_fake.return_val = true;
    battery_balancing_needed_fake.return_val = false;
    battery_balancing_done_fake.return_val = true;
    charging_current_within_limits_fake.return_val = true;
    battery_is_fully_charged_fake.return_val = false;
    battery_is_depleted_fake.return_val = false;
    encoder_get_event_fake.return_val = ENCODER_EVENT_NONE;
}

void tearDown(void) {
}

void test_receiving_to_balancing_when_needed(void) {
    mode_get_fake.return_val = MODE_RECEIVING;
    battery_balancing_needed_fake.return_val = true;
    
    state_manager_update_mode();
    
    TEST_ASSERT_EQUAL(MODE_BALANCING, mode_set_fake.arg0_val);
}

void test_receiving_to_overtemp_when_too_hot(void) {
    mode_get_fake.return_val = MODE_RECEIVING;
    battery_in_charge_temp_range_fake.return_val = false;
    
    state_manager_update_mode();
    
    TEST_ASSERT_EQUAL(MODE_BATTERY_OVERTEMP, mode_set_fake.arg0_val);
}

void test_receiving_to_charging_fault_when_current_out_of_range(void) {
    mode_get_fake.return_val = MODE_RECEIVING;
    charging_current_within_limits_fake.return_val = false;
    
    state_manager_update_mode();
    
    TEST_ASSERT_EQUAL(MODE_CHARGING_FAULT, mode_set_fake.arg0_val);
}

void test_receiving_to_monitoring_when_fully_charged(void) {
    mode_get_fake.return_val = MODE_RECEIVING;
    battery_is_fully_charged_fake.return_val = true;
    
    state_manager_update_mode();
    
    TEST_ASSERT_EQUAL(MODE_MONITORING, mode_set_fake.arg0_val);
}

void test_receiving_to_menu_on_button_press(void) {
    mode_get_fake.return_val = MODE_RECEIVING;
    encoder_get_event_fake.return_val = ENCODER_EVENT_BUTTON_PRESS;
    
    state_manager_update_mode();
    
    TEST_ASSERT_EQUAL(MODE_MENU, mode_set_fake.arg0_val);
}

void test_supplying_to_overtemp_when_too_hot(void) {
    mode_get_fake.return_val = MODE_SUPPLYING;
    battery_in_discharge_temp_range_fake.return_val = false;
    
    state_manager_update_mode();
    
    TEST_ASSERT_EQUAL(MODE_BATTERY_OVERTEMP, mode_set_fake.arg0_val);
}

void test_supplying_to_undervolt_when_depleted(void) {
    mode_get_fake.return_val = MODE_SUPPLYING;
    battery_is_depleted_fake.return_val = true;
    
    state_manager_update_mode();
    
    TEST_ASSERT_EQUAL(MODE_BATTERY_UNDER_MIN, mode_set_fake.arg0_val);
}

void test_supplying_to_balancing_when_needed(void) {
    mode_get_fake.return_val = MODE_SUPPLYING;
    battery_balancing_needed_fake.return_val = true;
    
    state_manager_update_mode();
    
    TEST_ASSERT_EQUAL(MODE_BALANCING, mode_set_fake.arg0_val);
}

void test_supplying_to_menu_on_button_press(void) {
    mode_get_fake.return_val = MODE_SUPPLYING;
    encoder_get_event_fake.return_val = ENCODER_EVENT_BUTTON_PRESS;
    
    state_manager_update_mode();
    
    TEST_ASSERT_EQUAL(MODE_MENU, mode_set_fake.arg0_val);
}

void test_monitoring_to_balancing_when_needed(void) {
    mode_get_fake.return_val = MODE_MONITORING;
    battery_balancing_needed_fake.return_val = true;
    
    state_manager_update_mode();
    
    TEST_ASSERT_EQUAL(MODE_BALANCING, mode_set_fake.arg0_val);
}

void test_monitoring_to_menu_on_button_press(void) {
    mode_get_fake.return_val = MODE_MONITORING;
    encoder_get_event_fake.return_val = ENCODER_EVENT_BUTTON_PRESS;
    
    state_manager_update_mode();
    
    TEST_ASSERT_EQUAL(MODE_MENU, mode_set_fake.arg0_val);
}

void test_balancing_to_balancing_when_not_done_balanced(void) {
    mode_get_fake.return_val = MODE_BALANCING;
    battery_balancing_done_fake.return_val = false;
    
    state_manager_update_mode();
    
    TEST_ASSERT_EQUAL(MODE_BALANCING, mode_set_fake.arg0_val);
}

void test_balancing_to_monitoring_when_balanced(void) {
    mode_get_fake.return_val = MODE_BALANCING;
    battery_balancing_done_fake.return_val = true;
    
    state_manager_update_mode();
    
    TEST_ASSERT_EQUAL(MODE_MONITORING, mode_set_fake.arg0_val);
}

void test_balancing_to_overtemp_when_too_hot(void) {
    mode_get_fake.return_val = MODE_BALANCING;
    battery_in_discharge_temp_range_fake.return_val = false;
    
    state_manager_update_mode();
    
    TEST_ASSERT_EQUAL(MODE_BATTERY_OVERTEMP, mode_set_fake.arg0_val);
}

void test_balancing_to_undervolt_when_depleted(void) {
    mode_get_fake.return_val = MODE_BALANCING;
    battery_is_depleted_fake.return_val = true;
    
    state_manager_update_mode();
    
    TEST_ASSERT_EQUAL(MODE_BATTERY_UNDER_MIN, mode_set_fake.arg0_val);
}

void test_menu_transition_to_selected_state(void) {
    mode_get_fake.return_val = MODE_MENU;
    encoder_get_event_fake.return_val = ENCODER_EVENT_BUTTON_PRESS;
    menu_get_selected_state_fake.return_val = MODE_SUPPLYING;
    
    state_manager_update_mode();
    
    TEST_ASSERT_EQUAL(MODE_SUPPLYING, mode_set_fake.arg0_val);
}

void test_dual_to_overtemp_when_charge_too_hot(void) {
    mode_get_fake.return_val = MODE_DUAL;
    battery_in_charge_temp_range_fake.return_val = false;
    
    state_manager_update_mode();
    
    TEST_ASSERT_EQUAL(MODE_BATTERY_OVERTEMP, mode_set_fake.arg0_val);
}

void test_dual_to_overtemp_when_discharge_too_hot(void) {
    mode_get_fake.return_val = MODE_DUAL;
    battery_in_discharge_temp_range_fake.return_val = false;
    
    state_manager_update_mode();
    
    TEST_ASSERT_EQUAL(MODE_BATTERY_OVERTEMP, mode_set_fake.arg0_val);
}

void test_dual_to_charging_fault_when_current_out_of_range(void) {
    mode_get_fake.return_val = MODE_DUAL;
    charging_current_within_limits_fake.return_val = false;
    
    state_manager_update_mode();
    
    TEST_ASSERT_EQUAL(MODE_CHARGING_FAULT, mode_set_fake.arg0_val);
}

void test_dual_to_monitoring_when_fully_charged(void) {
    mode_get_fake.return_val = MODE_DUAL;
    battery_is_fully_charged_fake.return_val = true;
    
    state_manager_update_mode();
    
    TEST_ASSERT_EQUAL(MODE_MONITORING, mode_set_fake.arg0_val);
}

void test_dual_to_undervolt_when_depleted(void) {
    mode_get_fake.return_val = MODE_DUAL;
    battery_is_depleted_fake.return_val = true;
    
    state_manager_update_mode();
    
    TEST_ASSERT_EQUAL(MODE_BATTERY_UNDER_MIN, mode_set_fake.arg0_val);
}

void test_dual_to_balancing_when_needed(void) {
    mode_get_fake.return_val = MODE_DUAL;
    battery_balancing_needed_fake.return_val = true;
    
    state_manager_update_mode();
    
    TEST_ASSERT_EQUAL(MODE_BALANCING, mode_set_fake.arg0_val);
}

void test_dual_to_menu_on_button_press(void) {
    mode_get_fake.return_val = MODE_DUAL;
    encoder_get_event_fake.return_val = ENCODER_EVENT_BUTTON_PRESS;
    
    state_manager_update_mode();
    
    TEST_ASSERT_EQUAL(MODE_MENU, mode_set_fake.arg0_val);
}

void test_default_case_handling(void) {
    mode_get_fake.return_val = (mode_e)99; // Invalid mode
    
    state_manager_update_mode();
    
    TEST_ASSERT_EQUAL(MODE_MONITORING, mode_set_fake.arg0_val);
}

int main(void) {
    UNITY_BEGIN();
    
    RUN_TEST(test_receiving_to_balancing_when_needed);
    RUN_TEST(test_receiving_to_overtemp_when_too_hot);
    RUN_TEST(test_receiving_to_charging_fault_when_current_out_of_range);
    RUN_TEST(test_receiving_to_monitoring_when_fully_charged);
    RUN_TEST(test_receiving_to_menu_on_button_press);
    
    RUN_TEST(test_supplying_to_overtemp_when_too_hot);
    RUN_TEST(test_supplying_to_undervolt_when_depleted);
    RUN_TEST(test_supplying_to_balancing_when_needed);
    RUN_TEST(test_supplying_to_menu_on_button_press);
    
    RUN_TEST(test_monitoring_to_balancing_when_needed);
    RUN_TEST(test_monitoring_to_menu_on_button_press);
    
    RUN_TEST(test_balancing_to_balancing_when_not_done_balanced);
    RUN_TEST(test_balancing_to_monitoring_when_balanced);
    RUN_TEST(test_balancing_to_overtemp_when_too_hot);
    RUN_TEST(test_balancing_to_undervolt_when_depleted);
    
    RUN_TEST(test_menu_transition_to_selected_state);
    
    RUN_TEST(test_dual_to_overtemp_when_charge_too_hot);
    RUN_TEST(test_dual_to_overtemp_when_discharge_too_hot);
    RUN_TEST(test_dual_to_charging_fault_when_current_out_of_range);
    RUN_TEST(test_dual_to_monitoring_when_fully_charged);
    RUN_TEST(test_dual_to_undervolt_when_depleted);
    RUN_TEST(test_dual_to_balancing_when_needed);
    RUN_TEST(test_dual_to_menu_on_button_press);

    RUN_TEST(test_default_case_handling);
    
    return UNITY_END();
}