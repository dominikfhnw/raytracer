SMALL   := -Wl,-z,norelro -Wl,-z,execstack -Wl,-z,noseparate-code -Wl,--build-id=none -Wl,--no-eh-frame-hdr -fno-asynchronous-unwind-tables -fno-stack-clash-protection -fno-stack-protector -fcf-protection=none -no-pie -fno-pie -fno-plt -fwhole-program -Wno-unknown-pragmas
CFLAGS0 := -g -Wall -Wextra -Wpedantic -std=c99
CFLAGS  := $(CFLAGS0) $(SMALL)
#FBDEV	:= -Oz -march=i386 -mtune=x86-64 -mfpmath=387 -T fbdev/ldscript -m32 -DFBDEV -nostdlib -ffreestanding -fbuiltin -std=gnu99
#FBDEV	:= -Oz -march=pentium2 -mtune=native -T fbdev/ldscript -Wl,--nmagic -m32 -DFBDEV -nostdlib -ffreestanding -fbuiltin -std=gnu99
FBDEV	:= -Oz -march=x86-64-v2 -mfpmath=387 -T fbdev/ldscript -Wl,--nmagic -m32 -DFBDEV -nostdlib -ffreestanding -fbuiltin -std=gnu99
DOS	:= -Oz -march=x86-64-v2 -mfpmath=387 -T dos32/ldscript -Wl,--nmagic -m16 -DDOS   -nostdlib -ffreestanding -fbuiltin -std=gnu99
ASM0	:= -g0 -fverbose-asm -S
WEEK	:= week3.c
ASM	=  $(ASM0) -masm=intel -o- 2>/dev/null | grep -vF -e ".loc" -e "\#APP" -e "\#NO_APP" -e "\# 0 " > $@.s
LIB	:= `sdl2-config --cflags --libs` -lm
SANIT	:= -fsanitize=undefined -fsanitize=address -fsanitize=pointer-compare -fsanitize=pointer-subtract -fsanitize=leak -fsanitize-address-use-after-scope -fanalyzer
SANIT	:=


.PHONY: week3 week2 week1 clang fb fbxx small dos fbmin

week3 week2 week1:
	$(CC)  $(CFLAGS0) $(SANIT)     -Ofast -fopenmp -march=native $@.c $(LIB) -o $@
	@$(CC) $(CFLAGS0) $(SANIT) -g0 -Ofast -fopenmp -march=native $@.c $(LIB) $(ASM)
	@ls -l $@

small:
	$(CC)  $(CFLAGS) $(SANIT) -DYOLO=1 -Oz -DDEBUG=0 $(WEEK) $(LIB) -o $@
	@$(CC) $(CFLAGS) $(SANIT) -DYOLO=1 -Oz -DDEBUG=0 $(WEEK) $(LIB) $(ASM)
	@./fbdev/pack.sh $@

fb:
	$(CC)   $(CFLAGS)     $(FBDEV) $(WEEK) -o $@
	@$(CC)  $(CFLAGS) -g0 $(FBDEV) $(WEEK) $(ASM)
	@./fbdev/pack.sh $@

fbxx:
	g++-12  $(CFLAGS)     $(FBDEV) $(WEEK) -std=c++20 -o $@
	@g++-12 $(CFLAGS) -g0 $(FBDEV) $(WEEK) -std=c++20 $(ASM)
	@./fbdev/pack.sh $@

dos:
	@#$(CC) $(CFLAGS) -g0 $(DOS)   -fverbose-asm -S $(WEEK) -masm=intel -o- 2>/dev/null | grep -vF -e ".loc" -e "#APP" -e "#NO_APP" -e "# 0 " > dos.s
	@#@$(CC) $(CFLAGS) -g0 $(DOS)   -fverbose-asm -S $(WEEK) -masm=intel -o- 2>/dev/null > dos.s
	@#$(CC)  $(CFLAGS)     $(DOS) -fverbose-asm -o $@.com        $(WEEK)
	$(CC)  $(CFLAGS0) -O0 -fverbose-asm -fno-asynchronous-unwind-tables -fno-stack-clash-protection -fno-stack-protector -fcf-protection=none -no-pie -fno-pie -fno-plt -fwhole-program -DDOS -nostdlib -ffreestanding -fbuiltin -std=gnu99    $(DOS) -fverbose-asm -o $@.com        $(WEEK)
	$(CC)  $(CFLAGS0) -O0 -fverbose-asm -fno-asynchronous-unwind-tables -fno-stack-clash-protection -fno-stack-protector -fcf-protection=none -no-pie -fno-pie -fno-plt -fwhole-program -DDOS -nostdlib -ffreestanding -fbuiltin -std=gnu99    $(DOS) -fverbose-asm -S -o $@.s       $(WEEK)
	@ndis2 -b16 $@.com
	@ls -l dos.com

fbmin:
	$(CC) -O2 -no-pie -fno-guess-branch-probability -m32 -DFBDEV -nostdlib -ffreestanding -fno-stack-protector -fwhole-program -o $@ $(WEEK)
	@ls -l $@

clang:
	clang        $(CFLAGS) $(FBDEV) -mfpmath=sse -fhosted              -o $@-1   $(WEEK)
	clang++-15   $(CFLAGS) $(FBDEV) -mfpmath=sse -fhosted -std=c++11 -o $@-1xx $(WEEK)
	clang        $(CFLAGS0) -DYOLO=0 -o $@-2                                     $(WEEK) $(LIB)
	clang++-15   $(CFLAGS0) -DYOLO=0 -o $@-2xx            -std=c++11           $(WEEK) $(LIB)
	@ls -l $@-1 $@-1xx $@-2 $@-2xx ||:
