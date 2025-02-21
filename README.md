### Pinout Diagram

| Pin | Function  | Description                        |
|-----|-----------|------------------------------------|
| D0  | ADC0      | Cell 1 Battery Voltage             |
| D1  | ADC1      | Cell 1 Battery Voltage             |
| D2  | ADC2      | Thermistor Voltage 1               |
| D3  | ADC3      | Thermistor Voltage 2               |
| D4  | ADC4      | Positive Charge Current Terminal   |
| D5  | ADC5      | Negative Charge Current Terminal   |
| B0  | INPUT     | Wake up button to exit sleep mode  |

### Current Measurements
```
       + V
        │
        │
 [D4]---│
        ├──[ Shunt Resistor ] (very small)
 [D5]---│
        |
        |
        ├──[ Load ]
        │
       GND
```
> Charge Current = (D4 - D5) / Shunt Resistor
