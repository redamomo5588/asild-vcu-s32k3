# Ethernet TSN Manual
## Time-Sensitive Networking for S32K3xx

**Document Version:** 1.0  
**Release Date:** November 2025  
**Standard:** IEEE 802.1 AVB/TSN

---

## Overview

This manual covers Time-Sensitive Networking (TSN) implementation on the S32K3xx GMAC (Gigabit Media Access Controller) for deterministic, low-latency automotive Ethernet communication.

## TSN Standards

| Standard | Description |
|----------|-------------|
| IEEE 802.1AS | Time synchronization (gPTP) |
| IEEE 802.1Qav | Credit-Based Shaper (CBS) |
| IEEE 802.1Qbv | Time-Aware Shaper (TAS) |
| IEEE 802.1Qbu | Frame Preemption |
| IEEE 802.1CB | Frame Replication and Elimination |

## Architecture

```
┌──────────────────────────────────────────────┐
│               Application Layer                      │
│  (AUTOSAR, TCP/IP, AVB Streaming)                  │
├──────────────────────────────────────────────┤
│           TSN Traffic Shaping                       │
│  ┌──────────────┐  ┌──────────────┐         │
│  │ TAS (802.1Qbv) │  │ CBS (802.1Qav) │         │
│  └──────────────┘  └──────────────┘         │
├──────────────────────────────────────────────┤
│        gPTP Time Synchronization                    │
│           (IEEE 802.1AS)                           │
├──────────────────────────────────────────────┤
│            GMAC Hardware                           │
│  (DMA, TX/RX Queues, TSN Features)                 │
├──────────────────────────────────────────────┤
│               PHY Layer                            │
│    (1000Base-T, RGMII Interface)                   │
└──────────────────────────────────────────────┘
```

## Time Synchronization (gPTP)

### Configuration
```c
/* gPTP initialization */
gptp_config_t gptpCfg = {
    .role = GPTP_ROLE_SLAVE,  /* Or GPTP_ROLE_MASTER */
    .domain = 0,               /* Domain 0 for automotive */
    .priority1 = 248,          /* Lower = higher priority */
    .clockClass = 248,
    .logSyncInterval = -3,     /* 125ms = 2^(-3) seconds */
    .logAnnounceInterval = 0,  /* 1 second */
};

GPTP_Init(&gptpCfg);
```

### Time Accuracy
- **Typical**: < 1 µs synchronization
- **Maximum**: < 10 µs

## Traffic Shaping

### Credit-Based Shaper (CBS)

**Use Case:** AVB audio/video streaming

```c
/* Configure CBS for TX queue 1 (Class A traffic) */
cbs_config_t cbsCfg = {
    .idleSlope = 3125,      /* bits/sec per 1/256 sec */
    .sendSlope = -28125,    /* (bandwidth - idleSlope) */
    .hiCredit = 153600,     /* Max credit */
    .loCredit = -1382400,   /* Min credit */
};

GMAC_ConfigureCBS(GMAC_INSTANCE, QUEUE_1, &cbsCfg);
```

### Time-Aware Shaper (TAS)

**Use Case:** Deterministic control messages

```c
/* Gate Control List */
tas_gate_entry_t gateList[] = {
    /* Time, Queue Mask (bit per queue) */
    {0,      0x01},  /* 0-100µs: Queue 0 only */
    {100000, 0xFE},  /* 100-1000µs: Queues 1-7 */
};

tas_config_t tasCfg = {
    .baseTime = 0,              /* Start immediately */
    .cycleTime = 1000000,       /* 1ms cycle */
    .gateList = gateList,
    .gateListLength = 2,
};

GMAC_ConfigureTAS(GMAC_INSTANCE, &tasCfg);
```

## Frame Preemption

**Purpose:** Interrupt low-priority frames to send high-priority frames

```c
/* Enable frame preemption */
GMAC_EnableFramePreemption(GMAC_INSTANCE, true);

/* Set queue preemption priority */
GMAC_SetPreemptionPriority(GMAC_INSTANCE, QUEUE_0, true);  /* Express */
GMAC_SetPreemptionPriority(GMAC_INSTANCE, QUEUE_1, false); /* Preemptable */
```

## Traffic Classes

| Class | Priority | Max Latency | Use Case |
|-------|----------|-------------|----------|
| 0 (Highest) | 7 | < 100 µs | Safety messages |
| 1 | 6 | < 250 µs | Control messages |
| 2 | 5 | < 2 ms | AVB Class A |
| 3 | 4 | < 50 ms | AVB Class B |
| 4-7 (Best Effort) | 0-3 | N/A | Background traffic |

## Configuration Example

```c
void ConfigureTSN_VCU(void) {
    /* 1. Initialize GMAC */
    GMAC_Init(&gmacConfig);
    
    /* 2. Enable TSN features */
    GMAC_EnableTSN(GMAC_INSTANCE, true);
    
    /* 3. Configure gPTP */
    GPTP_Init(&gptpConfig);
    
    /* 4. Configure TAS for deterministic control */
    GMAC_ConfigureTAS(GMAC_INSTANCE, &tasConfig);
    
    /* 5. Configure CBS for AVB streams */
    GMAC_ConfigureCBS(GMAC_INSTANCE, QUEUE_2, &cbsConfigClassA);
    
    /* 6. Enable frame preemption */
    GMAC_EnableFramePreemption(GMAC_INSTANCE, true);
    
    /* 7. Map traffic to queues */
    GMAC_MapPriorityToQueue(GMAC_INSTANCE, 7, QUEUE_0);  /* Safety */
    GMAC_MapPriorityToQueue(GMAC_INSTANCE, 6, QUEUE_1);  /* Control */
    GMAC_MapPriorityToQueue(GMAC_INSTANCE, 5, QUEUE_2);  /* AVB-A */
}
```

## Performance Metrics

| Metric | Value |
|--------|-------|
| Max Throughput | 1 Gbps |
| Min Frame Size | 64 bytes |
| Max Frame Size | 9000 bytes (Jumbo frames) |
| TX Queues | 8 (0-7) |
| RX Queues | 8 (0-7) |
| Hardware Timestamp | IEEE 1588 PTP |

## Troubleshooting

### Issue: Poor Time Sync
**Check:**
- gPTP domain correct
- Sync messages received
- PHY delay compensation

### Issue: Frame Drops
**Check:**
- CBS configuration
- Queue mapping
- Bandwidth allocation

---

**For Support:**  
Email: ethernet@yourcompany.com
