/* 
raytracer by dominikr

minifying todo:
* using smaller data format for the scene *could* save some space,
  but deflate is quite good at compacting it already
* simplify more stuff, rewrite stuff in assembler


*/

#define VRAND 1

#define P		0.5	// probability to bounce once more
#define SAMPLES		512	// number of samples for each pixel
#define MINDEPTH	3	// minimum number of steps
#define FIXED		1	// mindepth == maxdepth
#define MOOLTI		0	// gaussian distributed multisampling
#define BLUR		1	// focus blur
#define TEXTURE		0	// texture mapping
#define SLOW		1	// smaller, but potentially slower (not in SSE apparently)

#define EXIT		0	// clean exit vs sticking around in an endless loop
#define UPDATE		1	// update screen while calculation is running
#define BLUR_VAL	0.23f
#define OLDEYE		1	// old eye ray calc. new one doesn't work with alt.persp.
#define COSWEIGHT	1	// cosine-weighted distribution

#define WIDTH		600	// image width
#define HEIGHT		600	// image height
#define GAMMA		2.0	// image gamma

#define FLOAT_EXCEPTION	0	// croak if something's wrong with a float
#ifndef DEBUG
#define DEBUG		1	// some basic debug info and error messages
#endif

#define XORSHIFT	1	// faster PRNG, no slowdown in multithreading
#define NAKED		1	// this option is only for indecent people and demosceners
#define NUDGE		0.005	// amount to nudge the intersection point back
#define NUDGE_EN	0	// enable nudging at all?
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

#include "scene4.h"

CONSTEXPR vec3 eye2(const vec3 f, const FLOAT dx, const FLOAT dy, const coord_t x, const coord_t y)
{
	vec3 ret;

	ret = f;
	ret[0] = ret[0] + x*dx;
	ret[1] = ret[1] + y*dy;

	return ret;
}

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

	return ret;
}

CONSTEXPR FLOAT screen_convert(const coord_t width, const coord_t x)
{
	assert(x < width);

	int half = width / 2;	// FLOATING CONST ???
	int m = x - half; // screen_convert

	FLOAT result = (FLOAT)m / (FLOAT)half; // screen_convert
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
	FLOAT b = -2 * dotP(CE, d); // hitcheck
	FLOAT c = dotP(CE,CE) - r;

	FLOAT root = b*b - 4*c;	// FLOAT CONST 4

#if !SLOW
	if (root < 0)
		return 0;
#endif

	FLOAT l2 = (b - SQRT(root)) / 2;	// FLOAT CONST 2
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

CONST vec3 reflect(const vec3 in, const vec3 normal)
{
	const vec3 m = hadamard(normal, dotP3(in, normal));
	const vec3 p = sub(m, in);
	return add(scalar_mult(p,2), in);
}

CONSTEXPR vec3 BRDF(const color_t num, const vec3 in, const vec3 out, vec3 normal)
{
	vec3 result = scene[num].diffuse;
#if TEXTURE
	if ( scene[num].material == 3 ) {	// TEXTURED
		FLOAT phi   = atan2f(normal[2], normal[0])+4.8;
		FLOAT theta = acosf(normal[1]);
		FLOAT u = phi / (2*PI);
		FLOAT v = theta / PI;
		uint32_t pu = (uint32_t)floorf(image->w * u);
		uint32_t pv = (uint32_t)floorf(image->h * v);

		//pu = (pu+image->w/3) % image->w;
		//coord_t pv = image->w * v;
		tprintf("%d %d\n", pu, pv);
		uint32_t pixel = get_pixel(image, pu, pv);
		//return reverse_color(pixel);
		result = reverse_color(pixel);
	}
	if ( scene[num].material == 1 || scene[num].material == 3 ) {	// brdf REFLECTIVE
#else
	if ( scene[num].material == 1 ) {	// brdf REFLECTIVE
#endif
		// Compute reflection of `in` on `normal`
		const vec3 dr = reflect(in, normal);
		if (dotP(dr,out) > 1.0f - 0.01f) { // brdf reflective
			//putc(39,stderr);
			#define DI 4*PI
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
	color_t num = 0; // compute_color hitcheck
	for(color_t v=1; v<NUM_SPHERES; v++){
		FLOAT hit = hitcheck(scene[v], eye, ray);
		if (hit != 0 && hit < min) {  // compute_color hitcheck
		// raytracing in one weekend, get rid of shadow acne
		// same as nudging?? but cheaper in codesize
		//if (hit > NUDGE && hit < min) {
			min = hit; // compute_color hitcheck
			num = v;
		}
	}

#if MINDEPTH > 0
#  if FIXED
	if ( num == 0 || depth>=MINDEPTH ) // compute_color early return
#  else
	if ( num == 0 || (depth>=MINDEPTH && RAND() < P*RMAX) )
#  endif
#else
	if ( num == 0 || RAND() < P*RMAX )
#endif
		return scene[num].emission; // compute_color early return

	const vec3 hitpoint	= add(eye, scalar_mult(ray, min));
	#if NUDGE_EN
	const vec3 hp_nudge	= add(eye, scalar_mult(ray, min-NUDGE));
	#else
	# define hp_nudge hitpoint
	#endif
	const vec3 normal	= normV(scene[num].center, hitpoint); // compute_color normal
	vec3 nextdir;

	if (scene[num].material == 2) {				// compute_color MIRROR
		nextdir		= reflect(ray, normal);
	}
	else {
		#if COSWEIGHT
		nextdir		= norm(sub(randvec(),normal));
		#else
		nextdir		= randvec();
		#endif
	}
	const vec3 brdf		= BRDF(num, ray, nextdir, normal);
	FLOAT direction		= dotP(nextdir, normal);
	if (direction < 0) { // compute_color direction check
		nextdir = changesign(nextdir);
		direction = -direction;
	}
	
	#if !FIXED
	# if COSWEIGHT
	CONSTV FLOAT pconst = (FLOAT)(1)/(FLOAT)(1-P);
	# else
	CONSTV FLOAT pconst = (FLOAT)(2)/(FLOAT)(1-P);
	# endif
	#endif

#if FIXED
	const vec3 foo = brdf;
#else
	vec3 foo;	
#  if MINDEPTH > 0
	if(depth<=MINDEPTH) {
		foo = brdf;
	}
	else {
#  endif
		// compensate for times P caused us to miss
		#if 1 && COSWEIGHT
		foo = scalar_mult(brdf, pconst);
		#else
		foo = scalar_mult(scalar_mult(brdf, pconst),direction);
		#endif
#  if MINDEPTH > 0
	}
#  endif
#endif

	const vec3 moo = hadamard(foo, compute_color(hp_nudge, nextdir, depth+1)); // compute_color recurse
	return add(scene[num].emission,moo);
} // compute_color end


static void output(const void* surface,const coord_t x, const coord_t y, const vec3 sample)
{
#if 0
	vec3 out;
	out = scalar_mult(sample, 1/(FLOAT)SAMPLES);
	uint32_t pixel = colormap(out);
	set_pixel(surface, x, y, pixel);
#else
	#define RGB_CORR (65535/SAMPLES)
	vec3 gamma = vsqrt(sample*RGB_CORR);
	__m128i p = _mm_cvtps_epi32(gamma);
	__m128i p16 = _mm_packus_epi32(p, p);
	__m128i p8 = _mm_packus_epi16(p16, p16);
	set_pixel128(surface, x, y, p8);
#endif
}

static void render(const void* surface)
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

	#if !OLDEYE
	CONSTV FLOAT cornery = screen_convert(HEIGHT, 0);
	CONSTV FLOAT cornerx = screen_convert(WIDTH,  0);
	CONSTV FLOAT nexty   = screen_convert(HEIGHT, 1);
	CONSTV FLOAT nextx   = screen_convert(WIDTH,  1);
	CONSTV vec3 corner = eye_ray(fhat, rl, ul, cornerx, cornery);
	dvec(corner);
	CONSTV vec3 next = eye_ray(fhat, rl, ul, nextx, cornery);
	CONSTV vec3 diff = sub(next, corner);
	dvec(diff);
	CONSTV vec3 nextl = eye_ray(fhat, rl, ul, cornerx, nexty);
	CONSTV vec3 diffl = sub(nextl, corner);
	dvec(diffl);
	CONSTV FLOAT dx = diff[0];
	CONSTV FLOAT dy = diffl[1];
	dprintf("dfloats %f %f\n\n", dx, dy);
	#endif

#pragma omp parallel for schedule(static)
	for(int y=0; y < HEIGHT; y++){ // render y
		FLOAT fy = screen_convert(HEIGHT, y);
		for(int x=0; x < WIDTH; x++){ // render x
			FLOAT fx = screen_convert(WIDTH, x);
			#if OLDEYE
			vec3 ray = norm(eye_ray(fhat, rl, ul, fx, fy));
			#else
			vec3 ray = norm(eye2(corner, dx, dy, x, y));
			#endif
			#if MOOLTI
			vec3 ray0 = eye2(corner, dx, dy, x, y);
			#endif

			tprintf("%d/%d %f %f\n",x,y,fx,fy);
			tvec(ray);
			vec3 sample = {0, 0, 0}; // render init sample
			CONSTV vec3 local_eye = EYE;
			#if MOOLTI || BLUR
				for(coord_t v=0;v < SAMPLES;v++){ // render collect samples
					#if MOOLTI
					vec3 rray = ray0;
					rray[0] += frand()*frand()*dx;
					rray[1] += frand()*frand()*dy;
					const vec3 nray = norm(rray);
					#else
					#define nray ray
					//const vec3 nray = ray;
					#endif

					const vec3 focal = add(local_eye, scalar_mult(nray, FOCAL_LENGTH));
					#if BLUR
					vec3 reye = local_eye;
					#if VRAND
					vec3 gauss = vgauss();
					reye += gauss * BLUR_VAL; // render defocus blur
					#else
					reye[0] += frand()*frand()*BLUR_VAL;
					reye[1] += frand()*frand()*BLUR_VAL;
					#endif

					//const vec3 blur_ray = scalar_mult(normV(reye, focal),1);
					const vec3 blur_ray = normV(reye,focal);

					//vec3 single = compute_color(reye, norm(rray), 0);
					//vec3 single = compute_color(local_eye, blur_ray, 0);
					vec3 single = compute_color(reye, blur_ray, 0); // render call cmpute_color
					#else
					vec3 single = compute_color(local_eye, nray, 0);
					#endif
					sample = add(sample,single);
				}
			#else
				for(coord_t v=0;v < SAMPLES;v++){
					vec3 single = compute_color(local_eye, ray, 0);
					sample = add(sample,single);
				}
			#endif
			output(surface, x, y, sample);
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
