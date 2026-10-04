#if XORSHIFT
// G. Marsaglia, ‘Xorshift RNGs’, Journal of Statistical Software, vol. 8, pp. 1–6, Jul. 2003, doi: 10.18637/jss.v008.i14.

TLS uint32_t xorshift_state = 2463534242;

CONSTEXPR uint32_t xorshift32(void)
{
	uint32_t x = xorshift_state;
	x ^= x << 13;
	x ^= x >> 17;
	x ^= x << 5;
	xorshift_state = x;
	return x;
}
#define RAND(x)	xorshift32(x)
#define RMAX	UINT32_MAX

#else

#include <stdlib.h>
#define RAND(x)	rand(x)
#define RMAX	RAND_MAX

#endif


#if XORSHIFT && FLOAT == float
// inspired by https://blog.bithole.dev/blogposts/random-float/
// returns a floating point number between -1 and +1
CONSTEXPR float frand(void)
{
	int total_bits		=  32;
	int fraction_bits	=  23;
	int exp_offset		= 127;
	uint32_t mask;
	uint32_t exp;
	float off;

	off = 3;
	exp = 1;

	mask = (exp+exp_offset) << fraction_bits;
	union { uint32_t u32; float f; } u = { .u32 = xorshift32() >> (total_bits-fraction_bits) | mask };

	return u.f - off;
}

#else
// the same, but worse
CONSTEXPR FLOAT frand(void)
{
	return (FLOAT) ( ((FLOAT)RAND() - ((FLOAT)RMAX/2.0) ) / (FLOAT)RMAX);
}
#endif

