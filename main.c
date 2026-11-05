#include <stdio.h>
#include <stdint.h>

// 1. Take the first 10 readings from your Python 'Healthy Baseline' and multiply by 100
int32_t healthy_data[] = {2991, 2989, 2986, 3010, 2982, 2984, 3015, 2987, 3002, 2989};
int data_length = 10;

int main() {
    // 2. Initialize the baseline with the very first reading
    int32_t current_average = healthy_data[0]; 

    printf("Starting Integer EWMA Filter...\n");
    printf("Initial Baseline: %d\n\n", current_average);

    // 3. Loop through the incoming sensor data
    for(int i = 1; i < data_length; i++) {
        int32_t new_reading = healthy_data[i];
        
        /* 
           4. The Integer EWMA Formula
           Instead of heavy floating-point math (e.g., reading * 0.25), 
           we find the difference and divide by 4 using a bit-shift (>> 2).
           This is incredibly fast on microcontrollers.
        */
        current_average = current_average + ((new_reading - current_average) >> 2);
        
        printf("New Reading: %d | Updated Running Average: %d\n", new_reading, current_average);
    }
    
    return 0;
}