
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

┌─────────────────────────────────────────────────────────────┐<br>
│ Application Layer │<br>
│ ┌──────────┐ ┌──────────┐ ┌──────────┐ ┌──────────┐ │<br>
│  │ Motor │    │ Battery │ │ Vehicle │  │Diagnostic│ │<br>
│  │ Control │   │ Mgmt │     │ Mgmt │   │ Manager │ │<br>
│ └──────────┘ └──────────┘ └──────────┘ └──────────┘ │<br>
├─────────────────────────────────────────────────────────────┤<br>
│ AUTOSAR Runtime Environment │<br>
├─────────────────────────────────────────────────────────────┤<br>
│ MCAL/Driver Layer │<br>
│ ┌─────┐ ┌─────┐ ┌─────┐ ┌─────┐ ┌─────┐ ┌─────┐ │<br>
│ │ ADC │ │ PWM │ │ CAN │ │ ETH │ │ SPI │ │Flash│ ... │<br>
│ └─────┘ └─────┘ └─────┘ └─────┘ └─────┘ └─────┘ │<br>
├─────────────────────────────────────────────────────────────┤<br>
│ Hardware Abstraction │<br>
├─────────────────────────────────────────────────────────────┤<br>
│ S32K3xx Microcontroller (Cortex-M7) │<br>
└─────────────────────────────────────────────────────────────┘<br>

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

Power-Up Sequence:

VDD_HV ──────┐<br>

VDD_HV_A ├─── <10ms ───┐<br>

VDD_LV │ │<br>

VDDA │ │<br>

Release RESET ───┘ └─── System Ready<br>

Power-Down Sequence (Reverse order)

text

⚠️ **CAUTION**: Improper power sequencing may damage the device.

### 3.2 Clock Configuration

#### 3.2.1 External Oscillators

| Oscillator | Frequency | Accuracy | Load Cap | Application |
|------------|-----------|----------|----------|-------------|
| FXOSC | 8-40 MHz | ±50 ppm | 10-20 pF | System clock |
| SXOSC | 32.768 kHz | ±100 ppm | 12.5 pF | RTC, Wakeup |

#### 3.2.2 Recommended Crystal

**System Clock (FXOSC):**
- Frequency: 16 MHz
- Type: Fundamental mode
- ESR: < 100Ω
- Load Capacitance: 18 pF
- Example Part: Abracon ABM8-16.000MHZ-B2-T

**RTC Clock (SXOSC):**
- Frequency: 32.768 kHz
- Type: Tuning fork
- ESR: < 70kΩ
- Load Capacitance: 12.5 pF
- Example Part: Epson FC-135

### 3.3 Pin Configuration

#### 3.3.1 Mandatory Connections

JTAG/Debug (SWD):
PA0 → JTAG_TMS / SWD_DIO
PA1 → JTAG_TCK / SWD_CLK
PA4 → JTAG_TDI (optional for JTAG mode)
PA5 → JTAG_TDO / SWO (trace output)

Reset:
RESET_B → External reset (active low, with 10kΩ pull-up)

Boot Mode:
BOOTCFG[1:0] → Configure boot mode (00 = internal flash)

text

#### 3.3.2 Communication Interfaces

**CAN-FD (Primary):**
CAN0_TX → PB0 (FlexCAN0 Channel 0)
CAN0_RX → PB1
CAN1_TX → PB2 (FlexCAN1 Channel 0)
CAN1_RX → PB3

text

**Ethernet (GMAC):**
GMAC_TX_CLK → PE0
GMAC_TXD → PE1
GMAC_TXD → PE2​
GMAC_TXD → PE3
GMAC_TXD → PE4
GMAC_TX_EN → PE5
GMAC_RX_CLK → PE6
GMAC_RXD → PE7
GMAC_RXD → PE8​
GMAC_RXD → PE9
GMAC_RXD → PE10
GMAC_RX_DV → PE11

text

### 3.4 External Components

#### 3.4.1 Power Supply Filter

VDD_HV ──┬─── 10µF (Ceramic, X7R) ──┬─── VSS<br>
│ │<br>
└─── 100nF (Ceramic) ───────┘<br>

Place capacitors < 5mm from pin

text

#### 3.4.2 Reset Circuit

VDD_HV ──┬─── 10kΩ ───┬─── RESET_B<br>
│ │<br>
└─── 100nF ───┴─── VSS<br>

Optional: External reset supervisor (e.g., TPS3840)

text

---

## 4. Software Installation

### 4.1 Development Environment Setup

#### 4.1.1 Required Software

| Software | Version | Download Link |
|----------|---------|---------------|
| S32 Design Studio | v3.5+ | [NXP.com](https://www.nxp.com/design/software/development-software/s32-design-studio-ide) |
| GCC ARM Toolchain | 12.2+ | Included in S32DS |
| S32 Configuration Tools | Latest | Included in S32DS |
| FreeMaster | 3.2+ | [NXP FreeMaster](https://www.nxp.com/freemaster) |

#### 4.1.2 Installation Steps

**Step 1: Install S32 Design Studio**
Windows
s32ds_installer.exe

Linux
$ chmod +x s32ds_installer.run
$ ./s32ds_installer.run

text

**Step 2: Install USB Drivers (Windows Only)**
Device Manager → Update Driver → Browse:
C:\NXP\S32DS.3.5\S32DS\build_tools\debugger_drivers\

text

**Step 3: Verify Installation**
Check compiler version
$ arm-none-eabi-gcc --version
arm-none-eabi-gcc (GNU Arm Embedded Toolchain 12.2.Rel1) 12.2.0

Check debugger
$ pemicro_console -v
PEMicro Debug Plugin v4.x.x

text

### 4.2 Project Import

#### 4.2.1 Clone Repository

git clone https://github.com/redamomo5588/asild-vcu-s32k3.git
cd asild-vcu-s32k3
git checkout main

text

#### 4.2.2 Import into S32DS

1. Launch S32 Design Studio
2. **File → Import → General → Existing Projects**
3. Select repository root directory
4. Check all projects:
   - `VCU_Core`
   - `VCU_Platform`
   - `VCU_Application`
5. Click **Finish**

#### 4.2.3 Build Configuration

**Debug Configuration (Development):**
Configuration: Debug
Optimization: -O0
Debug Info: Full (-g3)
Warnings: All (-Wall -Wextra)

**Release Configuration (Production):**
Configuration: Release
Optimization: -O2
Debug Info: Minimal (-g)
Warnings: Error on Warning (-Werror)
Link Time Optimization: Enabled

### 4.3 Firmware Flashing

#### 4.3.1 Via JTAG/SWD (Development)

**Using S32DS:**
Right-click project → Debug As → S32DS Debug Configuration

Select debugger: PE Micro / Segger J-Link / Lauterbach

Click Debug

**Using Command Line:**
PE Micro
pemicro_console -device=S32K358 -load=VCU_Application.elf

Segger J-Link
JLinkExe -device S32K358 -if SWD -speed 4000

loadfile VCU_Application.hex
r
g

#### 4.3.2 Via CAN/Ethernet (Production Update)

**Bootloader Mode Entry:**
// Hold PTA10 low during reset
// Or send CAN message:
// ID: 0x7DF, Data: [0x10, 0x02, 0x10, 0x03, 0x00, 0x00, 0x00, 0x00]

**Update via CAN:**
Using ISO-TP bootloader
./scripts/flash_via_can.sh --port can0 --file VCU_Application.s19

**Update via Ethernet:**
Using TFTP
./scripts/flash_via_eth.sh --ip 192.168.1.100 --file VCU_Application.bin

---

## 5. Configuration

### 5.1 Clock Tree Configuration

#### 5.1.1 Typical Configuration

FXOSC (16 MHz)
↓
PLL0 (320 MHz) ── DIV2 ──→ Core Clock (160 MHz)
↓
PLL1 (200 MHz) ──────────→ Peripheral Clock (100 MHz)
↓
SIRC (32 kHz) ──────────→ RTC / Wakeup

#### 5.1.2 Clock Configuration Tool

**Using S32 Configuration Tools:**
Open S32 Configuration Tools

Tools → Clock Configuration Tool

Load preset: "Default_320MHz_Config.mex"

Modify as needed

Generate Code → Update Project

**Manual Configuration (clock_config.c):**
/* Core clock: 160 MHz (PLL0 / 2) */
CLOCK_DRV_Init(&clockManConfig_0);

/* Enable peripheral clocks */
PCC->PCCn[PCC_FlexCAN0_INDEX] |= PCC_PCCn_CGC_MASK;
PCC->PCCn[PCC_LPSPI0_INDEX] |= PCC_PCCn_CGC_MASK;

### 5.2 Pin Multiplexing

#### 5.2.1 Pin Configuration Tool

**Using S32 Configuration Tools:**
Tools → Pin Configuration Tool

Load IOMUX: S32K358_IOMUX.xlsx

Configure pins in Pins view

Verify conflicts in Routing view

Generate Code

#### 5.2.2 Example: CAN0 Configuration

/* CAN0_TX on PTB0, CAN0_RX on PTB1 /
SIUL2->MSCR[PTB0] = SIUL2_MSCR_SSS(1) | / Alt 1: CAN0_TX /
SIUL2_MSCR_OBE(1) | / Output enabled /
SIUL2_MSCR_SRC(3); / Fast slew rate */

SIUL2->MSCR[PTB1] = SIUL2_MSCR_SSS(0) | /* Input (GPIO) /
SIUL2_MSCR_IBE(1); / Input enabled */

SIUL2->IMCR[SIUL2_IMCR_CAN0_RX] = SIUL2_IMCR_SSS(2); /* Select PTB1 */

### 5.3 Memory Configuration

#### 5.3.1 Linker Script (S32K358_flash.ld)

MEMORY
{
FLASH_APP (rx) : ORIGIN = 0x00400000, LENGTH = 7M /* Application /
FLASH_DATA (rx) : ORIGIN = 0x10000000, LENGTH = 256K / NVM Data /
SRAM (rwx) : ORIGIN = 0x20000000, LENGTH = 1024K / Main SRAM /
SRAM_DMA (rwx) : ORIGIN = 0x20100000, LENGTH = 512K / DMA buffers */
}

SECTIONS
{
.text : { (.text) } > FLASH_APP
.rodata : { (.rodata) } > FLASH_APP
.data : { (.data) } > SRAM AT > FLASH_APP
.bss : { (.bss) } > SRAM
.dma_buffers : { *(.dma_buffers) } > SRAM_DMA
}

#### 5.3.2 Memory Protection (MPU)

/* Configure MPU regions */
MPU_Type * const mpu = MPU;

/* Region 0: Flash (Read-Execute only) /
mpu->RGD.WORD0 = 0x00400000; / Start /
mpu->RGD.WORD1 = 0x00BFFFFF; / End */
mpu->RGD.WORD2 = MPU_RGD_WORD2_M0RE(1) | MPU_RGD_WORD2_M0PE(1);

/* Region 1: SRAM (Read-Write) */
mpu->RGD.WORD0 = 0x20000000;​
mpu->RGD.WORD1 = 0x201FFFFF;​
mpu->RGD​.WORD2 = MPU_RGD_WORD2_M0RE(1) | MPU_RGD_WORD2_M0WE(1);

---

## 6. Operation

### 6.1 System Startup

#### 6.1.1 Boot Sequence

Power-On Reset (POR)
↓

Boot ROM Execution
├─→ Verify HSE firmware
├─→ Check secure boot signature
└─→ Load application
↓

Application Initialization
├─→ Clock configuration
├─→ Memory initialization
├─→ Peripheral initialization
└─→ Safety monitoring start
↓

Main Application Loop

#### 6.1.2 Startup Time Budget

| Phase | Typical Time | Max Time |
|-------|--------------|----------|
| POR to Boot ROM | 5 ms | 10 ms |
| Boot ROM execution | 20 ms | 50 ms |
| Application init | 50 ms | 100 ms |
| **Total to operational** | **75 ms** | **160 ms** |

### 6.2 Operating Modes

#### 6.2.1 Mode Descriptions

| Mode | Core State | Peripherals | Wake Source | Power |
|------|------------|-------------|-------------|-------|
| **RUN** | Active | All active | N/A | 100% |
| **STANDBY** | Low-power | Selected | CAN, WKPU, RTC | ~10% |

#### 6.2.2 Mode Transitions

**Enter STANDBY Mode:**
/* Configure wakeup sources /
WKPU->WIREER = WKPU_WIREER_WKPU0(1); / Wakeup on rising edge /
WKPU->WIFER = WKPU_WIFER_WIF0(1); / Clear pending flags */

/* Enter low-power mode /
MC_ME->PRTN0_COFB0_CLKEN = 0x00000000; / Disable clocks /
PMC->PMCSR = PMC_PMCSR_STANDBY(1);
__WFI(); / Wait for interrupt */

**Exit STANDBY Mode:**
/* Triggered automatically by wakeup source /
/ Resume from PC after __WFI() */

### 6.3 Communication Protocols

#### 6.3.1 CAN Configuration

**Standard CAN-FD Setup (500 kbps nominal, 2 Mbps data):**
flexcan_data_info_t dataInfo = {
.data_length = 64U,
.msg_id_type = FLEXCAN_MSG_ID_STD,
.enable_brs = true,
.fd_enable = true,
.fd_padding = 0xCC
};

FLEXCAN_DRV_Init(INST_CANCOM0, &canCom0_State, &canCom0_InitConfig0);
FLEXCAN_DRV_ConfigTxMb(INST_CANCOM0, 0, &dataInfo, 0x123);

**Send CAN Message:**
flexcan_data_info_t txInfo = { /* ... / };
uint8_t txData = { 0x01, 0x02, 0x03, / ... */ };
FLEXCAN_DRV_Send(INST_CANCOM0, 0, &txInfo, 0x456, txData);

#### 6.3.2 Ethernet Configuration

**GMAC Initialization (1000Base-T):**
status_t ENET_DRV_Init(uint8_t instance,
const enet_config_t *config,
const enet_buffer_config_t *bufferConfig,
const uint8_t *macAddr,
uint32_t enableMdio);

/* Example: 192.168.1.100 */
uint8_t macAddr = {0x00, 0x04, 0x9F, 0x12, 0x34, 0x56};
ENET_DRV_Init(0, &enetCfg, &enetBuffCfg, macAddr, 1);

---

## 7. Calibration

### 7.1 FreeMaster Connection

#### 7.1.1 Setup

**Via UART:**
Connect UART cable (USB-TTL converter)
TX → PTA2 (LPUART0_TX)
RX → PTA3 (LPUART0_RX)
GND → VSS

Open FreeMaster

Project → Connection Wizard

Select: COM port, Baud: 115200, 8N1

Connect

**Via CAN:**
Connect CAN interface (PCAN-USB, Kvaser, etc.)

Project → Options → Communication

Select: CAN, Channel: CAN0, Bitrate: 500 kbps

Enable: CAN-TP protocol

Connect

### 7.2 Calibration Parameters

#### 7.2.1 Parameter Structure

/* calibration/calib_params.h /
typedef struct {
/ Motor Control /
float motor_Kp; / Proportional gain /
float motor_Ki; / Integral gain /
float motor_Kd; / Derivative gain /
uint16_t motor_max_current; / Max current (mA) */

/* Battery Management */
float batt_voltage_min;      /* Min cell voltage (V) */
float batt_voltage_max;      /* Max cell voltage (V) */
float batt_temp_max;         /* Max temperature (°C) */

/* Vehicle Dynamics */
float wheel_radius;          /* Wheel radius (m) */
float gear_ratio;            /* Gear ratio */
} CalibParams_t;

/* Calibration data in RAM /
attribute((section(".calib_ram")))
CalibParams_t g_CalibParams = {
.motor_Kp = 0.5f,
.motor_Ki = 0.1f,
.motor_Kd = 0.05f,
.motor_max_current = 400,
/ ... */
};

#### 7.2.2 Online Calibration

**FreeMaster Variable Watch:**
Variable Watch → Add Variable

Enter: g_CalibParams.motor_Kp

Double-click value to edit

Changes take effect immediately

**Save to Flash:**
/* Save calibration to data flash /
status_t SaveCalibration(void) {
return FLASH_DRV_Program(&flashState,
DATA_FLASH_BASE,
sizeof(CalibParams_t),
(uint8_t)&g_CalibParams);
}

### 7.3 Calibration Workflow

#### 7.3.1 Motor Control Tuning

Set motor to safe initial values

Enable FreeMaster scope

Apply step input (e.g., torque request)

Observe response:

Overshoot → Reduce Kp

Oscillation → Reduce Kd

Steady-state error → Increase Ki

Iterate until desired response

Save final values

#### 7.3.2 ADC Offset Calibration

/* Perform ADC offset calibration */
void CalibrateADC(void) {
uint32_t sum = 0;

/* Average 1000 samples with input shorted to GND */
for (uint16_t i = 0; i < 1000; i++) {
    ADC_DRV_StartConversion(INST_ADC0, 0);
    while (!ADC_DRV_GetConvCompleteFlag(INST_ADC0, 0));
    sum += ADC_DRV_GetConvResult(INST_ADC0, 0);
}

g_CalibParams.adc_offset = sum / 1000;
}

---

## 8. Diagnostics

### 8.1 Diagnostic Communication

#### 8.1.1 UDS Protocol (ISO 14229)

**Supported Services:**

| SID | Service Name | Description |
|-----|--------------|-------------|
| 0x10 | DiagnosticSessionControl | Change diagnostic session |
| 0x11 | ECUReset | Reset ECU |
| 0x14 | ClearDiagnosticInformation | Clear DTCs |
| 0x19 | ReadDTCInformation | Read stored faults |
| 0x22 | ReadDataByIdentifier | Read data parameters |
| 0x27 | SecurityAccess | Unlock protected functions |
| 0x2E | WriteDataByIdentifier | Write parameters |
| 0x31 | RoutineControl | Execute test routines |
| 0x3E | TesterPresent | Keep session alive |

**Example: Read DTC**
Request: 0x19 0x02 0xFF
[ReadDTC] [ReportByStatusMask] [All DTCs]
Response: 0x59 0x02 [DTC_High] [DTC_Mid] [DTC_Low] [Status]

#### 8.1.2 XCP Protocol

**Configuration:**
/* XCP on CAN */
#define XCP_CAN_ID_MASTER 0x700
#define XCP_CAN_ID_SLAVE 0x701
#define XCP_MAX_DTO 8
#define XCP_MAX_CTO 8

### 8.2 Fault Codes (DTCs)

#### 8.2.1 DTC Format (ISO 14229-1)

Format: P/C/B/U + XXXX
P = Powertrain
C = Chassis
B = Body
U = Network

Example: P0A1F
P = Powertrain
0A1F = High voltage system interlock fault

#### 8.2.2 Common DTCs

| DTC | Description | Possible Cause |
|-----|-------------|----------------|
| P0A1F | HV Interlock Circuit | Safety interlock open |
| P0A80 | Replace Hybrid Battery Pack | Cell voltage out of range |
| P0AA6 | Hybrid Battery Voltage System Isolation Fault | Insulation fault detected |
| P0B12 | Hybrid Battery Voltage Sense Circuit | Voltage sensor fault |
| P0C5A | Motor Control Module Performance | Core lockstep mismatch |
| U0100 | Lost Communication With ECM/PCM | CAN bus fault |
| U0155 | Lost Communication With IP Cluster | Network timeout |

#### 8.2.3 DTC Status Bits

| Bit | Description |
|-----|-------------|
| 0 | testFailed |
| 1 | testFailedThisOperationCycle |
| 2 | pendingDTC |
| 3 | confirmedDTC |
| 4 | testNotCompletedSinceLastClear |
| 5 | testFailedSinceLastClear |
| 6 | testNotCompletedThisOperationCycle |
| 7 | warningIndicatorRequested |

### 8.3 Diagnostic Trouble Shooting

#### 8.3.1 Debug via Serial Console

**Connect UART (115200 8N1):**
$ screen /dev/ttyUSB0 115200

VCU> help
Available commands:
status - Show system status
dtc - Show stored DTCs
reset - Reset ECU
version - Show firmware version

VCU> status
System Status:
Core Clock: 160 MHz
CPU Load: 45%
Free Heap: 128 KB
Uptime: 00:15:32

#### 8.3.2 Live Data Monitoring

**FreeMaster Recorder:**
Recorder → Add Variables:

motor_speed_rpm

battery_voltage

battery_current

battery_temp

Set sampling: 10 ms

Trigger: On fault detection

Start recording

---

## 9. Troubleshooting

### 9.1 System Won't Boot

| Symptom | Possible Cause | Solution |
|---------|----------------|----------|
| No power LED | Power supply fault | Check VDD_HV voltage (5.0V ±10%) |
| Power LED on, no boot | Clock fault | Verify FXOSC crystal (16 MHz) |
| Boots, immediate reset | Watchdog timeout | Check application initialization |
| Stuck in bootloader | BOOT_CFG pins wrong | Verify BOOT_CFG = 00 (internal flash) |

### 9.2 Communication Failures

**CAN Bus Not Working:**
Check:

CAN transceiver power (3.3V or 5.0V)

CAN_H and CAN_L connections

120Ω termination resistors at both ends

Bit timing configuration matches network

Pin muxing correct (CAN0_TX/RX)

Test:

Linux CAN tools
$ candump can0
$ cansend can0 123#DEADBEEF

**Ethernet Not Linking:**
Check:

PHY power and reset

MDC/MDIO communication to PHY

Reference clock (25 MHz or 50 MHz)

Magnetics and RJ45 connector

Cable connected and good

Debug:

Read PHY registers via MDIO

Check PHY LEDs (Link, Activity)

### 9.3 Performance Issues

**High CPU Load:**
Analyze:

Enable profiling timer (STM)

Measure task execution times

Check interrupt rates

Verify DMA usage for bulk transfers

Optimize:

Move intensive tasks to DMA

Optimize algorithm (lookup tables, fixed-point)

Reduce interrupt priority/frequency

**Memory Leaks:**
Monitor:

Track heap usage over time

Use memory pool allocators

Static analysis tools (Valgrind, AddressSanitizer)

Tools:

Enable heap tracking
#define ENABLE_HEAP_MONITOR 1

---

## 10. Appendices

### Appendix A: Pin Assignment Table

See separate document: [S32K358_Pin_Assignment.xlsx](../platform/S32K358_IOMUX.xlsx)

### Appendix B: Memory Map

| Region | Start Address | End Address | Size | Description |
|--------|---------------|-------------|------|-------------|
| Program Flash | 0x0040_0000 | 0x00BF_FFFF | 8 MB | Application code |
| Data Flash | 0x1000_0000 | 0x1003_FFFF | 256 KB | Calibration data |
| SRAM | 0x2000_0000 | 0x201F_FFFF | 2 MB | RAM |
| TCM | 0x2020_0000 | 0x2022_FFFF | 192 KB | Tightly coupled |
| Peripherals | 0x4000_0000 | 0x4FFF_FFFF | 256 MB | Peripheral registers |

### Appendix C: Register Summary

See S32K3xx Reference Manual, Section 3: Memory Map

### Appendix D: Acronyms and Abbreviations

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
- Email: support@atlas-motors.ma
- Issue Tracker: https://github.com/redamomo5588/asild-vcu-s32k3/issues
