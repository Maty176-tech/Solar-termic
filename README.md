# ESP32 Solární regulátor

Plně funkční náhrada regulátoru SRS6-EP s podporou:
- 6× čidel PT1000 přes 3× MAX31865
- 5× výstupních relé
- 1× PWM výstup (volitelně 0–10 V)
- ST7789 displej 240×320 s grafickým menu
- výběr a editace všech schémat 1–46 (+ vlastní)
- webové rozhraní s konfigurací
- řízení prioritních zdrojů (solár, kotel, spirála)
- časové programy, ekvitermní řízení, výpočet výkonu
- uložené parametry v EEPROM
- plná lokalizace (čeština)

## Použití
1. Otevři projekt ve VSCode s PlatformIO
2. Nahraj firmware do ESP32
3. Nahraj obsah složky `/data/` pomocí `pio run --target uploadfs`
4. Připoj se na ESP32 (přes domácí WiFi), otevři IP adresu v prohlížeči
