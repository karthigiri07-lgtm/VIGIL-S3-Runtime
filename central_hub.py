import struct
import paho.mqtt.client as mqtt

# This tells Python how to decode your 32-byte C struct package
C_STRUCT_FORMAT = "<BBB H 27x"

def on_connect(client, userdata, flags, reason_code, properties=None):
    print("✅ Connected to the Fleet Network!")
    client.subscribe("vigils3/fleet/health")
    print("📡 Listening for 32-byte alerts from ESP32-S3 nodes...\n")

def on_message(client, userdata, msg):
    # Decode the raw binary transmission back into standard numbers
    try:
        data = struct.unpack(C_STRUCT_FORMAT, msg.payload)
        device_id = data[0]
        health_score = data[1]
        active_faults = data[2]
        
        # Print the Red Alert Dashboard UI
        print(f"⚠️ ALERT: FAULT CONFIRMED ON NODE {device_id} ⚠️")
        print(f"   -> Health Score:  {health_score}/100")
        print(f"   -> Fault Flag:    {active_faults}")
        print(f"   -> Action:        Isolating Node {device_id} from fleet.\n")
    except Exception as e:
        print(f"Error reading message: {e}")

print("--- VIGIL-S3 Fleet Command Center ---")
print("Connecting...")

# Connect to a public server to simulate the invisible Wi-Fi/ESP-NOW network
client = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2)
client.on_connect = on_connect
client.on_message = on_message

client.connect("broker.hivemq.com", 1883, 60)
client.loop_forever()