# Project Roadmap

**Document ID:** DOC-00-002  
**Version:** 1.0  
**Date:** 2025-11-24  
**Status:** Draft  
**Author:** VCU Development Team  
**Project:** ASIL-D Electric Motorcycle Vehicle Control Unit (VCU)  
**Target Platform:** NXP S32K348

---

## 1. Document Purpose

This roadmap defines the development timeline, milestones, deliverables, and key activities for the ASIL-D Vehicle Control Unit (VCU) project targeting electric motorcycle applications. The roadmap spans from project inception through the first functional prototype delivery in **8 weeks (Target: January 19, 2026)**.

---

## 2. Project Timeline Overview

**Project Start Date:** November 24, 2025  
**First Functional Prototype Target:** January 19, 2026 (Week 8)  
**Development Model:** Agile/Iterative with ISO 26262 safety considerations  
**Team Size:** Solo development with AI-assisted collaboration

---

## 3. Development Phases

### Phase 0: Foundation & Planning (Weeks 1-2)
**Duration:** November 24 - December 7, 2025  
**Status:** In Progress

#### Objectives:
- Complete project structure and documentation framework
- Define system architecture and safety requirements
- Set up development environment and toolchain
- Establish baseline communication protocols

#### Key Deliverables:
- [ ] Project documentation structure (doc/)
- [ ] System architecture definition
- [ ] ASIL classification and safety goals
- [ ] Development environment setup (S32 Design Studio, GCC toolchain)
- [ ] Version control and CI/CD pipeline
- [ ] Hardware interface definitions

#### Milestones:
- **M0.1:** Documentation framework complete (Week 1)
- **M0.2:** Toolchain verified, first LED blink on S32K348 (Week 2)

---

### Phase 1: Core Platform Bring-Up (Weeks 3-4)
**Duration:** December 8 - December 21, 2025

#### Objectives:
- Implement baremetal HAL and MCAL layers
- Enable lockstep core configuration and monitoring
- Establish boot sequence and system initialization
- Implement basic diagnostic and fault handling

#### Key Deliverables:
- [ ] Clock configuration and PLL setup
- [ ] GPIO, UART, and debug interfaces operational
- [ ] Lockstep core initialization and fault injection test
- [ ] Watchdog and system monitor baseline
- [ ] Flash/EEPROM driver for calibration storage
- [ ] Basic RTE (Runtime Environment) skeleton

#### Milestones:
- **M1.1:** Both cores running in lockstep, fault detection verified (Week 3)
- **M1.2:** Core HAL layer operational, UART debug working (Week 4)

---

### Phase 2: Communication Stack Implementation (Weeks 5-6)
**Duration:** December 22, 2025 - January 4, 2026

#### Objectives:
- Implement CAN-FD driver and protocol stack
- Integrate Ethernet driver with AES-GCM encryption
- Implement LIN driver for backup HMI communication
- Establish message routing and buffering mechanisms

#### Key Deliverables:
- [ ] CAN-FD driver (6 channels: Motor, BMS x3, OBD-II, Expansion)
- [ ] CAN message database and DBC file
- [ ] Ethernet driver with GMAC peripheral (100 Mbps)
- [ ] AES-GCM encryption layer for Ethernet-HMI link
- [ ] LIN driver for backup HMI alerts (ASIL-B path)
- [ ] Message queue and priority-based scheduler
- [ ] Communication diagnostics and error handling

#### Milestones:
- **M2.1:** CAN-FD operational, loopback test passed (Week 5)
- **M2.2:** Ethernet link established with encryption, LIN backup active (Week 6)

---

### Phase 3: Application Layer & Control Logic (Week 7)
**Duration:** January 5 - January 11, 2026

#### Objectives:
- Implement core VCU control functions
- Develop motor control interface and state machine
- Integrate BMS communication and battery management
- Implement safety monitoring and fault management

#### Key Deliverables:
- [ ] VCU state machine (Init, Standby, Ready, Active, Fault, Shutdown)
- [ ] Motor controller interface (torque/speed commands via CAN-FD)
- [ ] BMS interface (SOC, voltage, current, temperature monitoring)
- [ ] Power management and energy optimization logic
- [ ] Fault detection, classification, and response (ASIL-D)
- [ ] Emergency shutdown and safe state transitions
- [ ] HMI data aggregation and telemetry

#### Milestones:
- **M3.1:** VCU state machine operational, motor interface functional (Week 7)

---

### Phase 4: Integration, Testing & Validation (Week 8)
**Duration:** January 12 - January 19, 2026

#### Objectives:
- System integration and end-to-end testing
- Safety mechanism validation (lockstep, watchdog, fault handling)
- Performance optimization and timing analysis
- Documentation completion

#### Key Deliverables:
- [ ] Full system integration test on S32K348 hardware
- [ ] Lockstep fault injection and recovery validation
- [ ] CAN-FD, Ethernet, LIN communication validation
- [ ] Motor control and BMS integration test
- [ ] Safety mechanism verification report
- [ ] Timing analysis and WCET (Worst-Case Execution Time) assessment
- [ ] First functional prototype release package
- [ ] Test report and known issues log

#### Milestones:
- **M4.1:** Full system operational, all communication paths validated (Week 8)
- **M4.2:** First functional prototype delivered (January 19, 2026)

---

## 4. Post-Phase 4 (Weeks 9-16+): Refinement & Production Readiness
**Duration:** January 20, 2026 onwards

### Objectives:
- Refine control algorithms based on prototype testing
- Complete ISO 26262 documentation and traceability
- Implement advanced features (OTA updates, advanced diagnostics)
- Conduct Hardware-in-Loop (HIL) testing
- Prepare for ASIL-D certification audit

### Key Activities:
- Software safety analysis (FMEA, FTA, DFA)
- Code review and MISRA-C compliance verification
- Unit test coverage >95%, MC/DC coverage for safety functions
- Field testing on electric motorcycle prototype
- Calibration tuning and performance optimization
- Production documentation and manufacturing release

---

## 5. Risk Management & Mitigation

| Risk | Impact | Probability | Mitigation Strategy |
|------|--------|-------------|---------------------|
| Hardware availability delay | High | Low | Pre-order S32K348 eval board, use simulator in parallel |
| Lockstep integration complexity | High | Medium | Allocate extra time in Phase 1, leverage NXP reference code |
| Ethernet encryption overhead | Medium | Medium | Profile early, optimize AES-GCM implementation, consider HW acceleration |
| CAN-FD timing issues | Medium | Low | Use timing analysis tools, validate with oscilloscope |
| Scope creep beyond 8 weeks | High | High | Strict phase gate reviews, defer non-critical features |
| Tool/compiler issues | Medium | Low | Maintain fallback toolchain, document all build dependencies |

---

## 6. Dependencies & Prerequisites

### Hardware:
- NXP S32K348 evaluation board (or equivalent)
- CAN-FD transceiver breakout (TJA1153 or similar)
- Ethernet PHY (KSZ8081 or integrated MAC-PHY)
- LIN transceiver (TJA1020 or similar)
- JTAG/SWD debugger (J-Link, P&E Micro, or onboard OpenSDA)

### Software Tools:
- S32 Design Studio for ARM (v3.5 or later)
- ARM GCC toolchain (v10.3 or later)
- Git version control
- Python 3.x (for scripting and automation)
- CANoe/CANalyzer or Kvaser tools (for CAN testing)
- Wireshark (for Ethernet debugging)

### External Components:
- Motor controller with CAN-FD interface
- BMS units (3x) with CAN-FD interface
- HMI unit (i.MX6-based) with Ethernet and LIN

---

## 7. Success Criteria

The first functional prototype is considered successful if:

1. **Core Safety Functions:**
   - Lockstep cores operational with fault detection <10ms response time
   - Watchdog and system monitor functional
   - Safe state entry on critical fault within 100ms

2. **Communication:**
   - All 6 CAN-FD channels transmitting/receiving at 2 Mbps
   - Ethernet-HMI link encrypted and stable (>99% uptime)
   - LIN backup path operational for critical alerts

3. **Control Functions:**
   - VCU state machine transitions correctly
   - Motor torque commands transmitted and acknowledged
   - BMS data received and processed correctly

4. **Performance:**
   - Main control loop executes at 10ms (100 Hz)
   - Communication latency <5ms for critical messages
   - CPU utilization <70% under normal load

5. **Documentation:**
   - All Phase 0-4 documentation complete
   - Test report with results and known issues
   - User manual for prototype operation

---

## 8. Future Roadmap (Post-Week 8)

### Short-Term (Weeks 9-16):
- Advanced motor control strategies (torque vectoring, regen optimization)
- Over-the-Air (OTA) firmware update mechanism
- Enhanced diagnostics and fault logging
- HIL testing and validation

### Medium-Term (Weeks 17-24):
- ISO 26262 ASIL-D certification preparation
- MISRA-C compliance verification
- Field testing on electric motorcycle
- Production-ready firmware release

### Long-Term (6+ months):
- Multi-vehicle platform support (different motorcycle models)
- Advanced features: traction control, ABS integration, connectivity
- Cybersecurity enhancements (intrusion detection, secure boot)
- Continuous improvement based on field data

---

## 9. Change Log

| Version | Date | Author | Description |
|---------|------|--------|-------------|
| 1.0 | 2025-11-24 | VCU Team | Initial roadmap creation |

---

## 10. References

- `project_overview.md` - High-level project description
- `asil_classification.md` - ASIL level assignments and safety goals
- `lockstep_architecture_overview.md` - Lockstep core design details
- `development_interface_agreement.md` - Team interfaces and responsibilities
- ISO 26262:2018 - Road vehicles — Functional safety standard
- NXP S32K3 Reference Manual (RM)
- NXP S32K3 Hardware Design Guide (HDG)
