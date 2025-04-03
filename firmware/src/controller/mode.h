#ifndef MODE_H
#define	MODE_H

typedef enum {
  MODE_RECEIVING = 1,
  MODE_CHARGING_FAULT = 2,
  MODE_SUPPLYING = 3,
  MODE_MONITORING = 4,
  MODE_BALANCING = 5,
  MODE_BATTERY_OVERTEMP = 6,
  MODE_BATTERY_UNDER_MIN = 7,
  MODE_DUAL = 8,
  MODE_MENU = 9,
} mode_e;

mode_e mode_get();
void mode_set(mode_e mode);

#endif	/* MODE_H */