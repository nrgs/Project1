
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <cstdlib>
#include <ctime>


//function to load image of any format
SDL_Surface* loadImage(std::string path)
{
    SDL_Surface * surface = IMG_Load(path.c_str());;
    return surface;
}


//Creating a class for moving object (unicorn)
class Unicorn
{
public:
    Unicorn()
        : surface(loadImage("images/unicorn_m.png")), xSpeed(0), ySpeed(0)
    {
       rect.x = 140;
       rect.y = 140;
 
    }
    SDL_Surface * surface;
    SDL_Rect rect; 
    int xSpeed;
    int ySpeed;    
};


//Creating a class for 
class Labirint
{
public:
    Labirint()
        : surface(loadImage("images/star_m.png")), skipCell(false)
    {
        rect.w = 20;
        rect.h = 20;
    }
    SDL_Surface * surface;
    SDL_Rect rect;  
    bool skipCell;  
};


int main(int argc, char* argv[])
{
    //Constant values for screen dimentions
    const int SCREEN_WIDTH = 640; 
    const int SCREEN_HEIGHT = 480;


    //initializing SDL variables
    SDL_Surface *screen = NULL;
    SDL_Window *window = NULL;
    SDL_Surface *backgroundImage = NULL;
    SDL_Renderer* renderer = NULL;
    const Uint8 * key = SDL_GetKeyboardState(NULL);

    //Create unicorn object 
    Unicorn unicorn; 

    //Constatant values for labirint cell dimentions
    const int CELL_WIDTH = 32;
    const int CELL_HEIGHT = 24;
    const int game_speed = 4;

    //Generating random
    srand(time(NULL));
    //Create a labirint object as 2D array
    Labirint labirint[CELL_WIDTH][CELL_HEIGHT];

    //to cut out
    for (int i = 0; i < CELL_WIDTH; i++)
    {
        for (int j = 0; j < CELL_HEIGHT; j++)
        {
            labirint[i][j].rect.x = i * CELL_WIDTH;
            labirint[i][j].rect.y = j * CELL_HEIGHT;

        }
    }
    int blankCellpositionTop = 6;
    int blankCellpositionBottom = 15;
    for (int i = 0; i < 32; i++)
    {
        for(int j = blankCellpositionTop; j < blankCellpositionBottom; j++)
        {

            labirint[i][j].skipCell = true; 
            
        
        }
       
       
        blankCellpositionTop = rand()% + 6;
        blankCellpositionBottom = blankCellpositionTop + 10;

        
        

    }
    

    SDL_Init(SDL_INIT_VIDEO); 

    // create the window 
    window = SDL_CreateWindow("Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);

    screen = SDL_GetWindowSurface(window);


    bool quit = false;
    if(window != NULL)
    {
        //Load Background
        backgroundImage = IMG_Load("images/background_sm.png"); 
        SDL_Event event;

        //While game is running
        while(!quit)
        {
            int totalTime, timeSinceLastLoop, oldTotalTime;
            totalTime = SDL_GetTicks();
            timeSinceLastLoop = totalTime - oldTotalTime;
            oldTotalTime = totalTime;
             if(SDL_PollEvent(&event) != 0)
            {
               
                if(event.type == SDL_QUIT)
                {
                    quit = true;
                }

                if(key[SDL_SCANCODE_LEFT])
                {
                    unicorn.xSpeed = -1;
                    unicorn.rect.x += unicorn.xSpeed * timeSinceLastLoop / game_speed;
                }
                 if(key[SDL_SCANCODE_RIGHT])
                {
                    unicorn.xSpeed = 1;
                    unicorn.rect.x += unicorn.xSpeed * timeSinceLastLoop / game_speed;
                }
                 if(key[SDL_SCANCODE_UP])
                {
                    unicorn.ySpeed = -1;
                    unicorn.rect.y += unicorn.ySpeed * timeSinceLastLoop/game_speed;
                }
                 if(key[SDL_SCANCODE_DOWN])
                {   
                    unicorn.ySpeed = 1;
                    unicorn.rect.y += unicorn.ySpeed * timeSinceLastLoop /game_speed;
                }

            }

           //Move background
            for (int i = 0; i < CELL_WIDTH; i++)
            {
                for (int j = 0; j < CELL_HEIGHT; j++)
                {
                    labirint[i][j].rect.x -= std::abs(unicorn.xSpeed) * timeSinceLastLoop/game_speed;
                }
            }







      
            
            //Apply the images
            SDL_BlitSurface(backgroundImage, NULL, screen, NULL); 
             for (int i = 0; i < CELL_WIDTH; i++)
            {
                for (int j = 0; j < CELL_HEIGHT; j++)
                {
                    if(labirint[i][j].skipCell == false)
                    {

                        SDL_BlitSurface(labirint[i][j].surface, NULL, screen, &labirint[i][j].rect); 
                    }
                }
            }
            SDL_BlitSurface(unicorn.surface, NULL, screen, &unicorn.rect); 
        
            
            //Update the surface
            SDL_UpdateWindowSurface(window);
  
            
        }

        SDL_FreeSurface(backgroundImage);
        SDL_FreeSurface(unicorn.surface);
    }
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

