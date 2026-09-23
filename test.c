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

#if FBDEV
#include "fbdev/fbdev.h"
#else
#include "sdl.h"
#endif

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
