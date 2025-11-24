# SafetyLib API Reference
## Safety Monitoring and Diagnostics Library

**Version:** 1.0  
**Release Date:** Nov 2025

---

### Fault Management
```c
void Safety_ReportFault(uint32_t faultId, const char *desc);
void Safety_ClearFault(uint32_t faultId);
bool Safety_IsFaultActive(uint32_t faultId);
```
- Reports, clears, and checks status for system faults.

### Debounce Filter
```c
bool Safety_DebounceFault(FaultDebounce_t *fault);
```
- Applies debounce logic to filter transient faults.

### Safe State Control
```c
void Safety_TransitionToSafeState(SafeState_t state);
```
- Switches system to defined safe operational modes (normal, degraded, limp-home, emergency shutdown).

### Self-Test Functions
```c
status_t Safety_RunLBIST(void);
status_t Safety_RunMBIST(void);
```
- Executes logic and memory built-in self-tests.

### Diagnostic Data
```c
void Safety_LogEventData(uint32_t faultCode);
LockstepFaultRecord_t Safety_ReadFaultRecord(void);
```
- Logs and reads fault data.

---

**See also:** [safety_manual.md](../safety_manual.md) for details and integration examples.
