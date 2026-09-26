SMALL   := -Wl,-z,norelro -Wl,-z,execstack -Wl,-z,noseparate-code -Wl,--build-id=none -fno-asynchronous-unwind-tables -fno-stack-clash-protection -fno-stack-protector -fcf-protection=none -no-pie -fno-pie -fno-plt -fwhole-program -Wl,--no-eh-frame-hdr
CFLAGS0 := -g -Wall -Wextra -Wpedantic -std=c99
CFLAGS  := -Ofast $(CFLAGS0) $(SMALL)
FBDEV	:= -Oz -march=x86-64-v2 -T fbdev/ldscript -m32 -DFBDEV -nostdlib -ffreestanding -fbuiltin -std=gnu99
WEEK	:= week2.c

.PHONY: week2 week1 clang fb small

week2 week1:
	$(CC) $(CFLAGS0) -DYOLO=0 -o $@ $@.c `sdl2-config --cflags --libs` -lm
	@ls -l $@

small:
	$(CC)  $(CFLAGS) -DYOLO=1 -o $@                $(WEEK) `sdl2-config --cflags --libs` -lm
	@#$(CC) $(CFLAGS) -DYOLO=1 -E		       $(WEEK) `sdl2-config --cflags --libs` -lm > preproc.c
	@$(CC) $(CFLAGS) -DYOLO=1 -g0 -fverbose-asm -S $(WEEK) `sdl2-config --cflags --libs` -lm 2>/dev/null
	@./fbdev/pack.sh $@

fb:
	$(CC)  $(CFLAGS)     $(FBDEV) -o $@            $(WEEK)
	@$(CC) $(CFLAGS) -g0 $(FBDEV) -fverbose-asm -S $(WEEK) -masm=intel -o- 2>/dev/null | grep -vF -e ".loc" -e "#APP" -e "#NO_APP" -e "# 0 " > verb.s
	@./fbdev/pack.sh $@

fbmin:
	$(CC) -O1 -no-pie -fno-guess-branch-probability -m32 -DFBDEV -nostdlib -ffreestanding -fno-stack-protector -fwhole-program -o fbvan test.c
	ls -l fbvan

clang:
	clang $(CFLAGS) $(FBDEV) -fhosted -o $@-1 $(WEEK)
	clang $(CFLAGS0) -DYOLO=0 -o $@-2 $(WEEK) `sdl2-config --cflags --libs` -lm
	@ls -l $@-1 $@-2
