
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>

int main(int argc, char* argv[])
{
    SDL_Surface *screen = NULL;
    SDL_Window *window = NULL;
    SDL_Surface *shipImage = NULL;
    SDL_Surface *backgroundImage = NULL;
    SDL_Rect rect;
    SDL_Renderer* renderer = NULL;

    SDL_Init(SDL_INIT_VIDEO); 

    // create the window 
    window = SDL_CreateWindow("Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, 0);

    /*Do we need to crate renderer?*/

    screen = SDL_GetWindowSurface(window);
    bool quit = false;

    /* set sprite position */
    rect.x = 200;
    rect.y = 200;
    if(window != NULL)
    {
        backgroundImage = IMG_Load("background.png"); //Load Background
        shipImage = IMG_Load("unicorn2.png"); // loads image

       // SDL_BlitSurface(backgroundImage, NULL, screen, NULL); 
       
        SDL_Event event;
        while(!quit)
        {
            
            while(SDL_PollEvent(&event) != 0)
            {
                //User requests quit
                switch(event.type)
                {
                    case SDL_QUIT:
                            quit = true;
                            break;

                    case SDL_KEYDOWN:
                            rect.y -= 10;
                            break;
                    case SDL_KEYUP:
                            rect.y += 10;
                            break;       
                }
                // if(event.type == SDL_QUIT)
                // {
                //     quit = true;
                // }
                // else if(event.type == SDL_KEYDOWN)
                // {
                //     rect.y += 10;
                // }
                // else if(event.type == SDL_KEYUP)
                // {
                //     rect.y -= 10;
                // }

            }

            //Clear screen
           // SDL_RenderClear(renderer);

            //Apply the images
            SDL_BlitSurface(backgroundImage, NULL, screen, NULL); 
            SDL_BlitSurface(shipImage, NULL, screen, &rect); 
            
            //Update screen
            //SDL_RenderPresent(renderer);
            //Update the surface
            SDL_UpdateWindowSurface(window);
  
            
        }

        SDL_FreeSurface(backgroundImage);
        SDL_FreeSurface(shipImage);
    }
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

