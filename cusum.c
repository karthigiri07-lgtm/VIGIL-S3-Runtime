#include <stdio.h>
#include <stdint.h>

// 1. Data transitioning from healthy (~30.00C) to slow heating (~35.12C)
// Scaled by 100 for integer math (from your Python script output)
int32_t heating_data[] = {3015, 3050, 3100, 3150, 3200, 3350, 3447, 3464, 3491, 3512};
int data_length = 10;

int main() {
    // 2. Setup CUSUM parameters based on the VIGIL-S3 model
    int32_t baseline = 3000;       // Expected normal temp (30.00C)
    int32_t allowed_wobble = 25;   // Ignore normal random noise up to 0.25C
    int32_t cusum_score = 0;       // The running total of small deviations
    int32_t fault_threshold = 500; // Trigger alarm if the score hits this limit

    printf("Starting Integer CUSUM Filter for Slow Heating...\n\n");

    // 3. Loop through the incoming sensor data
    for(int i = 0; i < data_length; i++) {
        int32_t reading = heating_data[i];
        
        // Calculate how far above the healthy baseline this reading is
        int32_t deviation = reading - baseline;
        
        // 4. The CUSUM Logic: Spot "not normal"
        if (deviation > allowed_wobble) {
            // Add the excess deviation to our running score
            cusum_score = cusum_score + (deviation - allowed_wobble);
        } else {
            // If the board cools down, slowly forgive past errors (floor at 0)
            if (cusum_score > 0) {
                cusum_score -= 10; 
                if (cusum_score < 0) cusum_score = 0;
            }
        }

        printf("Reading: %d | Deviation: %d | CUSUM Score: %d\n", reading, deviation, cusum_score);

        // 5. State Machine: Shift to CONFIRMED fault
        if (cusum_score > fault_threshold) {
            printf("\n>> STATE: CONFIRMED FAULT - Slow Heating Detected!\n");
            // In the Smart India Hackathon final code, this is where you trigger 
            // the mitigation (e.g., slowing the CPU to cool off)
            break; 
        }
    }
    
    return 0;
}