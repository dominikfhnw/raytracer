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

*/

#define GAMMA 2.2
#define FLOAT float
#define DEBUG 0
#ifndef YOLO
#define YOLO  0		// activate if you don't care about handling edge cases or being overly precise
#endif

#if DEBUG
#define dprintf(...) printf(__VA_ARGS__)
#else
#define dprintf(...)
#define NDEBUG 1
#endif

void render(void*, int, int);
#if FBDEV
#include "fbdev/fbdev.h"
#else
#include "sdl.h"
#endif

typedef struct vec3 {
	FLOAT x;
	FLOAT y;
	FLOAT z;
} vec3;

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

// treat "surface" as an opaque data structure
void render(void* surface, int w, int h)
{
	vec3 red  = { 1, 0, 0 };
	vec3 green  = { 0, 1, 0 };
	
	for(int j=0; j < h; j++){
		for(int i=0; i < w; i++){
			FLOAT amount = (FLOAT)i/(FLOAT)w;
			//vec3 color = lerp(red, green, amount);
			vec3 color = lerp(green, red, amount);
			dprintf("amount %f\n", amount);
			uint32_t pixel = colormap(color);
			set_pixel(surface, i, j, pixel);
		}
	}
}
