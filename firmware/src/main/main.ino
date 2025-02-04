#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include "constants.h"
#include "lcd.h"

void setup() {
  configure_lcd();
}

void loop() {
  
  redraw_lcd();

  delay(500);

}
