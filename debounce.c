#include <stdio.h>
#include <stdint.h>

// 1. Bus error data (0 = healthy, 1 = failed attempt) from your Python script
int32_t bus_data[] = {0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0};
int data_length = 19;

int main() {
    // 2. Setup N-of-M Debounce parameters based on VIGIL-S3
    int N_threshold = 4; // We need 4 bad readings...
    int M_window = 6;    // ...within a rolling window of the last 6 readings
    
    // A simple array to act as our rolling memory window
    int window[6] = {0, 0, 0, 0, 0, 0}; 
    int window_index = 0;

    printf("Starting N-of-M Debounce Filter for Stuck Bus...\n\n");

    // 3. Loop through incoming bus data
    for(int i = 0; i < data_length; i++) {
        int reading = bus_data[i];
        
        // 4. Update the rolling window (overwriting the oldest reading)
        window[window_index] = reading;
        window_index = (window_index + 1) % M_window; 
        
        // 5. Count how many errors (1s) are currently in our window
        int error_count = 0;
        for(int j = 0; j < M_window; j++) {
            error_count += window[j]; 
        }
        
        printf("Reading: %d | Errors in last 6 readings: %d", reading, error_count);
        
        // 6. The State Machine Logic
        if (error_count >= N_threshold) {
            printf(" -> [STATE: CONFIRMED FAULT - Bus Stuck!]\n");
            printf("\nInitiating escalating fixes: Toggling clock line 9 times...\n");
            // In the real firmware, if toggling fails, you then reset the driver, 
            // and finally power-cycle the sensor.
            break;
        } else if (error_count > 0) {
            printf(" -> [STATE: SUSPECT - Watching closely...]\n");
        } else {
            printf(" -> [STATE: HEALTHY]\n");
        }
    }
    
    return 0;
}