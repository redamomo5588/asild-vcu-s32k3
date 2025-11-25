# Project Overview: ASIL-D Electric Motorcycle VCU

**Document ID:** PRJ-OVW-001  
**Version:** 1.0  
**Date:** 2025-11-24  
**Author:** Development Team  
**Status:** Active

---

## 1. Introduction

This document provides a comprehensive overview of the ASIL-D Vehicle Control Unit (VCU) development project for electric motorcycles. The project aims to deliver a production-ready, safety-certified electronic control unit capable of managing all critical vehicle functions with the highest level of functional safety compliance (ASIL-D per ISO 26262).

---

## 2. Project Goals and Objectives

### 2.1 Primary Goals
- Develop a production-grade VCU for electric motorcycles meeting ASIL-D requirements
- Implement robust motor control, battery management coordination, and vehicle dynamics control
- Achieve comprehensive functional safety coverage through lockstep processing and safety mechanisms
- Deliver a maintainable, scalable, and certifiable system architecture

### 2.2 Technical Objectives
- **Safety:** ASIL-D compliance for all critical vehicle control functions
- **Reliability:** 99.99% uptime during normal operating conditions
- **Performance:** Real-time response times <10ms for critical control loops
- **Security:** Encrypted communication channels, secure boot, and HSE integration
- **Maintainability:** Modular architecture enabling future feature additions
- **Testability:** >95% code coverage with comprehensive unit and integration tests

### 2.3 Business Objectives
- Create a reusable VCU platform for multiple electric motorcycle variants
- Establish in-house OEM capability for safety-critical embedded systems
- Build foundation for future ADAS and connected vehicle features

---

## 3. Project Scope

### 3.1 In Scope
- **Hardware Platform:** NXP S32K348 microcontroller (dual Cortex-M7 cores in lockstep)
- **Core Functions:**
  - Motor control coordination (torque, regeneration, thermal management)
  - Battery management system (BMS) coordination (3 BMS units)
  - Vehicle state management and mode control
  - Diagnostic services (UDS, OBD-II)
  - Safety monitoring and fault management
  - Communication gateway (CAN-FD, Ethernet, LIN)
- **Safety Features:**
  - Lockstep core monitoring
  - Watchdog supervision
  - Memory protection (MPU/MMU)
  - End-to-end communication protection
  - Plausibility checks and redundancy
- **Development Artifacts:**
  - Complete ISO 26262 documentation suite
  - Unit, integration, and HIL test environments
  - Safety manual and validation reports

### 3.2 Out of Scope
- HMI application development (handled by separate i.MX6 unit)
- Motor controller firmware (interfaced via CAN-FD)
- BMS firmware (interfaced via CAN-FD)
- Infotainment features
- Cloud connectivity (future phase)

---

## 4. System Overview

### 4.1 High-Level Architecture
The VCU serves as the central brain of the electric motorcycle, coordinating all powertrain and safety functions:

- **Processing Unit:** NXP S32K348 with dual Cortex-M7 cores (264 MHz) in lockstep mode
- **Communication Networks:**
  - **CAN-FD (6 channels):** Motor, BMS1, BMS2, BMS3, OBD-II, Expansion
  - **Ethernet (100 Mbps):** Primary VCU ↔ HMI communication (AES-GCM encrypted)
  - **LIN (ASIL-B):** Redundant VCU ↔ HMI for critical alerts
- **Safety Concept:** All safety-critical logic resides in the VCU; HMI is non-safety

### 4.2 Target Vehicle Platform
- **Vehicle Type:** Electric motorcycle (single-track, two-wheel vehicle)
- **Powertrain:** Electric motor with battery pack (multiple cells managed by 3 BMS units)
- **Operating Conditions:**
  - Temperature: -40°C to +85°C (automotive grade)
  - Voltage: 12V nominal (automotive electrical system)
  - EMC compliance per ISO 7637, ISO 11452

---

## 5. Stakeholders

### 5.1 Development Team
- **Project Lead / Chief Engineer:** Responsible for overall system architecture and safety concept
- **Software Engineer:** Responsible for baremetal firmware, MCAL, application logic, testing

### 5.2 Internal Stakeholders
- **Vehicle Integration Team:** Responsible for VCU integration into complete motorcycle
- **Manufacturing:** Responsible for production, flashing, and calibration
- **Quality Assurance:** Responsible for validation and certification support

### 5.3 External Stakeholders
- **Certification Body:** ISO 26262 auditors (as needed)
- **Component Suppliers:** NXP (microcontroller), motor controller OEM, BMS OEM

---

## 6. Success Criteria

The project will be considered successful when:

1. **Functional Completeness:** All defined vehicle control functions implemented and verified
2. **Safety Compliance:** Full ISO 26262 ASIL-D documentation and validation completed
3. **Performance Targets Met:** Real-time constraints and resource utilization within limits
4. **Test Coverage Achieved:** >95% code coverage with passing test cases
5. **Hardware Integration:** Successful HIL and vehicle-level testing completed
6. **Production Readiness:** Manufacturing process defined, calibration tools operational

---

## 7. Constraints and Assumptions

### 7.1 Technical Constraints
- Single microcontroller platform (S32K348) for cost and complexity management
- No external safety coprocessor (lockstep provides internal redundancy)
- Limited flash (6 MB) and RAM (512 KB) resources
- Real-time operating constraints (no RTOS overhead; baremetal)

### 7.2 Resource Constraints
- Solo development effort (two-person team: engineering lead + AI assistant)
- 8-week timeline for first functional release
- No external funding dependencies

### 7.3 Assumptions
- Motor controller and BMS units provide their own ASIL-rated safety functions
- HMI unit (i.MX6) is considered non-safety and does not require ASIL rating
- CAN-FD communication partners implement their own message validation
- Calibration data will be provided by vehicle testing team

---

## 8. Risk Overview

### 8.1 Technical Risks
| Risk | Impact | Mitigation |
|------|--------|-----------|
| Lockstep configuration complexity | High | Early prototyping, reference designs from NXP |
| Real-time performance bottlenecks | Medium | Profiling tools, optimized algorithms |
| HSE (Hardware Security Engine) integration | Medium | Incremental implementation, NXP support resources |
| Multi-CAN coordination timing | Medium | Time-triggered scheduling, priority management |

### 8.2 Schedule Risks
| Risk | Impact | Mitigation |
|------|--------|-----------|
| 8-week timeline aggressive | High | MVP approach, phased feature rollout |
| Hardware availability delays | Medium | Early procurement, backup development boards |

### 8.3 Certification Risks
| Risk | Impact | Mitigation |
|------|--------|-----------|
| ISO 26262 documentation gaps | High | Systematic documentation process, templates |
| Test coverage shortfalls | Medium | Automated testing, continuous integration |

---

## 9. Project Organization

### 9.1 Development Approach
- **Methodology:** Agile with safety compliance checkpoints
- **Version Control:** Git (GitHub repository: redamomo5588/asild-vcu-s32k3)
- **Build System:** CMake + Makefile
- **Documentation:** Markdown (lightweight, version-controlled)

### 9.2 Communication
- **Daily:** Development progress tracking
- **Weekly:** Milestone reviews and risk assessment
- **As-Needed:** Technical design reviews, safety analysis sessions

### 9.3 Key Deliverables
1. Complete source code (baremetal firmware)
2. ISO 26262 documentation suite (requirements, architecture, safety case)
3. Test environment (unit, integration, HIL)
4. Calibration tools and workflows
5. Production release package (binaries, flashing tools, manuals)

---

## 10. References

- ISO 26262:2018 - Road Vehicles - Functional Safety
- NXP S32K3xx Reference Manual
- Project Roadmap (roadmap.md)
- ASIL Classification (asil_classification.md)
- Lockstep Architecture Overview (lockstep_architecture_overview.md)

---

**Document History:**

| Version | Date | Author | Changes |
|---------|------|--------|---------|
| 1.0 | 2025-11-24 | Development Team | Initial release |
