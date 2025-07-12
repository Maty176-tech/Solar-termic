#include "stats.h"
#include <EEPROM.h>


#define DAILY_OFFSET   0      // 0–3
#define MONTHLY_OFFSET 4      // 4–7
#define YEARLY_OFFSET  8      // 8–11

#define EEPROM_SIZE 512
#define DAILY_OFFSET 100  // kam ukládáme výkon

void initStats() {
  EEPROM.begin(EEPROM_SIZE);  // musí být uvnitř funkce!
  // Lze rozšířit o načtení do proměnných
}

void logPower(float kW) {
  float daily = 0, monthly = 0, yearly = 0;

  EEPROM.get(DAILY_OFFSET, daily);
  EEPROM.get(MONTHLY_OFFSET, monthly);
  EEPROM.get(YEARLY_OFFSET, yearly);

  daily += kW * (5.0 / 60.0);     // každých 5 minut
  monthly += kW * (5.0 / 60.0);
  yearly += kW * (5.0 / 60.0);

  EEPROM.put(DAILY_OFFSET, daily);
  EEPROM.put(MONTHLY_OFFSET, monthly);
  EEPROM.put(YEARLY_OFFSET, yearly);
  EEPROM.commit();
}

float getDailyTotal() {
  float val = 0;
  EEPROM.get(DAILY_OFFSET, val);
  return val;
}

float getMonthlyTotal() {
  float val = 0;
  EEPROM.get(MONTHLY_OFFSET, val);
  return val;
}

float getYearlyTotal() {
  float val = 0;
  EEPROM.get(YEARLY_OFFSET, val);
  return val;
}
