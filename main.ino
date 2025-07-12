#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>

#include "menu.h"
#include "sensors.h"
#include "outputs.h"
#include "schemas.h"
#include "my_eeprom.h"
#include "server.cpp" // Webové rozhraní a API
#include "draw_schema.cpp" // Volitelně externě

// -------------------- TFT displej (ST7789) --------------------
#define TFT_CS     5
#define TFT_RST    22
#define TFT_DC     21

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

// -------------------- Tlačítka --------------------
#define BTN_UP     32
#define BTN_DOWN   33
#define BTN_ENTER  25
#define BTN_BACK   26

unsigned long lastButtonTime = 0;
int debounceDelay = 200;

void checkButtons() {
    if (millis() - lastButtonTime < debounceDelay) return;

    if (!digitalRead(BTN_UP)) {
        menuUp();
        lastButtonTime = millis();
    } else if (!digitalRead(BTN_DOWN)) {
        menuDown();
        lastButtonTime = millis();
    } else if (!digitalRead(BTN_ENTER)) {
        menuEnter();
        lastButtonTime = millis();
    } else if (!digitalRead(BTN_BACK)) {
        menuBack();
        lastButtonTime = millis();
    }
}

// -------------------- Setup --------------------
void setup() {
    Serial.begin(115200);

    pinMode(BTN_UP, INPUT_PULLUP);
    pinMode(BTN_DOWN, INPUT_PULLUP);
    pinMode(BTN_ENTER, INPUT_PULLUP);
    pinMode(BTN_BACK, INPUT_PULLUP);

    tft.init(240, 320);
    tft.setRotation(1);
    tft.fillScreen(ST77XX_BLACK);

    setupWiFi();
    initServer();

    initSensors();
    initOutputs();
    initStats();  // inicializace EEPROM pro statistiky

    loadSettings();
    drawMenu(); // První menu
}

// -------------------- Hlavní smyčka --------------------
unsigned long lastRead = 0;

void loop() {
    checkButtons();

    if (millis() - lastRead > 1000) {
        readSensors();
        handleSchemas(selectedSchema);  // aktivní schéma
        updateOutputs();                // výstupy
        drawSchemaGraphic(selectedSchema); // vykreslení
        lastRead = millis();
    }
}

