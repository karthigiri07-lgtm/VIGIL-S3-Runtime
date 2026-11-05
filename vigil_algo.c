#include <stdio.h>
#include <stdlib.h>
#include "vigil_algo.h"

// 1. Global Variables: These act as the chip's persistent memory
// Because FreeRTOS tasks run forever, we store the state outside the functions
SystemState current_state = HEALTHY;

// Persistent memory for CUSUM (Slow Heating)
int32_t cusum_score = 0;

// Persistent memory for N-of-M Debounce (Stuck Bus)
int window[6] = {0, 0, 0, 0, 0, 0};
int window_index = 0;

/* ---------------------------------------------------------
 * 2. CUSUM: Check for Slow Heating
 * --------------------------------------------------------- */
void check_slow_heating(int32_t live_temp_reading) {
    int32_t baseline = 3000;
    int32_t allowed_wobble = 25;
    
    int32_t deviation = live_temp_reading - baseline;
    
    if (deviation > allowed_wobble) {
        cusum_score += (deviation - allowed_wobble);
    } else if (cusum_score > 0) {
        cusum_score -= 10;
        if (cusum_score < 0) cusum_score = 0; // Floor at zero
    }

    if (cusum_score > 500) {
        current_state = CONFIRMED; // Shift the global state[cite: 2]
        printf(">> STATE SHIFT: CONFIRMED FAULT - Slow Heating Detected!\n");
    }
}

/* ---------------------------------------------------------
 * 3. Z-SCORE: Check for Power Surges
 * --------------------------------------------------------- */
void check_power_surge(int32_t live_power_reading) {
    int32_t baseline = 5000;
    int32_t max_wobble = 200;
    int32_t surge_multiplier = 5;

    int32_t deviation = abs(live_power_reading - baseline);

    if (deviation > (max_wobble * surge_multiplier)) {
        current_state = CONFIRMED;
        printf(">> STATE SHIFT: CONFIRMED FAULT - Power Surge Detected!\n");
    }
}

/* ---------------------------------------------------------
 * 4. DEBOUNCE: Check for Stuck Communication Bus
 * --------------------------------------------------------- */
void check_bus_status(int32_t bus_error_reading) {
    int N_threshold = 4;
    int M_window = 6;
    
    // Update the rolling window
    window[window_index] = bus_error_reading;
    window_index = (window_index + 1) % M_window;
    
    // Count errors in the window
    int error_count = 0;
    for(int j = 0; j < M_window; j++) {
        error_count += window[j];
    }
    
    // Update the state machine based on the error count[cite: 2]
    if (error_count >= N_threshold) {
        current_state = CONFIRMED;
        printf(">> STATE SHIFT: CONFIRMED FAULT - Bus Stuck! Initiating fixes...\n");
    } else if (error_count > 0 && current_state != CONFIRMED) {
        current_state = SUSPECT; // One odd reading, watching closely[cite: 2]
    } else if (error_count == 0 && current_state == SUSPECT) {
        current_state = RECOVERING; // Problem seems gone, waiting to trust it[cite: 2]
    }
}