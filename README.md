# VIGIL-S3: Cloud-Free Self-Diagnosis Runtime

**VIGIL-S3** is a lightweight, edge-based anomaly detection system designed for ESP32-S3 microcontroller networks. Built for the Smart India Hackathon, this project enables embedded devices to self-diagnose hardware faults—such as slow heating, power surges, and communication bus failures—without relying on cloud computing or heavy floating-point mathematics.

## 🧠 System Architecture & Philosophy
In memory-constrained embedded environments, heavy mathematical libraries drain CPU cycles and memory. VIGIL-S3 utilizes **integer-only mathematics** to process live sensor data in a fraction of a second.

The firmware architecture strictly separates mathematical logic from the hardware execution loop, ensuring modularity and easy integration into a FreeRTOS environment.

### Core Detection Algorithms
1. **CUSUM (Cumulative Sum) Filter:** Detects "slow heating" drifts by accumulating minor deviations over time, ignoring harmless random noise.
2. **Integer Z-Score Simulation:** Catches instantaneous power surges (spikes) by comparing absolute deviations against maximum expected wobble, triggering in < 0.2s.
3. **N-of-M Debounce:** Prevents false alarms on I2C/SPI buses by requiring *N* faulty readings within a rolling window of *M* attempts before confirming a stuck bus.

## 📂 Project Structure
The codebase is currently structured as a standalone C simulation to validate the integer logic prior to physical hardware flashing.

* `vigil_algo.h` / `vigil_algo.c`: The core library containing the state machine and integer math filters.
* `task_sim.c`: The execution loop (simulating a FreeRTOS `while(1)` task) that feeds live data arrays into the algorithms.
* `standalone_tests/`: Contains original isolated algorithm tests (`main.c`, `cusum.c`, `zscore.c`, `debounce.c`).

## 🚀 How to Run the Simulation
This project is built using the standard GCC compiler. To run the simulation and watch the state machine shift from HEALTHY to CONFIRMED FAULT:

1. Clone this repository.
2. Compile the core system:
   ```bash
   gcc task_sim.c vigil_algo.c -o vigil_system.exe
