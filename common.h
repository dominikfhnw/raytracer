#if DEBUG
#define dprintf(...) printf(__VA_ARGS__)
#else
#define dprintf(...)
#define NDEBUG 1
#endif

#if FLOAT == float
#define POW(x,y)	powf(x,y)
#define SQRT(x)		sqrtf(x)
#else
#define POW(x,y)	pow(x,y)
#define SQRT(x)		sqrt(x)
#endif

void render(void*, int, int);
#include "vec3.h"
