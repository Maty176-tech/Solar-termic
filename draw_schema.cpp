#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include "sensors.h"
#include "outputs.h"

extern Adafruit_ST7789 tft;
extern float teplota[6];
extern int selectedSchema;

// ---------- Pomocné funkce ----------
void drawTank(int x, int y, const char* label) {
    tft.drawRect(x, y, 40, 40, ST77XX_RED);
    tft.setCursor(x + 5, y + 15);
    tft.setTextColor(ST77XX_RED);
    tft.setTextSize(1);
    tft.print(label);
}

void drawPump(int x, int y, bool active) {
    tft.fillCircle(x, y, 8, active ? ST77XX_GREEN : ST77XX_BLUE);
    tft.setCursor(x - 4, y - 3);
    tft.setTextColor(ST77XX_BLACK);
    tft.print("P");
}

void drawValve(int x, int y, bool toTop) {
    if (toTop) {
        tft.fillTriangle(x, y, x - 5, y + 10, x + 5, y + 10, ST77XX_YELLOW);
    } else {
        tft.fillTriangle(x, y + 10, x - 5, y, x + 5, y, ST77XX_YELLOW);
    }
}

void drawHeater(int x, int y, bool active) {
    tft.fillRect(x, y, 20, 20, active ? ST77XX_ORANGE : ST77XX_BLUE);
    tft.setCursor(x + 2, y + 6);
    tft.setTextColor(ST77XX_BLACK);
    tft.setTextSize(1);
    tft.print("E");
}

void drawBoiler(int x, int y, bool active) {
    tft.drawRoundRect(x, y, 24, 30, 5, active ? ST77XX_RED : ST77XX_WHITE);
    tft.setCursor(x + 4, y + 10);
    tft.setTextColor(ST77XX_RED);
    tft.print("K");
}

void drawArrow(int x1, int y1, int x2, int y2, uint16_t color) {
    tft.drawLine(x1, y1, x2, y2, color);
    tft.drawCircle(x2, y2, 2, color);
}

void drawTemperatureBar() {
    tft.setTextSize(1);
    tft.setTextColor(ST77XX_WHITE);
    for (int i = 0; i < 6; i++) {
        tft.setCursor(10 + (i * 50), 210);
        tft.printf("T%d:%.1f", i + 1, teplota[i]);
    }
}

// ---------- Hlavní funkce ----------
void drawPredefinedSchema(int id) {
    tft.setTextSize(1);
    tft.setTextColor(ST77XX_WHITE);
    tft.setCursor(10, 10);
    tft.printf("Schéma %d", id);

    if (id == 1) {
        drawTank(160, 60, "TUV");
        drawPump(100, 80, true);
        tft.fillRect(20, 60, 30, 30, ST77XX_YELLOW);
        drawArrow(50, 75, 90, 75, ST77XX_WHITE);
        drawArrow(110, 75, 160, 75, ST77XX_WHITE);
    }

    if (id == 43) {
        drawTank(60, 30, "TUV");
        drawTank(60, 100, "AKU");
        drawValve(120, 50, true);     // přepínací ventil
        drawPump(90, 70, true);       // čerpadlo soláru
        drawBoiler(180, 30, false);   // kotel
        drawHeater(180, 100, false);  // spirála

        drawArrow(30, 70, 60, 45, ST77XX_WHITE);  // sol → TUV
        drawArrow(30, 70, 60, 115, ST77XX_WHITE); // sol → AKU
        drawArrow(90, 70, 120, 60, ST77XX_WHITE); // čerpadlo → ventil
        drawArrow(120, 60, 180, 45, ST77XX_RED);  // ventil → kotel
        drawArrow(120, 60, 180, 115, ST77XX_ORANGE); // ventil → spirála
    }

    // Můžeš doplnit i další schémata (2–42, 44–46)
}

void drawUserSchema() {
    // Zjednodušená verze – lze později propojit s konfigurací z menu
    drawTank(50, 30, "T1");
    drawPump(100, 80, true);
    drawHeater(160, 60, false);
    drawArrow(90, 80, 140, 80, ST77XX_GREEN);
}

void drawSchemaGraphic(int id) {
    tft.fillScreen(ST77XX_BLACK);

    if (id >= 1 && id <= 46) {
        drawPredefinedSchema(id);
    } else {
        drawUserSchema(); // uživatelský návrh
    }

    drawTemperatureBar();
}
