// Copyright (C) 2014 Petar Pofuk. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#ifndef OBISIDIAN_BASIC_TYPES_H_
#define OBISIDIAN_BASIC_TYPES_H_

#if defined(OS_LINUX)
#include <stdint.h>

typedef float f32;
typedef double f64;
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

#else // defines(OS_LINUX)
#ifdef _MSC_VER 
#include <xmmintrin.h>
typedef __declspec(deprecated) float f32_t; 
typedef __declspec(deprecated) unsigned __int8 u8_t;
typedef __declspec(deprecated) unsigned __int16 u16_t;
typedef __declspec(deprecated) unsigned __int32 u32_t; 
typedef __declspec(deprecated) unsigned __int64 u64_t; 
typedef __declspec(deprecated) __int8 i8_t; 
typedef __declspec(deprecated) __int16 i16_t; 
typedef __declspec(deprecated) __int32 i32_t; 
typedef __declspec(deprecated) __int64 i64_t;
typedef __declspec(deprecated) __m128 vf32_t;

typedef float f32; 
typedef unsigned __int8 u8;
typedef unsigned __int16 u16;
typedef unsigned __int32 u32; 
typedef unsigned __int64 u64; 
typedef __int8 i8; 
typedef __int16 i16; 
typedef __int32 i32; 
typedef __int64 i64;
typedef __m128 vf32;

#endif // _MSC_VER

#endif  // defined(OS_LINUX)

// A macro to disallow the copy constructor and operator= functions
// This should be used in the private: declarations for a class
// http://google-styleguide.googlecode.com/svn/trunk/cppguide.xml?showone=Copy_Constructors#Copy_Constructors
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);               \
  void operator=(const TypeName&)

#define DISALLOW_COPY(TypeName) TypeName(const TypeName&)

#define DISALLOW_ASSIGN(TypeName) void operator=(const TypeName&)

#endif  // OBISIDIAN_BASIC_TYPES_H_
