#undef	DEBUG
#undef	YOLO
#define YOLO	1		// take ALL the shortcuts
#define NDEBUG	1		// no asserts in this minimal libc
#define assert(x)

#define WIDTH	1920
#define HEIGHT	1080
#define SIZE    WIDTH*HEIGHT*4

float tanf(float);
#include "libcero.h"
#include "../common.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
FUNC void clear(uint32_t* fb)
{
	uint32_t v = 0;
	while(++v < WIDTH*HEIGHT)
		fb[v] = 0;
}


FUNC void set_pixel(uint32_t *surface, int x, int y, uint32_t pixel)
{
	// XXX constant "4"
	uint32_t* target_pixel = surface + (y * WIDTH) + x;
        *target_pixel = pixel;
}

#pragma GCC diagnostic pop

static void wait(void)
{
	#if 1
	do{}while(1);
	#else
	pause();
	#endif
}

__attribute__((used,noreturn,flatten,GCCATTR)) void _start(void){
	__asm__ __volatile__(INIT_BP);
	int fd = open("/dev/fb0", O_RDWR);
	#if 1
		if(fd < 0){
			__asm__ __volatile__("int3\n\t");
			__builtin_unreachable();
		}
	#endif
	void* mem = mmap(NULL, SIZE, PROT_WRITE, MAP_SHARED, fd, 0);
	render(mem, WIDTH, HEIGHT);
	//exit(0);
	wait();
	__builtin_unreachable();
}



