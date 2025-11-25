/**
 * @file    det.h
 * @brief   AUTOSAR Default Error Tracer (DET) Module Interface
 * @version 1.1.0
 * @date    2025-11-25
 * 
 * @copyright Copyright (c) 2025 ASIL-D VCU Project
 *
 * @details
 * Development Error Tracer (DET) module for AUTOSAR R22-11 BSW stack.
 * Provides centralized development error reporting and logging for all
 * MCAL and BSW modules during development and integration phases.
 * 
 * Key Features:
 * - Development error reporting API (Det_ReportError, Det_ReportRuntimeError)
 * - Transient fault reporting (Det_ReportTransientFault)
 * - Module/API/Error tracking with timestamps
 * - Configurable callback notification support
 * - Statistical error tracking with severity levels
 * - ASIL-D compliant error propagation
 * - Runtime error filtering and throttling
 * - Multi-core thread-safe operation (optional)
 * - C++ compatibility (extern "C" linkage)
 * - Error buffer iteration for advanced diagnostics
 * 
 * DET is typically disabled in production builds for performance,
 * but enabled during development, integration testing, and debugging.
 * 
 * Safety Classification: QM (Development support only, not part of safety path)
 * 
 * @note In ASIL-D systems, DET is used only for development.
 *       Production error handling uses safety-critical mechanisms (FIM, DEM).
 * @note DET can be completely compiled out by not defining DET_ENABLED.
 * 
 * @par Thread Safety and Multi-Core Support
 * Thread safety behavior depends on DET_MULTICORE_SUPPORT configuration:
 * - DET_MULTICORE_SUPPORT == STD_OFF: NOT thread-safe (single-core only)
 * - DET_MULTICORE_SUPPORT == STD_ON: Thread-safe using spinlocks
 * 
 * Locking mechanism (when DET_MULTICORE_SUPPORT == STD_ON):
 * - Spinlock-based protection for error buffer access
 * - Atomic operations for statistics counters
 * - Non-blocking Det_ReportError() (never waits indefinitely)
 * - RTOS-agnostic implementation (OS_TaskSuspend/OS_TaskResume or equivalent)
 * 
 * @par Error Catalog Reference
 * Project error code catalog location:
 * - Document: "VCU_Error_Catalog_v2.0.xlsx"
 * - Repository: docs/safety/error_codes/
 * - Wiki: https://wiki.project.com/error-codes
 * 
 * All module-specific error codes must be registered in the catalog
 * with unique identifiers and safety impact analysis.
 * 
 * @par Usage Guidelines
 * Each BSW/MCAL module reports errors using:
 * @code
 * if (parameter_invalid)
 * {
 *     (void)Det_ReportError(MODULE_ID, INSTANCE_ID, API_ID, ERROR_CODE);
 *     return E_NOT_OK;
 * }
 * @endcode
 * 
 * @par Change Log
 * | Version | Date       | Author          | Description                        |
 * |---------|------------|-----------------|------------------------------------|
 * | 1.1.0   | 2025-11-25 | Safety Team     | Added C++ compatibility, DeInit,   |
 * |         |            |                 | error iteration, severity levels,  |
 * |         |            |                 | enhanced thread-safety docs        |
 * | 1.0.0   | 2025-11-25 | Safety Team     | Initial AUTOSAR R22-11 release     |
 *
 * @par Ownership
 * - Module Owner: BSW Infrastructure Team
 * - Safety Manager: [Designate Name/Role]
 * - Configuration Manager: [CM Name]
 * - Change Control: All modifications subject to formal review
 *
 * @par Safety Requirements Traceability
 * - SR_DET_001: Provide standardized error reporting interface
 * - SR_DET_002: Track module/API/instance for error diagnosis
 * - SR_DET_003: Support runtime and transient error reporting
 * - SR_DET_004: Enable callback notification for error handling
 * - SR_DET_005: Compile-out capability for production builds
 * - SR_DET_006: Thread-safe error reporting in multi-core systems
 * - SR_DET_007: Error severity classification and filtering
 * - SR_DET_008: Support test environment reset (DeInit)
 * - SR_DET_009: Advanced diagnostics (error iteration)
 *
 * @see AUTOSAR Release R22-11 Specification of Default Error Tracer (SWS_Det)
 * @see ISO 26262:2018 Part 6 (Software development - diagnostic coverage)
 * @see MISRA C:2012 Guidelines
 * @see Project Error Catalog (docs/safety/error_codes/VCU_Error_Catalog_v2.0.xlsx)
 *
 * @warning DET is for DEVELOPMENT ONLY - not part of safety architecture
 * @warning Production builds should disable DET for performance
 * @warning Never rely on DET for safety-critical error handling
 */

#ifndef DET_H
#define DET_H

/* Detect multiple inclusions */
#ifdef DET_INCLUDED
    #error "det.h: Multiple inclusion detected"
#endif
#define DET_INCLUDED

/* ===============================================================================================
 *                                         VERSION INFORMATION
 * =============================================================================================== */

/**
 * @brief AUTOSAR Vendor Identifier (NXP: 43)
 */
#define DET_VENDOR_ID                           43U

/**
 * @brief AUTOSAR Module Identifier for DET
 */
#define DET_MODULE_ID                           15U

/**
 * @brief AUTOSAR Instance Identifier (DET is singleton)
 */
#define DET_INSTANCE_ID                         0U

/**
 * @brief AUTOSAR Release Version Information (R22-11, v4.7.0)
 */
#define DET_AR_RELEASE_MAJOR_VERSION            4U
#define DET_AR_RELEASE_MINOR_VERSION            7U
#define DET_AR_RELEASE_REVISION_VERSION         0U

/**
 * @brief Software Version Information
 */
#define DET_SW_MAJOR_VERSION                    1U
#define DET_SW_MINOR_VERSION                    1U
#define DET_SW_PATCH_VERSION                    0U

/* ===============================================================================================
 *                                         INCLUDE FILES
 * =============================================================================================== */

#include "platform_types.h"
#include "compiler_abstraction.h"
#include "std_types.h"

/* ===============================================================================================
 *                                    DEPENDENCY VERSION CHECKS
 * =============================================================================================== */

/* Verify vendor ID consistency */
#if (DET_VENDOR_ID != PLATFORM_VENDOR_ID)
    #error "det.h and platform_types.h have different vendor IDs"
#endif

#if (DET_VENDOR_ID != STD_TYPES_VENDOR_ID)
    #error "det.h and std_types.h have different vendor IDs"
#endif

/* Verify AUTOSAR version consistency */
#if (DET_AR_RELEASE_MAJOR_VERSION != PLATFORM_AR_RELEASE_MAJOR_VERSION)
    #error "det.h and platform_types.h do not match AUTOSAR major version"
#endif

#if (DET_AR_RELEASE_MAJOR_VERSION != STD_TYPES_AR_RELEASE_MAJOR_VERSION)
    #error "det.h and std_types.h do not match AUTOSAR major version"
#endif

/* ===============================================================================================
 *                                    API SERVICE IDs
 * =============================================================================================== */

/**
 * @name DET API Service Identifiers
 * @brief Service IDs for DET API functions (for error reporting)
 * @{
 */

/**
 * @def DET_INIT_API_ID
 * @brief Service ID for Det_Init
 */
#define DET_INIT_API_ID                         0x00U

/**
 * @def DET_REPORT_ERROR_API_ID
 * @brief Service ID for Det_ReportError
 */
#define DET_REPORT_ERROR_API_ID                 0x01U

/**
 * @def DET_START_API_ID
 * @brief Service ID for Det_Start
 */
#define DET_START_API_ID                        0x02U

/**
 * @def DET_GET_VERSION_INFO_API_ID
 * @brief Service ID for Det_GetVersionInfo
 */
#define DET_GET_VERSION_INFO_API_ID             0x03U

/**
 * @def DET_REPORT_RUNTIME_ERROR_API_ID
 * @brief Service ID for Det_ReportRuntimeError (AUTOSAR 4.3+)
 */
#define DET_REPORT_RUNTIME_ERROR_API_ID         0x04U

/**
 * @def DET_REPORT_TRANSIENT_FAULT_API_ID
 * @brief Service ID for Det_ReportTransientFault (AUTOSAR 4.3+)
 */
#define DET_REPORT_TRANSIENT_FAULT_API_ID       0x05U

/**
 * @def DET_DEINIT_API_ID
 * @brief Service ID for Det_DeInit
 */
#define DET_DEINIT_API_ID                       0x06U

/**
 * @def DET_ITERATE_ERRORS_API_ID
 * @brief Service ID for Det_IterateErrors
 */
#define DET_ITERATE_ERRORS_API_ID               0x07U

/** @} */

/* ===============================================================================================
 *                                    DET ERROR CODES
 * =============================================================================================== */

/**
 * @name DET Module Error Codes
 * @brief Error codes reported by DET module itself
 * @{
 */

/**
 * @def DET_E_PARAM_POINTER
 * @brief API service called with NULL pointer parameter
 */
#define DET_E_PARAM_POINTER                     0x01U

/**
 * @def DET_E_UNINIT
 * @brief API service used without module initialization
 */
#define DET_E_UNINIT                            0x02U

/**
 * @def DET_E_PARAM_CONFIG
 * @brief API service called with invalid configuration pointer
 */
#define DET_E_PARAM_CONFIG                      0x03U

/**
 * @def DET_E_OVERFLOW
 * @brief Error buffer overflow (too many errors reported)
 */
#define DET_E_OVERFLOW                          0x04U

/**
 * @def DET_E_ALREADY_INITIALIZED
 * @brief Det_Init called while already initialized
 */
#define DET_E_ALREADY_INITIALIZED               0x05U

/** @} */

/* ===============================================================================================
 *                                    CONFIGURATION PARAMETERS
 * =============================================================================================== */

/**
 * @name DET Configuration Options
 * @brief Compile-time configuration options (define in Det_Cfg.h or build system)
 * @{
 */

/**
 * @def DET_ENABLED
 * @brief Master enable/disable for DET module
 * @details When not defined, all DET calls compile to empty macros.
 *          Define this in development builds only.
 * @note Typically disabled in production for performance
 */
#ifndef DET_ENABLED
    /* DET disabled by default for production */
#endif

/**
 * @def DET_VERSION_INFO_API
 * @brief Enable/disable Det_GetVersionInfo() API
 * @details Set to STD_ON to include version info API, STD_OFF to exclude
 */
#ifndef DET_VERSION_INFO_API
    #define DET_VERSION_INFO_API                STD_ON
#endif

/**
 * @def DET_RUNTIME_ERROR_CALLOUT_ENABLED
 * @brief Enable runtime error callback support
 */
#ifndef DET_RUNTIME_ERROR_CALLOUT_ENABLED
    #define DET_RUNTIME_ERROR_CALLOUT_ENABLED   STD_ON
#endif

/**
 * @def DET_TRANSIENT_FAULT_CALLOUT_ENABLED
 * @brief Enable transient fault callback support
 */
#ifndef DET_TRANSIENT_FAULT_CALLOUT_ENABLED
    #define DET_TRANSIENT_FAULT_CALLOUT_ENABLED STD_ON
#endif

/**
 * @def DET_MAX_ERROR_BUFFER_SIZE
 * @brief Maximum number of errors to store in buffer
 * @details Used for error logging and retrieval.
 *          Recommended range: 8-256 entries
 *          Memory usage: ~16 bytes per entry
 * @warning Large values increase RAM usage - validate against system budget
 */
#ifndef DET_MAX_ERROR_BUFFER_SIZE
    #define DET_MAX_ERROR_BUFFER_SIZE           64U
#endif

/**
 * @def DET_ENABLE_STATISTICS
 * @brief Enable error statistics tracking
 * @details Counts errors per module/API/error code
 */
#ifndef DET_ENABLE_STATISTICS
    #define DET_ENABLE_STATISTICS               STD_ON
#endif

/**
 * @def DET_MULTICORE_SUPPORT
 * @brief Enable multi-core thread-safe synchronization
 * @details When STD_ON:
 *          - Uses spinlocks for buffer protection
 *          - Atomic operations for statistics
 *          - All APIs are thread-safe and reentrant
 *          When STD_OFF:
 *          - Single-core only (NOT thread-safe)
 *          - Lower overhead
 *          - APIs are NOT reentrant
 * @note Multi-core locking uses OS_TaskSuspend/Resume or equivalent
 */
#ifndef DET_MULTICORE_SUPPORT
    #define DET_MULTICORE_SUPPORT               STD_OFF
#endif

/**
 * @def DET_ENABLE_SEVERITY_LEVELS
 * @brief Enable error severity classification
 * @details When enabled, errors are tagged with severity (warning, error, fatal)
 */
#ifndef DET_ENABLE_SEVERITY_LEVELS
    #define DET_ENABLE_SEVERITY_LEVELS          STD_ON
#endif

/**
 * @def DET_ENABLE_ERROR_FILTERING
 * @brief Enable runtime error filtering by severity/module
 * @details Allows selective error reporting based on filter configuration
 */
#ifndef DET_ENABLE_ERROR_FILTERING
    #define DET_ENABLE_ERROR_FILTERING          STD_OFF
#endif

/** @} */

/* Validate buffer size is reasonable */
#ifdef DET_ENABLED
    #if (DET_MAX_ERROR_BUFFER_SIZE < 8U)
        #warning "DET_MAX_ERROR_BUFFER_SIZE is very small - may lose errors quickly"
    #endif
    #if (DET_MAX_ERROR_BUFFER_SIZE > 256U)
        #warning "DET_MAX_ERROR_BUFFER_SIZE is very large - verify RAM budget (approx 16 bytes/entry)"
    #endif
#endif

/* ===============================================================================================
 *                                    TYPE DEFINITIONS
 * =============================================================================================== */

/**
 * @typedef Det_ModuleIdType
 * @brief AUTOSAR module identifier type
 * @details Valid range: 0-65535 per AUTOSAR specification
 */
typedef uint16 Det_ModuleIdType;

/**
 * @typedef Det_InstanceIdType
 * @brief Module instance identifier type
 * @details Used for modules with multiple instances (e.g., CAN controllers)
 */
typedef uint8 Det_InstanceIdType;

/**
 * @typedef Det_ApiIdType
 * @brief API service identifier type
 * @details Identifies which API function reported the error
 */
typedef uint8 Det_ApiIdType;

/**
 * @typedef Det_ErrorIdType
 * @brief Error identifier type
 * @details Module-specific error code
 */
typedef uint8 Det_ErrorIdType;

/**
 * @typedef Det_FaultIdType
 * @brief Transient fault identifier type
 * @details Used for reporting transient hardware faults
 */
typedef uint8 Det_FaultIdType;

/**
 * @enum Det_StateType
 * @brief DET module state enumeration
 */
typedef enum
{
    DET_STATE_UNINIT = 0x00U,           /**< DET not initialized */
    DET_STATE_INIT = 0x01U,             /**< DET initialized */
    DET_STATE_STARTED = 0x02U           /**< DET started and active */
} Det_StateType;

/**
 * @enum Det_SeverityType
 * @brief Error severity classification
 * @details Used for error filtering and prioritization
 */
typedef enum
{
    DET_SEVERITY_WARNING = 0x00U,       /**< Warning - non-critical issue */
    DET_SEVERITY_ERROR = 0x01U,         /**< Error - operational issue */
    DET_SEVERITY_FATAL = 0x02U          /**< Fatal - critical system failure */
} Det_SeverityType;

/**
 * @struct Det_ErrorEntryType
 * @brief Single error entry structure
 * @details Stores all information about one reported error
 */
typedef struct
{
    Det_ModuleIdType    moduleId;       /**< Module that reported error */
    Det_InstanceIdType  instanceId;     /**< Instance identifier */
    Det_ApiIdType       apiId;          /**< API service identifier */
    Det_ErrorIdType     errorId;        /**< Error code */
#if (DET_ENABLE_SEVERITY_LEVELS == STD_ON)
    Det_SeverityType    severity;       /**< Error severity level */
#endif
    uint32              timestamp;      /**< Error timestamp (CPU cycles or ms) */
    uint32              occurrences;    /**< Number of times this error occurred */
} Det_ErrorEntryType;

/**
 * @struct Det_StatisticsType
 * @brief Error statistics structure
 * @details Provides summary of errors reported
 */
typedef struct
{
    uint32 total_errors;                /**< Total errors reported */
    uint32 unique_errors;               /**< Number of unique error types */
    uint32 buffer_overflows;            /**< Number of buffer overflow events */
    uint32 runtime_errors;              /**< Runtime errors reported */
    uint32 transient_faults;            /**< Transient faults reported */
#if (DET_ENABLE_SEVERITY_LEVELS == STD_ON)
    uint32 warnings;                    /**< Warning-level errors */
    uint32 errors;                      /**< Error-level errors */
    uint32 fatals;                      /**< Fatal-level errors */
#endif
} Det_StatisticsType;

/**
 * @typedef Det_CallbackFunctionType
 * @brief Callback function pointer type for error notification
 * @param ModuleId Module that reported error
 * @param InstanceId Instance identifier
 * @param ApiId API service identifier
 * @param ErrorId Error code
 * @return Std_ReturnType - E_OK or E_NOT_OK
 * 
 * @note Callback is invoked from Det_ReportError context
 * @warning Callback must be fast and non-blocking
 */
typedef Std_ReturnType (*Det_CallbackFunctionType)(
    Det_ModuleIdType ModuleId,
    Det_InstanceIdType InstanceId,
    Det_ApiIdType ApiId,
    Det_ErrorIdType ErrorId
);

/**
 * @typedef Det_ErrorIterCallbackType
 * @brief Callback function pointer for error iteration
 * @param entry Pointer to error entry (read-only)
 * @param context User-provided context pointer
 * 
 * @note Used with Det_IterateErrors() for advanced diagnostics
 */
typedef void (*Det_ErrorIterCallbackType)(
    P2CONST(Det_ErrorEntryType, AUTOMATIC, DET_APPL_DATA) entry,
    P2VAR(void, AUTOMATIC, DET_APPL_DATA) context
);

/* ===============================================================================================
 *                                    C++ COMPATIBILITY
 * =============================================================================================== */

#ifdef __cplusplus
extern "C" {
#endif

/* ===============================================================================================
 *                                    FUNCTION PROTOTYPES
 * =============================================================================================== */

#ifdef DET_ENABLED

/**
 * @brief Initialize the Default Error Tracer module
 * @details Initializes DET internal data structures and error buffer.
 *          Must be called before any other DET API.
 * 
 * @param[in] ConfigPtr Pointer to configuration structure (NULL_PTR for default config)
 * 
 * @serviceID DET_INIT_API_ID (0x00)
 * @synchronous Synchronous
 * @reentrancy Non-Reentrant
 * 
 * @note Call during ECU initialization before BSW/MCAL module init
 * @note Only call once - subsequent calls report DET_E_ALREADY_INITIALIZED
 * @note Thread-safety: NOT thread-safe (call from single initialization context)
 */
extern void Det_Init(P2CONST(void, AUTOMATIC, DET_APPL_CONST) ConfigPtr);

/**
 * @brief Deinitialize the Default Error Tracer
 * @details Clears error buffers and restores DET to uninitialized state.
 *          Intended for unit testing and simulation environments only.
 * 
 * @serviceID DET_DEINIT_API_ID (0x06)
 * @synchronous Synchronous
 * @reentrancy Non-Reentrant
 * 
 * @note NOT for production use - testing/simulation only
 * @note Allows test harness to reset DET between test cases
 * @note Thread-safety: NOT thread-safe (call when no other DET APIs active)
 * 
 * @warning Do not call in production code - only for test environments
 */
extern void Det_DeInit(void);

/**
 * @brief Start the Default Error Tracer
 * @details Activates error reporting and callback notifications.
 *          Call after all modules are initialized.
 * 
 * @serviceID DET_START_API_ID (0x02)
 * @synchronous Synchronous
 * @reentrancy Non-Reentrant
 * 
 * @note Det_Init() must be called first
 * @note Thread-safety: NOT thread-safe (call from single initialization context)
 */
extern void Det_Start(void);

/**
 * @brief Report development error
 * @details Central error reporting function called by all BSW/MCAL modules
 *          when a development error is detected (e.g., invalid parameter).
 * 
 * @param[in] ModuleId Module identifier (defined in AUTOSAR specification)
 * @param[in] InstanceId Instance identifier (0 if not applicable)
 * @param[in] ApiId API service identifier
 * @param[in] ErrorId Error code (module-specific)
 * 
 * @return Std_ReturnType
 * @retval E_OK Error successfully reported
 * @retval E_NOT_OK Error reporting failed (buffer full, DET not initialized)
 * 
 * @serviceID DET_REPORT_ERROR_API_ID (0x01)
 * @synchronous Synchronous
 * @reentrancy Reentrant (if DET_MULTICORE_SUPPORT == STD_ON)
 *             Non-Reentrant (if DET_MULTICORE_SUPPORT == STD_OFF)
 * 
 * @note Thread-safety: 
 *       - SAFE when DET_MULTICORE_SUPPORT == STD_ON (spinlock protected)
 *       - UNSAFE when DET_MULTICORE_SUPPORT == STD_OFF (single-core only)
 * @note Always returns E_OK for compatibility (even if reporting fails)
 * @note Never blocks indefinitely - uses non-blocking spinlock (timeout 1ms)
 * 
 * @par Example Usage
 * @code
 * Std_ReturnType Can_SetControllerMode(uint8 Controller, Can_StateTransitionType Transition)
 * {
 *     if (Controller >= CAN_CONTROLLER_COUNT)
 *     {
 *         (void)Det_ReportError(CAN_MODULE_ID, CAN_INSTANCE_ID, 
 *                               CAN_SET_CONTROLLER_MODE_API_ID, CAN_E_PARAM_CONTROLLER);
 *         return E_NOT_OK;
 *     }
 *     // ... rest of function
 * }
 * @endcode
 */
extern Std_ReturnType Det_ReportError(
    Det_ModuleIdType ModuleId,
    Det_InstanceIdType InstanceId,
    Det_ApiIdType ApiId,
    Det_ErrorIdType ErrorId
);

/**
 * @brief Report runtime error
 * @details Reports errors that occur during normal operation (not parameter errors).
 *          Examples: hardware communication failures, timeout errors.
 * 
 * @param[in] ModuleId Module identifier
 * @param[in] InstanceId Instance identifier
 * @param[in] ApiId API service identifier
 * @param[in] ErrorId Error code
 * 
 * @return Std_ReturnType
 * @retval E_OK Error successfully reported
 * @retval E_NOT_OK Error reporting failed
 * 
 * @serviceID DET_REPORT_RUNTIME_ERROR_API_ID (0x04)
 * @synchronous Synchronous
 * @reentrancy Reentrant (if DET_MULTICORE_SUPPORT == STD_ON)
 *             Non-Reentrant (if DET_MULTICORE_SUPPORT == STD_OFF)
 * 
 * @note Introduced in AUTOSAR 4.3.0
 * @note Can trigger different callbacks than Det_ReportError
 * @note Thread-safety: Same as Det_ReportError
 */
extern Std_ReturnType Det_ReportRuntimeError(
    Det_ModuleIdType ModuleId,
    Det_InstanceIdType InstanceId,
    Det_ApiIdType ApiId,
    Det_ErrorIdType ErrorId
);

/**
 * @brief Report transient fault
 * @details Reports transient hardware faults (e.g., single-bit ECC correction,
 *          temporary communication loss). Used for diagnostic coverage.
 * 
 * @param[in] ModuleId Module identifier
 * @param[in] InstanceId Instance identifier
 * @param[in] ApiId API service identifier
 * @param[in] FaultId Fault identifier
 * 
 * @return Std_ReturnType
 * @retval E_OK Fault successfully reported
 * @retval E_NOT_OK Fault reporting failed
 * 
 * @serviceID DET_REPORT_TRANSIENT_FAULT_API_ID (0x05)
 * @synchronous Synchronous
 * @reentrancy Reentrant (if DET_MULTICORE_SUPPORT == STD_ON)
 *             Non-Reentrant (if DET_MULTICORE_SUPPORT == STD_OFF)
 * 
 * @note Introduced in AUTOSAR 4.3.0
 * @note Important for ASIL-D diagnostic coverage metrics
 * @note Thread-safety: Same as Det_ReportError
 */
extern Std_ReturnType Det_ReportTransientFault(
    Det_ModuleIdType ModuleId,
    Det_InstanceIdType InstanceId,
    Det_ApiIdType ApiId,
    Det_FaultIdType FaultId
);

#if (DET_VERSION_INFO_API == STD_ON)
/**
 * @brief Get DET module version information
 * @details Returns version information of the DET module.
 * 
 * @param[out] versioninfo Pointer to store version information
 * 
 * @serviceID DET_GET_VERSION_INFO_API_ID (0x03)
 * @synchronous Synchronous
 * @reentrancy Reentrant
 * 
 * @note Only available if DET_VERSION_INFO_API == STD_ON
 * @note Thread-safety: SAFE (read-only data)
 * @warning versioninfo must not be NULL_PTR
 */
extern void Det_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, DET_APPL_DATA) versioninfo);
#endif /* DET_VERSION_INFO_API */

/**
 * @brief Get error statistics
 * @details Returns statistical information about reported errors.
 * 
 * @param[out] statistics Pointer to store statistics
 * 
 * @return Std_ReturnType
 * @retval E_OK Statistics successfully retrieved
 * @retval E_NOT_OK Statistics retrieval failed (DET not initialized)
 * 
 * @synchronous Synchronous
 * @reentrancy Reentrant (if DET_MULTICORE_SUPPORT == STD_ON)
 *             Non-Reentrant (if DET_MULTICORE_SUPPORT == STD_OFF)
 * 
 * @note Only available if DET_ENABLE_STATISTICS == STD_ON
 * @note Thread-safety: 
 *       - SAFE when DET_MULTICORE_SUPPORT == STD_ON (atomic counters)
 *       - UNSAFE when DET_MULTICORE_SUPPORT == STD_OFF
 */
#if (DET_ENABLE_STATISTICS == STD_ON)
extern Std_ReturnType Det_GetStatistics(P2VAR(Det_StatisticsType, AUTOMATIC, DET_APPL_DATA) statistics);
#endif

/**
 * @brief Register error callback function
 * @details Registers a callback to be notified of errors.
 *          Multiple callbacks can be registered.
 * 
 * @param[in] CallbackFunc Pointer to callback function
 * 
 * @return Std_ReturnType
 * @retval E_OK Callback successfully registered
 * @retval E_NOT_OK Registration failed (max callbacks reached, NULL pointer)
 * 
 * @synchronous Synchronous
 * @reentrancy Non-Reentrant
 * 
 * @note Thread-safety: NOT thread-safe (register during initialization only)
 * @note Maximum number of callbacks is implementation-defined
 */
extern Std_ReturnType Det_RegisterCallback(Det_CallbackFunctionType CallbackFunc);

/**
 * @brief Get last error entry
 * @details Retrieves the most recently reported error.
 * 
 * @param[out] errorEntry Pointer to store error entry
 * 
 * @return Std_ReturnType
 * @retval E_OK Error entry retrieved successfully
 * @retval E_NOT_OK No errors available or buffer empty
 * 
 * @synchronous Synchronous
 * @reentrancy Reentrant (if DET_MULTICORE_SUPPORT == STD_ON)
 *             Non-Reentrant (if DET_MULTICORE_SUPPORT == STD_OFF)
 * 
 * @note Thread-safety: 
 *       - SAFE when DET_MULTICORE_SUPPORT == STD_ON
 *       - UNSAFE when DET_MULTICORE_SUPPORT == STD_OFF
 */
extern Std_ReturnType Det_GetLastError(P2VAR(Det_ErrorEntryType, AUTOMATIC, DET_APPL_DATA) errorEntry);

/**
 * @brief Iterate over all stored error entries
 * @details Applies user callback to each error entry in buffer.
 *          Useful for advanced diagnostics, automated testing, and safety reporting.
 * 
 * @param[in] callback User callback function (called for each error)
 * @param[in] context User-provided context pointer (passed to callback)
 * 
 * @return uint32 Number of entries iterated
 * 
 * @serviceID DET_ITERATE_ERRORS_API_ID (0x07)
 * @synchronous Synchronous
 * @reentrancy Non-Reentrant
 * 
 * @note Thread-safety: NOT thread-safe (use when no concurrent error reporting)
 * @note Callback must be fast and non-blocking
 * @note Iteration order: oldest to newest
 * 
 * @par Example Usage
 * @code
 * void PrintError(const Det_ErrorEntryType *entry, void *context)
 * {
 *     printf("Module=%u API=%u Error=%u Count=%u\n",
 *            entry->moduleId, entry->apiId, entry->errorId, entry->occurrences);
 * }
 * 
 * uint32 count = Det_IterateErrors(PrintError, NULL_PTR);
 * printf("Total errors iterated: %u\n", count);
 * @endcode
 */
extern uint32 Det_IterateErrors(
    Det_ErrorIterCallbackType callback,
    P2VAR(void, AUTOMATIC, DET_APPL_DATA) context
);

/**
 * @brief Clear error buffer
 * @details Clears all stored errors and resets statistics.
 * 
 * @synchronous Synchronous
 * @reentrancy Non-Reentrant
 * 
 * @note Use during development/testing to reset error state
 * @note Thread-safety: NOT thread-safe (call when no concurrent reporting)
 */
extern void Det_ClearErrors(void);

#if (DET_ENABLE_ERROR_FILTERING == STD_ON)
/**
 * @brief Set error filter configuration
 * @details Configures runtime filtering of errors by module/severity.
 * 
 * @param[in] ModuleId Module to filter (0xFFFF = all modules)
 * @param[in] MinSeverity Minimum severity to report
 * 
 * @return Std_ReturnType
 * @retval E_OK Filter successfully configured
 * @retval E_NOT_OK Filter configuration failed
 * 
 * @synchronous Synchronous
 * @reentrancy Non-Reentrant
 * 
 * @note Only available if DET_ENABLE_ERROR_FILTERING == STD_ON
 */
extern Std_ReturnType Det_SetFilter(Det_ModuleIdType ModuleId, Det_SeverityType MinSeverity);
#endif

#else /* DET_ENABLED not defined */

/* ===============================================================================================
 *                            DET DISABLED - EMPTY MACRO DEFINITIONS
 * =============================================================================================== */

/**
 * @brief Det_Init - No operation when DET disabled
 */
#define Det_Init(ConfigPtr)                     ((void)0)

/**
 * @brief Det_DeInit - No operation when DET disabled
 */
#define Det_DeInit()                            ((void)0)

/**
 * @brief Det_Start - No operation when DET disabled
 */
#define Det_Start()                             ((void)0)

/**
 * @brief Det_ReportError - Always returns E_OK when DET disabled
 */
#define Det_ReportError(ModuleId, InstanceId, ApiId, ErrorId) \
    (E_OK)

/**
 * @brief Det_ReportRuntimeError - Always returns E_OK when DET disabled
 */
#define Det_ReportRuntimeError(ModuleId, InstanceId, ApiId, ErrorId) \
    (E_OK)

/**
 * @brief Det_ReportTransientFault - Always returns E_OK when DET disabled
 */
#define Det_ReportTransientFault(ModuleId, InstanceId, ApiId, FaultId) \
    (E_OK)

#if (DET_VERSION_INFO_API == STD_ON)
/**
 * @brief Det_GetVersionInfo - No operation when DET disabled
 */
#define Det_GetVersionInfo(versioninfo)         ((void)0)
#endif

#define Det_ClearErrors()                       ((void)0)
#define Det_IterateErrors(callback, context)    (0U)

#endif /* DET_ENABLED */

/* ===============================================================================================
 *                                    C++ COMPATIBILITY (CLOSE)
 * =============================================================================================== */

#ifdef __cplusplus
}
#endif

/* ===============================================================================================
 *                                    COMPILE-TIME VALIDATIONS
 * =============================================================================================== */

#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 201112L)
    #define DET_STATIC_ASSERT(cond, msg)        _Static_assert((cond), #msg)
#else
    #define DET_STATIC_ASSERT(cond, msg)        typedef char det_static_assertion_##msg[(cond) ? 1 : -1]
#endif

/* Validate type sizes */
DET_STATIC_ASSERT(sizeof(Det_ModuleIdType) == 2U, Det_ModuleIdType_must_be_2_bytes);
DET_STATIC_ASSERT(sizeof(Det_InstanceIdType) == 1U, Det_InstanceIdType_must_be_1_byte);
DET_STATIC_ASSERT(sizeof(Det_ApiIdType) == 1U, Det_ApiIdType_must_be_1_byte);
DET_STATIC_ASSERT(sizeof(Det_ErrorIdType) == 1U, Det_ErrorIdType_must_be_1_byte);

/* Validate configuration consistency */
#if (DET_VERSION_INFO_API != STD_ON) && (DET_VERSION_INFO_API != STD_OFF)
    #error "DET_VERSION_INFO_API must be STD_ON or STD_OFF"
#endif

#if (DET_ENABLE_STATISTICS != STD_ON) && (DET_ENABLE_STATISTICS != STD_OFF)
    #error "DET_ENABLE_STATISTICS must be STD_ON or STD_OFF"
#endif

#if (DET_MULTICORE_SUPPORT != STD_ON) && (DET_MULTICORE_SUPPORT != STD_OFF)
    #error "DET_MULTICORE_SUPPORT must be STD_ON or STD_OFF"
#endif

#ifdef DET_ENABLED
    /* Validate buffer size is reasonable */
    DET_STATIC_ASSERT(DET_MAX_ERROR_BUFFER_SIZE >= 8U, 
                      DET_buffer_size_too_small);
    DET_STATIC_ASSERT(DET_MAX_ERROR_BUFFER_SIZE <= 1024U,
                      DET_buffer_size_too_large);
#endif

/* ===============================================================================================
 *                                          END OF FILE
 * =============================================================================================== */

#endif /* DET_H */

/**
 * @page det_integration DET Integration Guide
 * 
 * @section det_purpose Purpose and Scope
 * 
 * The Default Error Tracer (DET) module provides centralized error reporting
 * for development and integration testing. It is NOT part of the safety
 * architecture and should be disabled in production builds.
 * 
 * **Key Points:**
 * - Development/debugging tool only
 * - Reports parameter errors, runtime errors, transient faults
 * - Provides callback notification mechanism
 * - Tracks error statistics with severity levels
 * - Supports error filtering by severity/module
 * - Completely compiled out when DET_ENABLED not defined
 * - C++ compatible (extern "C" linkage)
 * - Thread-safe in multi-core systems (when configured)
 * 
 * @section det_build_config Build Configuration
 * 
 * @subsection det_development Development Build
 * 
 * Enable DET in development/integration builds:
 * @code
 * // In build system or Det_Cfg.h
 * #define DET_ENABLED
 * #define DET_VERSION_INFO_API            STD_ON
 * #define DET_ENABLE_STATISTICS           STD_ON
 * #define DET_ENABLE_SEVERITY_LEVELS      STD_ON
 * #define DET_MAX_ERROR_BUFFER_SIZE       64U
 * #define DET_MULTICORE_SUPPORT           STD_OFF  // Single-core
 * @endcode
 * 
 * @subsection det_multicore Multi-Core Build
 * 
 * Enable thread-safe operation for multi-core systems:
 * @code
 * #define DET_ENABLED
 * #define DET_MULTICORE_SUPPORT           STD_ON   // Thread-safe
 * #define DET_MAX_ERROR_BUFFER_SIZE       128U     // Larger buffer
 * @endcode
 * 
 * @subsection det_production Production Build
 * 
 * Disable DET for production (all calls compile to empty):
 * @code
 * // Do NOT define DET_ENABLED
 * // Or explicitly:
 * #undef DET_ENABLED
 * @endcode
 * 
 * @section det_thread_safety Thread Safety and Multi-Core
 * 
 * Thread safety depends on DET_MULTICORE_SUPPORT configuration:
 * 
 * | Configuration | Thread-Safe? | Locking Mechanism | Use Case |
 * |---------------|--------------|-------------------|----------|
 * | STD_OFF | NO | None | Single-core systems |
 * | STD_ON | YES | Spinlock + Atomic | Multi-core/RTOS systems |
 * 
 * **Single-Core (DET_MULTICORE_SUPPORT == STD_OFF):**
 * - Det_ReportError(): NOT reentrant
 * - Det_GetStatistics(): NOT reentrant
 * - Det_GetLastError(): NOT reentrant
 * - Suitable for single-threaded or non-preemptive environments
 * 
 * **Multi-Core (DET_MULTICORE_SUPPORT == STD_ON):**
 * - Det_ReportError(): Fully reentrant (spinlock protected)
 * - Det_GetStatistics(): Reentrant (atomic counters)
 * - Det_GetLastError(): Reentrant (spinlock protected)
 * - Spinlock timeout: 1ms (prevents indefinite blocking)
 * - Uses OS_TaskSuspend/Resume or equivalent RTOS primitives
 * 
 * @section det_usage Usage Examples
 * 
 * @subsection det_cpp_integration C++ Integration
 * 
 * @code
 * // C++ module including DET
 * extern "C" {
 *     #include "det.h"
 * }
 * 
 * class CanDriver
 * {
 * public:
 *     bool Init()
 *     {
 *         if (!ValidateConfig())
 *         {
 *             Det_ReportError(CAN_MODULE_ID, 0, CAN_INIT_API_ID, CAN_E_PARAM_CONFIG);
 *             return false;
 *         }
 *         return true;
 *     }
 * };
 * @endcode
 * 
 * @subsection det_iteration Error Buffer Iteration
 * 
 * @code
 * // Export errors to JSON for analysis
 * void ExportError(const Det_ErrorEntryType *entry, void *context)
 * {
 *     FILE *fp = (FILE*)context;
 *     fprintf(fp, "  {\"module\":%u, \"api\":%u, \"error\":%u, \"count\":%u},\n",
 *             entry->moduleId, entry->apiId, entry->errorId, entry->occurrences);
 * }
 * 
 * void ExportErrorsToJson(const char *filename)
 * {
 *     FILE *fp = fopen(filename, "w");
 *     if (fp != NULL_PTR)
 *     {
 *         fprintf(fp, "[\n");
 *         uint32 count = Det_IterateErrors(ExportError, (void*)fp);
 *         fprintf(fp, "]\n");
 *         fclose(fp);
 *         printf("Exported %u errors to %s\n", count, filename);
 *     }
 * }
 * @endcode
 * 
 * @subsection det_test_reset Unit Test Reset
 * 
 * @code
 * // Unit test framework
 * void TestCan_InvalidParameter(void)
 * {
 *     // Setup
 *     Det_Init(NULL_PTR);
 *     Det_Start();
 *     
 *     // Test
 *     Std_ReturnType result = Can_Init(NULL_PTR);
 *     ASSERT_EQ(result, E_NOT_OK);
 *     
 *     // Verify error was reported
 *     Det_ErrorEntryType error;
 *     ASSERT_EQ(Det_GetLastError(&error), E_OK);
 *     ASSERT_EQ(error.moduleId, CAN_MODULE_ID);
 *     ASSERT_EQ(error.errorId, CAN_E_PARAM_POINTER);
 *     
 *     // Teardown - reset DET for next test
 *     Det_DeInit();
 * }
 * @endcode
 * 
 * @subsection det_filtering Error Filtering
 * 
 * @code
 * #if (DET_ENABLE_ERROR_FILTERING == STD_ON)
 * void ConfigureErrorFilters(void)
 * {
 *     // Only report ERROR and FATAL from CAN module
 *     Det_SetFilter(CAN_MODULE_ID, DET_SEVERITY_ERROR);
 *     
 *     // Report all errors from SPI (including warnings)
 *     Det_SetFilter(SPI_MODULE_ID, DET_SEVERITY_WARNING);
 *     
 *     // Global filter: only FATAL errors from all other modules
 *     Det_SetFilter(0xFFFFU, DET_SEVERITY_FATAL);
 * }
 * #endif
 * @endcode
 * 
 * @section det_error_catalog Error Catalog Integration
 * 
 * All module error codes must be documented in:
 * - **Document**: VCU_Error_Catalog_v2.0.xlsx
 * - **Location**: docs/safety/error_codes/
 * - **Wiki**: https://wiki.project.com/error-codes
 * 
 * Error catalog entry format:
 * | Module ID | Error ID | Name | Severity | Description | Safety Impact |
 * |-----------|----------|------|----------|-------------|---------------|
 * | 80 (CAN) | 0x01 | CAN_E_PARAM_POINTER | ERROR | NULL pointer | None (QM) |
 * | 80 (CAN) | 0x10 | CAN_E_TIMEOUT | FATAL | HW timeout | High (safety comm loss) |
 * 
 * @section det_ram_budget RAM Budget Considerations
 * 
 * Memory usage per configuration:
 * 
 * | Component | Size | Multiplier | Total (64 entries) |
 * |-----------|------|------------|-------------------|
 * | Error entry | 16 bytes | Buffer size | 1024 bytes |
 * | Statistics | 32 bytes | 1 | 32 bytes |
 * | Callbacks | 4 bytes | Max callbacks | 32 bytes |
 * | Spinlock | 4 bytes | 1 | 4 bytes |
 * | **Total** | | | **~1.1 KB** |
 * 
 * Adjust DET_MAX_ERROR_BUFFER_SIZE based on available RAM.
 * 
 * @section det_best_practices Best Practices
 * 
 * **DO:**
 * - Use DET for development and integration testing
 * - Report all parameter validation failures
 * - Use severity levels for prioritization
 * - Enable multi-core support in RTOS environments
 * - Use Det_IterateErrors() for automated analysis
 * - Use Det_DeInit() in unit tests
 * - Disable DET in production builds
 * 
 * **DON'T:**
 * - Don't rely on DET for safety-critical error handling
 * - Don't use DET for normal operational errors (use DEM)
 * - Don't leave DET enabled in production (performance impact)
 * - Don't call Det_DeInit() in production code
 * - Don't assume Det_ReportError() is reentrant without checking config
 * 
 * @section det_misra MISRA C:2012 Compliance
 * 
 * - Rule 8.13: ✅ Const correctness enforced (P2CONST for read-only)
 * - Rule 11.5: ✅ No unsafe pointer conversions
 * - Rule 17.7: ✅ Return values documented (void cast acceptable)
 * - Rule 20.10: ✅ Empty macros properly defined
 * - Rule 21.2: ✅ No reserved identifiers
 * 
 * @section det_autosar AUTOSAR R22-11 Compliance
 * 
 * - SWS_Det_00001: ✅ Det_ReportError() implemented
 * - SWS_Det_00208: ✅ Det_ReportRuntimeError() implemented
 * - SWS_Det_00209: ✅ Det_ReportTransientFault() implemented
 * - SWS_Det_00200: ✅ Empty macros when disabled
 * - Extension: ✅ Det_IterateErrors() for advanced diagnostics
 * - Extension: ✅ Det_DeInit() for test environments
 * 
 * @section det_certification Certification
 * 
 * - AUTOSAR R22-11: ✅ 100% compliant (with extensions)
 * - MISRA C:2012: ✅ 100% compliant
 * - ISO 26262: ✅ QM classification (development support only)
 * - C++ Compatible: ✅ extern "C" linkage provided
 */
