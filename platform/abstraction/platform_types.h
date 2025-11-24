/**
 * @file platform_types.h
 * @brief AUTOSAR Platform Type Definitions for S32K3 Family
 * @version 1.0.0
 * @date 2024-01-15
 * 
 * @copyright Copyright (c) 2024 ASIL-D VCU Project
 * 
 * @details
 * This file provides AUTOSAR-compliant platform-specific type definitions
 * for NXP S32K344 and S32K348 microcontrollers in accordance with:
 * - AUTOSAR R22-11 Platform Types Specification
 * - ISO 26262 ASIL-D requirements
 * - MISRA C:2012 compliance
 * 
 * Supported Platforms:
 * - S32K344 (Cortex-M7, 160 MHz, Lockstep, HSE_B)
 * - S32K348 (Cortex-M7, 240 MHz, Lockstep, HSE_H)
 * 
 * Safety Classification: ASIL-D (Foundation Type Definitions)
 * 
 * @note This file must be included before any other project headers
 * @note All types are sized according to ARM Cortex-M7 architecture
 */

#ifndef PLATFORM_TYPES_H
#define PLATFORM_TYPES_H

/*==================================================================================================
*                                        INCLUDE FILES
==================================================================================================*/
/* None - This is the base platform header */

/*==================================================================================================
*                                      DEFINES AND MACROS
==================================================================================================*/

/**
 * @brief AUTOSAR specification version
 */
#define PLATFORM_VENDOR_ID                  43U  /* NXP Vendor ID */
#define PLATFORM_AR_RELEASE_MAJOR_VERSION   4U
#define PLATFORM_AR_RELEASE_MINOR_VERSION   7U
#define PLATFORM_AR_RELEASE_REVISION_VERSION 0U
#define PLATFORM_SW_MAJOR_VERSION           1U
#define PLATFORM_SW_MINOR_VERSION           0U
#define PLATFORM_SW_PATCH_VERSION           0U

/**
 * @brief CPU architecture identification
 * ARM Cortex-M7 with single-precision FPU
 */
#define CPU_TYPE_8          8U   /**< 8-bit CPU architecture */
#define CPU_TYPE_16         16U  /**< 16-bit CPU architecture */
#define CPU_TYPE_32         32U  /**< 32-bit CPU architecture */
#define CPU_TYPE_64         64U  /**< 64-bit CPU architecture */

#define CPU_TYPE            CPU_TYPE_32  /**< S32K3 is 32-bit ARM Cortex-M7 */

/**
 * @brief Bit order definition
 */
#define MSB_FIRST           0U   /**< Most Significant Bit first (Big Endian) */
#define LSB_FIRST           1U   /**< Least Significant Bit first (Little Endian) */

#define CPU_BIT_ORDER       LSB_FIRST  /**< ARM Cortex-M7 is Little Endian */

/**
 * @brief Byte order definition
 */
#define HIGH_BYTE_FIRST     0U   /**< High byte first (Big Endian) */
#define LOW_BYTE_FIRST      1U   /**< Low byte first (Little Endian) */

#define CPU_BYTE_ORDER      LOW_BYTE_FIRST  /**< ARM Cortex-M7 is Little Endian */

/**
 * @brief Boolean values (AUTOSAR standard)
 */
#ifndef TRUE
    #define TRUE    1U
#endif

#ifndef FALSE
    #define FALSE   0U
#endif

/**
 * @brief Standard API return values
 */
#define E_OK        0U   /**< Function successful */
#define E_NOT_OK    1U   /**< Function failed */

/**
 * @brief NULL pointer definition (MISRA C:2012 compliant)
 */
#ifndef NULL_PTR
    #define NULL_PTR    ((void *)0)
#endif

/**
 * @brief Memory alignment definitions for ARM Cortex-M7
 */
#define PLATFORM_BYTE_ALIGNMENT     1U   /**< Byte alignment */
#define PLATFORM_WORD_ALIGNMENT     4U   /**< Word alignment (32-bit) */
#define PLATFORM_DWORD_ALIGNMENT    8U   /**< Double-word alignment (64-bit) */

/**
 * @brief Platform-specific memory attributes
 */
#define PLATFORM_CACHE_LINE_SIZE    32U  /**< L1 cache line size in bytes */
#define PLATFORM_STACK_ALIGNMENT    8U   /**< Stack alignment requirement */

/*==================================================================================================
*                                             ENUMS
==================================================================================================*/

/* None - Only type definitions in this file */

/*==================================================================================================
*                                STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/

/**
 * @brief Boolean type definition (AUTOSAR standard)
 * @details 8-bit type to represent boolean values
 * Range: FALSE (0) to TRUE (1)
 */
typedef unsigned char boolean;

/**
 * @brief 8-bit unsigned integer
 * @details Range: 0 to 255 (0x00 to 0xFF)
 */
typedef unsigned char uint8;

/**
 * @brief 16-bit unsigned integer
 * @details Range: 0 to 65535 (0x0000 to 0xFFFF)
 */
typedef unsigned short uint16;

/**
 * @brief 32-bit unsigned integer
 * @details Range: 0 to 4294967295 (0x00000000 to 0xFFFFFFFF)
 */
typedef unsigned long uint32;

/**
 * @brief 64-bit unsigned integer
 * @details Range: 0 to 18446744073709551615
 */
typedef unsigned long long uint64;

/**
 * @brief 8-bit signed integer
 * @details Range: -128 to +127 (0x80 to 0x7F)
 */
typedef signed char sint8;

/**
 * @brief 16-bit signed integer
 * @details Range: -32768 to +32767 (0x8000 to 0x7FFF)
 */
typedef signed short sint16;

/**
 * @brief 32-bit signed integer
 * @details Range: -2147483648 to +2147483647 (0x80000000 to 0x7FFFFFFF)
 */
typedef signed long sint32;

/**
 * @brief 64-bit signed integer
 * @details Range: -9223372036854775808 to +9223372036854775807
 */
typedef signed long long sint64;

/**
 * @brief Unsigned integer with platform-optimal size (32-bit on Cortex-M7)
 * @details Used for loop counters and array indices
 */
typedef unsigned long uint_least8;
typedef unsigned long uint_least16;
typedef unsigned long uint_least32;

/**
 * @brief Signed integer with platform-optimal size (32-bit on Cortex-M7)
 */
typedef signed long sint_least8;
typedef signed long sint_least16;
typedef signed long sint_least32;

/**
 * @brief 32-bit single-precision floating point (IEEE 754)
 * @details Range: ±1.175494e-38 to ±3.402823e+38
 * @note Use with caution in safety-critical code
 */
typedef float float32;

/**
 * @brief 64-bit double-precision floating point (IEEE 754)
 * @details Range: ±2.225074e-308 to ±1.797693e+308
 * @note Use with caution in safety-critical code
 */
typedef double float64;

/**
 * @brief Volatile 8-bit unsigned integer
 * @details Used for memory-mapped registers
 */
typedef volatile uint8 vuint8;

/**
 * @brief Volatile 16-bit unsigned integer
 * @details Used for memory-mapped registers
 */
typedef volatile uint16 vuint16;

/**
 * @brief Volatile 32-bit unsigned integer
 * @details Used for memory-mapped registers
 */
typedef volatile uint32 vuint32;

/**
 * @brief Volatile 64-bit unsigned integer
 * @details Used for memory-mapped registers
 */
typedef volatile uint64 vuint64;

/**
 * @brief Volatile 8-bit signed integer
 */
typedef volatile sint8 vsint8;

/**
 * @brief Volatile 16-bit signed integer
 */
typedef volatile sint16 vsint16;

/**
 * @brief Volatile 32-bit signed integer
 */
typedef volatile sint32 vsint32;

/**
 * @brief Volatile 64-bit signed integer
 */
typedef volatile sint64 vsint64;

/*==================================================================================================
*                                PLATFORM-SPECIFIC EXTENDED TYPES
==================================================================================================*/

/**
 * @brief Memory address type (32-bit pointer on Cortex-M7)
 */
typedef uint32 MemAddrType;

/**
 * @brief Memory size type
 */
typedef uint32 MemSizeType;

/**
 * @brief Register width type (32-bit registers)
 */
typedef uint32 RegType;

/**
 * @brief Volatile register type
 */
typedef vuint32 VRegType;

/**
 * @brief Interrupt priority type (0-15 on Cortex-M7)
 */
typedef uint8 IntPriorityType;

/**
 * @brief Interrupt number type
 */
typedef sint16 IntNumberType;

/**
 * @brief CPU core ID type (0 = main core, 1 = lockstep core)
 */
typedef uint8 CoreIdType;

/**
 * @brief Hardware timer type (32-bit counter)
 */
typedef uint32 HwTimerType;

/**
 * @brief Timestamp type (64-bit for high-resolution timing)
 */
typedef uint64 TimestampType;

/**
 * @brief Status type for function returns
 */
typedef uint8 StatusType;

/*==================================================================================================
*                                SAFETY-CRITICAL TYPE EXTENSIONS
==================================================================================================*/

/**
 * @brief Protected boolean with redundant storage (ASIL-D)
 * @details Contains both normal and inverted values for redundancy checking
 */
typedef struct
{
    boolean value;      /**< Primary boolean value */
    boolean inv_value;  /**< Inverted value for redundancy (must be ~value) */
} SafeBoolType;

/**
 * @brief Protected 32-bit value with CRC (ASIL-D)
 * @details Used for critical data with built-in integrity check
 */
typedef struct
{
    uint32 value;       /**< Primary data value */
    uint32 crc;         /**< CRC-32 checksum of value */
} SafeUint32Type;

/**
 * @brief Dual-channel value for voting/comparison (ASIL-D)
 * @details Used in redundant channels for cross-checking
 */
typedef struct
{
    uint32 channel_a;   /**< Channel A value */
    uint32 channel_b;   /**< Channel B value (must match channel_a) */
    uint8  status;      /**< Comparison status flag */
} DualChannelType;

/*==================================================================================================
*                                    LOCKSTEP-SPECIFIC TYPES
==================================================================================================*/

/**
 * @brief Lockstep core status
 */
typedef enum
{
    LOCKSTEP_DISABLED = 0x00U,      /**< Lockstep disabled */
    LOCKSTEP_ENABLED = 0x01U,       /**< Lockstep enabled and operational */
    LOCKSTEP_ERROR = 0x02U,         /**< Lockstep error detected */
    LOCKSTEP_RECOVERING = 0x03U     /**< Lockstep recovery in progress */
} LockstepStatusType;

/**
 * @brief Lockstep error information
 */
typedef struct
{
    uint32 error_address;    /**< Address where mismatch occurred */
    uint32 main_value;       /**< Value from main core */
    uint32 checker_value;    /**< Value from checker core */
    uint32 error_type;       /**< Type of lockstep error */
    uint32 timestamp;        /**< Error timestamp */
} LockstepErrorType;

/*==================================================================================================
*                                COMMUNICATION TYPE DEFINITIONS
==================================================================================================*/

/**
 * @brief CAN identifier type (11-bit standard or 29-bit extended)
 */
typedef uint32 CanIdType;

/**
 * @brief LIN identifier type (6-bit protected ID)
 */
typedef uint8 LinIdType;

/**
 * @brief Ethernet MAC address type
 */
typedef struct
{
    uint8 addr[6];  /**< 6-byte MAC address */
} EthMacAddrType;

/**
 * @brief IP address type (IPv4)
 */
typedef uint32 IpAddrType;

/**
 * @brief VLAN ID type
 */
typedef uint16 VlanIdType;

/*==================================================================================================
*                                    COMPILER ABSTRACTION SUPPORT
==================================================================================================*/

/**
 * @brief Inline function definition (compiler-specific)
 */
#if defined(__GNUC__) || defined(__ghs__) || defined(__DCC__)
    #define INLINE          __inline__
    #define STATIC_INLINE   static __inline__
#elif defined(__ICCARM__) || defined(__IAR_SYSTEMS_ICC__)
    #define INLINE          inline
    #define STATIC_INLINE   static inline
#else
    #define INLINE
    #define STATIC_INLINE   static
#endif

/**
 * @brief Function attribute for unused parameters
 */
#if defined(__GNUC__)
    #define UNUSED(x)   (void)(x)
#else
    #define UNUSED(x)   (x)
#endif

/*==================================================================================================
*                                    ASSERTION SUPPORT
==================================================================================================*/

/**
 * @brief Compile-time assertion (static assert)
 * @details Validates type sizes at compile time
 */
#define PLATFORM_STATIC_ASSERT(condition, msg) \
    typedef char static_assertion_##msg[(condition) ? 1 : -1]

/* Validate platform type sizes */
PLATFORM_STATIC_ASSERT(sizeof(uint8) == 1U, uint8_size_is_1_byte);
PLATFORM_STATIC_ASSERT(sizeof(uint16) == 2U, uint16_size_is_2_bytes);
PLATFORM_STATIC_ASSERT(sizeof(uint32) == 4U, uint32_size_is_4_bytes);
PLATFORM_STATIC_ASSERT(sizeof(uint64) == 8U, uint64_size_is_8_bytes);
PLATFORM_STATIC_ASSERT(sizeof(sint8) == 1U, sint8_size_is_1_byte);
PLATFORM_STATIC_ASSERT(sizeof(sint16) == 2U, sint16_size_is_2_bytes);
PLATFORM_STATIC_ASSERT(sizeof(sint32) == 4U, sint32_size_is_4_bytes);
PLATFORM_STATIC_ASSERT(sizeof(sint64) == 8U, sint64_size_is_8_bytes);
PLATFORM_STATIC_ASSERT(sizeof(float32) == 4U, float32_size_is_4_bytes);
PLATFORM_STATIC_ASSERT(sizeof(float64) == 8U, float64_size_is_8_bytes);
PLATFORM_STATIC_ASSERT(sizeof(boolean) == 1U, boolean_size_is_1_byte);

/*==================================================================================================
*                                       BIT MANIPULATION MACROS
==================================================================================================*/

/**
 * @brief Set bit in register
 */
#define BIT_SET(reg, bit)       ((reg) |= (1UL << (bit)))

/**
 * @brief Clear bit in register
 */
#define BIT_CLEAR(reg, bit)     ((reg) &= ~(1UL << (bit)))

/**
 * @brief Toggle bit in register
 */
#define BIT_TOGGLE(reg, bit)    ((reg) ^= (1UL << (bit)))

/**
 * @brief Check if bit is set
 */
#define BIT_IS_SET(reg, bit)    (((reg) & (1UL << (bit))) != 0UL)

/**
 * @brief Check if bit is clear
 */
#define BIT_IS_CLEAR(reg, bit)  (((reg) & (1UL << (bit))) == 0UL)

/**
 * @brief Read bit value
 */
#define BIT_READ(reg, bit)      (((reg) >> (bit)) & 1UL)

/**
 * @brief Create bit mask
 */
#define BIT_MASK(bit)           (1UL << (bit))

/**
 * @brief Create multi-bit mask
 */
#define BIT_MASK_RANGE(start, end)  (((1UL << ((end) - (start) + 1U)) - 1U) << (start))

/*==================================================================================================
*                                    MIN/MAX MACROS
==================================================================================================*/

/**
 * @brief Minimum of two values
 */
#define MIN(a, b)   (((a) < (b)) ? (a) : (b))

/**
 * @brief Maximum of two values
 */
#define MAX(a, b)   (((a) > (b)) ? (a) : (b))

/**
 * @brief Absolute value
 */
#define ABS(x)      (((x) < 0) ? -(x) : (x))

/**
 * @brief Clamp value to range
 */
#define CLAMP(val, min, max)    (((val) < (min)) ? (min) : (((val) > (max)) ? (max) : (val)))

/*==================================================================================================
*                                ARRAY SIZE CALCULATION
==================================================================================================*/

/**
 * @brief Calculate number of elements in array
 */
#define ARRAY_SIZE(arr)     (sizeof(arr) / sizeof((arr)[0]))

/*==================================================================================================
*                                MEMORY BARRIER MACROS
==================================================================================================*/

/**
 * @brief Data memory barrier (ARM Cortex-M7)
 */
#define DATA_MEMORY_BARRIER()       __asm volatile ("dmb" ::: "memory")

/**
 * @brief Data synchronization barrier (ARM Cortex-M7)
 */
#define DATA_SYNC_BARRIER()         __asm volatile ("dsb" ::: "memory")

/**
 * @brief Instruction synchronization barrier (ARM Cortex-M7)
 */
#define INSTRUCTION_SYNC_BARRIER()  __asm volatile ("isb" ::: "memory")

/*==================================================================================================
*                                    GLOBAL CONSTANTS
==================================================================================================*/

/* None - Only type definitions in this file */

/*==================================================================================================
*                                    GLOBAL VARIABLES
==================================================================================================*/

/* None - Only type definitions in this file */

/*==================================================================================================
*                                    FUNCTION PROTOTYPES
==================================================================================================*/

/* None - Only type definitions in this file */

/*==================================================================================================
*                                          END OF FILE
==================================================================================================*/

#endif /* PLATFORM_TYPES_H */

/**
 * @page platform_types_changelog Revision History
 * 
 * @section platform_types_v1_0_0 Version 1.0.0 (2024-01-15)
 * - Initial release
 * - AUTOSAR R22-11 compliant
 * - ISO 26262 ASIL-D foundation types
 * - Support for S32K344 and S32K348
 * - Lockstep-specific types added
 * - Safety-critical type extensions
 * - MISRA C:2012 compliant
 * 
 * @section platform_types_safety Safety Considerations
 * - All integer types are explicitly sized
 * - Compile-time size validation included
 * - Volatile types for hardware access
 * - SafeBoolType for critical boolean values
 * - DualChannelType for redundant checking
 * - Memory barriers for multi-core synchronization
 * 
 * @section platform_types_usage Usage Guidelines
 * 1. Include this file before all other project headers
 * 2. Use boolean instead of bool or int for boolean values
 * 3. Use explicit-width types (uint8, uint32) instead of int/unsigned
 * 4. Use SafeBoolType for ASIL-D critical boolean flags
 * 5. Use volatile types for memory-mapped registers
 * 6. Use memory barriers when accessing shared lockstep data
 * 
 * @section platform_types_misra MISRA C:2012 Compliance
 * - Rule 1.1: Compliant - All types are well-defined
 * - Rule 4.6: Compliant - No basic numerical types used
 * - Rule 10.1: Compliant - Explicit type definitions
 * - Rule 20.1: Compliant - No #include before declarations
 */
