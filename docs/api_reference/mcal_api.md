# MCAL API Reference
## Microcontroller Abstraction Layer (MCAL) Functions

**Version:** 1.0  
**Release Date:** Nov 2025

---

### ADC Interface
```c
status_t ADC_Init(const adc_config_t *cfg);
status_t ADC_ReadChannel(uint8_t instance, uint8_t channel);
status_t ADC_Calibrate(uint8_t instance);
```
- Initializes, reads, and calibrates analog-to-digital converter channels.

### PWM Driver
```c
status_t PWM_Init(const pwm_config_t *cfg);
status_t PWM_SetDuty(uint8_t instance, uint16_t duty_percent);
```
- Configures PWM channels and sets output duty cycles.

### CAN Driver
```c
status_t CAN_Init(const can_config_t *cfg);
status_t CAN_Send(uint32_t id, const uint8_t *data, uint8_t len);
status_t CAN_Receive(uint32_t *id, uint8_t *data, uint8_t *len);
```
- Initializes CAN interface, sends, and receives frames.

### Watchdog
```c
status_t Watchdog_Init(uint32_t timeout_ms);
void Watchdog_Service(void);
```
- Configures and services software watchdog timer.

### General Purpose IO
```c
status_t GPIO_SetPinMux(uint8_t pin, uint8_t mux);
void GPIO_WritePin(uint8_t pin, uint8_t val);
uint8_t GPIO_ReadPin(uint8_t pin);
```
- Configures, sets, and reads digital IO pins.

---

**See also:** [user_manual.md](../user_manual.md) for typical MCAL usage.
