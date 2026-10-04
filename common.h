#include <float.h>
#include <stdint.h>
#include <stdbool.h>
#define PI		3.14159265358979323846

#define CONST		__attribute__((const,nothrow)) static
#if __cplusplus >= 202002L
#define CONSTEXPR	constexpr CONST
#define CONSTV		constexpr
#else
#define CONSTEXPR	CONST
#define CONSTV		const
#endif

#if DEBUG
#define dprintf(...) printf(__VA_ARGS__)
#else
#define dprintf(...)
#define NDEBUG 1
#endif
#define dvec(a) dprintf("vec %-8s%f %f %f\n", #a, a.x, a.y, a.z)

#if FLOAT == float
#define POW(x,y)	powf(x,y)
#define SQRT(x)		sqrtf(x)
#define TAN(x)		__builtin_tanf(x)
#define FLOAT_MAX	FLT_MAX
#else
#define POW(x,y)	pow(x,y)
#define SQRT(x)		sqrt(x)
#define TAN(x)		__builtin_tan(x)
#define FLOAT_MAX	DBL_MAX
#endif

static void render(void*, int, int);
#include "rand.h"
#include "vec3.h"
#include "ray.h"
