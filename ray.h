typedef struct sphere {
	vec3	center;
	FLOAT	radius;
	vec3	diffuse;
	vec3	emission;
} sphere;


CONSTEXPR uint8_t map_component(FLOAT c)
{
	if (c > 1)
		c = 1;
	if (c < 0)
		c = 0;

	#if YOLO				// assumes GAMMA == 2.0
		return 255 * SQRT(c);
	#else
		return 255 * POW(c, 1/GAMMA);
	#endif

}

CONSTEXPR uint32_t colormap(const vec3 color)
{
	uint8_t r = map_component(color.x);
	uint8_t g = map_component(color.y);
	uint8_t b = map_component(color.z);

	//dprintf("lin %u %u %u\n", r, g, b);
	return (r << 16) + (g << 8) + (b << 0); // bgra32
	// return (r << 0) + (g << 8) + (b << 16); // rgba32
}

#if NEWRAND
ONLYCE vec3 randvecNEW(void)
{
	float u = int_to_float(xorshift32());
	assert(u >= 0);
	assert(u <= 1);
	float v = int_to_float(xorshift32());
	assert(v >= 0);
	assert(v <= 1);
	float rad = PI/180;
	float theta = 2.0f * PI * u;
	float phi = acos(2.0f * v - 1.0f);

	float x = cos(theta) * sin(phi) * rad;
	float y = sin(theta) * sin(phi) * rad;
	float z = cos(phi) * rad;

	vec3 result = {x, y, z};
	return result;
}
#endif

ONLYCE vec3 randvecOLD(void)
{
	vec3 rand = { frand(), frand(), frand() };
	rand = norm(rand);
	return rand;
}

ONLYCE vec3 randvec(void)
{
	vec3 rand;
#if NEWRAND
	rand = randvecNEW();
#else
	rand = randvecOLD();
#endif

#if 0
	FLOAT l = dotP(rand,rand);
	if (l > 1.0+EPSILON) {
		dvec(rand);
		dprintf("ERR %f\n",l);
		exit(4);
	}
	assert(l <= 1.0+EPSILON);
#endif
	return rand;
}
