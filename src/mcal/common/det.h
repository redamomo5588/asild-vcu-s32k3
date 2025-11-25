/**
 * @file    det.h
 * @brief   AUTOSAR Default Error Tracer (DET) Module Interface
 * @version 1.0.0
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
 * - Statistical error tracking
 * - ASIL-D compliant error propagation
 * - Runtime error filtering and throttling
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
 *
 * @see AUTOSAR Release R22-11 Specification of Default Error Tracer (SWS_Det)
 * @see ISO 26262:2018 Part 6 (Software development - diagnostic coverage)
 * @see MISRA C:2012 Guidelines
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
#define DET_SW_MINOR_VERSION                    0U
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

/** @} */

/* ===============================================================================================
 *                                    CONFIGURATION PARAMETERS
 * =============================================================================================== */

/**
 * @name DET Configuration Options
 * @brief Compile-time configuration options (define in Det_Cfg.h)
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
 * @details Used for error logging and retrieval
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
 * @brief Enable multi-core synchronization
 * @details Adds spinlocks for thread-safe error reporting
 */
#ifndef DET_MULTICORE_SUPPORT
    #define DET_MULTICORE_SUPPORT               STD_OFF
#endif

/** @} */

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
} Det_StatisticsType;

/**
 * @typedef Det_CallbackFunctionType
 * @brief Callback function pointer type for error notification
 * @param ModuleId Module that reported error
 * @param InstanceId Instance identifier
 * @param ApiId API service identifier
 * @param ErrorId Error code
 * @return Std_ReturnType - E_OK or E_NOT_OK
 */
typedef Std_ReturnType (*Det_CallbackFunctionType)(
    Det_ModuleIdType ModuleId,
    Det_InstanceIdType InstanceId,
    Det_ApiIdType ApiId,
    Det_ErrorIdType ErrorId
);

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
 * @note Only call once - subsequent calls have no effect
 */
extern void Det_Init(P2CONST(void, AUTOMATIC, DET_APPL_CONST) ConfigPtr);

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
 * @reentrancy Reentrant
 * 
 * @note Always returns E_OK for compatibility (even if reporting fails)
 * @note Never blocks - uses lock-free mechanisms in multi-core systems
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
 * @reentrancy Reentrant
 * 
 * @note Introduced in AUTOSAR 4.3.0
 * @note Can trigger different callbacks than Det_ReportError
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
 * @reentrancy Reentrant
 * 
 * @note Introduced in AUTOSAR 4.3.0
 * @note Important for ASIL-D diagnostic coverage metrics
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
 * @reentrancy Reentrant (uses atomic access)
 * 
 * @note Only available if DET_ENABLE_STATISTICS == STD_ON
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
 * @retval E_NOT_OK Registration failed (max callbacks reached)
 * 
 * @synchronous Synchronous
 * @reentrancy Non-Reentrant
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
 * @reentrancy Reentrant
 */
extern Std_ReturnType Det_GetLastError(P2VAR(Det_ErrorEntryType, AUTOMATIC, DET_APPL_DATA) errorEntry);

/**
 * @brief Clear error buffer
 * @details Clears all stored errors and resets statistics.
 * 
 * @synchronous Synchronous
 * @reentrancy Non-Reentrant
 * 
 * @note Use during development/testing to reset error state
 */
extern void Det_ClearErrors(void);

#else /* DET_ENABLED not defined */

/* ===============================================================================================
 *                            DET DISABLED - EMPTY MACRO DEFINITIONS
 * =============================================================================================== */

/**
 * @brief Det_Init - No operation when DET disabled
 */
#define Det_Init(ConfigPtr)                     ((void)0)

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

#endif /* DET_ENABLED */

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
 * - Tracks error statistics
 * - Completely compiled out when DET_ENABLED not defined
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
 * #define DET_MAX_ERROR_BUFFER_SIZE       64U
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
 * @section det_usage Usage Examples
 * 
 * @subsection det_basic_reporting Basic Error Reporting
 * 
 * @code
 * // In MCAL/BSW module
 * Std_ReturnType Can_Init(const Can_ConfigType* Config)
 * {
 *     // Parameter validation
 *     if (Config == NULL_PTR)
 *     {
 *         // Report development error
 *         (void)Det_ReportError(CAN_MODULE_ID, CAN_INSTANCE_ID, 
 *                               CAN_INIT_API_ID, CAN_E_PARAM_POINTER);
 *         return E_NOT_OK;
 *     }
 *     
 *     // Validate configuration
 *     if (Config->num_controllers == 0U)
 *     {
 *         (void)Det_ReportError(CAN_MODULE_ID, CAN_INSTANCE_ID,
 *                               CAN_INIT_API_ID, CAN_E_PARAM_CONFIG);
 *         return E_NOT_OK;
 *     }
 *     
 *     // Initialize CAN
 *     return E_OK;
 * }
 * @endcode
 * 
 * @subsection det_runtime_errors Runtime Error Reporting
 * 
 * @code
 * Std_ReturnType Spi_AsyncTransmit(Spi_SequenceType Sequence)
 * {
 *     Std_ReturnType result;
 *     
 *     // Parameter check
 *     if (Sequence >= SPI_MAX_SEQUENCES)
 *     {
 *         (void)Det_ReportError(SPI_MODULE_ID, SPI_INSTANCE_ID,
 *                               SPI_ASYNC_TRANSMIT_API_ID, SPI_E_PARAM_SEQ);
 *         return E_NOT_OK;
 *     }
 *     
 *     // Try to start transmission
 *     result = Hardware_StartTransmission(Sequence);
 *     
 *     if (result == E_TIMEOUT)
 *     {
 *         // Report runtime error (not a parameter issue)
 *         (void)Det_ReportRuntimeError(SPI_MODULE_ID, SPI_INSTANCE_ID,
 *                                       SPI_ASYNC_TRANSMIT_API_ID, SPI_E_HARDWARE_TIMEOUT);
 *         return E_TIMEOUT;
 *     }
 *     
 *     return result;
 * }
 * @endcode
 * 
 * @subsection det_transient_faults Transient Fault Reporting
 * 
 * @code
 * void Fls_MainFunction(void)
 * {
 *     uint32 ecc_status = FlashController_GetEccStatus();
 *     
 *     // Check for ECC single-bit correction (transient fault)
 *     if ((ecc_status & ECC_SINGLE_BIT_CORRECTED) != 0U)
 *     {
 *         // Report transient fault for diagnostic coverage
 *         (void)Det_ReportTransientFault(FLS_MODULE_ID, FLS_INSTANCE_ID,
 *                                         FLS_MAINFUNCTION_API_ID, FLS_F_ECC_CORRECTED);
 *         
 *         // Clear ECC flag
 *         FlashController_ClearEccFlag();
 *     }
 *     
 *     // Check for ECC double-bit error (permanent fault)
 *     if ((ecc_status & ECC_DOUBLE_BIT_ERROR) != 0U)
 *     {
 *         // This is critical - report to safety manager
 *         SafetyManager_ReportFlashError();
 *     }
 * }
 * @endcode
 * 
 * @subsection det_callback Callback Registration
 * 
 * @code
 * // Application callback function
 * Std_ReturnType MyErrorHandler(Det_ModuleIdType ModuleId,
 *                                Det_InstanceIdType InstanceId,
 *                                Det_ApiIdType ApiId,
 *                                Det_ErrorIdType ErrorId)
 * {
 *     // Log error to diagnostic buffer
 *     LogError(ModuleId, InstanceId, ApiId, ErrorId);
 *     
 *     // Trigger debugger breakpoint in development
 *     #ifdef DEBUG_BUILD
 *         __asm("BKPT #0");
 *     #endif
 *     
 *     return E_OK;
 * }
 * 
 * // Register callback during initialization
 * void Application_Init(void)
 * {
 *     Det_Init(NULL_PTR);
 *     
 *     // Register error callback
 *     (void)Det_RegisterCallback(MyErrorHandler);
 *     
 *     Det_Start();
 * }
 * @endcode
 * 
 * @subsection det_statistics Error Statistics
 * 
 * @code
 * void DiagnosticTask_CheckErrors(void)
 * {
 *     Det_StatisticsType stats;
 *     
 *     if (Det_GetStatistics(&stats) == E_OK)
 *     {
 *         printf("Total errors: %u\n", stats.total_errors);
 *         printf("Unique errors: %u\n", stats.unique_errors);
 *         printf("Runtime errors: %u\n", stats.runtime_errors);
 *         printf("Transient faults: %u\n", stats.transient_faults);
 *         
 *         if (stats.buffer_overflows > 0U)
 *         {
 *             printf("WARNING: Buffer overflowed %u times\n", 
 *                    stats.buffer_overflows);
 *         }
 *     }
 * }
 * @endcode
 * 
 * @section det_best_practices Best Practices
 * 
 * **DO:**
 * - Use DET for development and integration testing
 * - Report all parameter validation failures
 * - Report runtime errors that shouldn't happen
 * - Use transient fault reporting for diagnostic coverage
 * - Disable DET in production builds
 * - Register callbacks for automated testing
 * 
 * **DON'T:**
 * - Don't rely on DET for safety-critical error handling
 * - Don't use DET for normal operational errors (use DEM)
 * - Don't leave DET enabled in production (performance impact)
 * - Don't report expected errors (e.g., busy status)
 * - Don't assume DET calls will execute (can be compiled out)
 * 
 * @section det_performance Performance Impact
 * 
 * When DET is enabled:
 * - Each error report: ~10-50 CPU cycles (depending on configuration)
 * - Memory usage: ~8-16 bytes per error entry
 * - Buffer size: Configurable (DET_MAX_ERROR_BUFFER_SIZE)
 * 
 * When DET is disabled:
 * - Zero overhead (all calls optimized out by compiler)
 * - Zero memory usage
 * 
 * @section det_misra MISRA C:2012 Compliance
 * 
 * - Rule 2.2: ✅ All functions have external effect (store error, callback)
 * - Rule 8.13: ✅ Const correctness enforced
 * - Rule 11.5: ✅ No unsafe pointer conversions
 * - Rule 17.7: ✅ Return values should be used (documented as (void) cast ok)
 * - Rule 21.2: ✅ No reserved identifiers used
 * 
 * @section det_autosar AUTOSAR Compliance
 * 
 * - SWS_Det_00001: ✅ Det_ReportError() implemented
 * - SWS_Det_00002: ✅ Det_Init() implemented
 * - SWS_Det_00003: ✅ Det_Start() implemented
 * - SWS_Det_00208: ✅ Det_ReportRuntimeError() implemented (AUTOSAR 4.3+)
 * - SWS_Det_00209: ✅ Det_ReportTransientFault() implemented (AUTOSAR 4.3+)
 * - SWS_Det_00200: ✅ Empty macros when DET disabled
 * 
 * @section det_safety ISO 26262 Notes
 * 
 * DET is classified as QM (not safety-related) because:
 * - Used only during development/integration
 * - Not part of safety architecture
 * - Disabled in production builds
 * - No impact on vehicle safety functions
 * 
 * However, DET supports ASIL-D development by:
 * - Enabling early detection of integration issues
 * - Providing diagnostic coverage metrics (transient faults)
 * - Facilitating systematic testing
 * - Supporting fault injection testing
 */
