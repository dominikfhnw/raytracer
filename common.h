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
#define dvec(a)  dprintf("vec %-12s %f %f %f\n",    #a, a[0], a[1], a[2])
#define dvec4(a) dprintf("vec %-12s 0 %f  1 %f  2 %f  3 %f\n", #a, a[0], a[1], a[2], a[3])
#define dv4si(a) dprintf("v4i %-12s 0 %d  1 %d  2 %d  3 %d\n", #a, ((__v4si)a)[0], ((__v4si)a)[1], ((__v4si)a)[2], ((__v4si)a)[3])


#if TRACE
#define tprintf(...) dprintf(__VA_ARGS__)
#define tvec(a) dprintf("vec %-12s%f %f %f\n", #a, a[0], a[1], a[2])
#else
#define tprintf(...)
#define tvec(a)
#endif

#if FLOAT == float
#define POW(x,y)	powf(x,y)
#define SQRT(x)		__builtin_sqrtf(x)
#define TAN(x)		__builtin_tanf(x)
#define FLOAT_MAX	FLT_MAX
#else
#define POW(x,y)	pow(x,y)
#define SQRT(x)		__builtin_sqrt(x)
#define TAN(x)		__builtin_tan(x)
#define FLOAT_MAX	DBL_MAX
#endif

#include "vec3.h"
#include "rand.h"
#include "ray.h"
static void render(const void*);
