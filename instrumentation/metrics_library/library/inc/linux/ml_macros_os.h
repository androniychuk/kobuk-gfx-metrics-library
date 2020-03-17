/******************************************************************************\

Copyright © 2020, Intel Corporation

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
IN THE SOFTWARE.

@file ml_macros_os.h

@brief Os specific macros.

\******************************************************************************/

#pragma once
// clang-format off

//////////////////////////////////////////////////////////////////////////
/// @brief Pragma definition.
//////////////////////////////////////////////////////////////////////////
#define ML_PRAGMA( x )              _Pragma( #x )

//////////////////////////////////////////////////////////////////////////
/// @brief Inline macros.
//////////////////////////////////////////////////////////////////////////
#define ML_INLINE                   inline
#define ML_FORCE_INLINE             __attribute__( ( always_inline ) ) inline

//////////////////////////////////////////////////////////////////////////
/// @brief Export definition.
//////////////////////////////////////////////////////////////////////////
#define ML_EXPORT( RETURN_TYPE )    RETURN_TYPE __attribute__( ( visibility( "default" ) ) )

//////////////////////////////////////////////////////////////////////////
/// @brief ML_LINUX.
//////////////////////////////////////////////////////////////////////////
#if defined( __linux__ )
    #define ML_LINUX                1
#else
    #define ML_LINUX                0
#endif

// clang-format on
