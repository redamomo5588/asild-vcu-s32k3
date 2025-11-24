# ASIL-D Vehicle Control Unit - NXP S32K3xx Platform

<div align="center">

![Build Status](https://img.shields.io/badge/build-passing-brightgreen)
![Safety](https://img.shields.io/badge/ISO_26262-ASIL--D-critical)
![Security](https://img.shields.io/badge/EVITA-Full-blue)
![License](https://img.shields.io/badge/license-Proprietary-red)

**Production-grade automotive VCU firmware with ASIL-D functional safety compliance**

[Features](#key-features) • [Getting Started](#getting-started) • [Documentation](#documentation) • [Support](#support)

</div>

---

## 🎯 Project Overview

Production-grade **Vehicle Control Unit (VCU)** firmware designed for automotive applications requiring **ASIL-D functional safety compliance**. Built on the NXP S32K3xx microcontroller family (S32K358/S32K356/S32K348/S32K338/S32K328), this platform provides a robust, secure, and safety-certified foundation for advanced vehicle control systems.

### 🚗 Target Applications

<table>
<tr>
<td width="50%">

- ⚡ **Electric Vehicle (EV)** Powertrain Control
- 🔋 **Hybrid Electric Vehicle (HEV)** Management
- 🛡️ **Advanced Driver Assistance Systems (ADAS)**

</td>
<td width="50%">

- 🎮 **Vehicle Motion Control**
- 🔌 **Battery Management Systems (BMS)**
- 🔧 **Motor Control Units (MCU)**

</td>
</tr>
</table>

---

## ✨ Key Features

### 🖥️ Core Architecture

| Component | Specification |
|-----------|---------------|
| **Processor** | ARM Cortex-M7 @ 320 MHz |
| **Flash Memory** | Up to 12 MB with ECC |
| **SRAM** | Up to 2.25 MB with ECC |
| **Cache** | 16 KB I-Cache + 16 KB D-Cache |
| **TCM** | 96 KB Tightly Coupled Memory |
| **FPU** | IEEE-754 compliant Single Precision |

### 🛡️ Safety (ASIL-D)

✓ Lockstep core operation with cycle-by-cycle comparison<br>
✓ ECC on all memories (Flash, SRAM, TCM)<br>
✓ FCCU (Fault Collection and Control Unit)<br>
✓ Voltage monitors and clock monitors<br>
✓ XRDC (Extended Resource Domain Controller)<br>
✓ Memory protection units (MPU)<br>
✓ Self-test capabilities (STCU2, LBIST, MBIST)<br>

text

### 🔒 Security (EVITA Full)

- **Hardware Security Engine (HSE_B)**
  - AES-128/192/256
  - RSA up to 4096-bit
  - ECC up to 521-bit
- **Cryptographic Services**
  - SHA-2/SHA-3 hashing
  - True Random Number Generator (TRNG)
- **Secure Operations**
  - Secure boot with authenticated firmware
  - Key management and hardware-backed storage
  - Encrypted communication channels

### 🔌 Communication Interfaces

<table>
<tr>
<th>Interface</th>
<th>Features</th>
</tr>
<tr>
<td><strong>CAN</strong></td>
<td>FlexCAN with CAN-FD support (ISO 11898-1)</td>
</tr>
<tr>
<td><strong>Ethernet</strong></td>
<td>GMAC (Gigabit) with AVB/TSN support</td>
</tr>
<tr>
<td><strong>LIN</strong></td>
<td>Via LPUART with LIN stack</td>
</tr>
<tr>
<td><strong>SPI</strong></td>
<td>LPSPI with DMA support</td>
</tr>
<tr>
<td><strong>I²C</strong></td>
<td>LPI2C for sensor communication</td>
</tr>
<tr>
<td><strong>UART</strong></td>
<td>LPUART for debug/diagnostics</td>
</tr>
</table>

### ⚙️ Analog & Motor Control

- **12-bit SAR ADC** with up to 72 channels
- **eMIOS**: 72 channels for PWM/IC/OC
- **LCU** (Logic Control Unit) for hardware trigger management
- **BCTU** (Body Cross-triggering Unit) for synchronized ADC sampling
- **LPCMP**: Low-power comparators

---

## 📁 Project Structure

asild-vcu-s32k3/

├── 📂 calibration/ # Calibration data and parameters<br>
├── 📂 ci/ # Continuous Integration scripts<br>
├── 📂 config/ # Hardware and software configuration<br>
├── 📂 doc/ # Technical documentation (internal)<br>
├── 📂 docs/ # User-facing documentation<br>
│ ├── 📂 api_reference/ # API documentation<br>
│ ├── 📂 certification/ # Safety/security certifications<br>
│ ├── 📄 calibration_workflows.md<br>
│ ├── 📄 diagnostic_workflows.md<br>
│ ├── 📄 ethernet_tsn_manual.md<br>
│ ├── 📄 hse_manual.md<br>
│ ├── 📄 lockstep_manual.md<br>
│ ├── 📄 operator_guide.md<br>
│ ├── 📄 safety_manual.md<br>
│ ├── 📄 troubleshooting.md<br>
│ └── 📄 user_manual.md<br>
├── 📂 platform/ # Platform-specific code<br>
├── 📂 scripts/ # Build and utility scripts<br>
├── 📂 security/ # Security keys and certificates<br>
├── 📂 simulation/ # Software-in-the-Loop (SIL) models<br>
├── 📂 src/ # Source code<br>
├── 📂 test/ # Unit, integration, and HIL tests<br>
└── 📂 tools/ # Development tools<br>

text

---

## 🚀 Getting Started

### Prerequisites

| Tool | Version | Description |
|------|---------|-------------|
| **IDE** | S32 Design Studio for Arm v3.5+ | Development environment |
| **Compiler** | GCC ARM Embedded | Included in S32DS |
| **Debugger** | PE Micro / Lauterbach Trace32 | Hardware debugging |
| **Hardware** | S32K3xx EVB or custom board | Target platform |

**Additional Tools:**
- S32 Configuration Tools (Clock, Pin, Peripheral)
- Model-Based Design Toolbox (optional for Simulink)
- FreeMaster for runtime calibration

### Quick Start

Clone the repository
git clone https://github.com/redamomo5588/asild-vcu-s32k3.git
cd asild-vcu-s32k3

Build the project
make all

Flash to target
make flash

Run tests
make test

text

---

## 🛡️ Safety Compliance

This project is designed to meet **ISO 26262 ASIL-D** requirements:

| Requirement | Implementation |
|-------------|----------------|
| **Systematic Fault Handling** | All critical functions include safety mechanisms |
| **Random Hardware Fault Detection** | Diagnostic coverage >99% |
| **Software Safety Requirements** | MISRA C:2012 compliant |
| **Traceability** | Full requirements traceability matrix |
| **Documentation** | Safety Manual, Technical Safety Concept, Safety Plan |

> 📖 For detailed safety information, see [Safety Manual](docs/safety_manual.md)

---

## 🔐 Security Features

**EVITA Full** security architecture implementation:

- ✅ Secure boot with authenticated firmware
- ✅ Encrypted communication channels
- ✅ Hardware-backed key storage
- ✅ Anti-tampering mechanisms
- ✅ Secure firmware update (FOTA)

> 📖 See [HSE Manual](docs/hse_manual.md) for detailed security implementation

---

## 📚 Documentation

| Document | Description |
|----------|-------------|
| 📘 [User Manual](docs/user_manual.md) | Complete user guide for system operation |
| 🛡️ [Safety Manual](docs/safety_manual.md) | Safety requirements and implementation |
| 🔄 [Lockstep Manual](docs/lockstep_manual.md) | Core lockstep configuration and diagnostics |
| 🔒 [HSE Manual](docs/hse_manual.md) | Hardware Security Engine usage |
| 🌐 [Ethernet TSN Manual](docs/ethernet_tsn_manual.md) | Time-Sensitive Networking configuration |
| ⚙️ [Calibration Workflows](docs/calibration_workflows.md) | Calibration procedures |
| 🔧 [Diagnostic Workflows](docs/diagnostic_workflows.md) | Diagnostic and debugging procedures |
| ❓ [Troubleshooting](docs/troubleshooting.md) | Common issues and solutions |
| 👤 [Operator Guide](docs/operator_guide.md) | End-user operation guide |

---

## 🔄 Development Workflow

### Branch Strategy

- `main` → Production-ready code
- `develop` → Integration branch
- `feature/*` → Feature development
- `hotfix/*` → Critical fixes

### Testing Strategy

1. **Unit Tests** → Component-level verification
2. **Integration Tests** → Module interaction verification
3. **HIL Tests** → Hardware-in-the-loop validation
4. **SIL Tests** → Software-in-the-loop simulation

### Code Quality Standards

| Standard | Requirement |
|----------|-------------|
| **MISRA C:2012** | Mandatory rules compliance |
| **CERT C** | Coding standard adherence |
| **Static Analysis** | Polyspace, CodeSonar |
| **Code Coverage** | >95% (MC/DC for ASIL-D functions) |

---

## 🤝 Contributing

Please read [CONTRIBUTORS.md](CONTRIBUTORS.md) for contribution guidelines.

---

## 📄 License

See [LICENSE](LICENSE) file for licensing information.

---

## 💬 Support

### Get Help

- 🐛 **Issue Tracker**: [GitHub Issues](https://github.com/redamomo5588/asild-vcu-s32k3/issues)
- 📖 **Documentation**: [docs/](docs/)
- 📧 **Contact**: support@yourcompany.com

---

## 📋 Version History

See [CHANGELOG.md](CHANGELOG.md) for release notes and version history.

---

## 🔗 References

- 📘 [S32K3xx Reference Manual](https://mm.digikey.com/Volume0/opasdata/d220001/medias/docus/7161/S32K3xx-Manual.pdf)
- 🛡️ [ISO 26262 Functional Safety Standard](https://www.iso.org/standard/68383.html)
- 🚗 [AUTOSAR Adaptive Platform](https://www.autosar.org/)
- 📡 [CAN-FD Specification](https://www.can-cia.org/can-knowledge/can/can-fd/)

---

<div align="center">

**⚠️ IMPORTANT NOTICE ⚠️**

*This is production-grade firmware intended for automotive safety applications.*  
*All modifications must follow the established safety and development processes.*

---

**Made with ❤️ for Automotive Safety**

</div>
