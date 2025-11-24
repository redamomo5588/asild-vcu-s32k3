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

**See the full manual for more details on setup, connections, pinouts, clocking, component choices, and best practices.**

---

## 4. Software Installation

### 4.1 Development Environment Setup

**See the full manual for step-by-step IDE, toolchain, and debugger installation instructions.**

---

## 5. Configuration

### 5.1 Clock Tree Configuration

### 5.2 Pin Multiplexing

### 5.3 Memory Configuration

**See the full manual for recommended clock, pin, and memory settings, example linker scripts, and best practices for safety configuration**

---

## 6. Operation

### 6.1 System Startup

### 6.2 Operating Modes

### 6.3 Communication Protocols

**See the manual for boot sequence, expected operational timing, typical mode transitions, and peripheral initialization examples.**

---

## 7. Calibration

### 7.1 FreeMaster Connection

### 7.2 Calibration Parameters

### 7.3 Calibration Workflow

**Refer to the manual for calibration steps, variables, sample code, and workflow diagrams.**

---

## 8. Diagnostics

### 8.1 Diagnostic Communication

### 8.2 Fault Codes (DTCs)

### 8.3 Diagnostic Trouble Shooting

**See the full manual for UDS, XCP, fault code tables, and troubleshooting tips.**

---

## 9. Troubleshooting

### 9.1 System Won't Boot

### 9.2 Communication Failures

### 9.3 Performance Issues

**Visit troubleshooting section for checklists and debugging commands.**

---

## 10. Appendices

### Appendix A: Pin Assignment Table

### Appendix B: Memory Map

### Appendix C: Register Summary

### Appendix D: Acronyms and Abbreviations

---

**Document Revision History**

| Version | Date | Author | Changes |
|---------|------|--------|---------|
| 1.0 | 2025-11-24 | Engineering Team | Initial release |

---

**For Technical Support:**
- Email: support@yourcompany.com
- Issue Tracker: https://github.com/redamomo5588/asild-vcu-s32k3/issues
