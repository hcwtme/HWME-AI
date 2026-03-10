# ESP32-S3 Firmware Code for IR Control, Wi-Fi Management, HTTP API Server, Relay Control & Sensor Reading

#include <WiFi.h>
#include <WebServer.h>
#include <IRremote.h>
#include <DHT.h>

#define RELAY_PIN 2
#define DHT_PIN 4
#define DHT_TYPE DHT22 // or DHT11

// Wi-Fi credentials
const char* ssid = "your-ssid";
const char* password = "your-password";

// IR remote initialization
IRrecv irrecv(GPIO_NUM_14);
decode_results results;

// DHT Sensor initialization
DHT dht(DHT_PIN, DHT_TYPE);

WebServer server(80);

void setup() {
    Serial.begin(115200);
    pinMode(RELAY_PIN, OUTPUT);
    digitalWrite(RELAY_PIN, LOW);

    // Setup Wi-Fi connection
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }
    Serial.println("Connected to WiFi!");

    irrecv.enableIRIn(); // Start the IR receiver
    dht.begin();
    setupRoutes();
    server.begin();
}

void setupRoutes() {
    server.on("/status", HTTP_GET, handleStatus);
    server.on("/relay/on", HTTP_GET, []() {
        digitalWrite(RELAY_PIN, HIGH);
        server.send(200, "text/plain", "Relay On");
    });
    server.on("/relay/off", HTTP_GET, []() {
        digitalWrite(RELAY_PIN, LOW);
        server.send(200, "text/plain", "Relay Off");
    });
    server.onNotFound([]() {
        server.send(404, "text/plain", "Not Found");
    });
}

void loop() {
    server.handleClient();
    if (irrecv.decode(&results)) {
        Serial.print("IR Code: ");
        Serial.println(results.value);
        irrecv.resume();
    }
}

void handleStatus() {
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();
    String status = "Temperature: "+ String(temperature) + " °C\nHumidity: " + String(humidity) + " %";
    server.send(200, "text/plain", status);
}