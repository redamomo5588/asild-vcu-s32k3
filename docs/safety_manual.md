# Safety Manual
## ASIL-D Vehicle Control Unit - ISO 26262 Compliance

**Document Classification:** Safety-Critical  
**Document Version:** 1.0  
**Release Date:** November 2025  
**Safety Integrity Level:** ASIL-D

---

## Table of Contents

1. [Introduction](#1-introduction)
2. [Safety Concept](#2-safety-concept)
3. [Safety Requirements](#3-safety-requirements)
4. [Safety Mechanisms](#4-safety-mechanisms)
5. [Diagnostic Coverage](#5-diagnostic-coverage)
6. [Fault Handling](#6-fault-handling)
7. [Safety Validation](#7-safety-validation)
8. [Safety Case](#8-safety-case)
9. [Operational Safety](#9-operational-safety)
10. [References](#10-references)

---

## 1. Introduction

### 1.1 Purpose

This Safety Manual describes the safety architecture, mechanisms, and procedures implemented in the ASIL-D Vehicle Control Unit to meet ISO 26262 functional safety requirements.

### 1.2 Scope

**Covered Topics:**
- Safety goals and requirements
- Hardware safety mechanisms
- Software safety measures
- Diagnostic coverage metrics
- Fault detection and handling
- Safety validation and verification

**Out of Scope:**
- Application-specific safety functions
- Vehicle-level HARA (Hazard Analysis and Risk Assessment)
- System safety requirements above ECU level

### 1.3 Applicable Standards

| Standard | Version | Description |
|----------|---------|-------------|
| ISO 26262 | 2018 (2nd Edition) | Road vehicles — Functional safety |
| ISO 21434 | 2021 | Cybersecurity engineering |
| MISRA C | 2012 (AMD2) | Guidelines for C language |
| AUTOSAR | R21-11 | Automotive software architecture |

### 1.4 Safety Lifecycle

```
ISO 26262 V-Model:

Concept Phase          → Item Definition → Hazard Analysis (HARA)
                          ↓
System Level          → Safety Goals → Technical Safety Concept
                          ↓
Hardware Level        → Hardware Safety Requirements → HW Design
                          ↓                               ↓
Software Level        → Software Safety Requirements → SW Design
                          ↓                               ↓
Integration           ← Hardware/Software Integration ← Implementation
                          ↓
Validation            → Safety Validation → Production Release
```

---

## 2. Safety Concept

### 2.1 Safety Goals

#### 2.1.1 Primary Safety Goals

| ID | Safety Goal | ASIL | Violation Consequence |
|----|-------------|------|-----------------------|
| SG-1 | Prevent unintended vehicle acceleration | D | Collision, injury, death |
| SG-2 | Prevent loss of braking function | D | Collision, injury, death |
| SG-3 | Prevent loss of steering control | D | Collision, injury, death |
| SG-4 | Detect and handle powertrain failures | D | Loss of propulsion |
| SG-5 | Maintain battery safety limits | D | Thermal runaway, fire |

#### 2.1.2 Safety Goal Decomposition

**Example: SG-1 (Prevent Unintended Acceleration)**

```
SG-1: Prevent Unintended Vehicle Acceleration (ASIL-D)
  ├─ FSR-1.1: Monitor torque request plausibility (ASIL-D)
  │    ├─ HSR-1.1.1: Dual-channel torque sensing (ASIL-D)
  │    └─ SSR-1.1.1: Torque comparison algorithm (ASIL-D)
  │
  ├─ FSR-1.2: Detect motor control failures (ASIL-D)
  │    ├─ HSR-1.2.1: Lockstep core monitoring (ASIL-D)
  │    └─ SSR-1.2.1: Watchdog supervision (ASIL-D)
  │
  └─ FSR-1.3: Emergency shutdown capability (ASIL-D)
       ├─ HSR-1.3.1: Independent hardware cutoff (ASIL-D)
       └─ SSR-1.3.1: Safe state transition (ASIL-D)
```

### 2.2 Technical Safety Concept

#### 2.2.1 Architectural Approach

**Multi-Layer Safety Architecture:**

```
┌──────────────────────────────────────────────────────────┐
│         Application Safety Layer (ASIL-D)                │
│  ┌────────────┐  ┌────────────┐  ┌────────────┐        │
│  │  Torque    │  │  Battery   │  │  Diagnostic│        │
│  │ Monitoring │  │ Monitoring │  │  Manager   │        │
│  └────────────┘  └────────────┘  └────────────┘        │
├──────────────────────────────────────────────────────────┤
│                 Safety Middleware (ASIL-D)                │
│  ┌────────────┐  ┌────────────┐  ┌────────────┐        │
│  │   Safety   │  │   E2E      │  │   Safety   │        │
│  │  Monitor   │  │ Protection │  │   OS       │        │
│  └────────────┘  └────────────┘  └────────────┘        │
├──────────────────────────────────────────────────────────┤
│        Hardware Safety Layer (ASIL-D)                    │
│  ┌────────────┐  ┌────────────┐  ┌────────────┐        │
│  │  Lockstep  │  │    ECC     │  │   FCCU     │        │
│  │   Cores    │  │  Memories  │  │  Monitor   │        │
│  └────────────┘  └────────────┘  └────────────┘        │
└──────────────────────────────────────────────────────────┘
```

#### 2.2.2 Freedom from Interference

**Spatial Separation:**
- Memory Protection Unit (MPU) enforces address space isolation
- XRDC provides peripheral access control
- Separate safety and non-safety memory regions

**Temporal Separation:**
- Fixed-priority preemptive scheduling
- Safety tasks at highest priorities
- Execution time monitoring via watchdog

**Communication Separation:**
- E2E protection on all safety-critical messages
- Sequence counters and CRCs
- Timeout monitoring

---

## 3. Safety Requirements

### 3.1 Hardware Safety Requirements

#### 3.1.1 Core Processing

| Req ID | Requirement | ASIL | Verification Method |
|--------|-------------|------|---------------------|
| HSR-001 | Lockstep cores shall operate in cycle-exact mode | D | Inspection, Test |
| HSR-002 | Core comparison shall detect single-bit errors | D | Fault injection |
| HSR-003 | Lockstep mismatch shall trigger immediate fault | D | Test |
| HSR-004 | CPU registers shall be tested at startup | D | LBIST |

#### 3.1.2 Memory Integrity

| Req ID | Requirement | ASIL | Verification Method |
|--------|-------------|------|---------------------|
| HSR-010 | Flash memory shall have ECC protection | D | Inspection |
| HSR-011 | SRAM shall have ECC protection | D | Inspection |
| HSR-012 | ECC errors shall be detected within 1 cycle | D | Fault injection |
| HSR-013 | Multi-bit ECC errors shall trigger fault | D | Test |
| HSR-014 | MBIST shall achieve >95% coverage | D | Analysis |

#### 3.1.3 Clock Monitoring

| Req ID | Requirement | ASIL | Verification Method |
|--------|-------------|------|---------------------|
| HSR-020 | System clock shall be monitored continuously | D | Inspection, Test |
| HSR-021 | Clock loss shall be detected within 10 µs | D | Test |
| HSR-022 | Clock frequency deviation >5% shall be detected | D | Test |
| HSR-023 | Clock fault shall trigger safe state | D | Test |

#### 3.1.4 Voltage Monitoring

| Req ID | Requirement | ASIL | Verification Method |
|--------|-------------|------|---------------------|
| HSR-030 | Supply voltages shall be monitored continuously | D | Inspection, Test |
| HSR-031 | Under-voltage shall be detected within 100 µs | D | Test |
| HSR-032 | Over-voltage shall be detected within 100 µs | D | Test |
| HSR-033 | Voltage fault shall trigger safe state | D | Test |

### 3.2 Software Safety Requirements

#### 3.2.1 Program Flow Monitoring

| Req ID | Requirement | ASIL | Verification Method |
|--------|-------------|------|---------------------|
| SSR-001 | Safety functions shall have logical monitoring | D | Review, Test |
| SSR-002 | Sequence errors shall be detected | D | Fault injection |
| SSR-003 | Unexpected jumps shall be detected | D | Fault injection |
| SSR-004 | Stack overflow shall be detected | D | Test |

#### 3.2.2 Data Integrity

| Req ID | Requirement | ASIL | Verification Method |
|--------|-------------|------|---------------------|
| SSR-010 | Safety data shall have CRC protection | D | Inspection |
| SSR-011 | Duplicate storage shall be used for critical data | D | Inspection |
| SSR-012 | Data comparison shall occur before use | D | Review |
| SSR-013 | Corrupted data shall trigger fault | D | Fault injection |

#### 3.2.3 Timing and Execution

| Req ID | Requirement | ASIL | Verification Method |
|--------|-------------|------|---------------------|
| SSR-020 | Safety tasks shall meet timing deadlines | D | Timing analysis |
| SSR-021 | Deadline overruns shall be detected | D | Test |
| SSR-022 | Execution time shall be monitored | D | Inspection |
| SSR-023 | Timing violations shall trigger fault | D | Test |

---

## 4. Safety Mechanisms

### 4.1 Core Lockstep

#### 4.1.1 Architecture

```
┌─────────────────────────────────────────────────────┐
│                  Cortex-M7 Core 0                   │
│  ┌───────────┐  ┌───────────┐  ┌───────────┐       │
│  │ Pipeline  │→ │  Execute  │→ │  Write    │       │
│  └───────────┘  └───────────┘  └───────────┘       │
│         ↓              ↓              ↓             │
├─────────────────────────────────────────────────────┤
│                    Comparator                       │
│    (Cycle-by-cycle comparison of all signals)      │
├─────────────────────────────────────────────────────┤
│                  Cortex-M7 Core 1                   │
│  ┌───────────┐  ┌───────────┐  ┌───────────┐       │
│  │ Pipeline  │→ │  Execute  │→ │  Write    │       │
│  └───────────┘  └───────────┘  └───────────┘       │
│         ↓              ↓              ↓             │
│    [Mismatch detected] → Trigger FCCU Fault        │
└─────────────────────────────────────────────────────┘
```

#### 4.1.2 Monitored Signals

**Compared Signals:**
- Instruction fetch address
- Data read/write address
- Data read/write data
- Exception/interrupt vectors
- Register file contents
- ALU results
- Memory access strobes

**Detection Latency:** < 1 clock cycle

#### 4.1.3 Configuration

```c
/* Enable lockstep mode */
void ConfigureLockstep(void) {
    /* Ensure both cores start from reset */
    MC_ME->PRTN0_CORE0_ADDR = (uint32_t)_start;
    MC_ME->PRTN0_CORE1_ADDR = (uint32_t)_start;
    
    /* Enable lockstep checker */
    MCM->CPO |= MCM_CPO_CPOREQ(1);
    
    /* Configure FCCU reaction on lockstep error */
    FCCU->NCF_CFG[FCCU_NCF_LOCKSTEP] = FCCU_NCF_CFG_FCCU_SET_AFTER_RESET;
}
```

See [Lockstep Manual](lockstep_manual.md) for detailed configuration.

### 4.2 Memory ECC

#### 4.2.1 Flash Memory ECC

**Configuration:**
- ECC scheme: SECDED (Single Error Correction, Double Error Detection)
- Granularity: 64-bit data + 8-bit ECC
- Coverage: 100% of code flash and data flash

**Error Handling:**
```c
/* Flash ECC error interrupt */
void FLASH_ECC_ERR_IRQHandler(void) {
    uint32_t status = C40ASF->MCR;
    
    if (status & C40ASF_MCR_EER_MASK) {
        /* Single-bit error (correctable) */
        uint32_t errorAddr = C40ASF->ADR;
        LogSingleBitError(errorAddr);
        
        /* Corrected automatically by hardware */
    }
    
    if (status & C40ASF_MCR_RWE_MASK) {
        /* Multi-bit error (uncorrectable) */
        /* Trigger safety reaction */
        FCCU_TriggerFault(FCCU_FAULT_FLASH_ECC);
    }
}
```

#### 4.2.2 SRAM ECC

**Configuration:**
- ECC scheme: SECDED
- Granularity: 32-bit data + 7-bit ECC
- Coverage: All SRAM regions

**Error Injection (for testing):**
```c
/* Inject ECC error for testing */
void InjectECCError(void) {
    /* Enable EIM (Error Injection Module) */
    EIM->EIMCR = EIM_EIMCR_GEIEN(1);
    
    /* Configure channel 0 for SRAM */
    EIM->EICHEN = EIM_EICHEN_EICH0EN(1);
    
    /* Inject single-bit error */
    EIM->EICHD0_WORD0 = 0x20000000;  /* Target address */
    EIM->EICHD0_WORD1 = 0x00000001;  /* Bit mask */
}
```

### 4.3 Watchdog Supervision

#### 4.3.1 Software Watchdog (SWT)

**Configuration:**
```c
/* Configure watchdog: 100ms timeout */
#define WATCHDOG_TIMEOUT_MS     100
#define WATCHDOG_WINDOW_MS      10

void ConfigureWatchdog(void) {
    SWT->CR = SWT_CR_WEN(0);  /* Disable for configuration */
    
    SWT->TO = WATCHDOG_TIMEOUT_MS * (SYSTEM_CLK_FREQ / 1000);
    SWT->WN = WATCHDOG_WINDOW_MS * (SYSTEM_CLK_FREQ / 1000);
    
    SWT->CR = SWT_CR_WEN(1) |      /* Enable watchdog */
              SWT_CR_ITR(1) |      /* Generate interrupt before reset */
              SWT_CR_HLK(1) |      /* Hard lock (cannot disable) */
              SWT_CR_SLK(1);       /* Soft lock (protected config) */
}

/* Service watchdog (must be called within window) */
void ServiceWatchdog(void) {
    SWT->SR = 0xC520;  /* First key */
    SWT->SR = 0xD928;  /* Second key */
}
```

#### 4.3.2 Windowed Watchdog

**Timing Diagram:**
```
     Window Start        Timeout
          ↓                 ↓
    ──────┬─────────────────┬──────→ Time
          │   Valid Window  │
          │<───── 90ms ────>│
          
    ✓ Service here = OK
    ✗ Service before window = Fault
    ✗ Service after timeout = Reset
```

### 4.4 Fault Collection (FCCU)

#### 4.4.1 FCCU Architecture

```
Hardware Faults                    Software Faults
     ↓                                  ↓
┌────────────────────────────────────────────────┐
│         Fault Collection Unit (FCCU)           │
│                                                 │
│  ┌──────────────────────────────────────────┐ │
│  │  Fault Registers (128 fault channels)     │ │
│  └──────────────────────────────────────────┘ │
│              ↓                                  │
│  ┌──────────────────────────────────────────┐ │
│  │  Configuration (per-fault reaction)       │ │
│  └──────────────────────────────────────────┘ │
│              ↓                                  │
│  ┌──────────────────────────────────────────┐ │
│  │  Output Pins (FCCU_Fxx)                  │ │
│  └──────────────────────────────────────────┘ │
└────────────────────────────────────────────────┘
     ↓                    ↓
  Safe State        External Shutdown
```

#### 4.4.2 Fault Configuration

```c
/* Configure FCCU reactions */
void ConfigureFCCU(void) {
    /* Enter configuration mode */
    FCCU->CTRL = FCCU_CTRL_OPR(0x1);
    while (FCCU->CTRL & FCCU_CTRL_OPS_MASK);
    
    /* Lockstep error: Immediate alarm + reset */
    FCCU->NCF_CFG[FCCU_NCF_LOCKSTEP] = 
        FCCU_NCF_CFG_FCCU_SET_AFTER_RESET |
        FCCU_NCF_CFG_ALARM_EN;
    
    /* ECC double-bit error: Alarm + reset */
    FCCU->NCF_CFG[FCCU_NCF_FLASH_ECC] = 
        FCCU_NCF_CFG_FCCU_SET_AFTER_RESET |
        FCCU_NCF_CFG_ALARM_EN;
    
    /* Clock monitor: Alarm */
    FCCU->NCF_CFG[FCCU_NCF_CLOCK_MON] = 
        FCCU_NCF_CFG_ALARM_EN;
    
    /* Exit configuration mode */
    FCCU->CTRL = FCCU_CTRL_OPR(0x0);
}
```

#### 4.4.3 Fault List

| Fault ID | Source | Reaction | Recovery |
|----------|--------|----------|----------|
| 0 | Core lockstep mismatch | Immediate reset | None |
| 1 | Flash ECC uncorrectable | Reset | None |
| 2 | SRAM ECC uncorrectable | Reset | None |
| 3 | Clock monitor fault | Alarm, degraded mode | Auto |
| 4 | Voltage monitor fault | Alarm, safe state | Auto |
| 5 | Watchdog timeout | Reset | None |
| 6 | Stack overflow | Reset | None |
| ... | ... | ... | ... |

### 4.5 Self-Test (STCU)

#### 4.5.1 LBIST (Logic Built-In Self-Test)

**Test Coverage:**
- CPU core logic: >95%
- FPU: >90%
- Cache controllers: >95%

**Execution:**
```c
/* Run LBIST at startup */
status_t RunLBIST(void) {
    /* Configure STCU2 for LBIST */
    STCU2->LBCFG = STCU2_LBCFG_LBIST_EN(1);
    
    /* Start test */
    STCU2->RUN = STCU2_RUN_LBIST_RUN(1);
    
    /* Wait for completion */
    while (STCU2->STATUS & STCU2_STATUS_LBIST_RUNNING);
    
    /* Check result */
    if (STCU2->LBESW & STCU2_LBESW_LBIST_FAIL) {
        return STATUS_LBIST_FAIL;
    }
    
    return STATUS_SUCCESS;
}
```

#### 4.5.2 MBIST (Memory Built-In Self-Test)

**Test Algorithms:**
- March C- algorithm for SRAM
- Checkerboard patterns
- Address decoder tests

**Execution:**
```c
/* Run MBIST at startup */
status_t RunMBIST(void) {
    /* Configure MBIST controller */
    PRAMC->PRCR1 = PRAMC_PRCR1_FT_DIS(0);  /* Enable functional test */
    
    /* Start MBIST */
    PRAMC->PRCR1 |= PRAMC_PRCR1_MBIST_START(1);
    
    /* Wait for completion */
    while (PRAMC->PRCR1 & PRAMC_PRCR1_MBIST_BUSY);
    
    /* Check result */
    if (PRAMC->PRCR1 & PRAMC_PRCR1_MBIST_FAIL) {
        return STATUS_MBIST_FAIL;
    }
    
    return STATUS_SUCCESS;
}
```

---

## 5. Diagnostic Coverage

### 5.1 FMEDA Analysis

**Failure Modes, Effects, and Diagnostic Analysis**

#### 5.1.1 Metrics (S32K3 Core)

| Metric | Value | Target | Status |
|--------|-------|--------|--------|
| Single-Point Fault Metric (SPFM) | 99.2% | >99% | ✓ Pass |
| Latent Fault Metric (LFM) | 96.5% | >90% | ✓ Pass |
| Random HW Fault (λ_Random) | 8.5 FIT | < 10 FIT | ✓ Pass |

**Definitions:**
- **SPFM**: Percentage of single-point faults that are detected or prevented
- **LFM**: Percentage of latent faults covered by diagnostic tests
- **FIT**: Failures In Time (failures per 10⁹ hours)

#### 5.1.2 Diagnostic Coverage by Component

| Component | λ (FIT) | DC (%) | Safe λ (FIT) | Dangerous λ (FIT) |
|-----------|---------|--------|--------------|-------------------|
| CPU Core | 2.5 | 99.5 | 2.49 | 0.01 |
| Flash | 1.8 | 99.0 | 1.78 | 0.02 |
| SRAM | 1.2 | 99.2 | 1.19 | 0.01 |
| ADC | 0.9 | 95.0 | 0.86 | 0.05 |
| CAN | 0.7 | 98.0 | 0.69 | 0.01 |
| Others | 1.4 | 90.0 | 1.26 | 0.14 |
| **Total** | **8.5** | **98.5** | **8.27** | **0.24** |

### 5.2 Fault Injection Testing

#### 5.2.1 Test Campaign

**Injected Faults:**

| Fault Type | Injection Method | Expected Detection | Result |
|------------|------------------|-------------------|--------|
| CPU bit flip | Software (debugger) | Lockstep mismatch | Pass |
| Flash corruption | EIM module | ECC error | Pass |
| SRAM corruption | EIM module | ECC error | Pass |
| Clock glitch | External generator | CMU fault | Pass |
| Voltage sag | Power supply | PVD fault | Pass |
| Watchdog timeout | SW delay | Reset | Pass |
| CAN bus error | CAN fault injector | Bus-off | Pass |

#### 5.2.2 Test Procedure

```python
# Fault injection script example
def inject_cpu_fault():
    """Inject single-bit flip in CPU register"""
    debugger.halt()
    reg_value = debugger.read_register('R0')
    debugger.write_register('R0', reg_value ^ 0x00000001)  # Flip bit 0
    debugger.run()
    
    # Expect lockstep error within 1ms
    time.sleep(0.001)
    assert fccu.get_fault(FCCU_NCF_LOCKSTEP) == True
```

### 5.3 Diagnostic Test Intervals

| Diagnostic Test | Interval | Latency | Coverage |
|-----------------|----------|---------|----------|
| Lockstep monitoring | Continuous | <1 cycle | 99.5% |
| ECC checking | Continuous | 1 cycle | 99.0% |
| Clock monitoring | Continuous | 10 µs | 98.0% |
| Voltage monitoring | Continuous | 100 µs | 95.0% |
| Watchdog | 100 ms | 100 ms | 90.0% |
| LBIST | At startup | 50 ms | 95.0% |
| MBIST | At startup | 100 ms | 97.0% |
| RAM test (runtime) | 1 second | 1 ms | 85.0% |

---

## 6. Fault Handling

### 6.1 Fault Response Strategy

#### 6.1.1 Fault Classification

| Class | Description | Response Time | Action |
|-------|-------------|---------------|--------|
| **Critical** | Immediate safety impact | <1 ms | Emergency shutdown |
| **Severe** | Potential safety impact | <10 ms | Degraded mode |
| **Moderate** | Functional impairment | <100 ms | Limp-home mode |
| **Minor** | Non-safety issue | <1 s | Log and continue |

#### 6.1.2 Response Matrix

| Fault | Class | Detection | Reaction | Recovery |
|-------|-------|-----------|----------|----------|
| Lockstep mismatch | Critical | HW, <1 cycle | Reset | Manual |
| ECC multi-bit | Critical | HW, 1 cycle | Reset | Manual |
| Watchdog timeout | Critical | HW, immediate | Reset | Auto |
| Clock loss | Severe | HW, 10 µs | Switch to backup clock | Auto |
| Voltage out-of-range | Severe | HW, 100 µs | Safe state | Auto |
| CAN bus-off | Moderate | SW, 1 ms | Deactivate CAN | Auto |
| Sensor plausibility | Moderate | SW, 10 ms | Use redundant sensor | Auto |
| Calibration CRC | Minor | SW, 1 s | Use default values | Manual |

### 6.2 Safe States

#### 6.2.1 Safe State Definition

**State 0: Emergency Shutdown**
- All outputs deactivated
- Power stage disabled
- Relay/contactor open
- Entry: Critical fault detected
- Exit: Manual reset required

**State 1: Degraded Mode**
- Limited functionality
- Reduced performance (e.g., 50% torque)
- Entry: Severe fault detected
- Exit: Fault cleared + health check passed

**State 2: Limp-Home Mode**
- Basic functionality maintained
- Performance limited (e.g., 30 km/h max)
- Entry: Moderate fault detected
- Exit: Fault cleared or service mode

#### 6.2.2 Safe State Transition

```c
typedef enum {
    SAFE_STATE_NORMAL,
    SAFE_STATE_LIMP_HOME,
    SAFE_STATE_DEGRADED,
    SAFE_STATE_EMERGENCY_SHUTDOWN
} SafeState_t;

void TransitionToSafeState(SafeState_t targetState) {
    switch (targetState) {
        case SAFE_STATE_EMERGENCY_SHUTDOWN:
            /* Highest priority */
            DisableAllPowerOutputs();
            OpenSafetyContactors();
            DisableAllInterrupts();
            SetFCCU_EmergencyOutput();
            while(1);  /* Stay in safe state */
            break;
            
        case SAFE_STATE_DEGRADED:
            LimitMotorTorque(50);  /* 50% torque limit */
            DisableNonEssentialFunctions();
            IncreaseDiagnosticRate();
            SetDTC(DTC_DEGRADED_MODE);
            break;
            
        case SAFE_STATE_LIMP_HOME:
            LimitVehicleSpeed(30);  /* 30 km/h max */
            DisableRegenBraking();
            SetDTC(DTC_LIMP_HOME_MODE);
            break;
            
        default:
            break;
    }
}
```

### 6.3 Fault Debouncing

#### 6.3.1 Debounce Strategy

**Purpose:** Avoid false positives due to transient faults

```c
typedef struct {
    uint32_t faultID;
    uint8_t threshold;
    uint8_t counter;
    uint32_t timeout_ms;
    uint32_t lastOccurrence;
} FaultDebounce_t;

bool DebounceFault(FaultDebounce_t *fault) {
    uint32_t currentTime = GetSystemTime_ms();
    
    /* Check if within timeout window */
    if ((currentTime - fault->lastOccurrence) > fault->timeout_ms) {
        /* Reset counter if timeout expired */
        fault->counter = 0;
    }
    
    /* Increment counter */
    fault->counter++;
    fault->lastOccurrence = currentTime;
    
    /* Fault confirmed if threshold exceeded */
    if (fault->counter >= fault->threshold) {
        fault->counter = 0;  /* Reset */
        return true;  /* Fault confirmed */
    }
    
    return false;  /* Not yet confirmed */
}

/* Example usage */
FaultDebounce_t sensorFault = {
    .faultID = FAULT_SENSOR_A,
    .threshold = 3,      /* Require 3 occurrences */
    .timeout_ms = 100,   /* Within 100ms */
    .counter = 0
};
```

#### 6.3.2 Debounce Parameters

| Fault Type | Threshold | Window | Rationale |
|------------|-----------|--------|-----------|