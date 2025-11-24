# Diagnostic Workflows
## S32K3xx VCU Diagnostics Guide

**Document Version:** 1.0  
**Release Date:** November 2025

---

## Overview
Comprehensive diagnostic procedures for S32K3xx VCU troubleshooting and validation.

## Diagnostic Tools
- UDS diagnostic tester
- FreeMaster
- Serial console
- CAN bus analyzer
- Oscilloscope

## Diagnostic Protocols

### UDS (ISO 14229)

**Supported Services:**
| SID | Service | Description |
|-----|---------|-------------|
| 0x10 | DiagnosticSessionControl | Enter diagnostic mode |
| 0x11 | ECUReset | Reset ECU |
| 0x14 | ClearDiagnosticInformation | Clear DTCs |
| 0x19 | ReadDTCInformation | Read fault codes |
| 0x22 | ReadDataByIdentifier | Read data |
| 0x27 | SecurityAccess | Unlock protected functions |
| 0x2E | WriteDataByIdentifier | Write data |
| 0x31 | RoutineControl | Execute routines |
| 0x3E | TesterPresent | Keep session alive |

**Example: Read DTCs**
```
Request:  0x19 0x02 0xFF
Response: 0x59 0x02 [DTC_High] [DTC_Mid] [DTC_Low] [Status]
```

### XCP Protocol

**Configuration:**
```c
#define XCP_CAN_ID_MASTER    0x700
#define XCP_CAN_ID_SLAVE     0x701
#define XCP_MAX_DTO          8
```

## Common Diagnostic Scenarios

### Scenario 1: No CAN Communication
**Symptoms:** No CAN messages received  
**Checks:**
1. Verify CAN transceiver power
2. Check CAN_H and CAN_L connections
3. Measure bus termination (60Ω)
4. Check bit timing configuration
5. Verify pin muxing

**Tools:**
```bash
candump can0
cansend can0 123#DEADBEEF
```

### Scenario 2: Lockstep Fault
**Symptoms:** System reset, FCCU fault  
**Checks:**
1. Read fault record from NVM
2. Analyze program counter
3. Check for ECC errors
4. Verify power supply stability

**Debug:**
```c
LockstepFaultRecord_t fault;
ReadFaultFromFlash(&fault);
printf("PC: 0x%08X\n", fault.programCounter);
```

### Scenario 3: ADC Measurement Error
**Symptoms:** Incorrect sensor readings  
**Checks:**
1. Verify reference voltage
2. Check ADC calibration
3. Measure signal with oscilloscope
4. Verify pin configuration

**Calibration:**
```c
ADC_Calibrate(ADC_INSTANCE_0);
float offset = ADC_GetOffset(ADC_INSTANCE_0, CHANNEL_0);
```

## Fault Code Reference

### Motor Control DTCs
| DTC | Description | Action |
|-----|-------------|--------|
| P0B00 | Hybrid Battery Voltage Low | Check battery |
| P0B12 | Hybrid Battery Voltage Sensor | Check sensor connection |
| P0C5A | Motor Control Module Performance | Check lockstep |

### Communication DTCs
| DTC | Description | Action |
|-----|-------------|--------|
| U0100 | Lost Communication With ECM | Check CAN bus |
| U0155 | Lost Communication With Cluster | Check network |

## Diagnostic Routines

### Routine: ADC Self-Test
```c
status_t DiagnosticRoutine_ADC_SelfTest(void) {
    /* Short inputs to GND */
    GPIO_SetPinMux(ADC_TEST_PIN, MUX_GPIO);
    GPIO_WritePin(ADC_TEST_PIN, 0);
    
    /* Measure */
    uint16_t value = ADC_ReadChannel(ADC_INSTANCE_0, CHANNEL_0);
    
    /* Should be near zero */
    if (value > ADC_THRESHOLD_LOW) {
        return STATUS_FAIL;
    }
    
    return STATUS_PASS;
}
```

---

**For Support:**  
Email: diagnostics@yourcompany.com
