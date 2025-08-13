#include <WiFi.h>
#include <PubSubClient.h>

// WiFi credentials
const char* ssid = "OnePlus Nord CE 2 Lite 5G";
const char* password = "1111111111";

// MQTT Broker settings
const char* mqtt_server = "10.16.104.1";  // Your PC's local IP running Mosquitto
const int mqtt_port = 1883;

// Create WiFi & MQTT client objects
WiFiClient espClient;
PubSubClient client(espClient);

// Callback: handles incoming subscribed messages
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (unsigned int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Example: turn on/off onboard LED if topic is "esp32/control/led"
  if (String(topic) == "esp32/control/led") {
    if ((char)payload[0] == '1') {
      digitalWrite(LED_BUILTIN, HIGH);
    } else {
      digitalWrite(LED_BUILTIN, LOW);
    }
  }
}

// Connect to WiFi
void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println(WiFi.localIP());
}

// Reconnect to MQTT if disconnected
void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP32Client")) {
      Serial.println("connected");
      client.subscribe("esp32/control/led");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Publish test message every 5 seconds
  static unsigned long lastMsg = 0;
  if (millis() - lastMsg > 5000) {
    lastMsg = millis();
    String msg = "Hello from ESP32";
    client.publish("esp32/sensors/test", msg.c_str());
    Serial.println("Published: " + msg);
  }
}
