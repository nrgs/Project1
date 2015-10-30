
//By Narges Zare and Viktorria Lavrenchenko
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include "SDL_net.h"
using std::cout;
using std::cerr;
using std::endl;
// A programs communicating via the network must agree on which port they
// will use. The computer uses the port number to know which program network
// messages are being sent to.
 
const int PORT = 3750;

// The client and server will send each other messages. 1 means "I pressed
// left," 0 means "I pressed right."

const int LEFT = 1;
const int RIGHT = 0;

const int UNICORN_POS = 0;
int message;

//The code to send unicorn position 
void sendUnicornPosition(int x, int y, TCPsocket sock)
{
    message = UNICORN_POS;
    int sent;
    sent = SDLNet_TCP_Send(sock, &message, sizeof(message));
    if (sent != sizeof(sock, message))
    cerr << "SDLNet_TCP_Send: " << SDLNet_GetError() << endl;
    sent = SDLNet_TCP_Send(sock, &x, sizeof(x));
    if (sent != sizeof(sock, x))
    cerr << "SDLNet_TCP_Send: " << SDLNet_GetError() << endl;
    sent = SDLNet_TCP_Send(sock, &y, sizeof(y));
    if (sent != sizeof(sock, y))
    cerr << "SDLNet_TCP_Send: " << SDLNet_GetError() << endl;
}


//Constant values for screen dimentions
const int SCREEN_WIDTH = 640; 
const int SCREEN_HEIGHT = 480;

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


const Uint8 * key = SDL_GetKeyboardState(NULL);


// Prototypes for server and client functions.

void beTheServer();
void beTheClient(const char servername[]);

//---------------------Main Functiion--------------------//

int main(int argc, const char *argv[])
{
    // Is this program the host, or the client?
    bool operating_as_host;

    // Check the command line arguments.
    if (argc == 1)
    {
        operating_as_host = true;
        cout << "Operating as host: clients connect to me." << endl;
    }
    else if (argc == 2)
    {
        operating_as_host = false;
        cout << "Operating as client, connecting to " << argv[1] << endl;
    }
    else
    {
        cerr << "Too many command line arguments" << endl;
        return 1;
    }
    
    // Initialize SDL.
    if (SDL_Init(0) == -1)
    {
        cerr << "SDL_Init: " << SDL_GetError() << endl;
        return 1;
    }

    // Initialize SDL_net.
    if (SDLNet_Init() == -1)
    {
        cerr << "SDLNet_Init: " << SDLNet_GetError() << endl;
        return 1;
    }

    if (operating_as_host)
        beTheServer();
    else
        beTheClient(argv[1]);
    
    // Shut everything down.
    SDLNet_Quit();
    TTF_Quit();
    SDL_Quit();

    return(0);
}


void beTheClient(const char servername[])
{
    TTF_Init();
     //initializing SDL variables
    SDL_Surface *screen = NULL;
    SDL_Window *window = NULL;
    SDL_Surface *backgroundImage = NULL;
    SDL_Surface *gameOverImage = NULL;
    SDL_Surface *firstImage = NULL;
    SDL_Renderer* renderer = NULL;

     //************************Make a window to receive input*********************************//
    
    SDL_Window *win = SDL_CreateWindow("Client Screen", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                         SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    screen = SDL_GetWindowSurface(win);
    backgroundImage = IMG_Load("images/background_sm.png"); 
   // SDL_BlitSurface(backgroundImage, NULL, screen, NULL); 

    //************************End of Make a window to receive input*************************//

    // Connection information
    IPaddress ip;

    // Resolve the argument into an IPaddress
    if (SDLNet_ResolveHost(&ip, servername, PORT) == -1)
    {
        cerr << "SDLNet_ResolveHost: " << SDLNet_GetError() << endl;
        return;
    }

    // Open a socket to communicate with the server
    TCPsocket sock = SDLNet_TCP_Open(&ip);
    if(!sock)
    {
        cerr << "SDLNet_TCP_Open: " << SDLNet_GetError() << endl;
        return;
    }

    // Create a "socket set" with the new socket, so that you can check
    // to see if the server has sent anything.

    SDLNet_SocketSet set = SDLNet_AllocSocketSet(1);
    if(!set)
        cerr << "SDLNet_AllocSocketSet: " << SDLNet_GetError() << endl;
    else
    {
        if (SDLNet_TCP_AddSocket(set, sock) == -1)
            cerr << "SDLNet_AddSocket: " << SDLNet_GetError() << endl;
    }

    //Variables for network
    int serverUnicornX = 0;
    int serverUnicornY = 0;
    //Protocal protocalReceive;

    //Create unicorn Object 
    Unicorn unicorn;
    unicorn.rect.x = 30;

    //Create server unicorn 
    Unicorn serverUnicorn;
    serverUnicorn.xSpeed = 0;



   //Generating random
    srand(time(NULL));    
    //All about score font
    font = TTF_OpenFont("fonts/OpenSans-Bold.ttf", 24);
    
    //Position for the score and score initialization
    scoreBoardRect.x = 0; 
    scoreBoardRect.y = 0;
    scoreBoardRect.h = 100;
    scoreBoardRect.w = 100;
    int score = 0; 


    // Main client loop
    bool quit = false;
    while (!quit)
    {

         //Calculating delta
        int totalTime, delta, oldTotalTime;
        totalTime = SDL_GetTicks();
        delta = totalTime - oldTotalTime;
        oldTotalTime = totalTime;

        SDL_Event event;
       // bool send_something = false;
        //int message;

        while (SDL_PollEvent(&event))
        {
           // send_something = false;
            
            if(event.type == SDL_QUIT)
            {
                quit = true;
            }
         }   

            if(key[SDL_SCANCODE_LEFT])
            {
                //send_something = true;
               // message = UNICORN_POS;
                unicorn.xSpeed = -1;
                unicorn.rect.x += unicorn.xSpeed * delta / 5;
            }
             if(key[SDL_SCANCODE_RIGHT])
            {
                //send_something = true;
                //message = UNICORN_POS;
                unicorn.xSpeed = 1;
                unicorn.rect.x += 10;
            }

             if(key[SDL_SCANCODE_UP])
            {

                //send_something = true;
                //message = UNICORN_POS;
                unicorn.ySpeed = -1;
                unicorn.rect.y += unicorn.ySpeed * delta /5;  
            }

             if(key[SDL_SCANCODE_DOWN])
            {   
                //send_something = true;
                //message = UNICORN_POS;
                unicorn.ySpeed = 1;
                unicorn.rect.y += unicorn.ySpeed * delta /5;
    
            }
            serverUnicorn.rect.x = serverUnicornX;
            serverUnicorn.rect.y = serverUnicornY;

            if (serverUnicorn.rect.x != 0 && serverUnicorn.rect.x != 305)
            {
                serverUnicorn.xSpeed = 1;
            }
    

        //draw labirint
        SDL_BlitSurface(backgroundImage, NULL, screen, NULL); 

        //draw server unicorn
        if(serverUnicorn.rect.x != 0 || serverUnicorn.rect.y != 0)
        {
        SDL_BlitSurface(serverUnicorn.surface, NULL, screen, &serverUnicorn.rect);
        }
        //draw client unicorn 
        SDL_BlitSurface(unicorn.surface, NULL, screen, &unicorn.rect);

        // //update the screen
        // SDL_UpdateWindowSurface(win);

         //send coordinates for the unicorn to the server
        sendUnicornPosition(unicorn.rect.x, unicorn.rect.y, sock);
        
        //Message message;
        //Recieving from server
        while (SDLNet_CheckSockets(set, 0))
        {
            
            int got = SDLNet_TCP_Recv(sock, &message, sizeof(serverUnicornX));
            
            if (got <= 0)
            {
                cerr << "Connection problem, quitting..." << endl;
                return;
            }
            if(message == UNICORN_POS)
            {
                
                SDLNet_TCP_Recv(sock, &serverUnicornX, sizeof(serverUnicornX));
                SDLNet_TCP_Recv(sock, &serverUnicornY, sizeof(serverUnicornY));
            }
            
            //There should be information to recieve labirint here
          
        }

       
         //Update the surface
        SDL_UpdateWindowSurface(win);
         
        // Because our loop doesn't do much, wait a bit before going again,
        // to keep from overwhelming the CPU.
        SDL_Delay(50);
    }

    SDLNet_TCP_Close(sock);
    SDL_DestroyWindow(win); 
    win = NULL;
}


void beTheServer()
{
    //initializing SDL variables
    SDL_Surface *screen = NULL;
    SDL_Window *window = NULL;
    SDL_Surface *backgroundImage = NULL;
    SDL_Surface *gameOverImage = NULL;
    SDL_Surface *firstImage = NULL;
    SDL_Renderer* renderer = NULL;

    SDL_Init(SDL_INIT_VIDEO);

    //************************Make a window to receive input*********************************//
    SDL_Window *win = SDL_CreateWindow("Server Screen", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                         SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    screen = SDL_GetWindowSurface(win);
    backgroundImage = IMG_Load("images/background_sm.png"); 
    //************************End of Make a window to receive input*************************//

    // Connection information
    IPaddress ip;

    if (SDLNet_ResolveHost(&ip, NULL, PORT) == -1)
    {
        cerr << "SDLNet_ResolveHost: " << SDLNet_GetError() << endl;
        return;
    }

    // Open the socket to listen for connections from the client
    TCPsocket listener = SDLNet_TCP_Open(&ip);
    if(!listener)
    {
        cerr << "SDLNet_TCP_Open: " << SDLNet_GetError() << endl;
        return;
    }

    // Prepare a SocketSet so we can check for messages from the client
    SDLNet_SocketSet set = SDLNet_AllocSocketSet(1);

    //************************************Game Initialization***********************************************//
    TTF_Init();

   //Generating random
    srand(time(NULL));    
    //All about score font
    font = TTF_OpenFont("fonts/OpenSans-Bold.ttf", 24);
    
    //Position for the score and score initialization
    scoreBoardRect.x = 0; 
    scoreBoardRect.y = 0;
    scoreBoardRect.h = 100;
    scoreBoardRect.w = 100;
    int score = 0; 

    //Create unicorn object 
    Unicorn unicorn; 
    //Create unicorn object for client screen 
    Unicorn clientUnicorn;

    //Constatant values for labirint cell dimentions
    const int CELL_WIDTH = 32;
    const int CELL_HEIGHT = 24;
    const int GAME_SPEED = 0.6;

    
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
       
       //lame labirint
        blankCellpositionTop = rand()% + 6;
        blankCellpositionBottom = blankCellpositionTop + 10;
    }


    //*************************************End Game Initalization*******************************************//


    //**************************************Game Loop********************************************************//
    

    bool quit = false;

    //Variables for the network connection 
    TCPsocket client = 0;

    //******************Variables for the network*******************************
    int clientUnicornX;
    int clientUnicornY;
    //Message message;
    // int network_maze_index;
    //****************************************************************************
    
    SDL_Event event;
    // Main server loop 
    while (!quit)
    {
         //Calculating delta
        int totalTime, delta, oldTotalTime;
        totalTime = SDL_GetTicks();
        delta = totalTime - oldTotalTime;
        oldTotalTime = totalTime;

        //Server unicorn position
        int serverUnicornX = unicorn.rect.x;
        int serverUnicornY = unicorn.rect.y;

        //Try to send a message to client screen
        // bool send_something = false;
        // int message;

        
        while (SDL_PollEvent(&event))
        {
            //send_something = true;
            //message = UNICORN_POS;
            if(event.type == SDL_QUIT)
            {
                quit = true;
            }
        }

            if(key[SDL_SCANCODE_LEFT])
            {
                unicorn.xSpeed = -1;
                unicorn.rect.x += unicorn.xSpeed * delta /5;
                //send_something = true;
                //message = UNICORN_POS;
            }
             if(key[SDL_SCANCODE_RIGHT])
            {
                unicorn.xSpeed = 1;
                unicorn.rect.x += unicorn.xSpeed * delta /5;
               // send_something = true;
                //message = UNICORN_POS;
            }

             if(key[SDL_SCANCODE_UP])
            {

                unicorn.ySpeed = -1;
                unicorn.rect.y += unicorn.ySpeed * delta /5;
                //send_something = true;
               // message = UNICORN_POS;  
            }

             if(key[SDL_SCANCODE_DOWN])
            {   
                unicorn.ySpeed = 1;
                unicorn.rect.y += unicorn.ySpeed * delta /5;
               // send_something = true;
                //message = UNICORN_POS;
    
            }

       
        //Move background: based on discussion with David
        if(unicorn.xSpeed != 0)
        {
            for (int i = 0; i < CELL_WIDTH; i++)
            {
                for (int j = 0; j < CELL_HEIGHT; j++)
                {
                    labirint[i][j].rect.x -= delta / 5;
                }
             }
        }

        //Updating position of the unicorn client based on recieving through networking
        clientUnicorn.rect.x = clientUnicornX;
        clientUnicorn.rect.y = clientUnicornY;
       
       //Move backgroun
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


        //draw labirint
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

        //Check for the colision

        //Do not draw the unicorn at position (0, 0)
        if(clientUnicorn.rect.x != 0 || clientUnicorn.rect.y != 0)
        {
        //Apply client unicorn
        SDL_BlitSurface(clientUnicorn.surface, NULL, screen, &clientUnicorn.rect);
        }
        //Apply server unicorn
        SDL_BlitSurface(unicorn.surface, NULL, screen, &unicorn.rect);

        //Display score
        std::string score_displayed = "Score: " + std::to_string(score);
        scoreBoard = TTF_RenderText_Solid(font, score_displayed.c_str(), textColor);
        
        SDL_BlitSurface(scoreBoard, NULL, screen, &scoreBoardRect);

        // //Update the surface
        //  SDL_UpdateWindowSurface(win);
          
        //****************************End Game loop*************************************************//


        // If the client socket is still NULL, no-one has connected yet.
        // Check to see if someone wants to connect.

        if (client == 0)
        {
            client = SDLNet_TCP_Accept(listener);

            // If it isn't zero any more, the client socket is now connected.
            // Add it to the SocketSet so that we can check it for data later.

            if (client != 0)
                if (SDLNet_TCP_AddSocket(set, client) == -1)
                    cerr << "SDLNet_AddSocket: " << SDLNet_GetError() << endl;
        }

        // If we're connected to a client, we may have data to send, and we
        // should check to see if they've sent any data to us.

        if (client != 0)
        {
            //By this you should send the labirint
            // if (send_something)
            // {
            //     int sent = SDLNet_TCP_Send(client, &message, sizeof(message));
            //     if (sent != sizeof(message))
            //         cerr << "SDLNet_TCP_Send: " << SDLNet_GetError() << endl;
            // }

            //Send the position of the client unicorn
            sendUnicornPosition(unicorn.rect.x, unicorn.rect.y, client);
            
            while (SDLNet_CheckSockets(set, 0))
            {
                int got = SDLNet_TCP_Recv(client, &message, sizeof(message));
                if (got <= 0)
                {
                    cerr << "Connection problem, quitting..." << endl;
                    return;
                }


                if(message == UNICORN_POS)
                {
                    SDLNet_TCP_Recv(client, &clientUnicornX, sizeof(clientUnicornX));
                    SDLNet_TCP_Recv(client, &clientUnicornY, sizeof(clientUnicornY));
                }
               
            }

            //message = DONT_SEND_MAZE;
        }
         
         //Update the surface
         SDL_UpdateWindowSurface(win);
          
        // Because our loop doesn't do much, wait a bit before going again,
        // to keep from overwhelming the CPU.
        SDL_Delay(50);
    }

    SDLNet_TCP_Close(listener);
    SDLNet_TCP_Close(client);
    SDL_DestroyWindow(win);
    win = NULL;
}
