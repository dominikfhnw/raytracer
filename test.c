#include <SDL.h>
 
int main(int argc, char ** argv)
{
    SDL_Init(SDL_INIT_VIDEO);
 
    SDL_Window * screen = SDL_CreateWindow("My SDL Empty Window",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, 0);
    SDL_Delay(3000);
 
    SDL_Quit();
 
    return 0;
}
