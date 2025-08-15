# ESP32 IoT Device Firmware

This directory contains the Arduino sketch for the ESP32 IoT device that communicates with the dashboard backend via MQTT.

## 📁 Files

- **`sketch_oct4a_copy_20250813210221.ino`** - Main Arduino sketch
- **`config.h`** - Configuration file with WiFi, MQTT, and device settings

## 🔧 Configuration

### WiFi Settings
Edit `config.h` to update your WiFi credentials:
```cpp
const char* WIFI_SSID = "Your_WiFi_Name";
const char* WIFI_PASSWORD = "Your_WiFi_Password";
```

### MQTT Settings
Update MQTT broker settings in `config.h`:
```cpp
const char* MQTT_SERVER = "10.16.104.1";  // Your MQTT broker IP
const int MQTT_PORT = 1883;
const char* MQTT_CLIENT_ID = "ESP32_Device_01";
```

### Device Settings
Configure device identification:
```cpp
#define DEVICE_ID "esp32-01"
#define DEVICE_NAME "ESP32 Sensor Node"
```

## 📡 MQTT Topics

### Subscriptions (Incoming Commands)
- **`esp32/control/led`** - LED control commands ("0" = OFF, "1" = ON)

### Publications (Outgoing Data)
- **`devices/esp32-01/telemetry`** - Device telemetry data (JSON)
- **`devices/esp32-01/status`** - Device status updates
- **`esp32/sensors/test`** - Test messages (backward compatibility)

## 📊 Telemetry Data Format

The device publishes telemetry data in JSON format:
```json
{
  "device_id": "esp32-01",
  "timestamp": 123456789,
  "uptime": 12345,
  "free_heap": 200000,
  "wifi_rssi": -45
}
```

## 🔌 Hardware Setup

### Required Components
- ESP32 Development Board
- USB Cable for programming
- (Optional) External sensors

### Pin Configuration
- **Built-in LED** - Used for LED control demo
- **GPIO Pins** - Available for sensors (configure in `config.h`)

## 🚀 Installation

### Prerequisites
1. **Arduino IDE** with ESP32 board support
2. **Required Libraries:**
   - WiFi (built-in)
   - PubSubClient (install via Library Manager)

### Steps
1. **Install ESP32 Board Support:**
   - File → Preferences
   - Add: `https://dl.espressif.com/dl/package_esp32_index.json`
   - Tools → Board → Boards Manager
   - Search "ESP32" and install

2. **Install Libraries:**
   - Sketch → Include Library → Manage Libraries
   - Search "PubSubClient" by Nick O'Leary and install

3. **Configure Settings:**
   - Open `config.h`
   - Update WiFi credentials
   - Update MQTT broker IP address
   - Modify device ID if needed

4. **Upload to ESP32:**
   - Connect ESP32 via USB
   - Select correct board: Tools → Board → ESP32 Dev Module
   - Select correct port: Tools → Port
   - Click Upload

## 🔧 Testing

### Serial Monitor
1. Open Serial Monitor (115200 baud)
2. Look for connection messages:
   ```
   === ESP32 IoT Device Starting ===
   Device ID: esp32-01
   Connecting to Your_WiFi_Name
   WiFi connected
   IP address: 192.168.1.100
   Attempting MQTT connection...connected
   Subscribed to: esp32/control/led
   ```

### LED Control Test
Send MQTT command to control LED:
```bash
# Turn LED ON
mosquitto_pub -h YOUR_BROKER_IP -t "esp32/control/led" -m "1"

# Turn LED OFF  
mosquitto_pub -h YOUR_BROKER_IP -t "esp32/control/led" -m "0"
```

Or use the Python test script:
```bash
cd ../../scripts
python test_mqtt.py led 1 YOUR_BROKER_IP
```

## 🐛 Troubleshooting

### WiFi Connection Issues
- Verify SSID and password in `config.h`
- Check if ESP32 is in range of WiFi router
- Ensure WiFi network is 2.4GHz (ESP32 doesn't support 5GHz)

### MQTT Connection Issues
- Verify MQTT broker is running
- Check IP address in `config.h`
- Ensure ESP32 and broker are on same network
- Check firewall settings

### Upload Issues
- Ensure correct board and port selected
- Try holding BOOT button during upload
- Check USB cable and drivers

## 📈 Extending Functionality

### Adding Sensors
1. **Define pins in `config.h`:**
   ```cpp
   #define DHT_PIN 2
   #define TEMP_SENSOR_PIN A0
   ```

2. **Include sensor libraries:**
   ```cpp
   #include <DHT.h>
   ```

3. **Read sensors in `loop()`:**
   ```cpp
   float temperature = dht.readTemperature();
   float humidity = dht.readHumidity();
   ```

4. **Add to telemetry JSON:**
   ```cpp
   telemetryMsg += "\"temperature\":" + String(temperature) + ",";
   telemetryMsg += "\"humidity\":" + String(humidity) + ",";
   ```

### Debug Options
Enable/disable debug output in `config.h`:
```cpp
#define DEBUG_ENABLED true  // Set to false to disable debug
```

## 🔗 Integration

This firmware is designed to work with:
- **Backend:** FastAPI server with MQTT handler
- **Frontend:** Next.js dashboard (coming soon)
- **MQTT Broker:** Mosquitto or any MQTT broker

For complete system setup, see the main project README.
