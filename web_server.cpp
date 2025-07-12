#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include "menu.h"
#include "sensors.h"

AsyncWebServer server(80);

// -------------------- Připojení WiFi --------------------
const char* ssid = "NAZEV_TVE_WIFI";
const char* password = "TVE_HESLO";

void setupWiFi() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    Serial.print("Připojuji se na WiFi...");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi připojena. IP adresa: ");
    Serial.println(WiFi.localIP());
}

// -------------------- API: GET stav --------------------
void setupAPI() {
    server.on("/api/state", HTTP_GET, [](AsyncWebServerRequest *request) {
        StaticJsonDocument<1024> doc;

        JsonArray temps = doc.createNestedArray("temps");
        for (int i = 0; i < 6; i++) temps.add(teplota[i]);

        JsonObject config = doc.createNestedObject("config");
        config["schema"] = selectedSchema;
        config["deltaT"] = deltaT;
        config["pwmMode"] = pwmMode;
        config["pwmMin"] = pwmMin;
        config["pwmMax"] = pwmMax;
        config["timeStart"] = timeStart;
        config["timeEnd"] = timeEnd;
        config["flowRate"] = flowRate;

        String json;
        serializeJson(doc, json);
        request->send(200, "application/json", json);
    });

    // -------------------- API: POST config --------------------
    server.on("/api/config", HTTP_POST, [](AsyncWebServerRequest *request){}, NULL,
        [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
            StaticJsonDocument<1024> doc;
            DeserializationError err = deserializeJson(doc, data);
            if (err) {
                request->send(400, "application/json", "{\"error\":\"JSON error\"}");
                return;
            }

            selectedSchema = doc["schema"] | selectedSchema;
            deltaT        = doc["deltaT"] | deltaT;
            pwmMode       = doc["pwmMode"] | pwmMode;
            pwmMin        = doc["pwmMin"] | pwmMin;
            pwmMax        = doc["pwmMax"] | pwmMax;
            timeStart     = doc["timeStart"] | timeStart;
            timeEnd       = doc["timeEnd"] | timeEnd;
            flowRate      = doc["flowRate"] | flowRate;

            request->send(200, "application/json", "{\"status\":\"OK\"}");
        }
    );

    // Web UI
    server.serveStatic("/", LittleFS, "/").setDefaultFile("index.html");
}

// -------------------- Inicializace --------------------
void initServer() {
    if (!LittleFS.begin()) {
        Serial.println("LittleFS nepřipojen");
        return;
    }
    setupAPI();
    server.begin();
}
