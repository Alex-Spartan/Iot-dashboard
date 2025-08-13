import asyncio
import json
from fastapi import FastAPI, WebSocket, WebSocketDisconnect
from mqtt_client import MQTTHandler

app = FastAPI()
mqtt_handler = MQTTHandler()
connected_websockets = []

# Create async telemetry handler
async def handle_telemetry(topic, data):
    message = {"topic": topic, "data": data}
    await broadcast(message)

async def broadcast(message):
    if connected_websockets:
        # Create a copy of the list to avoid modification during iteration
        websockets_copy = connected_websockets.copy()
        for websocket in websockets_copy:
            try:
                await websocket.send_text(json.dumps(message))
            except Exception as e:
                print(f"Error sending to websocket: {e}")
                if websocket in connected_websockets:
                    connected_websockets.remove(websocket)

@app.on_event("startup")
async def startup_event():
    # Set the event loop reference and connect
    mqtt_handler.set_event_loop(asyncio.get_running_loop())
    mqtt_handler.add_telemetry_listener(handle_telemetry)
    mqtt_handler.connect()

@app.websocket("/ws")
async def websocket_endpoint(websocket: WebSocket):
    await websocket.accept()
    connected_websockets.append(websocket)
    try:
        while True:
            await websocket.receive_text()
    except WebSocketDisconnect:
        if websocket in connected_websockets:
            connected_websockets.remove(websocket)

@app.get("/")
async def root():
    return {"message": "ESP Dashboard API"}

@app.post("/command/{device_id}")
async def send_command(device_id: str, command: dict):
    mqtt_handler.publish_command(device_id, command)
    return {"status": "command sent", "device_id": device_id, "command": command}