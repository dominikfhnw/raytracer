COMMIT	:= $(shell git rev-parse --short HEAD)
DIRTY	:= $(shell git status --untracked-files=no --porcelain)
ifneq ($(DIRTY),)
COMMIT	:= $(COMMIT)+
endif

SMALL   := -Wl,--build-id=none -no-pie -fno-pie -fno-plt -fwhole-program -fipa-pta -fomit-frame-pointer -fcf-protection=none -Wno-unknown-pragmas -falign-functions=1 -falign-jumps=1 -falign-labels=1 -falign-loops=1 -fno-prefetch-loop-arrays -fno-ipa-cp-clone
#SMALL   := -no-pie -fno-pie -fno-plt -fwhole-program -fipa-pta -fomit-frame-pointer -fcf-protection=none -Wno-unknown-pragmas
CFLAGS0 := -g -Wall -Wextra -Wpedantic -DCOMMIT=\"$(COMMIT)\"
CFLAGS  := $(CFLAGS0) $(SMALL)

#FBDEV	:= -Ofast -ffast-math -march=x86-64-v2 -T fbdev/ldscript -m32 -DFBDEV -nostdlib -ffreestanding -std=gnu99
FBDEV	:= -Oz -ffast-math -march=pentium4 -mfpmath=387 -T fbdev/ldscript -m32 -DFBDEV -nostdlib -ffreestanding
WEEK	:= week3.c
ASM	=  -g0 -fverbose-asm -S -masm=intel -o- 2>/dev/null | grep -vF -e ".loc" -e "\#APP" -e "\#NO_APP" -e "\# 0 " > $@.s
LIB	:= `sdl2-config --cflags --libs` -lm -lSDL2_image
SANIT	:= -fsanitize=undefined -fsanitize=address -fsanitize=pointer-compare -fsanitize=pointer-subtract -fsanitize=leak -fsanitize-address-use-after-scope -fanalyzer

FAST	 = -Ofast -fopenmp -march=native -DNDEBUG $@.c
DEBUG	 = $(SANIT) -Og $@.c
W	 = $(CXX) $(CFLAGS0) -std=c++20 $(FAST) $(LIB)

.PHONY: week3 week2 week1 clang fb fbxx small

week3 week2 week1:
	$(W) -o $@
	@#$(W) $(ASM)
	@ls -l $@

small:
	$(CC)   $(CFLAGS) -DYOLO=1 -Oz -DDEBUG=0 -DNDEBUG $(WEEK) $(LIB) -o $@
	@$(CC)  $(CFLAGS) -DYOLO=1 -Oz -DDEBUG=0 -DNDEBUG $(WEEK) $(LIB) $(ASM)
	@./fbdev/pack.sh $@

fb:
	$(CC)   $(CFLAGS) $(FBDEV) $(WEEK) -std=gnu99 -o $@
	@$(CC)  $(CFLAGS) $(FBDEV) $(WEEK) -std=gnu99 $(ASM)
	@nm -C -S -t d --size-sort $@
	@RELEASE=0 ./fbdev/pack.sh $@

fbxx:
	g++-12  $(CFLAGS) $(FBDEV) $(WEEK) -std=c++20 -o $@
	@g++-12 $(CFLAGS) $(FBDEV) $(WEEK) -std=c++20 $(ASM)
	@nm -C -S -t d --size-sort $@
	@RELEASE=1 ./fbdev/pack.sh $@

clang:
	clang-15   $(CFLAGS) $(FBDEV) -fhosted            -o $@-1   $(WEEK)
	clang++-15 $(CFLAGS) $(FBDEV) -fhosted -std=c++11 -o $@-1xx $(WEEK)
	clang-15   $(CFLAGS0) -DYOLO=0                    -o $@-2   $(WEEK) $(LIB)
	clang++-15 $(CFLAGS0) -DYOLO=0         -std=c++11 -o $@-2xx $(WEEK) $(LIB)
	@ls -l $@-1 $@-1xx $@-2 $@-2xx ||:
