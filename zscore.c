#include <stdio.h>
#include <stdint.h>
#include <stdlib.h> // required for the abs() function

// 1. Power data scaled by 100 (from your Python script)
// Hovers around 50.00mA (5000), then suddenly spikes to 498.53mA (49853)
int32_t power_data[] = {5022, 4908, 5062, 4966, 4919, 49853, 50220, 50131};
int data_length = 8;

int main() {
    // 2. Setup Z-Score parameters based on VIGIL-S3 model
    int32_t baseline = 5000;         // Expected normal current (50.00mA)
    int32_t max_wobble = 200;        // The maximum expected normal deviation (+/- 2.00mA)
    int32_t surge_multiplier = 5;    // A reading 5x larger than the max wobble is a surge

    printf("Starting Integer Z-Score Filter for Power Surges...\n\n");

    // 3. Loop through the incoming sensor data
    for(int i = 0; i < data_length; i++) {
        int32_t reading = power_data[i];
        
        // 4. Calculate absolute deviation from normal
        int32_t deviation = abs(reading - baseline);
        
        printf("Reading: %d | Deviation: %d ", reading, deviation);

        // 5. The Z-Score Logic: Compare deviation to usual random wobble
        if (deviation > (max_wobble * surge_multiplier)) {
            printf("\n>> STATE: CONFIRMED FAULT - Power Surge Detected in 0.2s!\n");
            // In the real firmware, this immediately triggers isolation of the faulty part
            break; 
        } else {
            printf("| Status: HEALTHY (Within normal wobble)\n");
        }
    }
    
    return 0;
}