# Calibration Workflows
## S32K3xx VCU Calibration Guide

**Document Version:** 1.0  
**Release Date:** November 2025

---

## Overview
This document describes calibration procedures for the S32K3xx-based VCU, including parameter adjustment, validation, and deployment workflows.

## Tools Required
- FreeMaster v3.2+
- CAN interface (PCAN-USB, Kvaser, etc.)
- UART USB adapter
- S32K3xx EVB or production board

## Calibration Parameters

### Motor Control
```c
typedef struct {
    float motor_Kp;              /* Proportional gain */
    float motor_Ki;              /* Integral gain */
    float motor_Kd;              /* Derivative gain */
    uint16_t motor_max_rpm;      /* Max speed (RPM) */
    uint16_t motor_max_current;  /* Max current (mA) */
    float motor_deadband;        /* Deadband (%) */
} MotorCalib_t;
```

### Battery Management
```c
typedef struct {
    float cell_voltage_min;      /* Min cell voltage (V) */
    float cell_voltage_max;      /* Max cell voltage (V) */
    float pack_current_max;      /* Max pack current (A) */
    float temp_max_charge;       /* Max charge temp (°C) */
    float temp_max_discharge;    /* Max discharge temp (°C) */
    uint8_t soc_table[101];      /* State of Charge LUT */
} BatteryCalib_t;
```

## Calibration Workflow

### Step 1: Connect to VCU
1. Connect CAN or UART interface
2. Open FreeMaster
3. Load project file: `calibration/VCU_Calib.pmpx`
4. Connect to target

### Step 2: Read Current Values
1. Variable Watch → Add variables
2. Select calibration group
3. Record current values

### Step 3: Modify Parameters
1. Double-click value in Variable Watch
2. Enter new value
3. Press Enter
4. Observe real-time behavior

### Step 4: Validate Changes
1. Run validation tests
2. Check for faults
3. Verify performance

### Step 5: Save to NVM
1. Execute `SaveCalibration()` function
2. Reset VCU
3. Verify values persisted

## Safety Considerations
- Always use default safe values initially
- Validate each change before saving
- Keep backup of working calibration
- Test in safe environment before deployment

---

**For Support:**  
Email: calibration@yourcompany.com
