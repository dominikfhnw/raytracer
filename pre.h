#if FLOAT_EXCEPTION
# ifndef _GNU_SOURCE
# define _GNU_SOURCE
# endif
#endif

#ifndef NOSIMD
# define _MM_MALLOC_H_INCLUDED
# if __FMA__
#  include <immintrin.h>
# else
#  include <smmintrin.h>
# endif
#endif

#ifndef YOLO
#define YOLO 0
#endif

#define CALL regparm(3)
#define STATIC 1
#if STATIC
#define FUNC __attribute__((CALL)) static
#else
#define FUNC
#endif

#define CONST		__attribute__((const,nothrow,CALL)) static
#define PURE0		__attribute__((pure,nothrow,CALL)) static
#if __cplusplus >= 202002L
#define CONSTEXPR	constexpr CONST
#define CONSTV		constexpr
#define PURE		constexpr PURE0
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

#if __cplusplus
#define NOMANGLE extern "C"
#else
#define NOMANGLE
#endif


#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
