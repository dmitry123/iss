#ifndef __ISS_DEFINE__
#define __ISS_DEFINE__

#include <stdarg.h>
#include <stdlib.h>

#if defined(_WIN32) || defined(_WIN64)
#  define ISS_WINDOWS
#  if defined(_WIN64)
#    define ISS_X64
#    define ISS_X86
#  else
#    define ISS_X86
#  endif
#elif defined(__APPLE__) || defined(__APPLE_CC__)
#  define ISS_APPLE
#  if defined(__amd64__)
#    define ISS_X64
#    define ISS_X86
#  else
#    define ISS_X86
#  endif
#else
#  error "Unsupported platform!"
#endif

#if defined(_MSC_VER)
#  if _MSC_VER == 1800
#    define ISS_MSVC 12
#  elif _MSC_VER == 1700
#    define ISS_MSVC 11
#  elif _MSC_VER == 1600
#    define ISS_MSVC 10
#  else
#    error "Unsupported Microsoft Visual C compiler!";
#  endif
#elif defined(__clang__)
#  define ISS_CLANG
#  if defined(__llvm__)
#    define ISS_LLVM
#  endif
#else
#  error "Unsupported compiler!"
#endif

#if defined(_MSC_VER)
#  define ISS_DO_PRAGMA(x) __pragma(x)
#  define __ISSSTR2__(x) #x
#  define __ISSSTR1__(x) __ISSSTR2__(x)
#  define __ISSMSVCLOC__ __FILE__ "("__ISSSTR1__(__LINE__)") : "
#  define ISS_MSG_PRAGMA(_msg) ISS_DO_PRAGMA(message (__ISSMSVCLOC__ _msg))
#elif defined(__GNUC__)
#  define ISS_DO_PRAGMA(x) _Pragma (#x)
#  define ISS_MSG_PRAGMA(_msg) ISS_DO_PRAGMA(message (_msg))
#else
#  define ISS_DO_PRAGMA(x)
#  define ISS_MSG_PRAGMA(_msg)
#endif

#define ISS_WARNING(x) ISS_MSG_PRAGMA("WARNING: " x)
#define ISS_TODO(x) ISS_MSG_PRAGMA("TODO: " x)

#if !defined(TRUE)
#  define TRUE 1
#endif
#if !defined(FALSE)
#  define FALSE 0
#endif
#if !defined(NULL)
#  define NULL 0
#endif

#if defined(ISS_WINDOWS)
#  if !defined(WIN32_LEAN_AND_MEAN)
#    define WIN32_LEAN_AND_MEAN
#  endif
#  if defined(ISS_MSVC) && !defined(_CRT_SECURE_NO_WARNINGS)
#    define _CRT_SECURE_NO_WARNINGS
#  endif
#  if defined(ISS_MSVC) && !defined(_USE_MATH_DEFINES)
#    define _USE_MATH_DEFINES
#  endif
#endif

typedef char Sint8, *Sint8P;
typedef unsigned char Uint8, *Uint8P;
typedef short Sint16, *Sint16P;
typedef unsigned short Uint16, *Uint16P;
typedef int Sint32, *Sint32P;
typedef unsigned int Uint32, *Uint32P;
typedef long long Sint64, *Sint64P;
typedef unsigned long long Uint64, *Uint64P;
typedef unsigned long long Long, *LongP;
typedef float Float32, *Float32P;
typedef double Float64, *Float64P;
typedef long double Float96, *Float96P;
typedef const char* StringC;
typedef char* String;
typedef unsigned char Bool, *BoolP;
typedef void Void, *VoidP;
typedef class Image *ImagePtr, *const ImagePtrC;

#pragma warning(disable:4251) /* DLL-Import */
#pragma warning(disable:4996) /* Printf/Scanf secure warnings */
#pragma warning(disable:4006) /* DLL-Export */

#ifndef _LIB
#  ifdef CORE_EXPORTS
#    define ISS_API __declspec(dllexport)
#  else
#    define ISS_API __declspec(dllimport)
#  endif
#else
#  define ISS_API
#endif

#endif // ~__ISS_ENGINE_DEFINE__