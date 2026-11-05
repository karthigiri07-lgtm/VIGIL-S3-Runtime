#ifndef VIGIL_ALGO_H
#define VIGIL_ALGO_H

#include <stdint.h>

// 1. The VIGIL-S3 State Machine
typedef enum {
    HEALTHY,
    SUSPECT,
    CONFIRMED,
    RECOVERING
} SystemState;

// 2. The 32-Byte ESP-NOW Health Message
typedef struct {
    uint8_t device_id;         // Who is sending this? (1 byte)
    uint8_t health_score;      // Score from 0 to 100 (1 byte)
    uint8_t active_faults;     // Which faults are active? (1 byte)
    uint16_t bus_error_count;  // Counter for bus failures (2 bytes)
    // The rest of the 32 bytes can be reserved for future sensor data
} HealthPayload;

// 3. Function Prototypes
void check_slow_heating(int32_t live_temp_reading);
void check_power_surge(int32_t live_power_reading);
void check_bus_status(int32_t bus_error_reading);

#endif