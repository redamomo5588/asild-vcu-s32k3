# ASIL-D Compliance Evidence
## Certification Evidence Matrix for S32K3xx VCU

**Document Version:** 1.0  
**Release Date:** November 2025
**Prepared by:** Atlas Automotive Corp. Morocco

---

## Overview
This file provides structured evidence of compliance for ISO 26262 ASIL-D certification of the S32K3xx-based Vehicle Control Unit (VCU) platform. It details the documentation, processes, test results, and traceability required for functional safety assessment and audit.

---

## 1. Safety Lifecycle Documents

| Document | Description | Reference |
|----------|-------------|-----------|
| Hazard Analysis and Risk Assessment (HARA) | Identification and classification of hazards | doc/safety/HARA_v2.0.pdf |
| Functional Safety Concept | High-level safety architecture | doc/safety/FSC_v2.1.pdf |
| Technical Safety Concept  | Detailed hardware-software architecture | doc/safety/TSC_v2.1.pdf |
| SYS, SW, HW Requirements  | Item/system, software, hardware requirements | doc/requirements |
| Implementation Records    | Development artifacts, code audit results | repo/
| Traceability Matrix       | Req->test and req->architecture mapping | doc/safety/Safety_Requirements_Traceability_Matrix.xlsx |
| Safety Manual             | Product safety architecture/process | ../safety_manual.md |
| Validation Report         | Full test and coverage report | doc/safety/SVR_v1.0.pdf |
| Independent Assessment    | Third-party safety audit | doc/safety/ISA_v1.0.pdf |

---

## 2. Safety Case Claims & Evidence

| Claim | Evidence | Location |
|-------|----------|----------|
| HW design meets ASIL-D | FMEDA, test coverage >99% | ../safety_manual.md, FMEDA_S32K3_VCU.xlsx |
| Lockstep & ECC proven | Fault injection, self-tests | ../lockstep_manual.md |
| SW meets ASIL-D process | MC/DC 100%, MISRA checks | ../misra_compliance.md |
| Validation achieved | HIL, fault injection, coverage | Validation Report |
| All chains traceable | Traceability matrix | Traceability_Matrix.xlsx |
| User documentation available | User, safety, lockstep manuals | ../user_manual.md, ../safety_manual.md |

---

## 3. Test and Coverage Metrics

| Metric | Value | Reference |
|--------|-------|-----------|
| MC/DC Coverage | 100% | Coverage Report |
| Unit test coverage | >95% | Unit Test Suite |
| System test coverage | 100% | System Test Suite |
| Fault injection | Full, per-table | Validation Report |
| FMEDA results | SPFM >99%, LFM >96% | FMEDA_S32K3_VCU.xlsx |

---

## 4. Declarations

- This ASIL-D evidence package covers VCU HW/SW and integration up to, but not including, application-specific vehicle logic.
- All primary and supporting evidence, test results, and process conformance evidence are available upon request or audit.

---

**For certification or audit inquiries, contact:**
- Atlas Automotive Corp. Safety & Quality Team
- Email: certification@atlas-automotive.ma
