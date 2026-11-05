#include <stdio.h>
#include <stdint.h>
#include "vigil_algo.h"

// Bring in the global state from vigil_algo.c
extern SystemState current_state; 

// Simulated hardware sensor reads over time
int32_t live_temps[] = {3015, 3050, 3100, 3150, 3200, 3350, 3447, 3464};
int32_t live_powers[] = {5022, 4908, 5062, 4966, 4919, 49853, 50220, 50131}; 
int32_t live_buses[] = {0, 0, 0, 0, 0, 0, 0, 0};

int main() {
    printf("--- VIGIL-S3 FreeRTOS Task Simulation ---\n\n");

    // Simulate the infinite FreeRTOS task loop
    for(int tick = 0; tick < 8; tick++) {
        printf("[Tick %d] Reading sensors...\n", tick);

        // 1. Read hardware (simulated)
        int32_t temp = live_temps[tick];
        int32_t power = live_powers[tick];
        int32_t bus = live_buses[tick];

        // 2. Feed data into your VIGIL-S3 algorithms
        check_slow_heating(temp);
        check_power_surge(power);
        check_bus_status(bus);

        // 3. ESP-NOW Transmission Logic (The 32-byte Health Message)[cite: 2]
        if (current_state == CONFIRMED) {
            // Create a package using the struct blueprint we defined in the header
            HealthPayload my_message;
            my_message.device_id = 1;
            my_message.health_score = 15; // Score from 0 to 100[cite: 2]
            my_message.active_faults = 1; // Flagging that a fault is active[cite: 2]
            
            printf("   -> ESP-NOW TX: Broadcasting 32-byte health message to fleet!\n");
            printf("      [Payload] Device: %d | Health: %d/100 | Faults: %d\n", 
                   my_message.device_id, my_message.health_score, my_message.active_faults);
        } else {
            printf("   -> Status Normal.\n");
        }

        // 4. Simulate going to sleep (vTaskDelay) to give the CPU to other tasks
        printf("   -> Task yielding CPU...\n\n");
    }

    return 0;
}