#define STATIC 1
#if STATIC
#define CALL regparm(3)
#define FUNC __attribute__((always_inline,CALL)) static inline
#else
#define FUNC
#endif

#include <stddef.h>
#include <stdint.h>
#include <asm-generic/fcntl.h>
#define _SYS_MMAN_H
#include <bits/mman-linux.h>
#include <asm/unistd.h>
/*
#include <bits/types/struct_timespec.h>
*/

# ifndef __useconds_t_defined
typedef unsigned int useconds_t;
#  define __useconds_t_defined
# endif

#ifndef __off_t_defined
typedef signed long long int off_t;
# define __off_t_defined
#endif


#define STDERR_FILENO 2
#define STDOUT_FILENO 1

#if defined(__x86_64__)
	#define SYSCALL "syscall\n\t"
	#define ARG0 "D"
	#define ARG1 "S"
	#define ARG2 "d"
	#define ARG3 "r10"
	#define ARG4 "r8"
	#define ARG5 "r9"
	#define MMAP __NR_mmap
	#define INIT_BP "push %rsp\n\tpop %rbp\n\t"
#elif defined(__i386__)
	#define SYSCALL "int $0x80\n\t"
	#define ARG0 "b"
	#define ARG1 "c"
	#define ARG2 "d"
	#define ARG3 "esi"
	#define ARG4 "edi"
	#define ARG5 "ebp"
	#define MMAP __NR_mmap2
	#define INIT_BP "mov %esp, %ebp\n\t"
#endif


#if defined(__GNUC__) && !defined(__llvm__) && !defined(__INTEL_COMPILER)
#define GCCATTR naked
#else
#define GCCATTR
#endif

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
static size_t syscall0(size_t nr){
	size_t ret;
	asm volatile(
		SYSCALL
		: "=a" (ret) : "0" (nr)
	);
	return ret;
}

static size_t syscall1(size_t nr, size_t arg0){
	size_t ret;
	asm volatile(
		SYSCALL
		: "=a" (ret) : "0" (nr), ARG0 (arg0)
	);
	return ret;
}

static size_t syscall2(size_t nr, size_t arg0, size_t arg1){
	size_t ret;
	asm volatile(
		SYSCALL
		: "=a" (ret) : "0" (nr), ARG0 (arg0), ARG1 (arg1)
	);
	return ret;
}

static size_t syscall3(size_t nr, size_t arg0, size_t arg1, size_t arg2){
	size_t ret;
	asm volatile(
		SYSCALL
		: "=a" (ret) : "0" (nr), ARG0 (arg0), ARG1 (arg1), ARG2 (arg2)
	);
	return ret;
}

static size_t syscall5(size_t nr, size_t arg0, size_t arg1, size_t arg2, size_t arg3, size_t arg4){
	size_t ret;
	register size_t arg3r asm (ARG3) = arg3;
	register size_t arg4r asm (ARG4) = arg4;
	asm volatile(
		SYSCALL
		: "=a" (ret) : "0" (nr), ARG0 (arg0), ARG1 (arg1), ARG2 (arg2), "r" (arg3r), "r" (arg4r)
	);
	return ret;
}

FUNC void* mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset){
	(void)addr;
	(void)offset;
	void* ret;
	register size_t arg3r asm (ARG3) = flags;
	register size_t arg4r asm (ARG4) = fd;
	#if defined(__x86_64__)
		register size_t arg5r asm (ARG5) = 0;
	#endif
	asm volatile(
		#if defined(__i386__)
			"push %%ebp\n\t"
			"xor %%ebp,%%ebp\n\t"
		#endif
		SYSCALL
		#if defined(__i386__)
			"pop %%ebp\n\t"
			: "=a" (ret) : "0" (MMAP), ARG1 (length), ARG2 (prot), "r" (arg3r), "r" (arg4r)
		#else
			: "=a" (ret) : "0" (MMAP), ARG1 (length), ARG2 (prot), "r" (arg3r), "r" (arg4r), "r" (arg5r)
		#endif
	);
	return ret;
}

static size_t write(int fd, const void* buf, size_t count){
	return syscall3(__NR_write, fd, (size_t)buf, count);
}

/*
FUNC int usleep(useconds_t usec){
	volatile struct timespec sleep;
	sleep.tv_nsec = usec*1000;
	return syscall2(__NR_nanosleep, (size_t)&sleep, 0);
}
*/

static int open(const char *pathname, int flags){
	return syscall2(__NR_open, (size_t)pathname, flags);
}

static int print(const char* string) {
	return (int)write(STDOUT_FILENO, string, __builtin_strlen(string));
}

static int pause() {
	return syscall0(__NR_pause);
}

static void exit_dc(){
	syscall0(__NR_exit);
	__builtin_unreachable();
}

static void exit(int status){
	syscall1(__NR_exit, status);
	__builtin_unreachable();
}

static float sqrtf(float arg0) {
	float ret;
	asm volatile(
		"fsqrt"
		: "=t" (ret) : "0" (arg0)
	);
	return ret;
}

static double sqrt(double arg0) {
	double ret;
	asm volatile(
		"fsqrt"
		: "=t" (ret) : "0" (arg0)
	);
	return ret;
}

#pragma GCC diagnostic pop

