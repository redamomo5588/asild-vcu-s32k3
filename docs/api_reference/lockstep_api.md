# Lockstep API Reference
## S32K3xx Cortex-M7 Lockstep Functions

**Version:** 1.0  
**Release Date:** Nov 2025

---

### Lockstep Initialization
```c
status_t Lockstep_Init(void);
```
- Enables lockstep mode, configures FCCU reaction, registers handlers.
- Should be called during early startup.

### Status Query
```c
LockstepStatus_t Lockstep_GetStatus(void);
```
- Returns structure with current lockstep, comparator, and fault status.

### Fault Handling
```c
void Lockstep_FaultHandler(void);
```
- Invoked on lockstep mismatch interrupt via FCCU.
- Implements emergency system shutdown.

### Self-Test
```c
status_t Lockstep_SelfTest(void);
```
- Injects fault and verifies detection during startup.

---

**See also:** [lockstep_manual.md](../lockstep_manual.md) for usage and integration.
