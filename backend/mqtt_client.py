import json
import asyncio
import paho.mqtt.client as mqtt

class MQTTHandler:
    def __init__(self, broker_host="localhost", broker_port=1883):
        self.broker_host = broker_host
        self.broker_port = broker_port
        self.telemetry_callbacks = []
        self.client = mqtt.Client()
        self.client.on_connect = self.on_connect
        self.client.on_message = self.on_message
        self.loop = None  # Store reference to the main event loop

    def set_event_loop(self, loop):
        """Set the main event loop reference"""
        self.loop = loop

    def connect(self):
        self.client.connect(self.broker_host, self.broker_port, 60)
        self.client.loop_start()

    def on_connect(self, client, userdata, flags, rc):
        print("Connected to MQTT broker")
        client.subscribe("devices/+/telemetry")  # + matches any device ID

    def on_message(self, client, userdata, msg):
        try:
            payload = json.loads(msg.payload.decode())
        except json.JSONDecodeError:
            print("Invalid JSON")
            return
        
        # Schedule callbacks in the main event loop
        for cb in self.telemetry_callbacks:
            if self.loop and not self.loop.is_closed():
                if asyncio.iscoroutinefunction(cb):
                    # For async callbacks, schedule them in the main loop
                    asyncio.run_coroutine_threadsafe(cb(msg.topic, payload), self.loop)
                else:
                    # For sync callbacks, call directly
                    cb(msg.topic, payload)
            else:
                # Fallback for sync callbacks
                if not asyncio.iscoroutinefunction(cb):
                    cb(msg.topic, payload)

    def publish_command(self, device_id, command):
        topic = f"devices/{device_id}/commands"
        self.client.publish(topic, json.dumps(command))

    def add_telemetry_listener(self, callback):
        self.telemetry_callbacks.append(callback)