#CC := gcc
# -mfpmath=387
SMALL := -Wl,-z,norelro -Wl,-z,execstack -Wl,-z,noseparate-code -Wl,--gc-sections -Wl,--build-id=none -fno-asynchronous-unwind-tables -fno-stack-clash-protection -fno-stack-protector -fcf-protection=none -no-pie -fno-pie -fno-plt -fwhole-program -Wl,--no-eh-frame-hdr
CFLAGS := -Ofast -g -Wall -Wextra -Wpedantic -std=c99 $(SMALL)
#CFLAGS := -Ofast -g -Wall -Wextra
#CFLAGS := -g -Wall

default:
	$(CC) $(CFLAGS) -fverbose-asm -S test.c `sdl2-config --cflags --libs` -lm
	$(CC) $(CFLAGS) -o test test.c `sdl2-config --cflags --libs` -lm

small: default
	@ls -l test
	@cp test test2
	@sstrip test2
	@lz4pack test2
