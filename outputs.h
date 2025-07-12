#pragma once

// Režimy PWM výstupu
enum PWMMode {
    PWM_NORMAL = 0,
    PWM_INVERTED = 1,
    PWM_ANALOG_0_10V = 2
};

// Nastavení relé a PWM
void initOutputs();
void setRelay(int index, bool state);
void setPWM(int value, int mode);
