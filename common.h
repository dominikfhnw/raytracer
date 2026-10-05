#if FLOAT_EXCEPTION
#include <fenv.h>
#define ENABLE_FLOAT_EXCEPTIONS feenableexcept( FE_DIVBYZERO | FE_INVALID | FE_OVERFLOW | FE_UNDERFLOW )
#else
#define ENABLE_FLOAT_EXCEPTIONS
#endif

#if NO_TLS
#define __THREAD
#else
#define __THREAD __thread
#endif

#include <float.h>
#define PI		3.14159265358979323846

#if DEBUG
#define dprintf(...) printf(__VA_ARGS__)
#else
#define dprintf(...)
#define NDEBUG 1
#endif
#define dvec(a) dprintf("vec %-8s%f %f %f\n", #a, a.x, a.y, a.z)

#if TRACE
#define tprintf(...) dprintf(__VA_ARGS__)
#define tvec(a) dprintf("vec %-8s%f %f %f\n", #a, a.x, a.y, a.z)
#else
#define tprintf(...)
#define tvec(a)
#endif

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
#include "vec3.h"
#include "rand.h"
#include "ray.h"
