#include <U8g2lib.h>
#include "display.h"
#include "menu.h"
#include "debug.h"

#define TITLE_FONT u8g2_font_BBSesque_tr                
#define MEDIUM_FONT u8g2_font_smallsimple_tr              
#define SMALL_FONT u8g2_font_blipfest_07_tr

// Use page buffer mode to save RAM (1_HW_I2C instead of F_HW_I2C)
#ifdef USE_SSD1306
    U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);
#else
    U8G2_SH1106_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, OLED_RESET);
#endif

// Store logo in PROGMEM to save RAM
static const unsigned char PROGMEM logo_bits[] = {
    // Your original logo data
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc7, 0x01, 0x00, 0x00, 0x00, 0xe0, 0x00, 0x00, 
        0x00, 0x78, 0x00, 0xe0, 0xff, 0x01, 0x00, 0x00, 0xc7, 0x01, 0x0e, 0x38, 0x00, 0xe0, 0x00, 0x00, 
        0x00, 0x7c, 0x00, 0xe0, 0xff, 0x01, 0x00, 0x00, 0xc7, 0x01, 0x0e, 0x38, 0x00, 0xe0, 0x60, 0x00, 
        0x00, 0x7c, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0xc0, 0x01, 0x0e, 0x1c, 0x00, 0xe0, 0x60, 0x00, 
        0x00, 0x7e, 0x00, 0x00, 0x0e, 0x00, 0x80, 0x01, 0xc0, 0x01, 0x1c, 0x1c, 0x30, 0xe0, 0xf0, 0x00, 
        0x00, 0x7e, 0x00, 0x00, 0x0e, 0xf8, 0xf3, 0x0f, 0xc7, 0x01, 0x1c, 0x1c, 0xfc, 0xe1, 0xf8, 0x07, 
        0x00, 0x7f, 0x00, 0x00, 0x0e, 0xf8, 0xf3, 0x0f, 0xc7, 0x01, 0x1c, 0x0e, 0xfe, 0xe3, 0xf8, 0x07, 
        0x00, 0xff, 0x1f, 0x00, 0x0e, 0x78, 0x30, 0x0c, 0xc7, 0x01, 0x38, 0x0e, 0x87, 0xe3, 0xe0, 0x00, 
        0x80, 0xff, 0x1f, 0x00, 0x0e, 0x38, 0x00, 0x0e, 0xc7, 0x01, 0x38, 0x0e, 0x87, 0xe7, 0x60, 0x00, 
        0x80, 0xff, 0x0f, 0x00, 0x0e, 0x38, 0xc0, 0x0f, 0xc7, 0x01, 0x38, 0x07, 0x07, 0xe7, 0x60, 0x00, 
        0xc0, 0xff, 0x0f, 0x00, 0x0e, 0x38, 0xf0, 0x0f, 0xc7, 0x01, 0x70, 0x87, 0x03, 0xe7, 0x60, 0x00, 
        0xc0, 0xff, 0x07, 0x00, 0x0e, 0x38, 0x78, 0x0c, 0xc7, 0x01, 0x70, 0x07, 0x07, 0xe7, 0x60, 0x00, 
        0xe0, 0xff, 0x07, 0x00, 0x0e, 0x38, 0x38, 0x0c, 0xc7, 0x01, 0xf0, 0x03, 0x87, 0xe3, 0xe0, 0x00, 
        0xe0, 0xff, 0x03, 0x00, 0x0e, 0x38, 0x38, 0x1e, 0xc7, 0x01, 0xe0, 0x03, 0xcf, 0xe3, 0xe1, 0x00, 
        0x00, 0xf8, 0x03, 0x00, 0x0e, 0x38, 0xf8, 0x3f, 0xc7, 0x07, 0xe0, 0x03, 0xfe, 0xc1, 0xe3, 0x07, 
        0x00, 0xf0, 0x01, 0x00, 0x0e, 0x38, 0xf0, 0x39, 0x87, 0x07, 0xe0, 0x01, 0xfc, 0xc0, 0xc3, 0x07, 
        0x00, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

void display_draw_logo() {
    u8g2.firstPage();
    do {
        u8g2.drawXBMP(0, 12, 128, 40, logo_bits);
    } while (u8g2.nextPage());
}

void display_init() {
    Wire.begin(); // Ensure I2C is initialized
    if (!u8g2.begin()) {
        Serial.println("Display init failed!");
        return;
    }
    
    u8g2.setFont(u8g2_font_profont10_mf);  // Using ProFont which supports numbers well
    u8g2.setDisplayRotation(U8G2_R0);
    
    // Show init message
    u8g2.firstPage();
    do {
        u8g2.setCursor(0, 10);
        u8g2.print("Init OK");
    } while (u8g2.nextPage());
}

// Helper function to draw metrics using cursor positioning
void draw_metric(const char* label, float value, const char* unit, uint8_t x, uint8_t& y, uint8_t decimals = 2) {
    u8g2.setCursor(x, y);
    u8g2.print(label);
    u8g2.print(": ");
    u8g2.print(value, decimals);
    u8g2.print(unit);
    y += 8;
}

void display_update() {
    battery_state_t battery_state = battery_get_state();
    charging_state_t charging_state = charging_get_state();
    mode_e mode = mode_get();

    u8g2.firstPage();
    do {
        // Common display elements
        const uint8_t battery_width = 16;
        const uint8_t battery_height = 35;
        const uint8_t terminal_width = 6;
        const uint8_t terminal_height = 3;
        const uint8_t battery_spacing = 4;
        const uint8_t start_x = 0;
        const uint8_t start_y = 10;
        
        // Draw Battery 1
        u8g2.drawFrame(start_x, start_y + terminal_height, battery_width, battery_height);
        u8g2.drawBox(start_x + (battery_width - terminal_width)/2, start_y, terminal_width, terminal_height);
        
        // Battery 1 fill level
        float volt1 = battery_state.upper_cell_voltage_v;
        uint8_t fill_height1 = min((uint8_t)((volt1 * battery_height) / 4.2), battery_height);
        u8g2.drawBox(start_x + 2, start_y + terminal_height + battery_height - fill_height1, battery_width - 4, fill_height1);
        
        // Draw Battery 2
        uint8_t batt2_x = start_x + battery_width + battery_spacing;
        u8g2.drawFrame(batt2_x, start_y + terminal_height, battery_width, battery_height);
        u8g2.drawBox(batt2_x + (battery_width - terminal_width)/2, start_y, terminal_width, terminal_height);
        
        // Battery 2 fill level
        float volt2 = battery_state.lower_cell_voltage_v;
        uint8_t fill_height2 = min((uint8_t)((volt2 * battery_height) / 4.2), battery_height);
        u8g2.drawBox(batt2_x + 2, start_y + terminal_height + battery_height - fill_height2, battery_width - 4, fill_height2);
        
        // Voltage and temperature displays
        u8g2.setFont(SMALL_FONT);

        // Upper cell voltage
        u8g2.setCursor(start_x + (battery_width - 13)/2, start_y - 1);
        u8g2.print(volt1, 2);
        
        // Lower cell voltage
        u8g2.setCursor(batt2_x + (battery_width - 13)/2, start_y - 1);
        u8g2.print(volt2, 2);
        
        // Series temperature
        u8g2.setCursor(start_x + (battery_width - 16)/2, start_y + terminal_height + battery_height + 10);
        u8g2.print(battery_state.series_temperature_c, 1);
        u8g2.print("C");
        
        // Ground temperature
        u8g2.setCursor(batt2_x + (battery_width - 16)/2, start_y + terminal_height + battery_height + 10);
        u8g2.print(battery_state.ground_temperature_c, 1);
        u8g2.print("C");

        // Right-side content
        uint8_t text_start_x = start_x + 2 * battery_width + battery_spacing + 8;
        uint8_t y_pos = 10;
        
        float percentage_duty_cycle = charging_state.duty_cycle_uint8 * (100.0f/255.0f);

        switch (mode) {
            case MODE_RECEIVING:
                u8g2.setFont(TITLE_FONT);
                u8g2.setCursor(text_start_x, y_pos);
                u8g2.print("RECEIVING");
                u8g2.setFont(MEDIUM_FONT);
                y_pos += 8;

                draw_metric("CHARGE", battery_get_total_percentage(), "%", text_start_x, y_pos, 2);
                draw_metric("V-BAT-INA", charging_state.battery_metrics.ina.bus_voltage_v, "V", text_start_x, y_pos);
                draw_metric("I-BAT", charging_state.battery_metrics.ina.current_ma, "mA", text_start_x, y_pos);
                draw_metric("V-BUCK", charging_state.buck_voltage_v, "V", text_start_x, y_pos);
                draw_metric("Duty Cycle", percentage_duty_cycle, "%", text_start_x, y_pos, 0);
                draw_metric("Efficiency", charging_get_power_efficiency(), "%", text_start_x, y_pos, 0);
                
                u8g2.setCursor(text_start_x, y_pos);
                if (battery_state.lower_discharging) {
                    u8g2.print("DRAINING LOWER");
                } else if (battery_state.upper_discharging) {
                    u8g2.print("DRAINING UPPER");
                }
                y_pos += 8;
                break;

            case MODE_BALANCING:
                u8g2.setFont(TITLE_FONT);
                u8g2.setCursor(text_start_x, y_pos);
                u8g2.print("BALANCING");
                u8g2.setFont(MEDIUM_FONT);
                y_pos += 8;

                draw_metric("CHARGE", battery_get_total_percentage(), "%", text_start_x, y_pos, 2);
                draw_metric("CELL-DIFF", battery_get_voltage_difference(), "V", text_start_x, y_pos);
                draw_metric("Duty Cycle", percentage_duty_cycle, "%", text_start_x, y_pos, 0);
                
                u8g2.setCursor(text_start_x, y_pos);
                if (battery_state.lower_discharging) {
                    u8g2.print("DRAINING LOWER");
                } else if (battery_state.upper_discharging) {
                    u8g2.print("DRAINING UPPER");
                }
                y_pos += 8;
                break;

            case MODE_SUPPLYING:
                u8g2.setFont(TITLE_FONT);
                u8g2.setCursor(text_start_x, y_pos);
                u8g2.print("SUPPLYING");
                u8g2.setFont(MEDIUM_FONT);
                y_pos += 8;
                break;

            case MODE_MONITORING:
                u8g2.setFont(TITLE_FONT);
                u8g2.setCursor(text_start_x, y_pos);
                u8g2.print("MONITORING");
                u8g2.setFont(MEDIUM_FONT);
                y_pos += 8;

                draw_metric("CHARGE", battery_get_total_percentage(), "%", text_start_x, y_pos, 2);
                draw_metric("Duty Cycle", percentage_duty_cycle, "%", text_start_x, y_pos, 0);
                
                u8g2.setCursor(text_start_x, y_pos);
                if (battery_state.lower_discharging) {
                    u8g2.print("DRAINING LOWER");
                } else if (battery_state.upper_discharging) {
                    u8g2.print("DRAINING UPPER");
                }
                break;

            case MODE_CHARGING_FAULT:
                u8g2.setFont(TITLE_FONT);
                u8g2.setCursor(text_start_x, y_pos);
                u8g2.print("FAULT");
                u8g2.setFont(MEDIUM_FONT);
                y_pos += 8;
                break;

            case MODE_BATTERY_OVERTEMP:
                u8g2.setFont(TITLE_FONT);
                u8g2.setCursor(text_start_x, y_pos);
                u8g2.print("OVERTEMP");
                u8g2.setFont(MEDIUM_FONT);
                y_pos += 8;
                break;

            case MODE_BATTERY_UNDER_MIN:
                u8g2.setFont(TITLE_FONT);
                u8g2.setCursor(text_start_x, y_pos);
                u8g2.print("UNDER MIN");
                u8g2.setFont(MEDIUM_FONT);
                y_pos += 8;
                break;

            case MODE_MENU: {
                u8g2.clearBuffer();
                mode_e selected_mode = menu_get_selected_state();
                u8g2.setFont(TITLE_FONT);
                
                // Center menu items
                int textHeight = 12;
                int startX = (128 - 48) / 2;
                int startY = (64 - (textHeight * 2)) / 2;

                // Draw first menu option
                if (selected_mode == MODE_RECEIVING) {
                    u8g2.setCursor(startX - 12, startY);
                    u8g2.print("> ");
                }
                u8g2.setCursor(startX, startY);
                u8g2.print("RECEIVE");

                // Draw second menu option
                if (selected_mode == MODE_SUPPLYING) {
                    u8g2.setCursor(startX - 12, startY + textHeight + 8);
                    u8g2.print("> ");
                }
                u8g2.setCursor(startX, startY + textHeight + 8);
                u8g2.print("SUPPLY");
                break;
            }

            default:
                u8g2.setFont(TITLE_FONT);
                u8g2.setCursor(text_start_x, y_pos);
                u8g2.print("UNKNOWN MODE");
                u8g2.setFont(MEDIUM_FONT);
                y_pos += 8;
                break;
        }
    } while (u8g2.nextPage());
}