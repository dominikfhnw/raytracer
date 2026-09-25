#define PI           3.14159265358979323846

#if DEBUG
#define dprintf(...) printf(__VA_ARGS__)
#else
#define dprintf(...)
#define NDEBUG 1
#endif
#define dvec(a) dprintf("vec %-6s%f %f %f\n", #a, a.x, a.y, a.z)

#if FLOAT == float
#define POW(x,y)	powf(x,y)
#define SQRT(x)		sqrtf(x)
#define TAN(x)		tanf(x)
#else
#define POW(x,y)	pow(x,y)
#define SQRT(x)		sqrt(x)
#define TAN(x)		tan(x)
#endif

void render(void*, int, int);
#include "vec3.h"
