#define PUSH(x) "push	%k[" #x "]\n\t"
#define POP(x)  "pop	%k[" #x "]\n\t"

// LCG from Turbo Pascal 4.0+
#define RAND_MUL 134775813

__THREAD uint32_t rand_state = 0;

static inline uint32_t my_rand(void)
{
	rand_state *= RAND_MUL;
	rand_state++;
	return rand_state;
}

#define RAND(x)	my_rand(x)
#define RMAX	UINT32_MAX

static FLOAT frand(void)
{
	return (FLOAT) ( ((FLOAT)RAND() - ((FLOAT)RMAX/2.0) ) / (FLOAT)RMAX);
}

static __m128i sserand(void)
{
	__m128i result;
	uint32_t mul = RAND_MUL;
	uint32_t rand;

	__asm__ volatile ( // sserand
		"mov	%[rmem], %[rand]\n\t"
		"imul	%[mul], %[rand], %[rand]\n\t"
		"inc	%[rand]\n\t"
		PUSH(rand)

		"imul	%[mul], %[rand], %[rand]\n\t"
		"inc	%[rand]\n\t"
		PUSH(rand)

		"imul	%[mul], %[rand], %[rand]\n\t"
		"inc	%[rand]\n\t"
		PUSH(rand)

		"imul	%[mul], %[rand], %[rand]\n\t"
		"inc	%[rand]\n\t"
		PUSH(rand)

		"movdqu (%%esp), %[sse]\n\t"
		"mov	%[rand], %[rmem]\n\t"

		POP(rand)
		POP(rand)
		POP(rand)
		POP(rand)

		:
		[rand] "=r" (rand),
		[sse] "=x" (result)
		:
		[rmem] "m" (rand_state),
		[mul] "i" (mul)
		: "memory"
	);
	return result;
}

static __attribute__((noinline)) vec3 vfrand(void)
{
	__m128i rand;
	const FLOAT mul = 1/ (FLOAT)INT32_MAX;
	rand = sserand();
	vec3 randf = _mm_cvtepi32_ps(rand);
	return randf * mul; // vfrand
} // end vfrand

// TODO: this does not create a gaussian distribution in any way or form
static vec3 vgauss(void)
{
	vec3 rand = vfrand(); // vgauss
	vec3 low = _mm_setzero_ps();
	low = _mm_movehl_ps(low, rand);
	vec3 result = rand*low;
	return result;
}


