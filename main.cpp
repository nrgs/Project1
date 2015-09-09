#include "SDL.h"

int main(int argc, char *argv[]) {
    SDL_Window *win = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Texture *bitmapTex = NULL;
    SDL_Surface *bitmapSurface = NULL;
    int posX = 100, posY = 100, width = 640, height = 480;

    bool quit = false;
    SDL_Init(SDL_INIT_VIDEO);

    win = SDL_CreateWindow("Awesome Game", posX, posY, width, height, 0);

    renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

    bitmapSurface = SDL_LoadBMP("img/hello.bmp");
    bitmapTex = SDL_CreateTextureFromSurface(renderer, bitmapSurface);
    SDL_FreeSurface(bitmapSurface);

    while (!quit)
    {
        SDL_Event event;
        if (SDL_PollEvent(&event)) 
        {
           /* if(event.type == SDL_KEYDOWN)
                quit=true;
            if(event.type == SDL_KEYUP)
                quit=true;*/
            if (event.type == SDL_QUIT) {
                break;
            }
        }

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, bitmapTex, NULL, NULL);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(bitmapTex);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);
    SDL_Quit();

    return 0;
}
