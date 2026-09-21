CFLAGS := -Os -s
export CFLAGS

default:
	$(CC) $(CFLAGS) -o test test.c `sdl2-config --cflags --libs`
	@ls -l test
