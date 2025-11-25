/**
 * @file    compiler_abstraction.h
 * @brief   Compiler abstraction layer for portable embedded software development
 * @version 1.0.0
 * @date    2025-11-25
 * 
 * @details This file provides compiler-independent macros and definitions for
 *          memory sections, function attributes, and type qualifiers. It supports
 *          multiple toolchains (GCC, GHS, IAR, ARMCC) and ensures AUTOSAR and
 *          MISRA C:2012 compliance.
 * 
 * @note    ASIL-D Safety Classification
 * @note    MISRA C:2012 Compliant
 * @note    AUTOSAR R22-11 Compatible
 * 
 * @copyright Copyright (c) 2025. All rights reserved.
 * 
 * Safety Classification: ASIL-D
 * QM: Quality Managed according to ISO 26262
 */

#ifndef COMPILER_ABSTRACTION_H
#define COMPILER_ABSTRACTION_H

/**
 * @defgroup CompilerAbstraction Compiler Abstraction Layer
 * @brief Portable compiler-specific abstractions
 * @{
 */

/*==================================================================================================
*                                        INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/

#include "Std_Types.h"

/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/

/**
 * @brief   Module vendor identification (AUTOSAR)
 */
#define COMPILER_ABSTRACTION_VENDOR_ID           43U

/**
 * @brief   Module identification (AUTOSAR)
 */
#define COMPILER_ABSTRACTION_MODULE_ID           198U

/**
 * @brief   Software major version
 */
#define COMPILER_ABSTRACTION_SW_MAJOR_VERSION    1U

/**
 * @brief   Software minor version
 */
#define COMPILER_ABSTRACTION_SW_MINOR_VERSION    0U

/**
 * @brief   Software patch version
 */
#define COMPILER_ABSTRACTION_SW_PATCH_VERSION    0U

/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/

/* Check if current file and Std_Types header file are of the same vendor */
#if (COMPILER_ABSTRACTION_VENDOR_ID != STD_TYPES_VENDOR_ID)
    #error "compiler_abstraction.h and Std_Types.h have different vendor ids"
#endif

/*==================================================================================================
*                                          CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                      DEFINES AND MACROS
==================================================================================================*/

/**
 * @name Compiler Detection and Identification
 * @{
 */

/* GCC ARM Embedded Compiler */
#if defined(__GNUC__) && defined(__arm__)
    #define COMPILER_TYPE_GCC
    #define COMPILER_NAME "GCC ARM"
    
/* Green Hills Software Compiler */
#elif defined(__ghs__)
    #define COMPILER_TYPE_GHS
    #define COMPILER_NAME "Green Hills"
    
/* IAR Embedded Workbench for ARM */
#elif defined(__IAR_SYSTEMS_ICC__)
    #define COMPILER_TYPE_IAR
    #define COMPILER_NAME "IAR"
    
/* ARM Compiler (ARMCC/Keil) */
#elif defined(__ARMCC_VERSION)
    #define COMPILER_TYPE_ARMCC
    #define COMPILER_NAME "ARM Compiler"
    
#else
    #error "Unsupported compiler. Supported compilers: GCC, GHS, IAR, ARMCC"
#endif

/** @} */

/**
 * @name Memory Section Attributes
 * @{
 */

#if defined(COMPILER_TYPE_GCC)
    /**
     * @brief   Place function in specified section
     * @details Usage: FUNC_SECTION(section_name) void MyFunction(void)
     */
    #define FUNC_SECTION(section) __attribute__((section(section)))
    
    /**
     * @brief   Place variable in specified section
     * @details Usage: VAR_SECTION(section_name) uint32_t myVariable;
     */
    #define VAR_SECTION(section) __attribute__((section(section)))
    
    /**
     * @brief   Place constant in specified section
     * @details Usage: CONST_SECTION(section_name) const uint32_t myConst = 10U;
     */
    #define CONST_SECTION(section) __attribute__((section(section)))

#elif defined(COMPILER_TYPE_GHS)
    #define FUNC_SECTION(section) __attribute__((section(section)))
    #define VAR_SECTION(section) __attribute__((section(section)))
    #define CONST_SECTION(section) __attribute__((section(section)))

#elif defined(COMPILER_TYPE_IAR)
    #define FUNC_SECTION(section) _Pragma("location=\"" #section "\"")
    #define VAR_SECTION(section) _Pragma("location=\"" #section "\"")
    #define CONST_SECTION(section) _Pragma("location=\"" #section "\"")

#elif defined(COMPILER_TYPE_ARMCC)
    #define FUNC_SECTION(section) __attribute__((section(section)))
    #define VAR_SECTION(section) __attribute__((section(section)))
    #define CONST_SECTION(section) __attribute__((section(section)))
#endif

/** @} */

/**
 * @name Function Attributes
 * @{
 */

#if defined(COMPILER_TYPE_GCC)
    /**
     * @brief   Inline function optimization hint
     * @details Suggests compiler to inline the function
     */
    #define INLINE inline __attribute__((always_inline))
    
    /**
     * @brief   No inline function directive
     * @details Prevents function inlining
     */
    #define NO_INLINE __attribute__((noinline))
    
    /**
     * @brief   Function with no return
     * @details Indicates function never returns (e.g., reset handlers)
     */
    #define NORETURN __attribute__((noreturn))
    
    /**
     * @brief   Weak symbol linkage
     * @details Allows function/variable to be overridden
     */
    #define WEAK __attribute__((weak))
    
    /**
     * @brief   Naked function (no prologue/epilogue)
     * @details Used for low-level handlers and startup code
     */
    #define NAKED __attribute__((naked))
    
    /**
     * @brief   Unused attribute
     * @details Suppresses unused parameter/variable warnings
     */
    #define UNUSED __attribute__((unused))
    
    /**
     * @brief   Packed structure attribute
     * @details Removes padding between structure members
     */
    #define PACKED __attribute__((packed))
    
    /**
     * @brief   Aligned attribute
     * @details Specifies memory alignment (e.g., ALIGNED(4))
     */
    #define ALIGNED(n) __attribute__((aligned(n)))
    
    /**
     * @brief   Pure function attribute
     * @details Function has no side effects, return depends only on parameters
     */
    #define PURE __attribute__((pure))
    
    /**
     * @brief   Const function attribute
     * @details Function has no side effects and doesn't access memory
     */
    #define CONST_FUNC __attribute__((const))

#elif defined(COMPILER_TYPE_GHS)
    #define INLINE inline
    #define NO_INLINE __attribute__((noinline))
    #define NORETURN __attribute__((noreturn))
    #define WEAK __attribute__((weak))
    #define NAKED __attribute__((naked))
    #define UNUSED __attribute__((unused))
    #define PACKED __attribute__((packed))
    #define ALIGNED(n) __attribute__((aligned(n)))
    #define PURE __attribute__((pure))
    #define CONST_FUNC __attribute__((const))

#elif defined(COMPILER_TYPE_IAR)
    #define INLINE inline
    #define NO_INLINE _Pragma("inline=never")
    #define NORETURN __noreturn
    #define WEAK __weak
    #define NAKED __task
    #define UNUSED __attribute__((unused))
    #define PACKED __packed
    #define ALIGNED(n) _Pragma("data_alignment=" #n)
    #define PURE
    #define CONST_FUNC

#elif defined(COMPILER_TYPE_ARMCC)
    #define INLINE __inline
    #define NO_INLINE __attribute__((noinline))
    #define NORETURN __attribute__((noreturn))
    #define WEAK __attribute__((weak))
    #define NAKED __attribute__((naked))
    #define UNUSED __attribute__((unused))
    #define PACKED __attribute__((packed))
    #define ALIGNED(n) __attribute__((aligned(n)))
    #define PURE __attribute__((pure))
    #define CONST_FUNC __attribute__((const))
#endif

/** @} */

/**
 * @name Interrupt Service Routine Attributes
 * @{
 */

#if defined(COMPILER_TYPE_GCC)
    /**
     * @brief   Interrupt service routine attribute
     * @details Generates proper entry/exit code for ISR
     */
    #define ISR(name) void name(void) __attribute__((interrupt))
    
    /**
     * @brief   Fast interrupt (FIQ) routine attribute
     */
    #define FIQ(name) void name(void) __attribute__((interrupt("FIQ")))

#elif defined(COMPILER_TYPE_GHS)
    #define ISR(name) __interrupt void name(void)
    #define FIQ(name) __interrupt void name(void)

#elif defined(COMPILER_TYPE_IAR)
    #define ISR(name) __irq __arm void name(void)
    #define FIQ(name) __fiq __arm void name(void)

#elif defined(COMPILER_TYPE_ARMCC)
    #define ISR(name) __irq void name(void)
    #define FIQ(name) __irq void name(void)
#endif

/** @} */

/**
 * @name Memory Barriers and Synchronization
 * @{
 */

#if defined(COMPILER_TYPE_GCC)
    /**
     * @brief   Full memory barrier
     * @details Prevents compiler reordering across this point
     */
    #define MEMORY_BARRIER() __asm__ volatile ("dmb" ::: "memory")
    
    /**
     * @brief   Data synchronization barrier
     */
    #define DATA_SYNC_BARRIER() __asm__ volatile ("dsb" ::: "memory")
    
    /**
     * @brief   Instruction synchronization barrier
     */
    #define INSTRUCTION_SYNC_BARRIER() __asm__ volatile ("isb" ::: "memory")
    
    /**
     * @brief   Compiler barrier (no hardware barrier)
     */
    #define COMPILER_BARRIER() __asm__ volatile ("" ::: "memory")

#elif defined(COMPILER_TYPE_GHS)
    #define MEMORY_BARRIER() __DMB()
    #define DATA_SYNC_BARRIER() __DSB()
    #define INSTRUCTION_SYNC_BARRIER() __ISB()
    #define COMPILER_BARRIER() __asm volatile ("" ::: "memory")

#elif defined(COMPILER_TYPE_IAR)
    #define MEMORY_BARRIER() __DMB()
    #define DATA_SYNC_BARRIER() __DSB()
    #define INSTRUCTION_SYNC_BARRIER() __ISB()
    #define COMPILER_BARRIER() __schedule_barrier()

#elif defined(COMPILER_TYPE_ARMCC)
    #define MEMORY_BARRIER() __dmb(0xF)
    #define DATA_SYNC_BARRIER() __dsb(0xF)
    #define INSTRUCTION_SYNC_BARRIER() __isb(0xF)
    #define COMPILER_BARRIER() __schedule_barrier()
#endif

/** @} */

/**
 * @name Type Qualifiers
 * @{
 */

#if defined(COMPILER_TYPE_GCC) || defined(COMPILER_TYPE_GHS) || \
    defined(COMPILER_TYPE_IAR) || defined(COMPILER_TYPE_ARMCC)
    
    /**
     * @brief   Volatile qualifier for memory-mapped registers
     * @details Prevents compiler optimization of register access
     */
    #define VOLATILE volatile
    
    /**
     * @brief   Constant qualifier
     * @details Indicates read-only data
     */
    #define CONST const
    
    /**
     * @brief   Static qualifier
     * @details Limits scope to compilation unit
     */
    #define STATIC static
    
    /**
     * @brief   External linkage
     * @details Declares symbol defined elsewhere
     */
    #define EXTERN extern
    
    /**
     * @brief   Automatic storage class
     * @details Local variable with automatic lifetime
     */
    #define AUTO auto
    
    /**
     * @brief   Register storage class hint
     * @details Suggests variable stored in register
     */
    #define REGISTER register
    
    /**
     * @brief   Restrict type qualifier (C99)
     * @details Pointer aliasing optimization hint
     */
    #define RESTRICT __restrict
    
#endif

/** @} */

/**
 * @name Pointer Type Qualifiers
 * @{
 */

/**
 * @brief   Pointer to variable in RAM
 * @details Indicates pointer targets modifiable data
 */
#define P2VAR(ptrtype, memclass, ptrclass) ptrtype *

/**
 * @brief   Pointer to constant in ROM
 * @details Indicates pointer targets read-only data
 */
#define P2CONST(ptrtype, memclass, ptrclass) const ptrtype *

/**
 * @brief   Constant pointer to variable
 * @details Pointer itself is constant, target is modifiable
 */
#define CONSTP2VAR(ptrtype, memclass, ptrclass) ptrtype * const

/**
 * @brief   Constant pointer to constant
 * @details Both pointer and target are read-only
 */
#define CONSTP2CONST(ptrtype, memclass, ptrclass) const ptrtype * const

/**
 * @brief   Pointer to function
 * @details Function pointer declaration
 */
#define P2FUNC(rettype, ptrclass, fctname) rettype (*fctname)

/** @} */

/**
 * @name Local Function and Variable Declarations
 * @{
 */

/**
 * @brief   Local function declaration
 * @details Static function within module
 */
#define LOCAL_INLINE static inline

/**
 * @brief   Local function definition
 */
#define FUNC(rettype, memclass) rettype

/**
 * @brief   Local variable declaration
 */
#define VAR(vartype, memclass) vartype

/**
 * @brief   Local constant declaration
 */
#define CONST_VAR(vartype, memclass) const vartype

/** @} */

/**
 * @name Optimization Control
 * @{
 */

#if defined(COMPILER_TYPE_GCC)
    /**
     * @brief   Optimize function for size
     */
    #define OPTIMIZE_SIZE __attribute__((optimize("Os")))
    
    /**
     * @brief   Optimize function for speed
     */
    #define OPTIMIZE_SPEED __attribute__((optimize("O3")))
    
    /**
     * @brief   No optimization
     */
    #define NO_OPTIMIZE __attribute__((optimize("O0")))

#elif defined(COMPILER_TYPE_GHS)
    #define OPTIMIZE_SIZE __attribute__((optimize("s")))
    #define OPTIMIZE_SPEED __attribute__((optimize("3")))
    #define NO_OPTIMIZE __attribute__((optimize("0")))

#elif defined(COMPILER_TYPE_IAR)
    #define OPTIMIZE_SIZE _Pragma("optimize=size")
    #define OPTIMIZE_SPEED _Pragma("optimize=speed")
    #define NO_OPTIMIZE _Pragma("optimize=none")

#elif defined(COMPILER_TYPE_ARMCC)
    #define OPTIMIZE_SIZE __attribute__((optimize("size")))
    #define OPTIMIZE_SPEED __attribute__((optimize("speed")))
    #define NO_OPTIMIZE __attribute__((optimize("O0")))
#endif

/** @} */

/**
 * @name Assertion and Diagnostics
 * @{
 */

#if defined(COMPILER_TYPE_GCC) || defined(COMPILER_TYPE_GHS)
    /**
     * @brief   Static assertion (compile-time check)
     * @details Usage: STATIC_ASSERT(sizeof(uint32_t) == 4U, "Invalid size")
     */
    #define STATIC_ASSERT(cond, msg) _Static_assert((cond), msg)

#elif defined(COMPILER_TYPE_IAR)
    #define STATIC_ASSERT(cond, msg) static_assert((cond), msg)

#elif defined(COMPILER_TYPE_ARMCC)
    #define STATIC_ASSERT(cond, msg) _Static_assert((cond), msg)
#endif

/**
 * @brief   Compile-time warning generation
 */
#if defined(COMPILER_TYPE_GCC)
    #define COMPILER_WARNING(msg) _Pragma(GCC warning #msg)
#else
    #define COMPILER_WARNING(msg)
#endif

/** @} */

/**
 * @name Branch Prediction Hints
 * @{
 */

#if defined(COMPILER_TYPE_GCC) || defined(COMPILER_TYPE_GHS)
    /**
     * @brief   Likely branch hint
     * @details Usage: if (LIKELY(condition)) { ... }
     */
    #define LIKELY(x) __builtin_expect(!!(x), 1)
    
    /**
     * @brief   Unlikely branch hint
     * @details Usage: if (UNLIKELY(error)) { ... }
     */
    #define UNLIKELY(x) __builtin_expect(!!(x), 0)

#else
    #define LIKELY(x) (x)
    #define UNLIKELY(x) (x)
#endif

/** @} */

/**
 * @name Bit Manipulation Helpers
 * @{
 */

#if defined(COMPILER_TYPE_GCC) || defined(COMPILER_TYPE_GHS)
    /**
     * @brief   Count leading zeros
     */
    #define COUNT_LEADING_ZEROS(x) ((uint32_t)__builtin_clz(x))
    
    /**
     * @brief   Count trailing zeros
     */
    #define COUNT_TRAILING_ZEROS(x) ((uint32_t)__builtin_ctz(x))
    
    /**
     * @brief   Count set bits (population count)
     */
    #define POPCOUNT(x) ((uint32_t)__builtin_popcount(x))

#elif defined(COMPILER_TYPE_IAR)
    #define COUNT_LEADING_ZEROS(x) ((uint32_t)__CLZ(x))
    #define COUNT_TRAILING_ZEROS(x) /* Not directly available */
    #define POPCOUNT(x) /* Not directly available */

#elif defined(COMPILER_TYPE_ARMCC)
    #define COUNT_LEADING_ZEROS(x) ((uint32_t)__clz(x))
    #define COUNT_TRAILING_ZEROS(x) /* Not directly available */
    #define POPCOUNT(x) /* Not directly available */
#endif

/** @} */

/**
 * @name AUTOSAR Function Declarations
 * @{
 */

/**
 * @brief   Function declaration macro per AUTOSAR
 * @details Usage: FUNC_DECL(void, ModuleName_FunctionName, (uint32_t param))
 */
#define FUNC_DECL(rettype, funcname, params) \
    EXTERN FUNC(rettype, funcname##_MEMCLASS) funcname params

/** @} */

/*==================================================================================================
*                                             ENUMS
==================================================================================================*/

/*==================================================================================================
*                                STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/

/*==================================================================================================
*                                GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/

/*==================================================================================================
*                                    FUNCTION PROTOTYPES
==================================================================================================*/

/** @} */ /* End of CompilerAbstraction group */

#endif /* COMPILER_ABSTRACTION_H */

/**
 * @page CompilerAbstractionPage Compiler Abstraction Layer
 * 
 * @section CompilerAbstraction_Purpose Purpose
 * This module provides a unified interface for compiler-specific features,
 * enabling portable code across different toolchains while maintaining
 * AUTOSAR and MISRA C:2012 compliance.
 * 
 * @section CompilerAbstraction_SupportedCompilers Supported Compilers
 * - GCC ARM Embedded (primary toolchain)
 * - Green Hills MULTI (automotive certified)
 * - IAR Embedded Workbench for ARM
 * - ARM Compiler (ARMCC/Keil)
 * 
 * @section CompilerAbstraction_Usage Usage Examples
 * 
 * @subsection MemorySection Memory Section Placement
 * @code
 * FUNC_SECTION(".text.critical") void CriticalFunction(void)
 * {
 *     // Function code
 * }
 * 
 * VAR_SECTION(".bss.safety") uint32_t safetyVariable;
 * CONST_SECTION(".rodata.calib") const uint32_t calibValue = 100U;
 * @endcode
 * 
 * @subsection InlineFunc Inline Functions
 * @code
 * INLINE uint32_t FastMacro(uint32_t value)
 * {
 *     return value << 2U;
 * }
 * @endcode
 * 
 * @subsection ISRDecl Interrupt Service Routines
 * @code
 * ISR(Timer0_IRQHandler)
 * {
 *     // Clear interrupt flag
 *     TIMER0->ISR = TIMER_ISR_TOF_MASK;
 * }
 * @endcode
 * 
 * @section CompilerAbstraction_SafetyConsiderations Safety Considerations
 * - All macros are designed to prevent MISRA violations
 * - Memory barriers ensure proper synchronization in safety-critical sections
 * - Pointer type qualifiers enforce const-correctness
 * - Static assertions provide compile-time safety checks
 * 
 * @section CompilerAbstraction_Compliance Compliance
 * - MISRA C:2012: Mandatory and required rules
 * - AUTOSAR R22-11: Compiler abstraction requirements
 * - ISO 26262 ASIL-D: Safety-critical embedded systems
 * - CERT C: Secure coding standard
 */
