# User Manual
## ASIL-D Vehicle Control Unit - NXP S32K3xx Platform

**Document Version:** 1.0  
**Release Date:** November 2025  
**Target Audience:** System Integrators, Application Engineers, Calibration Engineers

---

## Table of Contents

1. [Introduction](#1-introduction)
2. [System Overview](#2-system-overview)
3. [Hardware Setup](#3-hardware-setup)
4. [Software Installation](#4-software-installation)
5. [Configuration](#5-configuration)
6. [Operation](#6-operation)
7. [Calibration](#7-calibration)
8. [Diagnostics](#8-diagnostics)
9. [Troubleshooting](#9-troubleshooting)
10. [Appendices](#10-appendices)

---

## 1. Introduction

### 1.1 Purpose
This User Manual provides comprehensive guidance for integrating, configuring, and operating the ASIL-D Vehicle Control Unit based on the NXP S32K3xx microcontroller platform.

### 1.2 Scope
This manual covers:
- Hardware setup and connections
- Software installation and configuration
- Normal operation procedures
- Calibration workflows
- Diagnostic procedures
- Common troubleshooting scenarios

### 1.3 Applicable Documents
| Document | Description |
|----------|-------------|
| [Safety Manual](safety_manual.md) | ISO 26262 ASIL-D compliance details |
| [Lockstep Manual](lockstep_manual.md) | Core lockstep configuration |
| [HSE Manual](hse_manual.md) | Security engine configuration |
| S32K3xx Reference Manual | NXP hardware reference |

---

## 2. System Overview

### 2.1 Architecture
```
┌─────────────────────────────────────────────────────────────┐
│                    Application Layer                         │
│  ┌──────────┐  ┌──────────┐  ┌──────────┐  ┌──────────┐   │
│  │  Motor   │  │ Battery  │  │  Vehicle │  │Diagnostic│   │
│  │ Control  │  │   Mgmt   │  │  Mgmt    │  │ Manager  │   │
│  └──────────┘  └──────────┘  └──────────┘  └──────────┘   │
├─────────────────────────────────────────────────────────────┤
│                 AUTOSAR Runtime Environment                  │
├─────────────────────────────────────────────────────────────┤
│                    MCAL/Driver Layer                         │
│  ┌─────┐ ┌─────┐ ┌─────┐ ┌─────┐ ┌─────┐ ┌─────┐         │
│  │ ADC │ │ PWM │ │ CAN │ │ ETH │ │ SPI │ │Flash│   ...   │
│  └─────┘ └─────┘ └─────┘ └─────┘ └─────┘ └─────┘         │
├─────────────────────────────────────────────────────────────┤
│                   Hardware Abstraction                       │
├─────────────────────────────────────────────────────────────┤
│              S32K3xx Microcontroller (Cortex-M7)            │
└─────────────────────────────────────────────────────────────┘
```

### 2.2 Key Specifications
| Parameter | Value |
|-----------|-------|
| **Core** | ARM Cortex-M7 @ 320 MHz |
| **Program Flash** | Up to 12 MB with ECC |
| **Data Flash** | Up to 256 KB |
| **SRAM** | Up to 2.25 MB with ECC |
| **Operating Voltage** | 3.0V - 5.5V |
| **Temperature Range** | -40°C to +150°C (junction) |
| **Safety Rating** | ISO 26262 ASIL-D |
| **Security** | EVITA Full (HSE_B) |

### 2.3 Supported Variants
| Part Number | Flash | SRAM | Package | CAN | Ethernet |
|-------------|-------|------|---------|-----|----------|
| S32K358 | 8 MB | 1.5 MB | 172-LQFP / 289-BGA | 8 | Gigabit |
| S32K356 | 6 MB | 1.0 MB | 172-LQFP / 289-BGA | 8 | Gigabit |
| S32K348 | 4 MB | 768 KB | 144-LQFP / 176-LQFP | 6 | 10/100 |
| S32K338 | 4 MB | 512 KB | 144-LQFP / 176-LQFP | 6 | 10/100 |
| S32K328 | 2 MB | 384 KB | 100-LQFP / 144-LQFP | 4 | - |

---

## 3. Hardware Setup
### 3.1 Power Supply Requirements
#### 3.1.1 Supply Connections
| Supply Pin | Voltage | Max Current | Description |
|------------|---------|-------------|-------------|
| VDD_HV_A | 5.0V ± 10% | 500 mA | Main analog supply |
| VDD_HV | 5.0V ± 10% | 2.5 A | Main digital supply |
| VDD_LV | 1.2V ± 5% | 1.0 A | Core logic supply |
| VDDA | 5.0V ± 5% | 100 mA | ADC analog supply |
| VSS | 0V (GND) | - | Ground reference |

#### 3.1.2 Power Sequencing
```
Power-Up Sequence:
1. VDD_HV     ──────┐
2. VDD_HV_A         ├─── <10ms ───┐
3. VDD_LV           │              │
4. VDDA             │              │
5. Release RESET ───┘              └─── System Ready
Power-Down Sequence (Reverse order)
```
⚠️ **CAUTION**: Improper power sequencing may damage the device.

### 3.2 Clock Configuration
#### 3.2.1 External Oscillators
| Oscillator | Frequency | Accuracy | Load Cap | Application |
|------------|-----------|----------|----------|-------------|
| FXOSC | 8-40 MHz | ±50 ppm | 10-20 pF | System clock |
| SXOSC | 32.768 kHz | ±100 ppm | 12.5 pF | RTC, Wakeup |

#### 3.2.2 Recommended Crystal
- System Clock (FXOSC): 16 MHz, Fundamental mode, ESR < 100Ω, Load Capacitance 18 pF
- RTC Clock (SXOSC): 32.768 kHz, Tuning fork, ESR < 70kΩ, Load Cap 12.5 pF

### 3.3 Pin Configuration
#### 3.3.1 Mandatory Connections
```
JTAG/Debug (SWD):
  PA0  → JTAG_TMS / SWD_DIO
  PA1  → JTAG_TCK / SWD_CLK
  PA4  → JTAG_TDI (optional for JTAG mode)
  PA5  → JTAG_TDO / SWO (trace output)
Reset:
  RESET_B → External reset (active low, with 10kΩ pull-up)
Boot Mode:
  BOOTCFG[1:0] → Configure boot mode (00 = internal flash)
```
#### 3.3.2 Communication Interfaces
```
CAN0_TX  → PB0
CAN0_RX  → PB1
CAN1_TX  → PB2
CAN1_RX  → PB3
GMAC_TXD[0–3], RXD[0–3], CLK, EN, DV → PE0–PE11
```
### 3.4 External Components
#### 3.4.1 Power Supply Filter
```
VDD_HV ──┬─── 10µF (Ceramic, X7R) ──┬─── VSS
         │                           │
         └─── 100nF (Ceramic) ───────┘
```
#### 3.4.2 Reset Circuit
```
VDD_HV ──┬─── 10kΩ ───┬─── RESET_B
         │             │
         └─── 100nF ───┴─── VSS
```

---

## 4. Software Installation
### 4.1 Development Environment Setup
#### 4.1.1 Required Software
| Software | Version | Download |
|----------|---------|----------|
| S32 Design Studio | v3.5+ | [NXP.com](https://www.nxp.com/design/software/development-software/s32-design-studio-ide) |
| GCC ARM Toolchain | 12.2+ | Included in S32DS |
| S32 Configuration Tools | Latest | Included in S32DS |
| FreeMaster | 3.2+ | [NXP FreeMaster](https://www.nxp.com/freemaster) |

#### 4.1.2 Installation Steps
1. Install S32 Design Studio
2. Install USB Drivers (Windows Only)
3. Verify Installation (`arm-none-eabi-gcc --version`)

### 4.2 Project Import
#### 4.2.1 Clone Repository
```bash
git clone https://github.com/redamomo5588/asild-vcu-s32k3.git
cd asild-vcu-s32k3
git checkout main
```
#### 4.2.2 Import into S32DS
1. Launch S32DS → File → Import → Existing Projects
2. Select repository root
3. Build, debug, and configure as per project docs

### 4.3 Firmware Flashing
- Via JTAG/SWD: Use S32DS debug
- Via CAN: Use bootloader scripts if configured

---

## 5. Configuration
### 5.1 Clock Tree Configuration
- Configure via S32 Config Tools or clock_config.c

### 5.2 Pin Multiplexing
- Use S32 Config Tools Pin wizard and reference IOMUX spreadsheet

### 5.3 Memory Configuration
- Reference linker script, enable ECC, set up MPU if required

---

## 6. Operation
### 6.1 System Startup
- Power on → Boot ROM → Application → Safety monitoring
### 6.2 Operating Modes
- Normal (all enabled), Standby (low-power)
### 6.3 Communication Protocols
- CAN, Ethernet, UART, SPI, LIN supported

---

## 7. Calibration
### 7.1 FreeMaster Connection
- UART or CAN supported
### 7.2 Calibration Parameters
- Adjust in FreeMaster or project config scripts
### 7.3 Calibration Workflow
- See separate calibration_workflows.md for details

---

## 8. Diagnostics
### 8.1 Diagnostic Communication
- UDS over CAN and XCP supported
### 8.2 Fault Codes (DTCs)
- ISO-format DTCs supported, see diagnostics section
### 8.3 Diagnostic Troubleshooting
- Use FreeMaster, serial console, or CAN for debug

---

## 9. Troubleshooting
### 9.1 System Won't Boot
| Symptom | Possible Cause | Solution |
|---------|----------------|----------|
| No LED | Power fault | Check VDD_HV voltage |
| Boot/stuck | Watchdog reset | Check startup init |
### 9.2 Communication Failures
| Symptom | Possible Cause | Solution |
|---------|----------------|----------|
| No CAN | Transceiver fault | Check connections |
| No ETH | PHY or pin config | Check clock, layout |
### 9.3 Performance Issues
| Symptom | Possible Cause | Solution |
|---------|----------------|----------|
| High CPU | Excessive interrupts | Optimize timing |
| Memory | Leaks | Use static analyzer |

---

## 10. Appendices
### A. Pin Assignment Table
See `platform/S32K358_IOMUX.xlsx`
### B. Memory Map
| Region | Start | End | Size | Use |
|--------|-------|-----|------|-----|
| Flash | 0x00400000 | ... | 8 MB | Application |
| Data Flash | 0x10000000 | ... | 256 KB | NVM |
| SRAM | 0x20000000 | ... | 2 MB | RAM |
| TCM | 0x20200000 | ... | 192 KB | Tightly coupled |
### C. Register Summary
See S32K3xx Reference Manual, Section 3
### D. Acronyms and Abbreviations
| Acronym | Definition |
|---------|------------|
| ADC | Analog-to-Digital Converter |
| ASIL | Automotive Safety Integrity Level |
| CAN | Controller Area Network |
| DTC | Diagnostic Trouble Code |
| ECU | Electronic Control Unit |
| FCCU | Fault Collection and Control Unit |
| HSE | Hardware Security Engine |
| ISO | International Organization for Standardization |
| MCAL | Microcontroller Abstraction Layer |
| PWM | Pulse Width Modulation |
| UDS | Unified Diagnostic Services |
| VCU | Vehicle Control Unit |
| XCP | Universal Measurement and Calibration Protocol |

---

**Document Revision History**
| Version | Date | Author | Changes |
|---------|------|--------|---------|
| 1.0 | 2025-11-24 | Engineering Team | Initial release |

---

**For Technical Support:**
- Email: support@yourcompany.com
- Issue Tracker: https://github.com/redamomo5588/asild-vcu-s32k3/issues
