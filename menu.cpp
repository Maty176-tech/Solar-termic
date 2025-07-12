// menu.cpp – Kompletní struktura s výpočtem výkonu, ΔT a řízením výstupů nebo přepínáním schémat
#include "menu.h"
#include <Adafruit_ST7789.h>
#include <EEPROM.h>
#include "sensors.h"
#include "outputs.h"
#include "schemas.h" // nově přidaná logika schémat

extern Adafruit_ST7789 tft;

// ------------------- PARAMETRY -------------------
int selectedSchema = 1;
int sensorSource = 0;
int sensorTarget = 1;
int deltaT = 5;
int hysteresis = 2;
int pwmMode = 0;
int pwmMin = 30;
int pwmMax = 100;
int pwmFlush = 10;
int timeStart = 360;
int timeEnd = 1320;
bool timeActive = false;
int flowRate = 300;
bool manualRelay[5] = {false, false, false, false, false};
int manualPWM = 0;

// ------------------- FUNKCE -------------------
float getDeltaT() {
    return teplota[sensorSource] - teplota[sensorTarget];
}

float calculatePower() {
    float dt = getDeltaT();
    float flow = flowRate / 3600.0;
    return 1.163 * flow * dt;
}

void updateOutputs() {
    if (selectedSchema >= 44) {
        float dt = getDeltaT();
        bool active = (!timeActive) || (millis() / 60000 % 1440 >= timeStart && millis() / 60000 % 1440 <= timeEnd);

        if (active && dt >= deltaT) {
            setRelay(0, true);
            int pwm = map(dt, deltaT, deltaT + 10, pwmMin * 2.55, pwmMax * 2.55);
            pwm = constrain(pwm, pwmMin * 2.55, pwmMax * 2.55);
            setPWM(pwm, pwmMode);
        } else {
            setRelay(0, false);
            setPWM(0, pwmMode);
        }
    } else {
        handleSchemas(); // logika všech ostatních schémat
    }
}

// ------------------- MENU DEFINICE -------------------
MenuItem systemMenu[] = {
    {"Schéma", MENU_VALUE, nullptr, &selectedSchema, 1, 46, nullptr, 0},
    {"Čidlo zdroj", MENU_VALUE, nullptr, &sensorSource, 0, 5, nullptr, 0},
    {"Čidlo cíl", MENU_VALUE, nullptr, &sensorTarget, 0, 5, nullptr, 0},
    {"Delta T", MENU_VALUE, nullptr, &deltaT, 1, 30, nullptr, 0},
    {"Hystereze", MENU_VALUE, nullptr, &hysteresis, 0, 10, nullptr, 0},
};

MenuItem pwmMenu[] = {
    {"PWM režim", MENU_VALUE, nullptr, &pwmMode, 0, 2, nullptr, 0},
    {"PWM min", MENU_VALUE, nullptr, &pwmMin, 0, 100, nullptr, 0},
    {"PWM max", MENU_VALUE, nullptr, &pwmMax, 0, 100, nullptr, 0},
    {"Proplach [s]", MENU_VALUE, nullptr, &pwmFlush, 0, 60, nullptr, 0},
};

MenuItem timeMenu[] = {
    {"Od [min]", MENU_VALUE, nullptr, &timeStart, 0, 1439, nullptr, 0},
    {"Do [min]", MENU_VALUE, nullptr, &timeEnd, 0, 1439, nullptr, 0},
    {"Aktivní?", MENU_VALUE, nullptr, (int*)&timeActive, 0, 1, nullptr, 0},
};

MenuItem infoMenu[] = {
    {"Teploty", MENU_ACTION, showTemperatures, nullptr, 0, 0, nullptr, 0},
    {"Delta T", MENU_ACTION, showDeltaT, nullptr, 0, 0, nullptr, 0},
    {"Výkon", MENU_ACTION, showPower, nullptr, 0, 0, nullptr, 0},
    {"PWM test", MENU_VALUE, nullptr, &manualPWM, 0, 255, nullptr, 0},
};

MenuItem mainMenu[] = {
    {"Systém", MENU_SUBMENU, nullptr, nullptr, 0, 0, systemMenu, 5},
    {"PWM", MENU_SUBMENU, nullptr, nullptr, 0, 0, pwmMenu, 4},
    {"Čas", MENU_SUBMENU, nullptr, nullptr, 0, 0, timeMenu, 3},
    {"Info", MENU_SUBMENU, nullptr, nullptr, 0, 0, infoMenu, 4},
};

int menuLength = sizeof(mainMenu) / sizeof(MenuItem);
MenuItem* currentMenu = mainMenu;
int currentLength = menuLength;
int currentIndex = 0;
bool editMode = false;
MenuItem* parentMenus[5];
int parentLengths[5];
int parentIndex[5];
int menuLevel = 0;

// ------------------- MENU FUNKCE -------------------
void drawMenu() {
    tft.fillScreen(ST77XX_BLACK);
    for (int i = 0; i < currentLength; i++) {
        tft.setCursor(10, i * 30 + 10);
        tft.setTextColor(i == currentIndex ? ST77XX_YELLOW : ST77XX_WHITE);
        tft.print(currentMenu[i].label);
        if (currentMenu[i].type == MENU_VALUE && currentMenu[i].valuePtr) {
            tft.print(": ");
            tft.print(*currentMenu[i].valuePtr);
        }
    }
}

void menuUp() {
    if (editMode && currentMenu[currentIndex].valuePtr) {
        if (*currentMenu[currentIndex].valuePtr < currentMenu[currentIndex].maxValue)
            (*currentMenu[currentIndex].valuePtr)++;
    } else {
        currentIndex = (currentIndex - 1 + currentLength) % currentLength;
    }
    drawMenu();
}

void menuDown() {
    if (editMode && currentMenu[currentIndex].valuePtr) {
        if (*currentMenu[currentIndex].valuePtr > currentMenu[currentIndex].minValue)
            (*currentMenu[currentIndex].valuePtr)--;
    } else {
        currentIndex = (currentIndex + 1) % currentLength;
    }
    drawMenu();
}

void menuEnter() {
    MenuItem& item = currentMenu[currentIndex];
    if (item.type == MENU_VALUE) {
        editMode = !editMode;
        if (!editMode && item.valuePtr)
            EEPROM.put((menuLevel * 10 + currentIndex) * sizeof(int), *item.valuePtr);
    } else if (item.type == MENU_SUBMENU) {
        parentMenus[menuLevel] = currentMenu;
        parentLengths[menuLevel] = currentLength;
        parentIndex[menuLevel] = currentIndex;
        menuLevel++;
        currentMenu = item.children;
        currentLength = item.childrenCount;
        currentIndex = 0;
    } else if (item.type == MENU_ACTION && item.action != nullptr) {
        item.action();
    }
    drawMenu();
}

void menuBack() {
    if (editMode) {
        editMode = false;
    } else if (menuLevel > 0) {
        menuLevel--;
        currentMenu = parentMenus[menuLevel];
        currentLength = parentLengths[menuLevel];
        currentIndex = parentIndex[menuLevel];
    }
    drawMenu();
}

// ------------------- INFO FUNKCE -------------------
void showTemperatures() {
    tft.fillScreen(ST77XX_BLACK);
    for (int i = 0; i < 6; i++) {
        tft.setCursor(10, i * 25 + 10);
        tft.printf("T%d: %.1f C", i + 1, teplota[i]);
    }
    delay(3000);
    drawMenu();
}

void showDeltaT() {
    tft.fillScreen(ST77XX_BLACK);
    float dt = getDeltaT();
    tft.setCursor(10, 50);
    tft.printf("Delta T: %.2f C", dt);
    delay(3000);
    drawMenu();
}

void showPower() {
    tft.fillScreen(ST77XX_BLACK);
    float power = calculatePower();
    tft.setCursor(10, 50);
    tft.printf("Výkon: %.2f kW", power);
    delay(3000);
    drawMenu();
}
