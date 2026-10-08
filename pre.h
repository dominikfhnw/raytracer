#if FLOAT_EXCEPTION
#define _GNU_SOURCE
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
#define ONLYCE		constexpr static
#else
#define CONSTEXPR	CONST
#define CONSTV		const
#define PURE		PURE0
#define ONLYCE		static
#endif

#if __cplusplus
#define NOMANGLE extern "C"
#else
#define NOMANGLE
#endif


#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
