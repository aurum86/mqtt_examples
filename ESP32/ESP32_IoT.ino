/*
  Modified for ESP32
*/

#include "thingProperties.h"
#include <WiFi.h>
#include <PubSubClient.h>
// #include <Adafruit_Sensor.h>
// #include <DHT.h>

// DHT sensor setup (replace with your actual sensor if different)
#define DHTPIN 4     // GPIO4
#define DHTTYPE DHT22
// DHT dht(DHTPIN, DHTTYPE);

WiFiClient espClient;
PubSubClient client(espClient);

void connectToWiFi() {
  Serial.print("Connecting to WiFi");
  WiFi.begin(SSID, PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void connectToMQTT() {
  client.setServer(remote_endpoint, remote_port);
  while (!client.connected()) {
    Serial.print("Connecting to MQTT...");
    if (client.connect("ESP32Client")) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  // dht.begin();
  connectToWiFi();
  connectToMQTT();
}

void loop() {
  if (!client.connected()) {
    connectToMQTT();
  }
  client.loop();

  // temperature = dht.readTemperature();
  temperature = rand() % 101;
  if (!isnan(temperature)) {
    String payload = String("{\"temperature\":") + temperature + "}";
    client.publish(mqtt_topic, payload.c_str());
    Serial.println("Published: " + payload);
  } else {
    Serial.println("Failed to read from DHT sensor!");
  }

  delay(1000);
}
