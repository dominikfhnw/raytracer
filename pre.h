#if FLOAT_EXCEPTION
# ifndef _GNU_SOURCE
# define _GNU_SOURCE
# endif
#endif

#define _MM_MALLOC_H_INCLUDED
#include <smmintrin.h>

#ifndef YOLO
#define YOLO 0
#endif

#define CALL regparm(3)
#define STATIC 1
#if STATIC
#define FUNC static
#else
#define FUNC
#endif

#define CONST		__attribute__((const,nothrow)) static
#define PURE0		__attribute__((pure,nothrow)) static
#if __cplusplus >= 202002L
#define CONSTEXPR	constexpr CONST
#define CONSTV		constexpr
#define PURE		constexpr PURE0
#define _GLIBCXX_OS_DEFINES 1
#include <type_traits>
#define IS_CONSTANT	std::is_constant_evaluated()
#else
#define CONSTEXPR	CONST
#define CONSTV		const
#define PURE		PURE0
#define IS_CONSTANT	false
#endif

#ifndef NOSIMD
#define NOSIMD 0
#endif
#define SISD (NOSIMD || IS_CONSTANT)

#ifndef __SSE__
#undef  NOSIMD
#define NOSIMD 1
#endif

#if __cplusplus
#define NOMANGLE extern "C"
#else
#define NOMANGLE
#endif


#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
