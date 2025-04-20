<img src="https://github.com/kj-49/trail-volt/blob/readme/assets/logo.png?raw=true" alt="menu" height="125"/>
**Trail Volt** is a portable, rechargeable lithium-ion battery pack developed for a third-year Electrical Engineering project course. It features dual charging capabilities—solar and hand-crank—and can power two mobile devices consecutively through USB-C ports.
It includes an intelligent battery management system with active cell balancing, temperature monitoring, and current sensing for safe and efficient operation. A rotary encoder and OLED screen provide an intuitive user interface, while onboard firmware manages charging, discharging, and fault protection modes.

### User Interface
<div style="display: flex; gap: 10px; align-items: flex-start;">
  <img src="https://github.com/kj-49/trail-volt/blob/main/assets/ui/menu.jpg?raw=true" alt="menu" height="125"/>
  <img src="https://github.com/kj-49/trail-volt/blob/main/assets/ui/dual.jpg?raw=true" alt="dual" height="125"/>
  <img src="https://github.com/kj-49/trail-volt/blob/main/assets/ui/balancing.jpg?raw=true" alt="balancing" height="125"/>
</div>

### Pinout Diagram

| Pin | Function      | Description                        |
|-----|---------------|------------------------------------|
| A0  | ADC0          | Thermistor 1                       |
| A1  | ADC1          | Thermistor 1                       |
| A2  | ADC2          | Total Cell Voltage                 |
| A3  | ADC3          | Lower Cell Voltage                 |
| A4  | SDA           | I2C SDA                            |
| A5  | SCL           | I2C SCL                            |
| A6  | -             | -                                  |
| A7  | -             | -                                  |
| D0  | -             | -                                  |
| D1  | DIGITAL IN    | Rotary Encoder Data Terminal       |
| D2  | DIGITAL IN    | Rotary Encoder Button Terminal     |
| D3  | DIGITAL IN    | Rotary Encoder Clock Terminal      |
| D4  | -             | -                                  |
| D5  | DIGITAL OUT   | Gate Driver Shutdown Signal        |
| D6  | DIGITAL OUT   | USB Enabled                        |
| D7  | DIGITAL OUT   | C1 Cell Balancing BJT Voltage      |
| D8  | DIGITAL OUT   | C2 Cell Balancing BJT Voltage      |
| D9  | ANALOG OUT    | Gate Driver PWM signal             |
| D10 | -             | -                                  |
| D11 | -             | -                                  |
| D12 | -             | -                                  |
| D13 | -             | -                                  |
