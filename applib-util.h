/**
 * @file applib-util.h
 * @brief Useful macros and simple definitions.
 * @author Nicu Tofan <nicu.tofan@gmail.com>
 * @copyright Copyright 2016 piles contributors. All rights reserved.
 * This file is released under the
 * [MIT License](http://opensource.org/licenses/mit-license.html)
 */

#ifndef GUARD_APPLIB_UTIL_H_INCLUDE
#define GUARD_APPLIB_UTIL_H_INCLUDE


// Microsof's Compiler does not define __func__
#ifdef _MSC_VER
#   define __func__ __FUNCTION__
#endif

// make sure NULL is defined
#ifndef NULL
#   define NULL ((void*)0)
#endif


/**
 * @def STRINGIFY
 * @brief Convert symbol to string at compile time.
 * @internal
 */
#ifndef STRINGIFY
#   define STRINGIFY(s) STRINGIFY_HELPER(s)
#   define STRINGIFY_HELPER(s) #s
#endif


/**
 * @def QSTRINGIFY
 * @brief Convert symbol to string at compile time and wrap in a light string class.
 * @internal
 */
#ifndef QSTRINGIFY
#   define QSTRINGIFY(s) QLatin1String (STRINGIFY(s))
#endif


/**
 * @def NULLIFY
 * @brief If the pointer is non-null it deletes it and set it to NULL.
 * @internal
 */
#define NULLIFY(__p__) if (__p__ != NULL) { \
    delete __p__; \
    __p__ = NULL; \
    }


/**
 * @def VAR_UNUSED
 * @brief Hide the warning about unused variables.
 * @internal
 */
#ifndef VAR_UNUSED
#   define VAR_UNUSED(expr) do { (void)(expr); } while (0)
#endif


/**
 * @def BREAKPOINT
 * @brief Only in debug builds - a breakpoint.
 * @internal
 */
#ifndef BREAKPOINT
#ifdef _MSC_VER
#   define BREAKPOINT  __debugbreak()
#else
#   define BREAKPOINT  __builtin_trap()
#endif
#endif // BREAKPOINT


/**
 * @def DEBUG_ON
 * @brief Used to mark portions enabled for debugging.
 * @internal
 *
 * This is not intended to be edited in order to enable or disable
 * debugging globally. Instead, use it to replace code like this:
 * @code
 * #if 1
 * // ...
 * #endif
 * @endcode
 * with code like this:
 * @code
 * #if DEBUG_ON
 * // ...
 * #endif // DEBUG
 * @endcode
 * and replace DEBUG_ON with DEBUG_OFF when the section should be disabled.
 * In this way the debug portions are clearly deimited and sxeparated from
 * sections disabled for other reasons.
 */
#ifndef DEBUG_ON
#   define DEBUG_ON   1
#endif // DEBUG_ON


/**
 * @def DEBUG_OFF
 * @brief Used to mark portions disabled from debugging.
 * @internal
 */
#ifndef DEBUG_OFF
#   define DEBUG_OFF   0
#endif // DEBUG_OFF


/**
 * @def TMP_A
 * @brief C style string from QString.
 * @internal
 */
#define TMP_A(__s__) __s__.toLatin1 ().constData ()


/**
 * @def APPLIB_ASSERT
 * @brief Assert coupled with a break point; prints file, line number, function.
 * @internal
 */
#define APPLIB_ASSERT(a) if (!(a)) {\
        printf("\nASSERT Failed: " STRINGIFY(a) "\n"); \
        printf("%s[%d]: %s\n\n",__FILE__, __LINE__, __func__); \
        BREAKPOINT; }


/**
 * @def APPLIB_FAILPOINT
 * @brief Assert coupled with a break point; prints file, line number, function.
 * @internal
 */
#define APPLIB_FAILPOINT(msg) \
        printf("\nASSERT Failed: " msg "\n"); \
        printf("%s[%d]: %s\n\n",__FILE__, __LINE__, __func__); \
        BREAKPOINT;


/**
 * @def APPLIB_ENTRY
 * @brief Trace the function or method entry.
 * @internal
 */
#define APPLIB_ENTRY(module) printf("-------{ %s ENTRY %s in %s[%d] }-----\n", module, __func__, __FILE__, __LINE__)


/**
 * @def APPLIB_EXIT
 * @brief Trace the function or method exit.
 * @internal
 */
#define APPLIB_EXIT(module) printf("-------{ %s EXIT %s in %s[%d] }-----\n", module, __func__, __FILE__, __LINE__)


/**
 * @def l1s
 * @brief Shorter name for QLatin1String.
 * @internal
 */
#define l1s QLatin1String


#endif // GUARD_APPLIB_UTIL_H_INCLUDE
