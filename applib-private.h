/**
 * @file applib-private.h
 * @brief Declarations for AppLib class
 * @author Nicu Tofan <nicu.tofan@gmail.com>
 * @copyright Copyright 2014 piles contributors. All rights reserved.
 * This file is released under the
 * [MIT License](http://opensource.org/licenses/mit-license.html)
 */

#ifndef GUARD_APPLIB_PRIVATE_H_INCLUDE
#define GUARD_APPLIB_PRIVATE_H_INCLUDE

#include <applib/applib-config.h>

#if 1
#    define APPLIB_DEBUGM printf
#else
#    define APPLIB_DEBUGM black_hole
#endif

#if 0
#    define APPLIB_TRACE_ENTRY printf("APPLIB ENTRY %s in %s[%d]\n", __func__, __FILE__, __LINE__)
#else
#    define APPLIB_TRACE_ENTRY
#endif

#if 0
#    define APPLIB_TRACE_EXIT printf("APPLIB EXIT %s in %s[%d]\n", __func__, __FILE__, __LINE__)
#else
#    define APPLIB_TRACE_EXIT
#endif


static inline void black_hole (...)
{}

#endif // GUARD_APPLIB_PRIVATE_H_INCLUDE
