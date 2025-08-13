# Setup Instructions

## Prerequisites

- Python 3.8+
- Node.js 16+ (for frontend)
- Arduino IDE (for ESP32)
- MQTT Broker (Mosquitto recommended)

## Backend Setup

1. **Navigate to backend directory:**
   ```bash
   cd backend
   ```

2. **Create virtual environment:**
   ```bash
   python -m venv venv
   ```

3. **Activate virtual environment:**
   ```bash
   # Windows
   venv\Scripts\activate
   
   # Linux/Mac
   source venv/bin/activate
   ```

4. **Install dependencies:**
   ```bash
   pip install -r requirements.txt
   ```

5. **Configure environment:**
   ```bash
   cp .env.example .env
   # Edit .env with your settings
   ```

6. **Start the server:**
   ```bash
   uvicorn main:app --reload --port 8000
   ```

## Frontend Setup (Coming Soon)

1. **Navigate to frontend directory:**
   ```bash
   cd frontend
   ```

2. **Install dependencies:**
   ```bash
   npm install
   ```

3. **Start development server:**
   ```bash
   npm run dev
   ```

## ESP32 Setup

1. **Open Arduino IDE**

2. **Install ESP32 Board Manager:**
   - File → Preferences
   - Add: `https://dl.espressif.com/dl/package_esp32_index.json`
   - Tools → Board → Boards Manager
   - Search "ESP32" and install

3. **Install Libraries:**
   - WiFi (built-in)
   - PubSubClient (for MQTT)
   - ArduinoJson (for JSON handling)

4. **Configure WiFi/MQTT:**
   - Open `firmware/esp32/esp32-sensor.ino`
   - Update WiFi credentials
   - Update MQTT broker address

5. **Upload to ESP32:**
   - Select correct board and port
   - Upload the sketch

## MQTT Broker Setup

### Option 1: Local Mosquitto

1. **Install Mosquitto:**
   - Windows: Download from mosquitto.org
   - Linux: `sudo apt install mosquitto mosquitto-clients`
   - Mac: `brew install mosquitto`

2. **Create config file** (`mosquitto.conf`):
   ```
   listener 1883
   allow_anonymous true
   ```

3. **Start broker:**
   ```bash
   mosquitto -c mosquitto.conf
   ```

### Option 2: Cloud MQTT

Use services like:
- AWS IoT Core
- Google Cloud IoT
- HiveMQ Cloud
- Eclipse Mosquitto

## Testing

1. **Test MQTT connection:**
   ```bash
   cd scripts
   python test_mqtt.py telemetry
   ```

2. **Test LED control:**
   ```bash
   python test_mqtt.py led 1 YOUR_ESP32_IP
   ```

3. **Monitor WebSocket:**
   - Open browser to `http://localhost:8000`
   - Open browser console to see WebSocket messages

## Troubleshooting

### Common Issues

1. **MQTT Connection Failed:**
   - Check broker is running
   - Verify IP address and port
   - Check firewall settings

2. **ESP32 Not Connecting:**
   - Verify WiFi credentials
   - Check serial monitor for errors
   - Ensure ESP32 and computer on same network

3. **WebSocket Not Working:**
   - Check FastAPI server is running
   - Verify port 8000 is available
   - Check browser console for errors
