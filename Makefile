SMALL := -Wl,-z,norelro -Wl,-z,execstack -Wl,-z,noseparate-code -Wl,--build-id=none -fno-asynchronous-unwind-tables -fno-stack-clash-protection -fno-stack-protector -fcf-protection=none -no-pie -fno-pie -fno-plt -fwhole-program -Wl,--no-eh-frame-hdr
CFLAGS0 := -g -Wall -Wextra -Wpedantic -std=c99
CFLAGS  := -Ofast $(CFLAGS0) $(SMALL)

vanilla:
	$(CC) $(CFLAGS0) -DYOLO=0 -o test test.c `sdl2-config --cflags --libs` -lm

default:
	$(CC) $(CFLAGS)  -DYOLO=1 -o test test.c `sdl2-config --cflags --libs` -lm
	@$(CC) $(CFLAGS) -DYOLO=1 -g0 -fverbose-asm -S test.c `sdl2-config --cflags --libs` -lm 2>/dev/null

small: default
	@./fbdev/pack.sh test

fb:
	$(CC) -T fbdev/ldscript  $(CFLAGS)     -m32 -Os -DFBDEV -nostdlib -ffreestanding -mfpmath=387 -std=gnu99 -o test-fb test.c
	@$(CC) -T fbdev/ldscript $(CFLAGS) -g0 -m32 -Os -DFBDEV -nostdlib -ffreestanding -mfpmath=387 -std=gnu99 -fverbose-asm -S test.c 2>/dev/null

fbsmall: fb
	@./fbdev/pack.sh test-fb
