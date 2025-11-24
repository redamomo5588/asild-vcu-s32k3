# Ethernet API Reference
## S32K3xx GMAC Driver Functions

**Version:** 1.0  
**Release Date:** Nov 2025

---

### GMAC Initialization
```c
status_t GMAC_Init(const gmac_config_t *config);
```
- Initializes the GMAC controller with the provided configuration.
- Must be called before any data transfer.

### Frame Transmission
```c
status_t GMAC_SendFrame(uint8_t *data, uint32_t len, uint8_t queue);
```
- Sends an Ethernet frame using the specified TX queue.
- Returns STATUS_OK if completed.

### Frame Reception
```c
status_t GMAC_ReceiveFrame(uint8_t *data, uint32_t *plen, uint8_t queue);
```
- Receives an Ethernet frame from the RX queue.
- Updates plen with actual received length.

### TSN Configuration
```c
status_t GMAC_EnableTSN(bool enable);
status_t GMAC_ConfigureCBS(uint8_t queue, const cbs_config_t *cfg);
status_t GMAC_ConfigureTAS(const tas_config_t *cfg);
```
- Enables TSN features and configures CBS/TAS as needed.

### PHY Access
```c
status_t GMAC_ReadPHY(uint8_t phy_addr, uint8_t reg_addr, uint16_t *value);
status_t GMAC_WritePHY(uint8_t phy_addr, uint8_t reg_addr, uint16_t value);
```
- Communicate with Ethernet PHY over MDIO.

---

**See also:** [ethernet_tsn_manual.md](../ethernet_tsn_manual.md) for usage and examples.
