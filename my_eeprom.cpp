#include <Arduino.h>
#include <EEPROM.h>
#include "menu.h"  // <-- aby byly dostupné proměnné jako selectedSchema
#include <nvs.h>
#define EEPROM_SIZE 512

void loadSettings() {
    EEPROM.begin(EEPROM_SIZE);  // nutné u ESP32!

    EEPROM.get(0, selectedSchema);
    EEPROM.get(4, sensorSource);
    EEPROM.get(8, sensorTarget);
    EEPROM.get(12, deltaT);
    EEPROM.get(16, hysteresis);
    EEPROM.get(20, pwmMode);
    EEPROM.get(24, pwmMin);
    EEPROM.get(28, pwmMax);
    EEPROM.get(32, pwmFlush);
    EEPROM.get(36, timeStart);
    EEPROM.get(40, timeEnd);
    EEPROM.get(44, timeActive);
    EEPROM.get(48, flowRate);
    EEPROM.get(52, ekvitermActive);
    EEPROM.get(56, ekvitermStart);
    EEPROM.get(60, ekvitermEnd);
    EEPROM.get(64, ekvitermTargetLow);
    EEPROM.get(68, ekvitermTargetHigh);
}

void saveSettings() {
    EEPROM.put(0, selectedSchema);
    EEPROM.put(4, sensorSource);
    EEPROM.put(8, sensorTarget);
    EEPROM.put(12, deltaT);
    EEPROM.put(16, hysteresis);
    EEPROM.put(20, pwmMode);
    EEPROM.put(24, pwmMin);
    EEPROM.put(28, pwmMax);
    EEPROM.put(32, pwmFlush);
    EEPROM.put(36, timeStart);
    EEPROM.put(40, timeEnd);
    EEPROM.put(44, timeActive);
    EEPROM.put(48, flowRate);
    EEPROM.put(52, ekvitermActive);
    EEPROM.put(56, ekvitermStart);
    EEPROM.put(60, ekvitermEnd);
    EEPROM.put(64, ekvitermTargetLow);
    EEPROM.put(68, ekvitermTargetHigh);

    EEPROM.commit();
}
