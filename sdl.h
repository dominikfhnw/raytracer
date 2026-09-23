#include <SDL.h>
#include <assert.h>
#include <math.h>
#include <stdbool.h>

#if FLOAT == float
#define POW(x,y)	powf(x,y)
#define SQRT(x)		sqrtf(x)
#else
#define POW(x,y)	pow(x,y)
#define SQRT(x)		sqrt(x)
#endif

void set_pixel(SDL_Surface *surface, int x, int y, uint32_t pixel)
{
	uint8_t *target_pixel;

	if (YOLO) {
		target_pixel = (uint8_t*)surface->pixels + (y * surface->pitch) + (x * 4);
	}
	else {
		target_pixel = (uint8_t*)surface->pixels + (y * surface->pitch) + (x * surface->format->BytesPerPixel);
	}

        *(uint32_t*)target_pixel = pixel;
}

void ORIGset_pixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
    Uint8 *target_pixel = (Uint8 *)surface->pixels + y * surface->pitch + x * 4;
    *(Uint32 *)target_pixel = pixel;
}

void wait()
{
	SDL_Event event;
	bool quit = false;
	while (!quit) {
		SDL_WaitEvent(&event);

		switch (event.type) {
		case SDL_QUIT:
			quit = true;
			break;
		}
	}
}


int main()
{
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window* window = SDL_CreateWindow("computer graphics dominikr",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, 0);
	SDL_Surface* surface = SDL_GetWindowSurface(window);
	if (SDL_MUSTLOCK(surface) && !YOLO) {
		SDL_LockSurface(surface);
	}

	render(surface, surface->w, surface->h);

	if (SDL_MUSTLOCK(surface) && !YOLO) {
		SDL_UnlockSurface(surface);
	}
	SDL_UpdateWindowSurface(window);

	wait();

	return 0;
}
