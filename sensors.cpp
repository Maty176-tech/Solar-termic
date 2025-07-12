#include "sensors.h"
#include <Adafruit_MAX31865.h>

// 3 moduly MAX31865 – každý pro 2 čidla
// Připojení přes SPI (např. GPIO18=CLK, 19=MISO, 23=MOSI), 3x CS pin
#define RREF 4300.0
#define RNOMINAL 1000.0

Adafruit_MAX31865 max1 = Adafruit_MAX31865(5);  // CS1
Adafruit_MAX31865 max2 = Adafruit_MAX31865(15); // CS2
Adafruit_MAX31865 max3 = Adafruit_MAX31865(4);  // CS3

float teplota[6] = {0};

void initSensors() {
    max1.begin(MAX31865_2WIRE);
    max2.begin(MAX31865_2WIRE);
    max3.begin(MAX31865_2WIRE);
}

void readSensors() {
    // Čidla 0 a 1 z modulu 1
    teplota[0] = max1.temperature(RNOMINAL, RREF);
    teplota[1] = max1.temperature(RNOMINAL, RREF);

    // Čidla 2 a 3 z modulu 2
    teplota[2] = max2.temperature(RNOMINAL, RREF);
    teplota[3] = max2.temperature(RNOMINAL, RREF);

    // Čidla 4 a 5 z modulu 3
    teplota[4] = max3.temperature(RNOMINAL, RREF);
    teplota[5] = max3.temperature(RNOMINAL, RREF);
}
