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
