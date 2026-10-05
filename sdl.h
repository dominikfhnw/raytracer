#include "pre.h"
#include <SDL.h>
#include <assert.h>
#include <math.h>
#include <sys/time.h>

#include "common.h"

static void set_pixel(void *window, const int x, const int y, const uint32_t pixel)
{
	#if YOLO
	# define pixel_bytes 4
	#else
	# define pixel_bytes surface->format->BytesPerPixel
	#endif
	SDL_Surface *surface = SDL_GetWindowSurface((SDL_Window*)window);

	uint8_t *target_pixel = (uint8_t*)surface->pixels + (y * surface->pitch) + (x * pixel_bytes);
        *(uint32_t*)target_pixel = pixel;
}

static void update(void *window)
{
	SDL_UpdateWindowSurface((SDL_Window*)window);
	SDL_Event event;
	SDL_PollEvent(&event);
	if (event.type == SDL_QUIT)
		exit(0);
}

static void wait(void)
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

#if DEBUG
static void check(const void* const ptr, const char* const str)
{
	if (ptr == NULL){
		const char* geterr = SDL_GetError();
		const char* err;
		if ( strlen(geterr) == 0 )
			err = "<no SDL error available>";
		else
			err = geterr;
		dprintf("%s: %s\n", str, err);
		exit(2);
	}
}
#else
#define check(x,y)
#endif

#define xstr(s) str(s)
#define str(s) #s
#if _OPENMP
#define OMP " omp"
#else
#define OMP
#endif
#define STATUS "dominikr samples " xstr(SAMPLES) " P " xstr(P) OMP

int main(void)
{
	dprintf("START\n");
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		check(NULL, "init fail");
		#if !DEBUG
			return 2;
		#endif
	}
	SDL_Window* window = SDL_CreateWindow(STATUS ,
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, 0);
	dprintf(STATUS "\n");
	check(window, "window init failed");
	SDL_Surface* surface = SDL_GetWindowSurface(window);
	check(surface, "getsurface failed");

	if (SDL_MUSTLOCK(surface) && !YOLO) {
		SDL_LockSurface(surface);
	}

	#if DEBUG
		struct timeval tv1;
		struct timeval tv2;

		gettimeofday(&tv1,NULL);
	#endif
	render(window, surface->w, surface->h);
	#if DEBUG
		gettimeofday(&tv2,NULL);

		long int t1 = tv1.tv_sec*1e6 + tv1.tv_usec;
		long int t2 = tv2.tv_sec*1e6 + tv2.tv_usec;
		long int diff = t2 - t1;
		dprintf("time: %fs\n", diff/1e6);
	#endif

	if (SDL_MUSTLOCK(surface) && !YOLO) {
		SDL_UnlockSurface(surface);
	}
	SDL_UpdateWindowSurface(window);

	dprintf("FIN\n");
	wait();

	return 0;
}
