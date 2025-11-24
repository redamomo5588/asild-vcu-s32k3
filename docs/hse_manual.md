# Hardware Security Engine (HSE) Manual
## S32K3xx Security Configuration and Implementation

**Document Version:** 1.0  
**Release Date:** November 2025  
**Security Level:** EVITA Full  
**Target Audience:** Security Engineers, System Architects

---

## Table of Contents

1. [Introduction](#1-introduction)
2. [HSE Architecture](#2-hse-architecture)
3. [Security Services](#3-security-services)
4. [Key Management](#4-key-management)
5. [Secure Boot](#5-secure-boot)
6. [Firmware Updates](#6-firmware-updates)
7. [Configuration](#7-configuration)
8. [API Reference](#8-api-reference)
9. [Security Best Practices](#9-security-best-practices)
10. [References](#10-references)

---

## 1. Introduction

### 1.1 Purpose

This manual describes the Hardware Security Engine (HSE_B) implementation on the NXP S32K3xx platform, covering cryptographic services, key management, secure boot, and security best practices for EVITA Full compliance.

### 1.2 HSE Overview

**What is HSE_B?**

The Hardware Security Engine (HSE_B) is a dedicated security coprocessor that provides:
- Hardware-accelerated cryptographic operations
- Secure key storage and management
- True Random Number Generator (TRNG)
- Secure boot and firmware authentication
- Tamper detection and response

**Security Level:** EVITA Full (Highest automotive security)

### 1.3 Key Features

| Feature | Capability |
|---------|------------|
| **Symmetric Crypto** | AES-128/192/256 (ECB, CBC, CTR, GCM, CMAC) |
| **Asymmetric Crypto** | RSA up to 4096-bit, ECC up to 521-bit |
| **Hash Functions** | SHA-2 (256/384/512), SHA-3, HMAC |
| **Key Storage** | Hardware-backed, encrypted storage |
| **TRNG** | NIST SP 800-90B compliant |
| **Secure Boot** | Multi-stage authenticated boot |
| **Firmware Update** | Encrypted, authenticated FOTA |

### 1.4 Security Standards Compliance

| Standard | Compliance |
|----------|------------|
| EVITA | Full |
| ISO 21434 | Cybersecurity engineering |
| FIPS 140-2 | Level 2 (planned certification) |
| Common Criteria | EAL4+ (planned) |

---

## 2. HSE Architecture

### 2.1 Block Diagram

```
┌────────────────────────────────────────────────────────────┐
│                   Application (Cortex-M7)                    │
│  ┌────────────────────────────────────────────────────┐ │
│  │           HSE Host Driver (API Layer)                │ │
│  └────────────────────────────────────────────────────┘ │
│                           ↓ MU (Messaging Unit)              │
├────────────────────────────────────────────────────────────┤
│         Hardware Security Engine (HSE_B Core)              │
│                                                              │
│  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐      │
│  │ AES Engine   │  │ RSA/ECC      │  │ SHA Engine   │      │
│  │ (HW Accel)   │  │ Engine       │  │ (HW Accel)   │      │
│  └──────────────┘  └──────────────┘  └──────────────┘      │
│                                                              │
│  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐      │
│  │ TRNG         │  │ Key Store    │  │ Firmware     │      │
│  │ (SP 800-90B) │  │ (Encrypted)  │  │ Manager      │      │
│  └──────────────┘  └──────────────┘  └──────────────┘      │
│                                                              │
│  ┌────────────────────────────────────────────────────┐ │
│  │         Secure NVM (OTP + Encrypted Flash)          │ │
│  └────────────────────────────────────────────────────┘ │
└────────────────────────────────────────────────────────────┘
```

### 2.2 Communication Interface

**Messaging Unit (MU)**

The HSE communicates with the host CPU via a dedicated Messaging Unit:
- 16 message channels (4 per MU instance)
- Asynchronous request/response model
- Interrupt-driven or polling mode
- DMA support for large data transfers

**Message Format:**
```c
typedef struct {
    uint32_t serviceId;      /* Service identifier */
    uint32_t channelId;      /* MU channel (0-15) */
    uint8_t  *pInput;        /* Input buffer */
    uint32_t inputLength;    /* Input size */
    uint8_t  *pOutput;       /* Output buffer */
    uint32_t outputLength;   /* Output size */
    hseSrvResponse_t response; /* Response status */
} hseMsg_t;
```

---

## 3. Security Services

### 3.1 Symmetric Cryptography

#### 3.1.1 AES Encryption/Decryption

**Supported Modes:**
- ECB (Electronic Codebook)
- CBC (Cipher Block Chaining)
- CTR (Counter Mode)
- GCM (Galois/Counter Mode)
- CCM (Counter with CBC-MAC)
- CMAC (Cipher-based MAC)

**Example: AES-256 CBC Encryption**
```c
hseSrvDescriptor_t aesEncrypt;
hseSymCipherSrv_t *pAesSrv;

/* Prepare service descriptor */
aesEncrypt.srvId = HSE_SRV_ID_SYM_CIPHER;
pAesSrv = &(aesEncrypt.hseSrv.symCipherReq);

pAesSrv->cipherAlgo = HSE_CIPHER_ALGO_AES;
pAesSrv->cipherBlockMode = HSE_CIPHER_BLOCK_MODE_CBC;
pAesSrv->cipherDir = HSE_CIPHER_DIR_ENCRYPT;
pAesSrv->keyHandle = AES_KEY_HANDLE;
pAesSrv->pIV = iv_buffer;  /* 16-byte IV */
pAesSrv->pInput = plaintext;
pAesSrv->inputLength = plaintext_len;
pAesSrv->pOutput = ciphertext;

/* Send request to HSE */
status = HSE_Send(MU_CHANNEL_0, &aesEncrypt);
```

#### 3.1.2 Performance

| Operation | Key Size | Throughput |
|-----------|----------|------------|
| AES-ECB Encrypt | 128-bit | 200 MB/s |
| AES-ECB Encrypt | 256-bit | 150 MB/s |
| AES-GCM Encrypt | 128-bit | 180 MB/s |
| AES-GCM Encrypt | 256-bit | 140 MB/s |

### 3.2 Asymmetric Cryptography

#### 3.2.1 RSA Operations

**Supported Operations:**
- RSA Sign (PKCS#1 v1.5, PSS)
- RSA Verify
- RSA Encrypt/Decrypt (OAEP)

**Key Sizes:** 1024, 2048, 3072, 4096-bit

**Example: RSA-2048 Signature**
```c
hseSrvDescriptor_t rsaSign;
hseRsaSignSrv_t *pRsaSrv;

rsaSign.srvId = HSE_SRV_ID_RSA_SIGN;
pRsaSrv = &(rsaSign.hseSrv.rsaSignReq);

pRsaSrv->signScheme = HSE_RSA_SIGN_RSASSA_PSS;
pRsaSrv->hashAlgo = HSE_HASH_ALGO_SHA_256;
pRsaSrv->keyHandle = RSA_PRIVATE_KEY_HANDLE;
pRsaSrv->pMessage = message;
pRsaSrv->messageLength = message_len;
pRsaSrv->pSignature = signature;
pRsaSrv->pSignatureLength = &signature_len;

status = HSE_Send(MU_CHANNEL_0, &rsaSign);
```

#### 3.2.2 ECC Operations

**Supported Curves:**
- NIST P-256, P-384, P-521
- Brainpool BP256R1, BP384R1, BP512R1
- Curve25519 (X25519, Ed25519)

**Operations:**
- ECDSA Sign/Verify
- ECDH Key Agreement
- EC Key Generation

**Example: ECDSA P-256 Signature**
```c
hseSrvDescriptor_t ecdsaSign;
hseEccSignSrv_t *pEccSrv;

ecdsaSign.srvId = HSE_SRV_ID_ECC_SIGN;
pEccSrv = &(ecdsaSign.hseSrv.eccSignReq);

pEccSrv->signScheme = HSE_ECC_SIGN_ECDSA;
pEccSrv->eccCurveId = HSE_ECC_CURVE_NISTP256;
pEccSrv->hashAlgo = HSE_HASH_ALGO_SHA_256;
pEccSrv->keyHandle = ECC_PRIVATE_KEY_HANDLE;
pEccSrv->pMessage = message;
pEccSrv->messageLength = message_len;
pEccSrv->pR = signature_r;  /* 32 bytes */
pEccSrv->pS = signature_s;  /* 32 bytes */

status = HSE_Send(MU_CHANNEL_0, &ecdsaSign);
```

### 3.3 Hash Functions

**Supported Algorithms:**
- SHA-2: SHA-224, SHA-256, SHA-384, SHA-512
- SHA-3: SHA3-224, SHA3-256, SHA3-384, SHA3-512
- HMAC with any SHA variant

**Example: SHA-256 Hash**
```c
hseSrvDescriptor_t hashSrv;
hseHashSrv_t *pHash;

hashSrv.srvId = HSE_SRV_ID_HASH;
pHash = &(hashSrv.hseSrv.hashReq);

pHash->hashAlgo = HSE_HASH_ALGO_SHA_256;
pHash->pInput = data;
pHash->inputLength = data_len;
pHash->pHash = hash_output;  /* 32 bytes for SHA-256 */

status = HSE_Send(MU_CHANNEL_0, &hashSrv);
```

### 3.4 Random Number Generation

**TRNG (True Random Number Generator)**
- NIST SP 800-90B compliant
- Entropy source: Ring oscillators
- Throughput: ~1 MB/s

**Example: Generate 32 Random Bytes**
```c
hseSrvDescriptor_t rngSrv;
hseRngSrv_t *pRng;

rngSrv.srvId = HSE_SRV_ID_GET_RANDOM_NUM;
pRng = &(rngSrv.hseSrv.getRngReq);

pRng->rngClass = HSE_RNG_CLASS_PTG3;  /* NIST SP 800-90B */
pRng->pRandomNum = random_buffer;
pRng->randomNumLength = 32;

status = HSE_Send(MU_CHANNEL_0, &rngSrv);
```

---

## 4. Key Management

### 4.1 Key Storage Architecture

**Key Catalog:**
- Total capacity: 128 key slots
- Storage: Encrypted in NVM
- Access control: Per-key permissions

**Key Types:**
- Symmetric keys (AES, HMAC)
- Asymmetric keys (RSA, ECC public/private)
- Shared secrets (DH, ECDH results)

### 4.2 Key Import

**Example: Import AES-256 Key**
```c
hseSrvDescriptor_t importKey;
hseImportKeySrv_t *pImport;

importKey.srvId = HSE_SRV_ID_IMPORT_KEY;
pImport = &(importKey.hseSrv.importKeyReq);

pImport->keyHandle = AES_KEY_HANDLE;
pImport->keyInfo.keyType = HSE_KEY_TYPE_AES;
pImport->keyInfo.keyBitLen = 256;
pImport->keyInfo.keyFlags = HSE_KF_USAGE_ENCRYPT | HSE_KF_USAGE_DECRYPT;
pImport->pKey = aes_key_material;  /* 32 bytes */
pImport->keyLen = 32;
pImport->cipher.cipherKeyHandle = KEK_HANDLE;  /* Key Encryption Key */

status = HSE_Send(MU_CHANNEL_0, &importKey);
```

### 4.3 Key Generation

**Example: Generate RSA-2048 Key Pair**
```c
hseSrvDescriptor_t genKey;
hseGenerateKeySrv_t *pGen;

genKey.srvId = HSE_SRV_ID_GENERATE_KEY;
pGen = &(genKey.hseSrv.generateKeyReq);

pGen->keyHandle = RSA_KEY_HANDLE;
pGen->keyInfo.keyType = HSE_KEY_TYPE_RSA_PAIR;
pGen->keyInfo.keyBitLen = 2048;
pGen->keyInfo.keyFlags = HSE_KF_USAGE_SIGN | HSE_KF_USAGE_VERIFY;
pGen->pPubExponent = pub_exponent;  /* Usually 65537 */
pGen->pubExponentLength = 4;

status = HSE_Send(MU_CHANNEL_0, &genKey);
```

### 4.4 Key Derivation

**Supported KDFs:**
- HKDF (HMAC-based KDF)
- PBKDF2
- TLS 1.2/1.3 PRF
- ANSI X9.63 KDF

**Example: HKDF-SHA256**
```c
hseSrvDescriptor_t kdfSrv;
hseKdfSrv_t *pKdf;

kdfSrv.srvId = HSE_SRV_ID_KEY_DERIVE;
pKdf = &(kdfSrv.hseSrv.kdfReq);

pKdf->kdfAlgo = HSE_KDF_ALGO_HKDF;
pKdf->hashAlgo = HSE_HASH_ALGO_SHA_256;
pKdf->secretKeyHandle = INPUT_KEY_HANDLE;
pKdf->pSalt = salt;
pKdf->saltLength = salt_len;
pKdf->pInfo = info;
pKdf->infoLength = info_len;
pKdf->derivedKeyHandle = OUTPUT_KEY_HANDLE;
pKdf->derivedKeyLength = 32;  /* 256 bits */

status = HSE_Send(MU_CHANNEL_0, &kdfSrv);
```

---

## 5. Secure Boot

### 5.1 Boot Flow

```
Power-On Reset
    ↓
Boot ROM
    ↓
Load HSE Firmware
    ↓
Verify HSE FW Signature (OTP Key)
    ↓
[HSE Active]
    ↓
Load Application Bootloader
    ↓
Verify Bootloader Signature (HSE)
    ↓
[Bootloader Active]
    ↓
Load Application Firmware
    ↓
Verify Application Signature (HSE)
    ↓
[Application Running]
```

### 5.2 Secure Boot Configuration

**Boot Authentication:**
```c
/* Configure secure boot in DCF */
typedef struct {
    uint32_t tag;                 /* 0xDCF00002 */
    uint32_t length;
    uint32_t enableSecureBoot;    /* 1 = Enable */
    uint32_t bootKeyHandle;       /* Key for signature verification */
    uint32_t bootAlgorithm;       /* RSA or ECDSA */
    uint32_t rollbackProtection;  /* Anti-rollback counter */
} DCF_SecureBoot_t;
```

**Example: Verify Boot Image**
```c
hseSrvDescriptor_t verifySrv;
hseBootImageVerifySrv_t *pVerify;

verifySrv.srvId = HSE_SRV_ID_BOOT_IMAGE_VERIFY;
pVerify = &(verifySrv.hseSrv.bootImageVerifyReq);

pVerify->pImageAddr = (uint8_t*)0x00400000;  /* Flash address */
pVerify->imageLength = 0x00100000;  /* 1 MB */
pVerify->pSignature = (uint8_t*)0x00500000;
pVerify->signatureLength = 256;  /* RSA-2048 */
pVerify->keyHandle = BOOT_KEY_HANDLE;

status = HSE_Send(MU_CHANNEL_0, &verifySrv);

if (status == HSE_SRV_RSP_OK) {
    /* Signature valid - boot application */
    JumpToApplication(0x00400000);
} else {
    /* Signature invalid - stay in safe mode */
    SafeMode();
}
```

### 5.3 Anti-Rollback Protection

**Monotonic Counter:**
- Stored in OTP or encrypted NVM
- Incremented with each firmware update
- Prevents installation of older (vulnerable) versions

```c
uint32_t currentVersion = HSE_GetFirmwareVersion();
uint32_t storedVersion = ReadSecureCounter();

if (currentVersion < storedVersion) {
    /* Rollback detected */
    return HSE_SRV_RSP_ANTI_ROLLBACK_VIOLATION;
}

/* Update counter after successful boot */
WriteSecureCounter(currentVersion);
```

---

## 6. Firmware Updates

### 6.1 FOTA (Firmware Over-The-Air)

**Update Flow:**
```
1. Download encrypted firmware package
2. Verify package signature (HSE)
3. Decrypt firmware (HSE)
4. Verify decrypted firmware signature
5. Check version (anti-rollback)
6. Write to inactive bank
7. Set boot flag
8. Reset
9. Boot from new bank
10. Verify boot successful
11. Commit or rollback
```

### 6.2 Secure Firmware Package Format

```c
typedef struct {
    uint32_t magic;            /* 0x48534546 "HSEF" */
    uint32_t version;          /* Firmware version */
    uint32_t length;           /* Encrypted data length */
    uint8_t  iv[16];           /* AES IV */
    uint8_t  signature[256];   /* RSA-2048 signature */
    uint8_t  encryptedData[];  /* AES-256-CBC encrypted firmware */
} SecureFirmwarePackage_t;
```

### 6.3 Firmware Update Example

```c
status_t SecureFirmwareUpdate(SecureFirmwarePackage_t *pkg) {
    /* Step 1: Verify package signature */
    if (HSE_VerifySignature(pkg->signature, pkg, pkg->length) != HSE_OK) {
        return STATUS_SIGNATURE_INVALID;
    }
    
    /* Step 2: Decrypt firmware */
    uint8_t *decrypted = malloc(pkg->length);
    if (HSE_Decrypt_AES256_CBC(pkg->encryptedData, pkg->length, 
                                decrypted, pkg->iv) != HSE_OK) {
        return STATUS_DECRYPTION_FAILED;
    }
    
    /* Step 3: Verify decrypted firmware */
    if (HSE_VerifyFirmwareIntegrity(decrypted, pkg->length) != HSE_OK) {
        return STATUS_INTEGRITY_CHECK_FAILED;
    }
    
    /* Step 4: Check version (anti-rollback) */
    if (pkg->version <= GetCurrentVersion()) {
        return STATUS_ROLLBACK_DETECTED;
    }
    
    /* Step 5: Write to flash */
    if (FlashWrite(INACTIVE_BANK_ADDR, decrypted, pkg->length) != STATUS_OK) {
        return STATUS_FLASH_WRITE_FAILED;
    }
    
    /* Step 6: Set boot flag and reset */
    SetBootBank(INACTIVE_BANK);
    SystemReset();
    
    return STATUS_OK;
}
```

---

## 7. Configuration

### 7.1 HSE Firmware Installation

**Required Files:**
- `hse_fw.bin` - HSE firmware binary
- `hse_fw.bin.sign` - Firmware signature

**Installation via S32 Design Studio:**
```bash
# Flash HSE firmware
./hse_flash_tool -device S32K358 -fw hse_fw.bin -sign hse_fw.bin.sign

# Verify installation
./hse_flash_tool -device S32K358 -verify
```

### 7.2 Key Provisioning

**Production Key Provisioning Flow:**
```
1. Generate keys in HSM (Hardware Security Module)
2. Encrypt keys with KEK (Key Encryption Key)
3. Program encrypted keys via JTAG
4. Lock key storage (OTP fuses)
5. Verify key installation
6. Destroy KEK
```

### 7.3 HSE Firmware Update

**Update HSE Firmware:**
```c
hseSrvDescriptor_t fwUpdate;
hseFirmwareUpdateSrv_t *pFwUpdate;

fwUpdate.srvId = HSE_SRV_ID_FIRMWARE_UPDATE;
pFwUpdate = &(fwUpdate.hseSrv.firmwareUpdateReq);

pFwUpdate->pFirmwareImage = new_hse_fw;
pFwUpdate->firmwareLength = new_hse_fw_len;
pFwUpdate->pSignature = new_hse_fw_signature;
pFwUpdate->signatureLength = 256;

status = HSE_Send(MU_CHANNEL_0, &fwUpdate);

/* HSE will reset after update */
WaitForHSEReady();
```

---

## 8. API Reference

### 8.1 Initialization

```c
/* Initialize HSE driver */
hseStatus_t HSE_Init(void);

/* Check HSE status */
hseStatus_t HSE_GetStatus(hseAttrSysStatus_t *pStatus);

/* Get HSE firmware version */
hseStatus_t HSE_GetFwVersion(hseAttrFwVersion_t *pVersion);
```

### 8.2 Service Request

```c
/* Send service request (synchronous) */
hseStatus_t HSE_Send(uint8_t muChannel, hseSrvDescriptor_t *pSrv);

/* Send service request (asynchronous) */
hseStatus_t HSE_SendAsync(uint8_t muChannel, hseSrvDescriptor_t *pSrv,
                           hseCallback_t callback);

/* Check service response */
hseStatus_t HSE_GetResponse(uint8_t muChannel, hseSrvResponse_t *pResp);
```

### 8.3 Key Management APIs

```c
/* Import key */
hseStatus_t HSE_ImportKey(hseKeyHandle_t keyHandle, 
                          hseKeyInfo_t *pKeyInfo,
                          const uint8_t *pKey, uint32_t keyLen);

/* Export public key */
hseStatus_t HSE_ExportPublicKey(hseKeyHandle_t keyHandle,
                                 uint8_t *pPublicKey, uint32_t *pKeyLen);

/* Generate key pair */
hseStatus_t HSE_GenerateKeyPair(hseKeyHandle_t keyHandle,
                                 hseKeyInfo_t *pKeyInfo);

/* Erase key */
hseStatus_t HSE_EraseKey(hseKeyHandle_t keyHandle);
```

---

## 9. Security Best Practices

### 9.1 Key Management

1. **Never expose private keys** - Keep private keys in HSE storage only
2. **Use KEK for key import** - Always encrypt keys during provisioning
3. **Limit key usage** - Set appropriate key flags (encrypt-only, sign-only)
4. **Rotate keys regularly** - Implement key rotation policy
5. **Backup keys securely** - Use HSM for key backup

### 9.2 Secure Boot

1. **Enable secure boot** - Mandatory for production
2. **Use strong signatures** - RSA-2048 or ECC-P256 minimum
3. **Implement anti-rollback** - Prevent downgrade attacks
4. **Verify entire chain** - Boot ROM → Bootloader → Application
5. **Lock boot configuration** - Use OTP fuses

### 9.3 Firmware Updates

1. **Encrypt firmware packages** - AES-256 minimum
2. **Sign encrypted packages** - Dual-layer protection
3. **Verify before flash** - Always verify before writing
4. **Use dual-bank updates** - Enable rollback if needed
5. **Log update attempts** - Audit trail for security

### 9.4 Side-Channel Protection

1. **Use HSE for crypto** - Hardware protections built-in
2. **Avoid software crypto** - No sensitive operations in software
3. **Minimize key lifetime** - Use session keys when possible
4. **Randomize timing** - Use HSE's built-in protections

---

## 10. References

### 10.1 Documentation

| Document | Version | Description |
|----------|---------|-------------|
| S32K3xx HSE Reference Manual | Rev. 4 | HSE detailed specification |
| HSE Firmware User Manual | Rev. 0.9 | HSE firmware API guide |
| EVITA Specification | Rev. 2.1 | Automotive security standard |
| ISO 21434 | 2021 | Cybersecurity engineering |

### 10.2 Standards

| Standard | Description |
|----------|-------------|
| NIST SP 800-90B | Random number generation |
| NIST FIPS 140-2 | Cryptographic module requirements |
| PKCS#11 | Cryptographic token interface |
| X.509 | Public key certificates |

---

**Document Revision History**

| Version | Date | Author | Changes |
|---------|------|--------|---------|
| 1.0 | 2025-11-24 | Security Team | Initial release |

---

**For Security Support:**
- Email: security@yourcompany.com
- Security Advisory: [security-advisories](../security/README.md)
- Issue Tracker: https://github.com/redamomo5588/asild-vcu-s32k3/issues
