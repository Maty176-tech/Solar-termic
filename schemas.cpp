// schemas.cpp – logika všech schémat topení + vlastní schéma 43
#include "schemas.h"
#include "sensors.h"
#include "outputs.h"

extern int selectedSchema;
extern float teplota[6];

void handleSchemas(int id) {
    switch (id) {
        case 1:
            // Schéma 1: jednoduchý solární ohřev TUV
            if (teplota[0] - teplota[1] > 5) {
                setRelay(0, true);
            } else {
                setRelay(0, false);
            }
            break;

        case 2:
            // Schéma 2: solár + el. spirála (spirála jako záloha)
            if (teplota[0] - teplota[1] > 5) {
                setRelay(0, true); // solární čerpadlo
                setRelay(1, false);
            } else if (teplota[1] < 45) {
                setRelay(0, false);
                setRelay(1, true); // sepnutí spirály
            } else {
                setRelay(0, false);
                setRelay(1, false);
            }
            break;

        case 43:
            // Schéma 43 – vlastní: Solár → TUV / AKU, + kotel a spirála
            if (teplota[0] > teplota[1] + 5) {
                setRelay(0, true);  // čerpadlo solár
                setRelay(2, true);  // ventil do TUV
            } else if (teplota[0] > teplota[3] + 5) {
                setRelay(0, true);  // čerpadlo solár
                setRelay(2, false); // ventil do AKU
            } else {
                setRelay(0, false);
            }

            if (teplota[1] < 45 && teplota[0] < teplota[1] + 2) {
                if (teplota[4] > 60) {
                    setRelay(3, true);  // kotel sepne čerpadlo
                    setRelay(4, false);
                } else {
                    setRelay(3, false);
                    setRelay(4, true);  // spirála
                }
            } else {
                setRelay(3, false);
                setRelay(4, false);
            }
            break;

        // TODO: přidat další schémata 3–42, 44–46

        default:
            setAllRelays(false);
            break;
    }
}

void setAllRelays(bool state) {
    for (int i = 0; i < 5; i++) {
        setRelay(i, state);
    }
}
