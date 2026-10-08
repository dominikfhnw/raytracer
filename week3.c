/* 
raytracer by dominikr

minifying todo:
* storing upper left corner of image plane instead of middle
  would prob save a bit
* storing radius^2 instead of radius could save a bit
* using smaller data format for the scene *could* save some space,
  but deflate is quite good at compacting it already
* simplify more stuff, rewrite stuff in assembler


*/

#define P		0.5	// probability to bounce once more
#define SAMPLES		4096	// number of samples for each pixel
#define UPDATE		1	// update screen while calculation is running
#define COSWEIGHT	1	// cosine-weighted distribution

#define WIDTH		600	// image width
#define HEIGHT		600	// image height
#define GAMMA		2.2	// image gamme

#define FLOAT_EXCEPTION	0	// croak if something's wrong with a float
#ifndef DEBUG
#define DEBUG		1	// some basic debug info and error messages
#endif


#define XORSHIFT	1	// faster PRNG, no slowdown in multithreading
#define NAKED		1	// this option is only for indecent people and demosceners
#define NUDGE		0.005	// amount to nudge the intersection point back
#define NUDGE_EN	1	// enable nudging at all?
#define EPSILON		0.0001	// used for float comparisons
//#define YOLO		1	// activate if you don't care about handling edge cases or being overly precise
#define PERSPECTIVE2	0
#define WEIRD		1	// somehow only one solution of the quadratic equation is needed

#define FLOAT		float
#define NUM_SPHERES	sizeof(scene)/sizeof(sphere)

#if FBDEV			// super secret, compiler-crashing mode
#include "fbdev/fbdev.h"
#elif DOS
#include "dos32/dos.h"
#else				// boring old SDL
#include "sdl.h"
#endif

#include "scene.h"

CONSTEXPR vec3 eye_ray(const vec3 f, const vec3 rl, const vec3 ul, const float x, const float y)
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

CONSTEXPR FLOAT screen_convert(const coord_t width, const coord_t x)
{
	assert(x < width);

	int half = width / 2;	// FLOATING CONST ???
	int m = x - half;

	FLOAT result = (FLOAT)m / (FLOAT)half;
	if (result > 1 || result < -1) {
		dprintf("h %d m %d\n", half, m);
		dprintf("in %d,%d o %f\n", width, x, result);
	}
	assert(result >= -1-EPSILON);
	assert(result <=  1+EPSILON);
	return result;
}

CONSTEXPR FLOAT hitcheck(const sphere s, const vec3 E, const vec3 d)
{
	FLOAT r = s.radius;
	vec3  C = s.center;

	vec3  CE = sub(E,C);
	FLOAT a = 1;
	FLOAT b = 2 * dotP(CE, d);
	// TODO: r*r could be precomputed
	FLOAT c = dotP(CE,CE) - (r * r);

	FLOAT root = b*b - 4*a*c;	// FLOAT CONST 4

#if 1
	if (root < 0)
		return 0;
#endif

	FLOAT l2 = (-b - SQRT(root)) / (2*a);	// FLOAT CONST 2
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

CONSTEXPR FLOAT getlambda(void)
{
	CONSTV FLOAT fov_rad = FOV * PI / 180;
	return TAN(fov_rad/2);
}

CONSTEXPR vec3 reflect(const vec3 in, const vec3 normal)
{
	const vec3 m = scalar_mult(normal, dotP(in, normal));
	const vec3 p = sub(m, in);
	return add(scalar_mult(p,2), in);
}

CONSTEXPR vec3 BRDF(const color_t num, const vec3 in, const vec3 out, vec3 normal)
{
	vec3 result = scalar_mult(scene[num].diffuse, 1.0/PI); // FLOAT CONST 1/PI... or 1 and PI??
	if ( scene[num].material == 1 ) {	// REFLECTIVE
		// Compute reflection of `in` on `normal`
		const vec3 dr = reflect(in, normal);
		if (dotP(dr,out) > 1 - 0.01) {
			//putc(39,stderr);
			#define DI 4
			vec3 dielec = {DI,DI,DI};
			result = add(result, dielec);
			//result = dielec;
		}
	}
	return result;
}

CONSTEXPR vec3 compute_color(const vec3 eye, const vec3 ray, const color_t depth)
{
	FLOAT min = FLOAT_MAX;	// FLOAT CONST
	color_t num = 0;
	for(color_t v=1; v<NUM_SPHERES; v++){
		FLOAT hit = hitcheck(scene[v], eye, ray);
		if (hit != 0 && hit < min) {
		// raytracing in one weekend, get rid of shadow acne
		// same as nudging?? but cheaper in codesize
		//if (hit > NUDGE && hit < min) {
			min = hit;
			num = v;
		}
	}

	if ( num == 0 || RAND() < P*RMAX )
		return scene[num].emission;

	const vec3 hitpoint	= add(eye, scalar_mult(ray, min));
	#if NUDGE_EN
	const vec3 hp_nudge	= add(eye, scalar_mult(ray, min-NUDGE));
	#else
	# define hp_nudge hitpoint
	#endif
	const vec3 normal	= normV(scene[num].center, hitpoint);
	vec3 nextdir;

	if (scene[num].material == 2) {				// MIRROR
		nextdir		= reflect(ray, normal);
	}
	else {
		#if COSWEIGHT
		//nextdir		= norm(add(normal,randvec()));
		//nextdir		= changesign(norm(add(normal,randvec())));
		nextdir		= norm(sub(randvec(),normal));
		#else
		nextdir		= randvec();
		#endif
	}
	const vec3 brdf		= BRDF(num, ray, nextdir, normal);
	FLOAT direction		= dotP(nextdir, normal);
	if (direction < 0) {
		nextdir = changesign(nextdir);
		direction = -direction;
	}
	
	#if 1 && COSWEIGHT
	CONSTV FLOAT pconst = (FLOAT)(PI)/(FLOAT)(1-P);
	#else
	CONSTV FLOAT pconst = (FLOAT)(2*PI)/(FLOAT)(1-P);
	#endif

	#if 1 && COSWEIGHT
	const vec3 foo = scalar_mult(brdf, pconst);
	#else
	const vec3 foo = scalar_mult(scalar_mult(brdf, pconst),direction);
	#endif

	const vec3 moo = hadamard(foo, compute_color(hp_nudge, nextdir, depth+1));
	return add(scene[num].emission,moo);
}

static void render(void* surface, const coord_t w, const coord_t h)
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

	ENABLE_FLOAT_EXCEPTIONS;

	dvec(rl);
	dvec(ul);
	dvec(fhat);
	dprintf("sizeof sphere %ld, sizeof scene %ld, num spheres %ld\n", sizeof(sphere), sizeof(scene), NUM_SPHERES);

#pragma omp parallel for schedule(static)
	//for(int y=150; y < 151; y++){
	for(int y=0; y < h; y++){
		FLOAT fy = screen_convert(h, y);
		// TODO: precompute as many things from the eye ray already here
		//for(int x=150; x < 152; x++){
		for(int x=0; x < w; x++){
			FLOAT fx = screen_convert(w, x);
			vec3 ray = eye_ray(fhat, rl, ul, fx, fy);

			tprintf("%d/%d %f %f\n",x,y,fx,fy);
			tvec(ray);
			vec3 sample = {0, 0, 0};
			for(coord_t w=0;w < SAMPLES;w++){
				vec3 single = compute_color(eye, ray, 0);
				tvec(single);
				sample = add(sample,single);
			}
			sample = scalar_mult(sample, 1/(FLOAT)SAMPLES);
			uint32_t pixel = colormap(sample);
			set_pixel(surface, x, y, pixel);

			//vec3 sample = compute_color(eye, ray); // single sample
			//set_pixel(surface, x, y, 0xff00ff); // week1
		}
		#if UPDATE
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
