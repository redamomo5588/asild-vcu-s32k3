# MISRA Compliance Statement
## S32K3xx VCU - MISRA C:2012 Process & Justification

**Version:** 1.0  
**Last Updated:** November 2025
**Prepared by:** Atlas Automotive Corp. Morocco

---

## Scope
This document outlines the process and results of achieving MISRA C:2012 (including AMD1/AMD2) compliance for all safety and core firmware developed for the S32K3xx-based VCU platform.

---

## 1. Compliance Process

- All source code developed for this VCU is written in C99 and subjected to static analysis for MISRA compliance.
- Code is analyzed using the *[insert tool, e.g. LDRA/TI Code Composer/MISRA Checker]* with most recent MISRA ruleset.
- All deviations are logged, assessed for safety impact, approved by Safety/Quality board, and documented in a deviations report.
- Peer reviews and automated CI checks enforce rule adherence.

## 2. Deviations & Justifications
- Allowable deviations (e.g., necessary type casting, hardware register access) are justified in accordance with safety plans.

## 3. Compliance Metrics
| Rule Set | Level | Violations | Allowed Deviations | Comments |
|----------|-------|-----------|--------------------|----------|
| MISRA C:2012 (Core) | Full | 0 (blocking) | List in dev_report.md | Blocking violations fail build |
| MISRA C:2012 AMD1/AMD2 | Full | 0 (blocking) | On record | |

## 4. Deviation Log (Example)
| File | Rule | Explanation/Justification |
|------|------|--------------------------|
| can_drv.c | 11.4 | Hardware register aliasing, safe by design |
| clock_mgr.c | 1.3 | Macro for register, no run-time eval |  

---

## 5. Declaration
> All code shipped for this product is free from blocking MISRA C:2012 violations, and any deviations are justified, reviewed, and tracked under the Safety Quality Plan.

**For process details or full deviation records, request:**  
Atlas Automotive Corp. Quality Team  
Email: quality@atlas-automotive.ma
