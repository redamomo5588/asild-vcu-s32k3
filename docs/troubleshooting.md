# Troubleshooting Guide
## S32K3xx VCU Common Issues and Solutions

**Document Version:** 1.0  
**Release Date:** November 2025

---

## System Issues

### Issue: System Won't Boot

**Symptom:** No LED, no communication  
**Possible Causes:**
1. Power supply fault
2. Clock fault
3. Watchdog timeout
4. Secure boot failure

**Solutions:**
```
Check:
1. VDD_HV voltage (5.0V ±10%)
2. Crystal oscillator (16 MHz)
3. BOOT_CFG pins
4. Flash memory integrity

Debug:
- Connect JTAG debugger
- Read reset reason register
- Check boot ROM status
```

### Issue: Frequent Resets

**Symptom:** System resets unexpectedly  
**Possible Causes:**
1. Watchdog timeout
2. Stack overflow
3. Lockstep mismatch
4. Voltage drop

**Solutions:**
```c
/* Check reset reason */
uint32_t reset_reason = MC_RGM->DES;

if (reset_reason & MC_RGM_DES_F_SWT) {
    /* Watchdog reset */
    /* Increase watchdog timeout or optimize code */
}

if (reset_reason & MC_RGM_DES_F_FCCU) {
    /* FCCU fault */
    /* Check FCCU fault status */
    uint32_t fccu_status = FCCU->NCFS;
}
```

## Communication Issues

### Issue: CAN Bus Errors

**Symptom:** CAN error frames, bus-off  
**Checks:**
```
1. Bit timing: 500 kbps nominal
   - PRESDIV, PROPSEG, PSEG1, PSEG2, RJW
2. Termination: 120Ω at both ends
3. Cable: Twisted pair, < 1m for 1 Mbps
4. Transceiver: Power, CANH/CANL connections
```

**Debug:**
```c
/* Check CAN error counters */
uint8_t txerr = (FlexCAN0->ECR & CAN_ECR_TXERRCNT_MASK) >> CAN_ECR_TXERRCNT_SHIFT;
uint8_t rxerr = (FlexCAN0->ECR & CAN_ECR_RXERRCNT_MASK) >> CAN_ECR_RXERRCNT_SHIFT;

printf("TX Errors: %d, RX Errors: %d\n", txerr, rxerr);
```

### Issue: Ethernet Not Linking

**Symptom:** No link LED, no communication  
**Checks:**
```
1. PHY power and reset
2. MDC/MDIO communication
3. Reference clock (25 MHz or 50 MHz)
4. TX/RX pins configuration
5. Cable connection
```

**Debug:**
```c
/* Read PHY registers via MDIO */
uint16_t phy_id1 = ENET_ReadPHY(PHY_ADDR, PHY_REG_ID1);
uint16_t phy_status = ENET_ReadPHY(PHY_ADDR, PHY_REG_STATUS);

printf("PHY ID: 0x%04X, Status: 0x%04X\n", phy_id1, phy_status);
```

## Performance Issues

### Issue: High CPU Load

**Symptom:** Tasks missing deadlines  
**Measurement:**
```c
/* Enable DWT cycle counter */
CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
DWT->CYCCNT = 0;
DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;

/* Measure function execution time */
uint32_t start = DWT->CYCCNT;
MyFunction();
uint32_t cycles = DWT->CYCCNT - start;
float time_us = (float)cycles / (CPU_FREQ_MHZ);
```

**Optimization:**
```
1. Use DMA for bulk transfers
2. Reduce interrupt frequency
3. Optimize algorithms (lookup tables)
4. Enable compiler optimization (-O2)
5. Use inline functions for small functions
```

### Issue: Memory Leaks

**Symptom:** Decreasing free heap over time  
**Detection:**
```c
/* Monitor heap usage */
extern uint8_t __heap_start__;
extern uint8_t __heap_end__;

uint32_t heap_size = &__heap_end__ - &__heap_start__;
uint32_t heap_used = GetHeapUsed();
float heap_percent = (float)heap_used / heap_size * 100.0f;

printf("Heap: %d/%d bytes (%.1f%%)\n", heap_used, heap_size, heap_percent);
```

**Solutions:**
```
1. Use static allocation where possible
2. Use memory pools
3. Enable heap tracking
4. Use static analysis tools
5. Review malloc/free pairs
```

## Safety Issues

### Issue: Lockstep Faults

**Symptom:** Frequent FCCU faults, resets  
**Analysis:**
```c
/* Read lockstep fault record */
LockstepFaultRecord_t fault;
if (ReadFaultRecord(&fault) == STATUS_OK) {
    printf("Lockstep fault at PC: 0x%08X\n", fault.programCounter);
    printf("Instruction: 0x%08X\n", *(uint32_t*)fault.programCounter);
}
```

**Causes:**
```
1. ECC error (transient or permanent)
2. Power supply noise
3. EMI interference
4. Temperature extremes
5. Silicon errata
```

**Solutions:**
```
1. Improve power supply filtering
2. Add shielding/ferrite beads
3. Improve thermal management
4. Check for applicable errata workarounds
5. Review PCB layout
```

## Debug Commands

### Serial Console Commands
```
VCU> help
Available commands:
  status    - Show system status
  reset     - Reset ECU
  dtc       - Show stored DTCs
  clear     - Clear DTCs
  version   - Show firmware version
  test      - Run self-tests

VCU> status
System Status:
  Core Clock: 160 MHz
  CPU Load: 42%
  Free Heap: 85 KB
  Uptime: 01:23:45
  Reset Reason: Watchdog

VCU> dtc
Stored DTCs:
  P0B12 - Battery Voltage Sensor (Permanent)
  U0100 - Lost Comm With ECM (Pending)
```

## Contact Support

**Technical Support:**
- Email: support@yourcompany.com
- Phone: +XX XXX XXX XXXX
- Issue Tracker: https://github.com/redamomo5588/asild-vcu-s32k3/issues

**Emergency (Safety Critical):**
- Hotline: +XX XXX XXX XXXX (24/7)
- Email: safety@yourcompany.com
