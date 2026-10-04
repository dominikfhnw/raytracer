/* 
References:
Coding style https://www.kernel.org/doc/html/latest/process/coding-style.html

going offtopic:
https://en.wikipedia.org/wiki/VESA_BIOS_Extensions#VBE_mode_numbers
secret modes, esp. what TomCatAbaddon posted https://www.pouet.net/topic.php?which=11672&page=1

AI: Google search for "SDL_MUSTLOCK"
"How do I calculate the cross product in C? I'm using a struct called vec3 that contains 3d points"

*/

#define P	0.5
#define SAMPLES	32
#define NUDGE	0.005
//#define NUDGE	0.0

#define XORSHIFT 1
#define NAKED	1

#define GAMMA	2.2
#ifndef DEBUG
#define DEBUG	1
#endif
#define WIDTH	600
#define HEIGHT	600
//#define YOLO  1		// activate if you don't care about handling edge cases or being overly precise
#define PERSPECTIVE2 0
#define WEIRD	0

#define FLOAT	float
#define NUM_SPHERES sizeof(scene)/sizeof(sphere)

#if FBDEV			// super secret, compiler-crashing mode
#include "fbdev/fbdev.h"
#elif DOS
#include "dos32/dos.h"
#else				// boring old SDL
#include "sdl.h"
#endif

#include "scene.h"

CONST vec3 eye_ray(const vec3 f, const vec3 rl, const vec3 ul, const float x, const float y)
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

CONST FLOAT screen_convert(const int width, const int x)
{
	assert(x <= width);

	int half = width / 2;
	int m = x - half;

	FLOAT result = (FLOAT)m / (FLOAT)half;
	assert(result >= -1);
	assert(result <=  1);
	return result;
}

CONST FLOAT hitcheck(const sphere s, const vec3 E, const vec3 d)
{
	FLOAT r = s.radius;
	vec3  C = s.center;

	vec3  CE = sub(E,C);
	FLOAT a = 1;
	FLOAT b = 2 * dotP(CE, d);
	// TODO: c could be precomputed per sphere
	FLOAT c = dotP(CE,CE) - (r * r);

	FLOAT root = b*b - 4*a*c;

#if 1
	if (root < 0)
		return 0;
#endif

	FLOAT l2 = (-b - SQRT(root)) / (2*a);
#if WEIRD
	if (l2 > 0)
		return l2;
	else
		return 0;
#else
	FLOAT l1 = (-b + SQRT(root)) / (2*a);
	if (l1 < l2 && l1 > 0)
		return l1;
	else if (l2 > 0)
		return l2;
	else
		return 0;
#endif

}

void find_closest_hitpoint(const vec3 eye, const vec3 ray, const int x, const int y, void* surface)
{
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
		pixel = colormap(scene[num].diffuse);
		set_pixel(surface, x, y, pixel);
	}
}

CONSTEXPR FLOAT getlambda(void)
{
	FLOAT fov_rad = FOV * PI / 180;
	return TAN(fov_rad/2);
}

CONSTEXPR FLOAT getrand(void)
{
	//TODO: does it also go negative?
	return (FLOAT) ( ((FLOAT)RAND() - ((FLOAT)RMAX/2.0) ) / (FLOAT)RMAX);
}

CONSTEXPR vec3 randvec(void)
{
	vec3 rand = { getrand(), getrand(), getrand() };
	rand = norm(rand);
	if (len(rand)>1)
		return randvec();
	return rand;

}

/*
vec3 BRDF(vec3 wi, vec3 wo)
{
}
*/

vec3 compute_color(const vec3 eye, const vec3 ray)
{
	FLOAT min = FLOAT_MAX;
	int   num = -1;
	for(unsigned int v=0; v<NUM_SPHERES; v++){
		FLOAT hit = hitcheck(scene[v], eye, ray);
		if (hit != 0 && hit < min) {
			min = hit;
			num = v;
		}
	}
	vec3 emission	= scene[num].emission;

#if 0
	if ( len(emission) > 2 )
		return emission;
#endif

	if ( RAND() < P*RMAX )
		return emission;


	vec3 hitpoint	= add(eye, scalar_mult(ray, min));
	vec3 hp_nudge	= add(eye, scalar_mult(ray, min-NUDGE));
	vec3 normal	= normV(scene[num].center, hitpoint);
	vec3 rand	= randvec();
	vec3 BRDF	= scalar_mult(scene[num].diffuse, 1.0/PI);

	FLOAT direction	= dotP(rand, normal);
	if (direction < 0) {
		rand = scalar_mult(rand, -1.0);
		direction = -direction;
	}

#if 1
	// (rand*n)*BRDF*compute_color
	vec3 bar = BRDF;
	//vec3 foo = scalar_mult(bar,(2.0*PI)/(1.0-P));
	vec3 foo = scalar_mult(scalar_mult(bar,(FLOAT)(2*PI)/(FLOAT)(1-P)),direction);
	vec3 moo = hadamard(foo, compute_color(hp_nudge, rand));
	return add(emission,moo);
	//return add(scene[num].emission,scalar_mult(foo,(2*PI)/(1-P)));

#else
	vec3 foo = scalar_mult(scalar_mult(BRDF,(FLOAT)(2*PI)/(FLOAT)(1-P)),direction);
	//vec3 foo = scalar_mult(scalar_mult(BRDF,(2.0*PI)/(1.0-P)),direction);
	//vec3 foo = scalar_mult(BRDF,(2.0*PI)/(1.0-P));
	vec3 bar = add(foo,emission);
	return hadamard(bar, compute_color(hp_nudge, rand));
	//return add(emission,moo);
#endif
}

void render(void* surface, const int w, const int h)
{
	CONSTV vec3 eye		= EYE;
	CONSTV vec3 look	= LOOK;
	CONSTV vec3 up		= UP;
	CONSTV vec3 f		= normV(eye, look);
	CONSTV vec3 r		= norm(crossP(up, f));
	CONSTV vec3 u		= norm(crossP(r,  f));

	CONSTV FLOAT lambda	= getlambda();

	CONSTV vec3 rl		= scalar_mult(r, lambda);
	CONSTV vec3 ul		= scalar_mult(u, lambda);
	CONSTV vec3 fhat	= f;

	// check if lambda has been precalculated
	// can only be checked if optimizer is turned on
#if 1
	#if __OPTIMIZE__
	dprintf("constcheck assert ENABLED\n");
	assert(__builtin_constant_p(lambda));
	#else
	dprintf("constcheck assert DISABLED\n");
	#endif
#endif

	dvec(rl);
	dvec(ul);
	dvec(fhat);
	dprintf("sizeof sphere %ld, sizeof scene %ld, num spheres %ld\n", sizeof(sphere), sizeof(scene), NUM_SPHERES);
	//dprintf("sqrt(-1) = %f\n", SQRT(-1));

#pragma omp parallel for schedule(static)
	for(int y=0; y < h; y++){
		FLOAT fy = screen_convert(h, y);
		// TODO: precompute as many things from the eye ray already here
		for(int x=0; x < w; x++){
			FLOAT fx = screen_convert(w, x);
			vec3 ray = eye_ray(fhat, rl, ul, fx, fy);

			vec3 sample = {0, 0, 0};
			for(int w=0;w < SAMPLES;w++){
				//vec3 single = limit(compute_color(eye, ray),3);
				vec3 single = compute_color(eye, ray);
				if (!normal(single))
					single = (vec3)GRAY;
				sample = add(sample,single);
			}
			sample = scalar_mult(sample, 1/(FLOAT)SAMPLES);
			set_pixel(surface, x, y, colormap(sample));
			//vec3 sample = compute_color(eye, ray); // single sample
			//set_pixel(surface, x, y, 0xff00ff); // week1
		}
		#if 1
			#if DEBUG
			putc(46, stderr);
			#endif
			update(surface);
		#endif
	}
	#if DEBUG
	putc(35, stderr);
	putc(10, stderr);
	#endif
}
