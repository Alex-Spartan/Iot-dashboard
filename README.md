# ESP32 IoT Dashboard

A real-time IoT dashboard system with ESP32 devices, FastAPI backend, and Next.js frontend.

## 🏗️ Project Structure

```
esp32-iot-dashboard/
├── backend/                 # FastAPI backend
│   ├── app/
│   │   ├── __init__.py
│   │   ├── main.py         # FastAPI application
│   │   ├── mqtt_client.py  # MQTT handler
│   │   ├── models/         # Pydantic models
│   │   ├── routes/         # API routes
│   │   └── config.py       # Configuration
│   ├── requirements.txt    # Python dependencies
│   ├── .env.example       # Environment variables template
│   └── README.md          # Backend documentation
├── frontend/               # Next.js frontend
│   ├── src/
│   │   ├── components/    # React components
│   │   ├── pages/         # Next.js pages
│   │   ├── hooks/         # Custom React hooks
│   │   └── utils/         # Utility functions
│   ├── package.json       # Node.js dependencies
│   ├── next.config.js     # Next.js configuration
│   └── README.md          # Frontend documentation
├── firmware/               # ESP32 code
│   ├── esp32-sensor/      # Main sensor code
│   │   ├── esp32-sensor.ino
│   │   ├── config.h       # WiFi/MQTT configuration
│   │   └── README.md      # Firmware documentation
│   └── libraries/         # Custom libraries (if any)
├── docs/                   # Documentation
│   ├── api.md             # API documentation
│   ├── setup.md           # Setup instructions
│   └── architecture.md    # System architecture
├── scripts/                # Utility scripts
│   ├── setup.sh           # Setup script
│   └── test_mqtt.py       # MQTT testing script
├── docker-compose.yml      # Docker setup (optional)
├── .gitignore             # Git ignore file
├── LICENSE                # License file
└── README.md              # Main project documentation
```

## 🚀 Features

- **Real-time Communication**: WebSocket + MQTT integration
- **Device Control**: Send commands to ESP32 devices
- **Data Visualization**: Live sensor data dashboard
- **Responsive UI**: Modern Next.js frontend
- **REST API**: FastAPI backend with automatic documentation

## 🛠️ Tech Stack

### Backend
- **FastAPI** - Modern Python web framework
- **Paho MQTT** - MQTT client library
- **WebSocket** - Real-time communication
- **Uvicorn** - ASGI server

### Frontend
- **Next.js** - React framework
- **TypeScript** - Type-safe JavaScript
- **Tailwind CSS** - Utility-first CSS
- **Socket.IO** - Real-time communication

### Hardware
- **ESP32** - Microcontroller
- **Arduino IDE** - Development environment
- **Various Sensors** - Temperature, humidity, etc.

## 📦 Installation

### Prerequisites
- Python 3.8+
- Node.js 16+
- Arduino IDE
- MQTT Broker (Mosquitto)

### Backend Setup
```bash
cd backend
python -m venv venv
source venv/bin/activate  # On Windows: venv\Scripts\activate
pip install -r requirements.txt
uvicorn app.main:app --reload --port 8000
```

### Frontend Setup


### Firmware Setup
1. Open `firmware/esp32/esp32-sensor.ino` in Arduino IDE
2. Configure WiFi and MQTT settings in `config.h`
3. Upload to ESP32

## 🔧 Configuration

### Environment Variables
Copy `.env.example` to `.env` and configure:
```env
MQTT_BROKER_HOST=localhost
MQTT_BROKER_PORT=1883
WEBSOCKET_PORT=8000
```

### MQTT Topics
- **Telemetry**: `devices/{device_id}/telemetry`
- **Commands**: `devices/{device_id}/commands`
- **Status**: `devices/{device_id}/status`

## 📡 API Endpoints

- `GET /` - Health check
- `POST /command/{device_id}` - Send command to device
- `WS /ws` - WebSocket for real-time data
- `GET /docs` - Interactive API documentation

## 🚀 Quick Start

1. **Clone the repository**
   ```bash
   git clone https://github.com/yourusername/esp32-iot-dashboard.git
   cd esp32-iot-dashboard
   ```

2. **Start MQTT Broker**
   ```bash
   mosquitto -c mosquitto.conf
   ```

3. **Run Backend**
   ```bash
   cd backend && uvicorn app.main:app --reload
   ```

4. **Run Frontend**
   ```bash
   cd frontend && npm run dev
   ```

5. **Upload ESP32 Code**
   - Configure WiFi/MQTT in `firmware/esp32-sensor/config.h`
   - Upload to your ESP32

## 📱 Usage

1. Open dashboard at `http://localhost:3000`
2. View real-time sensor data
3. Send commands to ESP32 devices
4. Monitor device status

## 🤝 Contributing

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Submit a pull request

## 📄 License

This project is licensed under the MIT License - see [LICENSE](LICENSE) file.

## 🆘 Support

- 📧 Email: your.email@example.com
- 🐛 Issues: [GitHub Issues](https://github.com/yourusername/esp32-iot-dashboard/issues)
- 📖 Docs: [Documentation](./docs/)
