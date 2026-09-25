typedef struct vec3 {
	FLOAT x;
	FLOAT y;
	FLOAT z;
} vec3;

typedef struct sphere {
	vec3	center;
	FLOAT	radius;
	vec3	color;
} sphere;


uint8_t map_component(FLOAT c)
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

uint32_t colormap(vec3 color)
{
	uint8_t r = map_component(color.x);
	uint8_t g = map_component(color.y);
	uint8_t b = map_component(color.z);

	//dprintf("lin %u %u %u\n", r, g, b);
	return (r << 16) + (g << 8) + (b << 0); // bgra32
	// return (r << 0) + (g << 8) + (b << 16); // rgba32
}

vec3 add(vec3 a, vec3 b)
{
	a.x = a.x + b.x;
	a.y = a.y + b.y;
	a.z = a.z + b.z;
	return a;
}

vec3 sub(vec3 a, vec3 b)
{
	a.x = a.x - b.x;
	a.y = a.y - b.y;
	a.z = a.z - b.z;
	return a;
}

FLOAT dotP(vec3 a, vec3 b)
{
	return a.x*b.x + a.y*b.y + a.z*b.z;
}

FLOAT len1(vec3 a)
{
	return SQRT(a.x*a.x + a.y*a.y + a.z*a.z);
}

FLOAT len(vec3 a)
{
	return SQRT(dotP(a,a));
}

vec3 norm(vec3 a)
{
	FLOAT l = len(a);
	a.x = a.x / l;
	a.y = a.y / l;
	a.z = a.z / l;
	return a;
}

vec3 crossP(vec3 a, vec3 b)
{
	vec3 result;
	result.x = (a.y * b.z) - (a.z * b.y);
	result.y = (a.z * b.x) - (a.x * b.z);
	result.z = (a.x * b.y) - (a.y * b.x);
	return result;
}

vec3 scalar_mult(vec3 a, FLOAT amount)
{
	a.x = a.x * amount;
	a.y = a.y * amount;
	a.z = a.z * amount;
	return a;
}

vec3 lerp(vec3 a, vec3 b, FLOAT amount)
{
	assert(amount >= 0);
	assert(amount <= 1);
	return add(scalar_mult(a, amount), scalar_mult(b, 1-amount));
}
