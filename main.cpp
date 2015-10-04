//By Narges Zare and Viktorria Lavrenchenko
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <cstdlib>
#include <ctime>
#include <iostream>


 //Constant values for screen dimentions
const int SCREEN_WIDTH = 640; 
const int SCREEN_HEIGHT = 480;

//function to load image of any format
SDL_Surface* loadImage(std::string path)
{
    SDL_Surface * surface = IMG_Load(path.c_str());;
    return surface;
}

//Box collision detector [Took from LazyFoo]
bool checkCollision(SDL_Rect a, SDL_Rect b)
{
   
     //The sides of the rectangles 
    int leftA, leftB; 
    int rightA, rightB;
    int topA, topB; 
    int bottomA, bottomB;

    //Calculate the sides of rect A 
    leftA = a.x; 
    rightA = a.x + a.w; 
    topA = a.y; 
    bottomA = a.y + a.h;

     //Calculate the sides of rect B 
    leftB = b.x; 
    rightB = b.x + b.w; 
    topB = b.y; 
    bottomB = b.y + b.h;

     //If any of the sides from A are outside of B 
    if( bottomA <= topB ) { return false; }
    if( topA >= bottomB ) { return false; }
    if( rightA <= leftB ) { return false; }
    if( leftA >= rightB ) { return false; }
    //If none of the sides from A are outside B 
    return true;
}

//Creating a class for moving object (unicorn)
class Unicorn
{
public:
     Unicorn()
        : surface(loadImage("images/unicorn.png")), xSpeed(0), ySpeed(0)
    {
       rect.x = 100;
       rect.y = 140;;
     }

    SDL_Surface * surface;
    SDL_Rect rect; 
   
    int xSpeed;
    int ySpeed;    
};

//Creating a class for Labirint
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


//Declaing tff font
TTF_Font* font;
//white color for text
SDL_Color textColor = { 255, 255, 255, 255 };
SDL_Surface * scoreBoard;
SDL_Rect scoreBoardRect;

int main(int argc, char* argv[])
{
   TTF_Init();

   //Generating random
    srand(time(NULL));

    //initializing SDL variables
    SDL_Surface *screen = NULL;
    SDL_Window *window = NULL;
    SDL_Surface *backgroundImage = NULL;
    SDL_Surface *gameOverImage = NULL;
    SDL_Surface *firstImage = NULL;
    SDL_Renderer* renderer = NULL;
    const Uint8 * key = SDL_GetKeyboardState(NULL);
    
    font = TTF_OpenFont("OpenSans-Bold.ttf", 24);
    
    //Position for the score:
    scoreBoardRect.x = 0; 
    scoreBoardRect.y = 0;
    scoreBoardRect.h = 100;
    scoreBoardRect.w = 100;

     int score = 0; 

    //Create unicorn object 
    Unicorn unicorn; 

    //Constatant values for labirint cell dimentions
    const int CELL_WIDTH = 32;
    const int CELL_HEIGHT = 24;
    const int game_speed = 6;

    
    //Create a labirint object as 2D array
    Labirint labirint[CELL_WIDTH][CELL_HEIGHT];

    //create a grid of 20x20 cells
    for (int i = 0; i < CELL_WIDTH; i++)
    {
        for (int j = 0; j < CELL_HEIGHT; j++)
        {
            labirint[i][j].rect.x = i * 20;
            labirint[i][j].rect.y = j * 20;

        }
    }

    //Remove stars randomly in the range of 5 to 18
    int blankCellpositionTop = 5;
    int blankCellpositionBottom = 18;
    for (int i = 0; i < 32; i++)
    {
        for(int j = blankCellpositionTop; j < blankCellpositionBottom; j++)
        {
            labirint[i][j].skipCell = true;     
        }
       
       //lame labyrinth 
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

            //Popping out first page of the game
            // firstImage = loadImage("images/firstPage.png");
            // SDL_BlitSurface(firstImage, NULL, screen, NULL); 
            
            //Calculating delta
            int totalTime, delta, oldTotalTime;
            totalTime = SDL_GetTicks();
            delta = totalTime - oldTotalTime;
            oldTotalTime = totalTime;

            //Keyboard
             if(SDL_PollEvent(&event) != 0)
            {
                if(event.type == SDL_QUIT)
                {
                    quit = true;
                }

                if(key[SDL_SCANCODE_LEFT])
                {
                    unicorn.xSpeed = -1;
                    unicorn.rect.x += unicorn.xSpeed * delta * 0.5;
                }
                 if(key[SDL_SCANCODE_RIGHT])
                {
                    
                    unicorn.xSpeed = 1;
                    unicorn.rect.x += unicorn.xSpeed * delta * 0.5;
                }

                 if(key[SDL_SCANCODE_UP])
                {

                    unicorn.ySpeed = -1;
                    unicorn.rect.y += unicorn.ySpeed * delta * 0.5;  
                }

                 if(key[SDL_SCANCODE_DOWN])
                {   
                    unicorn.ySpeed = 1;
                    unicorn.rect.y += unicorn.ySpeed * delta * 0.5;
        
                }
            }

             //Move background with David's help
            for (int i = 0; i < CELL_WIDTH; i++)
            {
                for (int j = 0; j < CELL_HEIGHT; j++)
                {
                    labirint[i][j].rect.x -= delta / 5;
                }
            }

            int rightMostColumn = labirint[0][0].rect.x;
            for (int i = 1; i < CELL_WIDTH; ++i)
            {
                if (labirint[i][0].rect.x + CELL_WIDTH > rightMostColumn)
                    rightMostColumn = labirint[i][0].rect.x + CELL_WIDTH;
            }
        
            for (int i = 0; i <CELL_WIDTH; ++i)
            {
                if (labirint[i][0].rect.x < 0)
                {
                    for (int j = 0; j < CELL_HEIGHT; ++j)
                    {
                         labirint[i][j].rect.x = rightMostColumn;
                     }
                }
            }

            //Check Collision
            for (int i = 0; i < CELL_WIDTH; i++)
            {
                for (int j = 0; j < CELL_HEIGHT; j++)
                {
                    if(labirint[i][j].skipCell == false)
                    {
                        if(checkCollision(unicorn.rect, labirint[i][j].rect))
                        {
                            //setting position for game over rect
                            SDL_Rect gameOver;
                            gameOver.x = 200;
                            gameOver.y = 200;

                           //poping out game over images
                            

                            gameOverImage = loadImage("images/gameOver.png");
                            SDL_BlitSurface(backgroundImage, NULL, screen, NULL); 
                            SDL_BlitSurface(gameOverImage, NULL, screen, &gameOver); 
                            std::string score_displayed = "Final Score: " + std::to_string(score);

                            //Display score
                            scoreBoard = TTF_RenderText_Solid(font, score_displayed.c_str(), textColor);
                            scoreBoardRect.x = 200;
                            scoreBoardRect.y = 320;
                            SDL_BlitSurface(scoreBoard, NULL, screen, &scoreBoardRect);
                            
                            SDL_UpdateWindowSurface(window);
                            SDL_Delay(3000);
                            quit = true;
                            break;
                        } 
                    } 
                }
            }

            //Calculating score
            int ConverToSecond = oldTotalTime / 30;
            score = 10 * ConverToSecond;

            //Apply the star images
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
            //Apply the unicorn image
            SDL_BlitSurface(unicorn.surface, NULL, screen, &unicorn.rect);

            std::string score_displayed = "Score: " + std::to_string(score);
            //Display score
            scoreBoard = TTF_RenderText_Solid(font, score_displayed.c_str(), textColor);
            
            SDL_BlitSurface(scoreBoard, NULL, screen, &scoreBoardRect);
            
            //Update the surface
            SDL_UpdateWindowSurface(window);    
        }

        SDL_FreeSurface(gameOverImage);
        SDL_FreeSurface(backgroundImage);
        SDL_FreeSurface(unicorn.surface);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

