/* 
References:
Coding style https://www.kernel.org/doc/html/latest/process/coding-style.html
SDL2 installation/basic compile https://wiki.libsdl.org/SDL2/Installation
SDL2 pixel drawing https://gigi.nullneuron.net/gigilabs/sdl2-pixel-drawing/
SDL2 pixel drawing https://gist.github.com/mmozeiko/729860eeb414f1a2ee345d9d3ab4dd4e
https://web.archive.org/web/20160326085538/http://content.gpwiki.org/index.php/SDL:Tutorials:Drawing_and_Filling_Circles
https://web.archive.org/web/20120420111824/http://www.libsdl.org/intro.en/usingvideo.html
SDL3 surfaces and colors https://www.studyplan.dev/sdl3/sdl3-surfaces-colors#sdl3-surfaces-and-colors

pixel formats: https://wiki.libsdl.org/SDL2/CategoryPixels
save "screenshot": https://wiki.libsdl.org/SDL3/SDL_SaveBMP

going offtopic:
https://en.wikipedia.org/wiki/VESA_BIOS_Extensions#VBE_mode_numbers
secret modes, esp. what TomCatAbaddon posted https://www.pouet.net/topic.php?which=11672&page=1

Coordinate system: right-handed, Z up

AI: Google search for "SDL_MUSTLOCK"
+ crossP, SDL set_pixel(?)
"How do I calculate the cross product in C? I'm using a struct called vec3 that contains 3d points"


Scene
{
	Sphere(Center=[-1001, 0, 0], r=1000, Color= Red) a
	Sphere(Center=[ 1001, 0, 0], r=1000, Color= Blue) b
	Sphere(Center=[ 0, 0,1001], r=1000, Color= Gray) c
	Sphere(Center=[ 0, -1001, 0], r=1000, Color= Gray) d
	Sphere(Center=[ 0, 1001, 0], r=1000, Color= White) e
	Sphere(Center=[ -0.6, -0.7,-0.6], r= 0.3, Color= Yellow) f
	Sphere(Center=[ 0.3, -0.4, 0.3], r= 0.6, Color=LightCyan) g
}
(Vec3 o, Vec3 d) CreateEyeRay(Vec3 Eye, Vec3 LookAt, float FOV, Vec2 Pixel) { … }
HitPoint FindClosestHitPoint(Scene s, Vec3 o, Vec3 d) { … }
Color ComputeColor(Scene s, Vec3 o, Vec3 d) { … }

*/

#define GAMMA 2.2
#define FLOAT float
#define DEBUG 1
//#define YOLO  1		// activate if you don't care about handling edge cases or being overly precise

#if FBDEV			// super secret, compiler-crashing mode
#include "fbdev/fbdev.h"
#else				// boring old SDL
#include "sdl.h"
#endif


#define EYE	{ 0, 0, -4 }
#define LOOK	{ 0, 0,  6 }
#define UP	{ 0, 1, 0 }
#define FOV	36

#define RED	{ 1, 0,  0 }
#define GREEN	{ 0, 1,  0 }
#define BLUE	{ 0, 0,  1 }
#define CYAN	{ 0.5, 0, 0.5 }
#define ZERO	{ 0, 0,  0 }


sphere scene[] = {
	{ {-1001, 0, 0}, 1000, RED },		// a
	{ { 1001, 0, 0}, 1000, BLUE },		// b
	{ {0.3, -0.4, 0.3}, 0.6, CYAN },	// g
};

/*
vec3 create_eye_ray(vec3 eye, vec3 lookat, FLOAT fov, int x, int y)
{

	dprintf("sizeof sphere %ld, sizeof spheres %ld, num spheres %ld\n", sizeof(sphere), sizeof(spheres), sizeof(spheres)/sizeof(sphere));
	vec3 f = sub(lookat, eye);
	FLOAT lambda = tanf(fov/2);

	vec3 up = UP;
	vec3 r  = crossP(up, f);
	vec3 u  = crossP(r, f);

	// d = f + x*r*lambda + y*u*lambda

	dprintf("vec f %f %f %f, lambda %f\n", f.x, f.y, f.z, lambda);
	dprintf("vec r %f %f %f\n", r.x, r.y, r.z);
	dprintf("vec u %f %f %f\n", u.x, u.y, u.z);
	vec3 n = norm(u);
	dprintf("vec ^u %f %f %f\n", n.x, n.y, n.z);

}
*/

vec3 eye_ray(vec3 f, vec3 rl, vec3 ul, int x, int y)
{
	vec3 xs = scalar_mult(rl, x);
	vec3 ys = scalar_mult(ul, y);

	vec3 ret;
	ret = add(f,   xs);
	ret = add(ret, ys);

	return ret;
}

void render(void* surface, int w, int h)
{
	vec3  eye	= EYE;
	vec3  lookat	= LOOK;
	FLOAT fov	= FOV * PI / 180;

	vec3 f = sub(lookat, eye);
	FLOAT lambda = tanf(fov/2);

	vec3 up = UP;
	vec3 r  = norm(crossP(up, f));
	vec3 u  = norm(crossP(r,  f));

	// u * lambda
	vec3 rl = scalar_mult(r, lambda);
	vec3 ul = scalar_mult(u, lambda);

	dprintf("vec rl %f %f %f\n", rl.x, rl.y, rl.z);
	dprintf("vec f %f %f %f\n", f.x, f.y, f.z);
	dprintf("vec ul %f %f %f\n", ul.x, ul.y, ul.z);
	dprintf("sizeof sphere %ld, sizeof scene %ld, num spheres %ld\n", sizeof(sphere), sizeof(scene), sizeof(scene)/sizeof(sphere));

	exit(0);
	//create_eye_ray(eye, lookat, fov, 1, 1);
	vec3 ray = eye_ray(f, rl, ul, 1, 1);
	dprintf("vec ray %f %f %f\n", ray.x, ray.y, ray.z);
	
	// week 1

	vec3 red   = RED;
	vec3 green = GREEN;

	for(int j=0; j < h; j++){
		for(int i=0; i < w; i++){
			FLOAT amount = (FLOAT)i/(FLOAT)w;
			//vec3 color = lerp(red, green, amount);
			vec3 color = lerp(green, red, amount);
			uint32_t pixel = colormap(color);
			set_pixel(surface, i, j, pixel);
		}
	}
}
