
#include "SDL.h"
#include <SDL_image.h>
int main(int argc, char* argv[])
{
    SDL_Surface *screen = NULL;
    SDL_Window *window = NULL;
    SDL_Surface *image = NULL;
    SDL_Rect rect;

    SDL_Init(SDL_INIT_VIDEO); 

    // create the window 
    window = SDL_CreateWindow("Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, 0);

    /*Do we need to crate renderer?*/

    screen = SDL_GetWindowSurface(window);
    bool quit = false;
    int positionX = 100;
    int positionY = 200;
    
    

    if(window != NULL)
    {
        image = IMG_Load("emoji11.png"); // loads image
        SDL_Event event;
        while(!quit)
        {
            rect.x = positionX; 
            rect.y = positionY;
            if(SDL_PollEvent(&event))
            {
                if(event.type == SDL_QUIT)
                {
                    quit = true;
                }
                if(event.type == SDL_KEYDOWN)
                {
                    positionY -= 10;
                }
                if(event.type == SDL_KEYUP)
                {
                    positionY +=10;
                } 

            }
            
            

            SDL_BlitSurface(image, NULL, screen, &rect); 
         SDL_UpdateWindowSurface(window);
            
           
        }

    }
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

