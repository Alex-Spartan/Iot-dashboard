# API Documentation

## Base URL
```
http://localhost:8000
```

## WebSocket Endpoint

### `/ws`
Real-time communication endpoint for receiving telemetry data and sending commands.

**Connection:**
```javascript
const ws = new WebSocket('ws://localhost:8000/ws');

ws.onmessage = (event) => {
    const data = JSON.parse(event.data);
    console.log('Received:', data);
};
```

**Message Format (Incoming - Telemetry):**
```json
{
    "topic": "devices/esp32-01/telemetry",
    "data": {
        "temp": 25.3,
        "humid": 60,
        "timestamp": 1634567890
    }
}
```

**Message Format (Outgoing - Commands):**
```json
{
    "type": "command",
    "deviceId": "esp32-01",
    "command": "led_on",
    "value": 1
}
```

## REST API Endpoints

### Health Check

#### `GET /`
Returns API status and information.

**Response:**
```json
{
    "message": "ESP Dashboard API",
    "status": "running",
    "version": "1.0.0"
}
```

### Device Commands

#### `POST /command/{device_id}`
Send a command to a specific ESP32 device.

**Parameters:**
- `device_id` (path): The ID of the target device

**Request Body:**
```json
{
    "command": "led_control",
    "value": 1,
    "timestamp": 1634567890
}
```

**Response:**
```json
{
    "status": "command sent",
    "device_id": "esp32-01",
    "command": {
        "command": "led_control",
        "value": 1,
        "timestamp": 1634567890
    }
}
```

**Example:**
```bash
curl -X POST "http://localhost:8000/command/esp32-01" \
     -H "Content-Type: application/json" \
     -d '{"command": "led_control", "value": 1}'
```

## MQTT Topics

### Telemetry Data
**Topic Pattern:** `devices/{device_id}/telemetry`

**Example:** `devices/esp32-01/telemetry`

**Payload:**
```json
{
    "temp": 25.3,
    "humid": 60,
    "pressure": 1013.25,
    "timestamp": 1634567890
}
```

### Commands
**Topic Pattern:** `devices/{device_id}/commands`

**Example:** `devices/esp32-01/commands`

**Payload:**
```json
{
    "command": "led_control",
    "value": 1,
    "timestamp": 1634567890
}
```

### Status Updates
**Topic Pattern:** `devices/{device_id}/status`

**Example:** `devices/esp32-01/status`

**Payload:**
```json
{
    "online": true,
    "last_seen": 1634567890,
    "battery": 85,
    "signal_strength": -45
}
```

## Error Codes

| Code | Description |
|------|-------------|
| 200  | Success |
| 400  | Bad Request - Invalid command format |
| 404  | Device Not Found |
| 500  | Internal Server Error |
| 503  | MQTT Broker Unavailable |

## Rate Limits

- WebSocket connections: 100 concurrent per IP
- REST API: 1000 requests per minute per IP
- MQTT messages: No limit (handled by broker)

## Authentication

Currently using anonymous access. In production, implement:
- JWT tokens for REST API
- WebSocket authentication
- MQTT username/password or certificates
