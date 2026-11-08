import struct
import time
import paho.mqtt.client as mqtt

# The exact same 32-byte C struct map from vigil_algo.h
C_STRUCT_FORMAT = "<BBB H 27x"

client = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2)
client.connect("broker.hivemq.com", 1883, 60)

print("--- ESP32-S3 Node Simulator ---")
print("Simulating Tick 5: Power Surge and Slow Heating Confirmed!")

# Pack the fake data into 32 raw bytes (Device 1, Score 15, Fault 1, 0 Bus Errors)
raw_32_bytes = struct.pack(C_STRUCT_FORMAT, 1, 15, 1, 0)

print("Broadcasting 32-byte health message to fleet...")
client.publish("vigils3/fleet/health", raw_32_bytes)
time.sleep(1)
print("Transmission sent.")