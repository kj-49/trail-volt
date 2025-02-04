#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#include "lcd.h"

#define i2c_Address 0x3c

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1   //   QT-PY / XIAO
Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void drawBatteryIndicator(int x, int y, int cellCharge[]) {
  int cellWidth = 15;
  int cellHeight = 20;
  int spacing = 3;

  for (int i = 0; i < 4; i++) {
    int cellX = x + i * (cellWidth + spacing);
    
    // Draw empty battery cell
    display.drawRect(cellX, y, cellWidth, cellHeight, SH110X_WHITE);

    // Determine fill height based on charge percentage
    int fillHeight = (cellCharge[i] * (cellHeight - 4)) / 100;
    
    // Draw filled portion
    display.fillRect(cellX + 2, y + (cellHeight - 2 - fillHeight), cellWidth - 4, fillHeight, SH110X_WHITE);
  }
}

void configure_lcd() {
  Serial.begin(9600);

  display.begin(i2c_Address, true);

  display.clearDisplay();
}

void redraw_lcd() {


  // Display temperature
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.print("Temperature: "); 
  display.print("27.2"); 
  display.println("C");
  display.println("");

  // Battery cell charge levels (test values)
  int cellCharge[4] = {90, 75, 50, 90}; // Percentages for each cell

  // Display capacity
  display.print("Capacity: ");
  int avgCharge = (cellCharge[0] + cellCharge[1] + cellCharge[2] + cellCharge[3]) / 4;
  display.print(avgCharge);
  display.println("%");

  // Draw battery indicator
  drawBatteryIndicator(0, 30, cellCharge);

  display.display();

  delay(500);
}





