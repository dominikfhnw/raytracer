typedef struct sphere {
	vec3	center;
	vec3	diffuse;
	vec3	emission;
	FLOAT	radius;
	uint8_t	material;
} sphere;

#if 0
// generates bigger code
typedef uint_least8_t color_t;
typedef int_least16_t coord_t;
#else
typedef uint32_t color_t;
typedef int32_t coord_t;
#endif

CONSTEXPR __attribute__((always_inline)) color_t map_component(FLOAT c)
{
	if (c > 1)
		c = 1;
	return 255 * c;		// 255 -> FLOAT CONST

}

CONST uint32_t colormap(const vec3 color)
{
	vec3 gamma = vsqrt(color);
	color_t r = map_component(gamma[0]);
	color_t g = map_component(gamma[1]);
	color_t b = map_component(gamma[2]);

	//dprintf("lin %u %u %u\n", r, g, b);
	return (r << 16) + (g << 8) + (b << 0); // bgra32
	// return (r << 0) + (g << 8) + (b << 16); // rgba32
}

#if TEXTURE
CONSTEXPR vec3 reverse_color(const uint32_t color)
{
	vec3 result;
	result[2] = pow((FLOAT)(color & 0x00FF) / 255.0, GAMMA);
	result[1] = pow((FLOAT)( (color & 0xFF00)>>8 ) / 255.0, GAMMA);
	result[0] = pow((FLOAT)( (color & 0xFF0000)>>16 ) / 255.0, GAMMA);
	return result;
}
#endif

vec3 randvec(void)
{
#if VRAND
	vec3 rand = vfrand(); // randvec
#else
	vec3 rand = { frand(), frand(), frand() };
#endif
	rand = norm(rand);
	return rand;
}
