
#include "SDL.h"
#include <SDL_image.h>

int main(int argc, char* argv[])
{
    SDL_Surface *screen = NULL;
    SDL_Window *window = NULL;
    SDL_Surface *shipImage = NULL;
    SDL_Surface *backgroundImage = NULL;
    SDL_Rect rect;

    SDL_Init(SDL_INIT_VIDEO); 

    // create the window 
    window = SDL_CreateWindow("Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, 0);

    /*Do we need to crate renderer?*/

    screen = SDL_GetWindowSurface(window);
    bool quit = false;

    /* set sprite position */
    int positionX = 200;
    int positionY = 200;
    rect.x = positionX;
    rect.y = positionY;
    if(window != NULL)
    {
        backgroundImage = IMG_Load("background.png"); //Load Background
        shipImage = IMG_Load("unicorn2.png"); // loads image
       
        SDL_Event event;
        while(!quit)
        {
            
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
                    positionY += 10;
                }
            }


            
        
            SDL_BlitSurface(backgroundImage, NULL, screen, NULL); 
            SDL_BlitSurface(shipImage, NULL, screen, &rect); 
            SDL_UpdateWindowSurface(window);
        }
    }
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

