#undef	FLOAT_EXCEPTION
#undef	DEBUG
#undef	YOLO
#define NO_TLS	1		// we don't support TLS
#define YOLO	1		// take ALL the shortcuts
#define NDEBUG	1		// no asserts in this minimal libc
#define assert(x)

#undef  WIDTH
#define WIDTH	1920
#undef  HEIGHT
#define HEIGHT	1080
#define SIZE    WIDTH*HEIGHT*4

#include "../pre.h"
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

#define update(x)

FUNC void set_pixel(void *surface, int x, int y, uint32_t pixel)
{
	// XXX constant "4"
	uint32_t* target_pixel = (uint32_t*)surface + (y * WIDTH) + x;
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

NOMANGLE __attribute__((used,noreturn,GCCATTR)) void _start(void){
	#if NAKED
	//__asm__ __volatile__(INIT_BP);
	#endif
	int fd = open("/dev/fb0", O_RDWR);
	#if 0
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



