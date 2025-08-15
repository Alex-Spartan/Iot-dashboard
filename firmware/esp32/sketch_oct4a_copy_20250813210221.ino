#include <WiFi.h>
#include <PubSubClient.h>
#include "config.h"

// Create WiFi & MQTT client objects
WiFiClient espClient;
PubSubClient client(espClient);

// Callback: handles incoming subscribed messages
void callback(char* topic, byte* payload, unsigned int length) {
  DEBUG_PRINT("Message arrived [");
  DEBUG_PRINT(topic);
  DEBUG_PRINT("] ");
  for (unsigned int i = 0; i < length; i++) {
    DEBUG_PRINT((char)payload[i]);
  }
  DEBUG_PRINTLN();

  // Example: turn on/off onboard LED if topic is LED control
  if (String(topic) == TOPIC_LED_CONTROL) {
    if ((char)payload[0] == '1') {
      digitalWrite(LED_PIN, HIGH);
      DEBUG_PRINTLN("LED turned ON");
    } else {
      digitalWrite(LED_PIN, LOW);
      DEBUG_PRINTLN("LED turned OFF");
    }
  }
}

// Connect to WiFi
void setup_wifi() {
  delay(10);
  DEBUG_PRINTLN();
  DEBUG_PRINT("Connecting to ");
  DEBUG_PRINTLN(WIFI_SSID);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  
  unsigned long startTime = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startTime < WIFI_TIMEOUT) {
    delay(500);
    DEBUG_PRINT(".");
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    DEBUG_PRINTLN("");
    DEBUG_PRINTLN("WiFi connected");
    DEBUG_PRINT("IP address: ");
    DEBUG_PRINTLN(WiFi.localIP());
  } else {
    DEBUG_PRINTLN("");
    DEBUG_PRINTLN("WiFi connection failed!");
  }
}

// Reconnect to MQTT if disconnected
void reconnect() {
  while (!client.connected()) {
    DEBUG_PRINT("Attempting MQTT connection...");
    if (client.connect(MQTT_CLIENT_ID)) {
      DEBUG_PRINTLN("connected");
      
      // Subscribe to control topics
      client.subscribe(TOPIC_LED_CONTROL);
      DEBUG_PRINT("Subscribed to: ");
      DEBUG_PRINTLN(TOPIC_LED_CONTROL);
      
      // Send online status
      client.publish(TOPIC_STATUS, "{\"online\":true}");
    } else {
      DEBUG_PRINT("failed, rc=");
      DEBUG_PRINT(client.state());
      DEBUG_PRINTLN(" try again in 5 seconds");
      delay(MQTT_RECONNECT_DELAY);
    }
  }
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(SERIAL_BAUD_RATE);
  
  DEBUG_PRINTLN("=== ESP32 IoT Device Starting ===");
  DEBUG_PRINT("Device ID: ");
  DEBUG_PRINTLN(DEVICE_ID);
  
  setup_wifi();
  client.setServer(MQTT_SERVER, MQTT_PORT);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Publish telemetry data at defined interval
  static unsigned long lastTelemetry = 0;
  if (millis() - lastTelemetry > TELEMETRY_INTERVAL) {
    lastTelemetry = millis();
    
    // Create JSON telemetry message
    String telemetryMsg = "{";
    telemetryMsg += "\"device_id\":\"" + String(DEVICE_ID) + "\",";
    telemetryMsg += "\"timestamp\":" + String(millis()) + ",";
    telemetryMsg += "\"uptime\":" + String(millis() / 1000) + ",";
    telemetryMsg += "\"free_heap\":" + String(ESP.getFreeHeap()) + ",";
    telemetryMsg += "\"wifi_rssi\":" + String(WiFi.RSSI());
    telemetryMsg += "}";
    
    client.publish(TOPIC_TELEMETRY, telemetryMsg.c_str());
    DEBUG_PRINT("Published telemetry: ");
    DEBUG_PRINTLN(telemetryMsg);
  }

  // Publish test message (backward compatibility)
  static unsigned long lastTest = 0;
  if (millis() - lastTest > 10000) {  // Every 10 seconds
    lastTest = millis();
    String msg = "Hello from " + String(DEVICE_ID);
    client.publish(TOPIC_SENSOR_DATA, msg.c_str());
    DEBUG_PRINT("Published test: ");
    DEBUG_PRINTLN(msg);
  }
}
