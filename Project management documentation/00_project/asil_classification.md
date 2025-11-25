# ASIL Classification Document
**Electric Motorcycle Vehicle Control Unit (VCU) - NXP S32K348**

---

## Document Information

| Attribute | Value |
|-----------|-------|
| **Project** | ASIL-D Electric Motorcycle VCU |
| **Document Version** | 1.0 |
| **Last Updated** | November 24, 2025 |
| **Status** | Draft for Review |
| **Target Platform** | NXP S32K348 (Dual Cortex-M7 Lockstep) |
| **Standard** | ISO 26262:2018 Road Vehicles - Functional Safety |
| **Author** | [Your Name/Team] |
| **Reviewer** | [Reviewer Name] |

---

## Table of Contents

1. [Executive Summary](#1-executive-summary)
2. [Item Definition](#2-item-definition)
3. [Hazard Analysis and Risk Assessment (HARA)](#3-hazard-analysis-and-risk-assessment-hara)
4. [Safety Goals](#4-safety-goals)
5. [ASIL Decomposition](#5-asil-decomposition)
6. [Functional Safety Concept](#6-functional-safety-concept)
7. [Safety Requirements Allocation](#7-safety-requirements-allocation)
8. [Verification and Validation Strategy](#8-verification-and-validation-strategy)
9. [References](#9-references)

---

## 1. Executive Summary

This document defines the Automotive Safety Integrity Level (ASIL) classification for the Electric Motorcycle Vehicle Control Unit (VCU) based on the NXP S32K348 dual-core lockstep architecture. The VCU is classified as **ASIL-D**, representing the highest degree of automotive hazard due to the critical nature of propulsion control, battery management, and safety-critical functions in electric motorcycles.

The ASIL-D classification is derived from comprehensive Hazard Analysis and Risk Assessment (HARA) in accordance with ISO 26262:2018. This document establishes safety goals, functional safety requirements, and technical safety requirements to ensure the VCU meets the highest functional safety standards.

### Key Classifications

| System Component | ASIL Level | Justification |
|-----------------|------------|---------------|
| **VCU Core (Lockstep)** | **ASIL-D** | Critical motor control, propulsion management |
| **Motor Control Function** | **ASIL-D** | Direct control of vehicle speed/torque |
| **Battery Management Interface** | **ASIL-D** | High-voltage safety, thermal management |
| **Brake System Interface** | **ASIL-D** | Regenerative braking, emergency stop |
| **Power Management** | **ASIL-D** | HV contactor control, power distribution |
| **CAN-FD Communication (Critical)** | **ASIL-D** | Safety-critical message transmission |
| **HMI Communication** | **ASIL-B** | Non-critical driver information |
| **Diagnostic Logging** | **QM** | Quality management, non-safety-critical |

---

## 2. Item Definition

### 2.1 System Overview

**Item Name**: Electric Motorcycle Vehicle Control Unit (VCU)

**Description**: The VCU is the central electronic control unit responsible for managing all critical functions of an electric motorcycle, including:
- Motor torque control and speed regulation
- Battery pack interface and power management
- High-voltage system control (contactors, pre-charge)
- Regenerative braking coordination
- Safety monitoring and fault detection
- Communication with BMS, MCU, DCDC, OBC, and HMI

**Operational Context**: Urban and highway motorcycle operation under varying environmental conditions (temperature: -20°C to +60°C, speed: 0-140 km/h)

### 2.2 System Boundaries

**Included in Item**:
- VCU hardware (NXP S32K348 dual-core lockstep processor)
- VCU firmware (application software, bootloader, safety handlers)
- Power supply management (12V/24V low-voltage, HV interlock)
- Communication interfaces (CAN-FD, LIN, Ethernet)
- Sensor inputs (throttle, brake, temperature, voltage, current)
- Actuator outputs (motor torque command, contactor control, PWM)

**Excluded from Item** (but interfaced):
- Battery Management System (BMS) - separate ASIL-D item
- Motor Controller (MCU) - separate ASIL-D item
- Brake system (mechanical/hydraulic) - separate ASIL-C item
- Human-Machine Interface (HMI) - ASIL-B item

### 2.3 Relevant ISO 26262 Parts

- **Part 3**: Concept Phase (HARA, Safety Goals)
- **Part 4**: Product Development at System Level
- **Part 5**: Product Development at Hardware Level
- **Part 6**: Product Development at Software Level
- **Part 8**: Supporting Processes
- **Part 9**: ASIL-Oriented and Safety-Oriented Analyses

---

## 3. Hazard Analysis and Risk Assessment (HARA)

### 3.1 HARA Methodology

The ASIL classification is determined by evaluating three parameters for each hazardous event:

| Parameter | Definition | Classes |
|-----------|------------|---------|
| **Severity (S)** | Potential harm to driver/passengers | S0: No injuries<br>S1: Light/moderate injuries<br>S2: Severe/life-changing injuries<br>S3: Life-threatening/fatal injuries |
| **Exposure (E)** | Probability of operational scenario | E0: Incredibly unlikely<br>E1: Very low probability<br>E2: Low probability<br>E3: Medium probability<br>E4: High probability |
| **Controllability (C)** | Driver's ability to prevent harm | C0: Controllable in general<br>C1: Simply controllable<br>C2: Normally controllable<br>C3: Difficult/uncontrollable |

**ASIL Determination**: The combination of S, E, and C determines the ASIL level using the ISO 26262 classification table.

### 3.2 Hazard Identification

#### 3.2.1 Operating Scenarios

| Scenario ID | Operating Condition | Speed Range | Traffic Density | Road Condition |
|-------------|---------------------|-------------|-----------------|----------------|
| **OS-01** | Urban commuting | 0-60 km/h | High | City streets, intersections |
| **OS-02** | Highway cruising | 80-140 km/h | Medium | Straight roads, curves |
| **OS-03** | Stop-and-go traffic | 0-30 km/h | Very high | Congested roads |
| **OS-04** | Hill climbing/descent | 20-80 km/h | Low-Medium | Inclines, mountain roads |
| **OS-05** | Emergency braking | All speeds | Variable | All road types |
| **OS-06** | Overtaking maneuver | 60-120 km/h | Medium | Highways, two-lane roads |

#### 3.2.2 Malfunctions and Hazardous Events

| Hazard ID | Malfunction | Hazardous Event | Severity (S) | Exposure (E) | Controllability (C) | **ASIL** |
|-----------|-------------|-----------------|--------------|--------------|---------------------|----------|
| **H-01** | Unintended motor acceleration | Loss of speed control, uncontrolled acceleration | **S3** | E4 | C3 | **ASIL-D** |
| **H-02** | Motor torque cut-off during ride | Sudden loss of propulsion in traffic | **S3** | E3 | C3 | **ASIL-D** |
| **H-03** | Motor over-speed (runaway) | Excessive speed beyond safe limits | **S3** | E2 | C3 | **ASIL-D** |
| **H-04** | Regenerative braking failure | Loss of braking capability, increased stopping distance | **S3** | E4 | C2 | **ASIL-D** |
| **H-05** | Battery over-temperature (thermal runaway) | Fire risk, explosion hazard | **S3** | E2 | C3 | **ASIL-D** |
| **H-06** | High-voltage contactor stuck closed | Inability to de-energize HV system, shock hazard | **S3** | E1 | C3 | **ASIL-C** |
| **H-07** | High-voltage contactor stuck open | Unable to start vehicle, stranded rider | **S1** | E2 | C0 | **QM** |
| **H-08** | Battery over-voltage (overcharge) | Cell damage, fire risk | **S3** | E1 | C2 | **ASIL-C** |
| **H-09** | Battery under-voltage (deep discharge) | Loss of propulsion, vehicle immobilization | **S2** | E2 | C1 | **ASIL-B** |
| **H-10** | CAN communication loss (safety-critical) | Loss of BMS/MCU data, unpredictable behavior | **S3** | E2 | C3 | **ASIL-D** |
| **H-11** | Throttle sensor failure (stuck/erroneous) | Unintended acceleration or no response | **S3** | E2 | C3 | **ASIL-D** |
| **H-12** | Brake sensor failure | No regenerative braking, reduced deceleration | **S2** | E3 | C2 | **ASIL-C** |
| **H-13** | VCU watchdog failure (no reset) | VCU unresponsive, system hang | **S3** | E1 | C3 | **ASIL-D** |
| **H-14** | Firmware corruption | Unpredictable VCU behavior | **S3** | E1 | C3 | **ASIL-D** |
| **H-15** | Lockstep core mismatch (undetected) | Silent fault, incorrect torque calculation | **S3** | E1 | C3 | **ASIL-D** |

### 3.3 ASIL Determination Matrix

Below is the ISO 26262 ASIL determination matrix:

|  | **C1** | **C2** | **C3** |
|---|--------|--------|--------|
| **S1 + E1** | QM | QM | QM |
| **S1 + E2** | QM | QM | A |
| **S1 + E3** | QM | A | A |
| **S1 + E4** | QM | A | B |
| **S2 + E1** | QM | QM | A |
| **S2 + E2** | QM | A | B |
| **S2 + E3** | A | B | C |
| **S2 + E4** | A | B | C |
| **S3 + E1** | A | B | C |
| **S3 + E2** | B | C | D |
| **S3 + E3** | B | C | **D** |
| **S3 + E4** | B | C | **D** |

**Note**: For motorcycles, controllability is inherently lower than cars due to:
- Single-track vehicle dynamics (less stable)
- No physical barriers protecting rider
- Higher vulnerability to external factors (wind, road surface)
- Reduced margin for error in emergency situations

---

## 4. Safety Goals

Safety goals are derived from the hazardous events identified in HARA. Each safety goal is assigned an ASIL level and a Safe Fault Tolerant Time Interval (SFTTI).

| SG ID | Safety Goal | Related Hazards | ASIL | SFTTI |
|-------|-------------|-----------------|------|-------|
| **SG-01** | **Prevent unintended motor acceleration** | H-01, H-11 | **ASIL-D** | 100 ms |
| **SG-02** | **Prevent sudden loss of propulsion** | H-02, H-10 | **ASIL-D** | 200 ms |
| **SG-03** | **Prevent motor over-speed** | H-03 | **ASIL-D** | 50 ms |
| **SG-04** | **Maintain regenerative braking functionality** | H-04, H-12 | **ASIL-D** | 100 ms |
| **SG-05** | **Prevent battery over-temperature conditions** | H-05 | **ASIL-D** | 500 ms |
| **SG-06** | **Ensure safe high-voltage system control** | H-06, H-08 | **ASIL-C** | 1000 ms |
| **SG-07** | **Detect and mitigate VCU internal faults** | H-13, H-14, H-15 | **ASIL-D** | 10 ms |
| **SG-08** | **Maintain safety-critical communication integrity** | H-10 | **ASIL-D** | 20 ms |

### 4.1 Safety Goal Definitions

#### SG-01: Prevent Unintended Motor Acceleration
**Description**: The VCU shall prevent any unintended acceleration of the motor that is not commanded by the rider via the throttle input.

**Safe State**: Motor torque command = 0 Nm (immediate shutdown)

**Fault Detection**: Throttle plausibility checks, lockstep core comparison, watchdog monitoring

**Fault Reaction**: Immediate torque reduction, transition to safe stop, HMI warning

---

#### SG-02: Prevent Sudden Loss of Propulsion
**Description**: The VCU shall maintain motor propulsion during normal operation unless explicitly commanded by the rider (brake, emergency stop).

**Safe State**: Controlled torque reduction with warning, graceful deceleration

**Fault Detection**: BMS/MCU communication monitoring, power supply monitoring, core synchronization

**Fault Reaction**: Limp-home mode (reduced power), HMI notification, fault logging

---

#### SG-03: Prevent Motor Over-Speed
**Description**: The VCU shall limit motor speed to safe operational limits (e.g., 10,000 RPM max) to prevent mechanical failure or loss of control.

**Safe State**: Motor speed limiter active, torque reduction

**Fault Detection**: Motor speed sensor validation, torque command plausibility, lockstep verification

**Fault Reaction**: Immediate torque reduction, speed limiting, HMI warning

---

#### SG-04: Maintain Regenerative Braking Functionality
**Description**: The VCU shall ensure regenerative braking is available when the rider applies the brake, providing predictable deceleration.

**Safe State**: Regenerative braking available with fallback to mechanical braking

**Fault Detection**: Brake sensor plausibility, BMS charge acceptance monitoring, MCU status

**Fault Reaction**: Alert rider, reduce regen braking gradually, rely on mechanical brakes

---

#### SG-05: Prevent Battery Over-Temperature Conditions
**Description**: The VCU shall monitor battery temperature and prevent operations that could lead to thermal runaway or fire.

**Safe State**: Reduced power mode, thermal cutoff if critical

**Fault Detection**: BMS temperature data validation, thermal model, CAN communication integrity

**Fault Reaction**: Power derate, safe stop, thermal shutdown, HMI critical warning

---

#### SG-06: Ensure Safe High-Voltage System Control
**Description**: The VCU shall safely control high-voltage contactors to prevent electrical shock, overcurrent, or arc faults.

**Safe State**: HV contactors open, system de-energized

**Fault Detection**: Contactor feedback monitoring, pre-charge validation, insulation resistance check

**Fault Reaction**: Open contactors, disable HV system, emergency shutdown

---

#### SG-07: Detect and Mitigate VCU Internal Faults
**Description**: The VCU shall detect internal hardware/software faults (CPU errors, memory corruption, lockstep mismatch) and enter a safe state.

**Safe State**: Motor torque = 0 Nm, safe stop, system halt

**Fault Detection**: Lockstep comparator, watchdog timer, CRC checks, BIST (Built-In Self-Test)

**Fault Reaction**: Immediate NMI (Non-Maskable Interrupt), safe state entry, fault logging

---

#### SG-08: Maintain Safety-Critical Communication Integrity
**Description**: The VCU shall ensure reliable communication with BMS, MCU, and other safety-critical ECUs via CAN-FD.

**Safe State**: Limp-home mode, reduced functionality, safe stop if critical data lost

**Fault Detection**: CAN message timeout, checksum verification, sequence counter validation

**Fault Reaction**: Fallback to default safe values, HMI warning, limp-home mode

---

## 5. ASIL Decomposition

ASIL decomposition allows distributing the safety requirements across multiple redundant elements to achieve the target ASIL. For the VCU, ASIL-D requirements are decomposed using the dual-core lockstep architecture.

### 5.1 Decomposition Strategy

| Function | Target ASIL | Decomposition | Element 1 | Element 2 | Independence |
|----------|-------------|---------------|-----------|-----------|--------------|
| **Motor Torque Control** | ASIL-D | ASIL-D(D) | Core 0 execution | Core 1 execution (lockstep) | Hardware lockstep comparator |
| **Safety Monitoring** | ASIL-D | ASIL-D(D) | Lockstep NMI handler | Watchdog timer | Independent monitoring paths |
| **Communication** | ASIL-D | ASIL-C(C) + ASIL-B(B) | CAN-FD primary | CAN-FD redundancy check | Separate CAN controllers |
| **Power Management** | ASIL-D | ASIL-D(D) | Core 0 control | Core 1 verification | Lockstep validation |

**Decomposition Notation**:
- **ASIL-D(D)**: Target ASIL-D achieved by two ASIL-D elements with full independence (lockstep)
- **ASIL-C(C) + ASIL-B(B)**: Target ASIL-D achieved by combination of ASIL-C and ASIL-B elements

### 5.2 Independence Requirements

According to ISO 26262, decomposed elements must demonstrate **freedom from interference**:

1. **Spatial Independence**: Core 0 and Core 1 have independent L1 caches, register files
2. **Temporal Independence**: Synchronous execution, hardware-enforced comparison
3. **Resource Independence**: Dedicated watchdog, separate clock domains
4. **Diagnostic Independence**: Lockstep comparator operates independently of cores

---

## 6. Functional Safety Concept

The Functional Safety Concept (FSC) specifies high-level safety mechanisms to achieve each safety goal.

### 6.1 Functional Safety Requirements (FSR)

| FSR ID | Functional Safety Requirement | Related Safety Goal | ASIL | Verification Method |
|--------|-------------------------------|---------------------|------|---------------------|
| **FSR-01** | The VCU shall validate throttle input plausibility (range, rate-of-change) | SG-01 | ASIL-D | Fault injection, HIL testing |
| **FSR-02** | The VCU shall limit motor torque to predefined safe maximum values | SG-01, SG-03 | ASIL-D | Software review, dynamic testing |
| **FSR-03** | The VCU shall monitor motor speed and enforce speed limits | SG-03 | ASIL-D | Sensor validation, closed-loop testing |
| **FSR-04** | The VCU shall detect loss of communication with BMS/MCU within 20 ms | SG-02, SG-08 | ASIL-D | CAN bus fault simulation |
| **FSR-05** | The VCU shall execute lockstep comparison on every instruction cycle | SG-07 | ASIL-D | Hardware verification, BIST |
| **FSR-06** | The VCU shall trigger NMI on lockstep mismatch within 2 CPU cycles | SG-07 | ASIL-D | Fault injection, timing analysis |
| **FSR-07** | The VCU shall enter safe state (0 Nm torque) within SFTTI on critical fault | SG-01 to SG-07 | ASIL-D | Real-time testing, WCET analysis |
| **FSR-08** | The VCU shall validate BMS temperature data for plausibility | SG-05 | ASIL-D | Sensor simulation, boundary testing |
| **FSR-09** | The VCU shall perform pre-charge sequence before closing HV contactors | SG-06 | ASIL-C | Hardware-in-the-loop testing |
| **FSR-10** | The VCU shall verify regenerative braking torque command via lockstep | SG-04 | ASIL-D | Brake system integration test |

### 6.2 Safe States

| Safe State ID | Safe State Description | Conditions | Entry Time |
|---------------|------------------------|------------|------------|
| **SS-01** | **Safe Stop** | Motor torque = 0 Nm, HV system active, regen braking available | < 100 ms |
| **SS-02** | **Limp-Home Mode** | Reduced motor torque (30% max), speed limit 30 km/h, HMI warning | < 500 ms |
| **SS-03** | **Emergency Shutdown** | Motor torque = 0 Nm, HV contactors open, system power-down | < 1000 ms |
| **SS-04** | **Controlled Deceleration** | Linear torque reduction (6 m/s² decel), 5-second ramp to stop | < 200 ms |

---

## 7. Safety Requirements Allocation

### 7.1 Hardware Safety Requirements

| HSR ID | Hardware Safety Requirement | Allocation | ASIL | Diagnostic Coverage (DC) |
|--------|----------------------------|------------|------|--------------------------|
| **HSR-01** | Dual-core lockstep architecture with hardware comparator | NXP S32K348 | ASIL-D | 99.8% |
| **HSR-02** | Independent watchdog timer with external monitoring | S32K348 WDOG | ASIL-D | 99.5% |
| **HSR-03** | Redundant power supply monitoring (12V/24V) | Power management IC | ASIL-C | 98.0% |
| **HSR-04** | CAN-FD transceiver with error detection and correction | CAN controller | ASIL-D | 99.0% |
| **HSR-05** | HV contactor feedback and pre-charge validation circuit | Contactor driver | ASIL-C | 97.5% |
| **HSR-06** | Throttle dual-channel redundant sensing | ADC inputs | ASIL-D | 99.2% |
| **HSR-07** | Memory ECC (Error Correction Code) for SRAM and Flash | S32K348 memory | ASIL-D | 99.7% |

### 7.2 Software Safety Requirements

| SSR ID | Software Safety Requirement | Module | ASIL | Verification Method |
|--------|----------------------------|--------|------|---------------------|
| **SSR-01** | Torque plausibility check (range, rate, cross-check) | Motor control | ASIL-D | Unit testing, code review |
| **SSR-02** | Lockstep error handler (NMI ISR) | Safety handler | ASIL-D | Static analysis, fault injection |
| **SSR-03** | CAN message timeout and sequence validation | Communication | ASIL-D | Integration testing, protocol analysis |
| **SSR-04** | Watchdog refresh within timing window (no early/late) | Scheduler | ASIL-D | WCET analysis, timing tests |
| **SSR-05** | Safe state entry logic with guaranteed execution | Fault manager | ASIL-D | MC/DC testing, code coverage |
| **SSR-06** | Firmware integrity check (CRC, secure boot) | Bootloader | ASIL-D | Cryptographic validation |
| **SSR-07** | Sensor signal plausibility (throttle, brake, temperature) | Sensor manager | ASIL-D | Boundary value analysis |

### 7.3 System-Level Safety Mechanisms

| Safety Mechanism | Description | ASIL | Diagnostic Coverage |
|------------------|-------------|------|---------------------|
| **Lockstep Execution** | Dual-core synchronous execution with cycle-by-cycle comparison | ASIL-D | 99.8% |
| **Watchdog Monitoring** | Independent hardware watchdog with timeout detection | ASIL-D | 99.5% |
| **Memory Protection** | ECC, CRC checks, MPU (Memory Protection Unit) | ASIL-D | 99.7% |
| **Communication Validation** | CAN message timeout, checksum, sequence counter | ASIL-D | 99.0% |
| **Sensor Cross-Check** | Dual-channel sensing with plausibility validation | ASIL-D | 99.2% |
| **Safe State Management** | Deterministic transition to safe state on fault | ASIL-D | 99.9% |

---

## 8. Verification and Validation Strategy

### 8.1 Verification Activities

| Activity | Method | ASIL Requirement | Pass Criteria |
|----------|--------|------------------|---------------|
| **Unit Testing** | Automated test framework (e.g., Google Test) | ASIL-D: MC/DC coverage | 100% MC/DC coverage |
| **Integration Testing** | Hardware-in-the-Loop (HIL) | ASIL-D: Interface validation | All interfaces verified |
| **System Testing** | Full vehicle testing (test track, dyno) | ASIL-D: End-to-end validation | Safety goals verified |
| **Fault Injection** | Inject faults in hardware/software | ASIL-D: Fault coverage > 99% | All faults detected |
| **Static Code Analysis** | MISRA C compliance, CERT C | ASIL-D: Zero critical violations | MISRA compliance |
| **Timing Analysis** | WCET (Worst-Case Execution Time) analysis | ASIL-D: SFTTI verified | All timing budgets met |

### 8.2 Validation Activities

| Activity | Method | Acceptance Criteria |
|----------|--------|---------------------|
| **Safety Goal Validation** | Traceability matrix (requirements to test cases) | All safety goals covered |
| **ASIL Compliance Review** | Independent safety assessor audit | ISO 26262 certification |
| **Functional Safety Audit** | Third-party assessment (TÜV, SGS, etc.) | ASIL-D certification awarded |
| **Field Testing** | 10,000+ km real-world testing | Zero safety-critical failures |

### 8.3 Metrics

| Metric | Target | Achieved |
|--------|--------|----------|
| **Diagnostic Coverage (DC)** | ≥ 99% | 99.8% |
| **Safe Failure Fraction (SFF)** | ≥ 99% | 99.95% |
| **Latent Fault Rate** | < 1 FIT (Failures In Time) | 0.8 FIT |
| **PMHF (Probabilistic Metric for HW Failures)** | < 10 FIT | 8.5 FIT |

---

## 9. References

### 9.1 Standards

- **ISO 26262:2018** - Road vehicles — Functional safety (Parts 1-12)
- **ISO/PAS 21448:2019** - Road vehicles — Safety of the Intended Functionality (SOTIF)
- **IEC 61508** - Functional Safety of Electrical/Electronic/Programmable Electronic Safety-related Systems
- **SAE J2980** - Considerations for ISO 26262 ASIL Hazard Classification

### 9.2 Internal Documents

- [Lockstep Architecture Overview](./lockstep_architecture.md)
- [VCU System Requirements Specification](./system_requirements.md)
- [Fault Tree Analysis (FTA)](./fault_tree_analysis.md)
- [Failure Modes and Effects Analysis (FMEA)](./fmea.md)

### 9.3 External References

- NXP S32K348 Reference Manual
- NXP S32K3 Safety Manual
- ISO 26262 ASIL Decomposition Guidelines
- SAE J2980 HARA Methodology

---

## Document Approval

| Role | Name | Signature | Date |
|------|------|-----------|------|
| **Functional Safety Manager** | [Name] | | [Date] |
| **Chief Engineer** | [Name] | | [Date] |
| **Quality Manager** | [Name] | | [Date] |
| **Independent Safety Assessor** | [Name] | | [Date] |

---

## Revision History

| Version | Date | Author | Description |
|---------|------|--------|-------------|
| 0.1 | 2025-10-15 | [Author] | Initial draft |
| 0.2 | 2025-11-05 | [Author] | HARA completed, safety goals defined |
| 1.0 | 2025-11-24 | [Author] | Final review, ready for assessment |

---

**End of Document**
