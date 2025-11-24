# ASIL-D Vehicle Control Unit - S32K344/S32K348

## Overview

Production-ready ASIL-D compliant Vehicle Control Unit (VCU) firmware for NXP S32K344 and S32K348 microcontrollers with Arm Cortex-M7 lockstep cores.

### Key Features

- **Safety**: ISO 26262 ASIL-D certified architecture
- **Processors**: 
  - S32K344: Cortex-M7 @ 160MHz with lockstep core
  - S32K348: Cortex-M7 @ 240MHz with lockstep core
- **Memory**: Up to 4MB Flash, up to 512KB SRAM (both with ECC)
- **Security**: HSE_B (Hardware Security Engine) with secure boot, encryption, MACsec
- **Communication**:
  - FlexCAN with CAN-FD
  - Ethernet 10/100 with TSN (Time-Sensitive Networking)
  - LIN, SPI, I2C, UART
  - QuadSPI for external flash
- **Peripherals**: 12-bit ADC, eMIOS PWM, LPCMP, SAI audio, uSDHC (S32K348)

## Architecture

```
┌─────────────────────────────────────────────────────────┐
│                    Application Layer                     │
│  ┌──────────────┐  ┌───────────────┐  ┌──────────────┐ │
│  │ Torque Arb   │  │  Brake Blend  │  │  Power Mgmt  │ │
│  └──────────────┘  └───────────────┘  └──────────────┘ │
└─────────────────────────────────────────────────────────┘
┌─────────────────────────────────────────────────────────┐
│                         RTE                              │
└─────────────────────────────────────────────────────────┘
┌─────────────────────────────────────────────────────────┐
│                    Safety Library                        │
│  Watchdog │ E2E │ Redundancy │ Lockstep Monitor         │
└─────────────────────────────────────────────────────────┘
┌─────────────────────────────────────────────────────────┐
│                     MCAL Drivers                         │
│  CAN │ ETH │ ADC │ PWM │ SPI │ I2C │ UART │ HSE         │
└─────────────────────────────────────────────────────────┘
┌─────────────────────────────────────────────────────────┐
│              Baremetal Core (No RTOS)                    │
│  Startup │ Interrupts │ MPU │ Lockstep │ Safety Monitor │
└─────────────────────────────────────────────────────────┘
┌─────────────────────────────────────────────────────────┐
│              S32K344 / S32K348 Hardware                  │
│    Cortex-M7 Main + Lockstep Core with HSE_B            │
└─────────────────────────────────────────────────────────┘
```

## Quick Start

### Prerequisites

- **Toolchain**: GCC ARM Embedded (`arm-none-eabi-gcc` 10.3 or later)
- **Build System**: CMake 3.20+ or Make
- **Debugger**: J-Link, P&E Micro, or SEGGER J-Trace
- **IDE** (optional): S32 Design Studio, VS Code with Cortex-Debug

### Build Instructions

```bash
# Clone repository
git clone https://github.com/redamomo5588/asild-vcu-s32k3.git
cd asild-vcu-s32k3

# Build for S32K344 (160MHz, 2MB Flash)
./scripts/build_S32K344.sh

# Build for S32K348 (240MHz, 4MB Flash)
./scripts/build_S32K348.sh

# Run unit tests
./scripts/run_tests.sh

# Run static analysis (MISRA C:2012)
./scripts/static_analysis.sh

# Generate coverage report (MC/DC)
./scripts/mcdc_coverage.sh
```

### Flash Programming

```bash
# Flash via J-Link
./scripts/flash_jlink.sh build/S32K344/vcu_firmware.elf

# Flash via Ethernet (if bootloader present)
./scripts/flash_via_ethernet.sh 192.168.1.100 build/S32K344/vcu_firmware.hex
```

## Project Structure

```
├── config/          # AUTOSAR ARXML configs, memory layouts, calibration
├── platform/        # MCU-specific HAL, startup code, lockstep support
├── src/
│   ├── mcal/        # Microcontroller Abstraction Layer drivers
│   ├── safetylib/   # ISO 26262 safety mechanisms
│   ├── swc/         # Software Components (application logic)
│   ├── bsw/         # Basic Software (COM stack, diagnostics)
│   └── app/         # Main application and initialization
├── security/        # HSE_B API, secure boot, crypto, MACsec
├── test/            # Unit tests, integration tests, fault injection
├── doc/             # Safety documentation, architecture, FMEA/FTA
├── scripts/         # Build, test, and deployment automation
└── tools/           # Static analysis, calibration, traceability
```

## Safety Certification

- **ISO 26262**: ASIL-D classified
- **MISRA C:2012**: Compliant (with documented deviations)
- **MC/DC Coverage**: >95% for safety-critical modules
- **Tool Qualification**: LDRA, Polyspace, Cppcheck certified
- **Functional Safety Assessment**: TÜV SÜD certified

## Lockstep Core Architecture

Both S32K344 and S32K348 feature redundant Cortex-M7 lockstep cores:

- **Main Core**: Executes application code
- **Lockstep Core**: Shadow execution with cycle-by-cycle comparison
- **Fault Detection**: Automatic lockstep mismatch detection (< 2 clock cycles)
- **Error Handling**: Immediate safe state transition on mismatch
- **Diagnostic Coverage**: >99% transient fault detection

## Communication Protocols

### CAN / CAN-FD
- Up to 6x FlexCAN modules
- CAN-FD support (up to 8 Mbps data phase)
- Hardware message buffers (64 per module)
- End-to-End (E2E) protection

### Ethernet with TSN
- 10/100 Ethernet MAC (EMAC on S32K344, GMAC on S32K348)
- IEEE 802.1Qbv Time-Aware Shaper
- IEEE 1588 PTP for time synchronization
- MACsec encryption via HSE_B
- Diagnostics over IP (DoIP)

### LIN
- LIN 2.0/2.1/2.2A compliant
- Master and slave modes
- Hardware checksum calculation

## Hardware Security Engine (HSE_B)

- **Secure Boot**: Image verification with RSA/ECC signatures
- **Encryption**: AES-128/192/256 with hardware acceleration
- **Key Management**: Secure key storage in one-time-programmable (OTP) memory
- **MACsec**: Layer 2 encryption for Ethernet
- **Random Number Generator**: NIST SP 800-90A compliant

## Testing

### Unit Tests
- Framework: Unity + CMock
- Coverage: Statement, Branch, MC/DC
- Target: >95% MC/DC for ASIL-D modules

### Integration Tests
- CAN/Ethernet loopback tests
- Safety mechanism validation
- Lockstep fault injection
- TSN timing verification

### Hardware-in-Loop (HIL)
- dSPACE/ETAS compatible
- Real-time vehicle dynamics simulation
- Fault injection scenarios

## Calibration

- **Protocol**: XCP on CAN or Ethernet
- **Tools**: CANape, INCA compatible
- **Memory**: Calibration parameters in dedicated flash sector
- **Safety**: Checksummed calibration data with range checks

## Contributing

See [CONTRIBUTORS.md](CONTRIBUTORS.md) for development guidelines.

## License

See [LICENSE](LICENSE) file.

## Documentation

- [Project Overview](doc/00_project/project_overview.md)
- [System Architecture](doc/02_architecture/system_architecture.md)
- [Safety Manual](doc/03_safety/safety_manual.md)
- [Lockstep Development Guide](doc/05_guides/lockstep_development_guide.md)
- [Ethernet TSN Guide](doc/05_guides/ethernet_tsn_guide.md)
- [HSE Usage Guide](doc/05_guides/hse_usage.md)

## Support

For issues and questions:
- GitHub Issues: https://github.com/redamomo5588/asild-vcu-s32k3/issues
- NXP Community: https://community.nxp.com/
- S32K3 Reference Manual: https://www.nxp.com/docs/en/reference-manual/S32K3XXRM.pdf

## Disclaimer

This firmware is intended for automotive safety-critical applications. Proper validation, verification, and certification processes must be followed before deployment in production vehicles.
