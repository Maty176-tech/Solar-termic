#include "outputs.h"
#include <Arduino.h>

// Výstupy (přizpůsob výběru pinů pro ESP32-WROOM)
const int relayPins[5] = {2, 4, 16, 17, 5};  // např. GPIO2, GPIO4, ...
const int pwmPin = 18; // PWM výstup

void initOutputs() {
    for (int i = 0; i < 5; i++) {
        pinMode(relayPins[i], OUTPUT);
        digitalWrite(relayPins[i], LOW);
    }
    ledcAttachPin(pwmPin, 0);      // kanál 0
    ledcSetup(0, 1000, 8);         // 1kHz, 8bit
    ledcWrite(0, 0);               // výchozí hodnota
}

void setRelay(int index, bool state) {
    if (index >= 0 && index < 5) {
        digitalWrite(relayPins[index], state ? HIGH : LOW);
    }
}

void setPWM(int value, int mode) {
    value = constrain(value, 0, 255);

    if (mode == PWM_INVERTED) {
        value = 255 - value;
    } else if (mode == PWM_ANALOG_0_10V) {
        // Pokud bude použit DAC (MCP4725) nebo RC filtr na analog
        // Zatím použij běžné PWM
        // Případně sem přidej analogWriteDAC() nebo I2C výstup
    }

    ledcWrite(0, value);
}
