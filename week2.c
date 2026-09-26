/* 
References:
Coding style https://www.kernel.org/doc/html/latest/process/coding-style.html

going offtopic:
https://en.wikipedia.org/wiki/VESA_BIOS_Extensions#VBE_mode_numbers
secret modes, esp. what TomCatAbaddon posted https://www.pouet.net/topic.php?which=11672&page=1

Coordinate system: right-handed, Z up

AI: Google search for "SDL_MUSTLOCK"
"How do I calculate the cross product in C? I'm using a struct called vec3 that contains 3d points"


(Vec3 o, Vec3 d) CreateEyeRay(Vec3 Eye, Vec3 LookAt, float FOV, Vec2 Pixel) { … }
HitPoint FindClosestHitPoint(Scene s, Vec3 o, Vec3 d) { … }
Color ComputeColor(Scene s, Vec3 o, Vec3 d) { … }

*/

#define GAMMA	2.2
#define DEBUG	1
#define WIDTH	600
#define HEIGHT	600
//#define YOLO  1		// activate if you don't care about handling edge cases or being overly precise
#define PERSPECTIVE2 1

#define FLOAT	float
#define CONST	__attribute__((const))
#define NUM_SPHERES sizeof(scene)/sizeof(sphere)

#if FBDEV			// super secret, compiler-crashing mode
#include "fbdev/fbdev.h"
#else				// boring old SDL
#include "sdl.h"
#endif

#include "scene.h"

vec3 eye_ray(vec3 f, vec3 rl, vec3 ul, float x, float y)
{
	assert(x >= -1);
	assert(x <=  1);

	assert(y >= -1);
	assert(y <=  1);

	vec3 xs = scalar_mult(rl, x);
	vec3 ys = scalar_mult(ul, y);

	vec3 ret;
	ret = add(f,   xs);
	ret = add(ret, ys);

	return norm(ret);
}

FLOAT screen_convert(int width, int x)
{
	assert(x <= width);

	int half = width / 2;
	int m = x - half;

	FLOAT result = (FLOAT)m / (FLOAT)half;
	assert(result >= -1);
	assert(result <=  1);
	return result;
}

FLOAT hitcheck(sphere s, vec3 E, vec3 d)
{
	FLOAT r = s.radius;
	vec3  C = s.center;

	vec3  CE = sub(E,C);
	FLOAT a = 1;
	FLOAT b = 2 * dotP(CE, d);
	FLOAT le = len(CE);
	FLOAT c = (le*le) - (r * r);

	FLOAT root = b*b - 4*a*c;
	if (root < 0)
		return 0;

	FLOAT l1 = (-b + SQRT(root)) / (2*a);
	FLOAT l2 = (-b - SQRT(root)) / (2*a);

	// XXX what if our scene has all objects strictly in front?
	if (l1 < l2 && l1 > 0)
		return l1;
	else if (l2 > 0)
		return l2;
	else
		return 0;
}

CONST FLOAT getlambda(void)
{
	FLOAT fov_rad = FOV * PI / 180;
	return TAN(fov_rad/2);
}

void render(void* surface, int w, int h)
{

	const vec3 eye		= EYE;
	const vec3 lookat	= LOOK;

	const vec3 f		= norm(sub(lookat, eye));
	const vec3 up		= UP;
	const vec3 r		= norm(crossP(up, f));
	const vec3 u		= norm(crossP(r,  f));

	const FLOAT lambda	= getlambda();

	const vec3 rl		= scalar_mult(r, lambda);
	const vec3 ul		= scalar_mult(u, lambda);
	const vec3 fhat		= f;

	// check if lambda has been precalculated
	// can only be checked if optimizer is turned on
	#if __OPTIMIZE__
	assert(__builtin_constant_p(lambda));
	#endif

	dvec(rl);
	dvec(ul);
	dvec(fhat);
	dprintf("sizeof sphere %ld, sizeof scene %ld, num spheres %ld\n", sizeof(sphere), sizeof(scene), NUM_SPHERES);

	for(int j=0; j < h; j++){
		FLOAT fy = screen_convert(h, j);
		for(int i=0; i < w; i++){
			FLOAT fx = screen_convert(w, i);

			vec3 ray = eye_ray(fhat, rl, ul, fx, fy);
			uint32_t pixel;

			FLOAT min = FLOAT_MAX;
			int   num = -1;
			for(unsigned int v=0; v<NUM_SPHERES; v++){
				FLOAT hit = hitcheck(scene[v], eye, ray);
				if (hit != 0 && hit < min) {
					min = hit;
					num = v;
				}
			}

			if (num >= 0) {
				pixel = colormap(scene[num].color);
				set_pixel(surface, i, j, pixel);
			}
		}
	}
}
