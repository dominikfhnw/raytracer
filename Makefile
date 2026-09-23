SMALL := -Wl,-z,norelro -Wl,-z,execstack -Wl,-z,noseparate-code -Wl,--gc-sections -Wl,--build-id=none -fno-asynchronous-unwind-tables -fno-stack-clash-protection -fno-stack-protector -fcf-protection=none -no-pie -fno-pie -fno-plt -fwhole-program -Wl,--no-eh-frame-hdr
CFLAGS0 := -g -Wall -Wextra -Wpedantic -std=c99
CFLAGS  := -Ofast $(CFLAGS0) $(SMALL)

vanilla:
	$(CC) $(CFLAGS0) -DYOLO=0 -o test test.c `sdl2-config --cflags --libs` -lm

default:
	$(CC) $(CFLAGS)  -DYOLO=1 -o test test.c `sdl2-config --cflags --libs` -lm
	@$(CC) $(CFLAGS) -DYOLO=1 -g0 -fverbose-asm -S test.c `sdl2-config --cflags --libs` -lm 2>/dev/null

small: default
	@ls -l test
	@cp test test2
	@sstrip test2
	@lz4pack test2

fb:
	$(CC) $(CFLAGS)      -m32 -Os -DFBDEV -nostdlib -ffreestanding -mfpmath=387 -std=gnu99 -o test-fb test.c -lm
	@$(CC) $(CFLAGS) -g0 -m32 -Os -DFBDEV -nostdlib -ffreestanding -mfpmath=387 -std=gnu99 -fverbose-asm -S test.c -lm 2>/dev/null

fbsmall: fb
	@ls -l test-fb
	@cp test-fb test-fb2
	@sstrip test-fb2
	@lz4pack test-fb2
