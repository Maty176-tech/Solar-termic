#pragma once

extern float teplota[6];  // naměřené teploty

void initSensors();       // inicializace všech 3 modulů MAX31865
void readSensors();       // čtení všech 6 čidel
