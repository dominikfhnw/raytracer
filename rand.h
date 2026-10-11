#if XORSHIFT
// G. Marsaglia, ‘Xorshift RNGs’, Journal of Statistical Software, vol. 8, pp. 1–6, Jul. 2003, doi: 10.18637/jss.v008.i14.

//__THREAD uint32_t rand_state = 2463534242;
__THREAD uint32_t rand_state = 0;

static uint32_t my_rand(void)
{
	//uint32_t x = rand_state;
#if 0
	x ^= x << 13;
	x ^= x >> 17;
	x ^= x << 5;
#else
	rand_state *= 134775813;
	rand_state++;
#endif
	//rand_state = x;
	return rand_state;
}
#define RAND(x)	my_rand(x)
#define RMAX	UINT32_MAX

#else

#include <stdlib.h>
#define RAND(x)	rand(x)
#define RMAX	RAND_MAX

#endif


#if XORSHIFT && FLOAT == float
// inspired by https://blog.bithole.dev/blogposts/random-float/
// returns a floating point number between -1 and +1
static float frand(void)
{
	int total_bits		=  32;
	int fraction_bits	=  23;
	int exp_offset		= 127;
	uint32_t mask;
	uint32_t exp;
	float off;

	off = 3; // FLOAT CONST
	exp = 1;

	mask = (exp+exp_offset) << fraction_bits;
	union { uint32_t u32; float f; } u = { .u32 = my_rand() >> (total_bits-fraction_bits) | mask };

	return u.f - off;
}

#else
// the same, but worse
static FLOAT frand(void)
{
	return (FLOAT) ( ((FLOAT)RAND() - ((FLOAT)RMAX/2.0) ) / (FLOAT)RMAX);
}
#endif

