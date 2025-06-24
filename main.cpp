
#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
//#include <SDL3/SDL_mixer.h>
#include <string>
#include <cmath> 
#include <time.h>
#define TILEOFFSET 72+50
#define TILEXOFFSET 13
#define TILEWIDTH 40
#define TILEINTEROFFSET 12
#define RECTXDISTANCE 140
#define TEXTWIDTH 84




/*
 Dices function :
 Always draw current dice pos
 If spinning increase dice pos and render number based on floored value
 Increase for a random amount of time
*/

/* 
 * Cases that can happen:
 * If dice = 1,2,3 select a piece and move it forward that many places
 * Can't happen if another same colour piece is already at that space unless it is 1 or 11 or 5
 * If another player's piece is on the square you moved to, send that piece back to start Unless this happens on tile index 5 or tile index 11 or 1 (obviously)
 * If dice = 4 select a piece to move back 1 space
 * Again same as before. Cant happen if piec already there, unless 0 or 10 or 4
 * And same about previous piece.
 */



struct Player {
  std::string name;
  int y1;
  int y2;
  
  SDL_Surface* colour;

  
};

enum InGameState {
  DICE,
  P1TURN,
  P2TURN,
  MOVING
};



enum OverallState {
  MENU,
  PNAME,
  GAME
};

//Variables to initialise SDL
const int winWidth = 480;
const int winHeight = 640;
SDL_Window* gameWindow = nullptr;
SDL_Surface* winSurface = nullptr;
std::string p1Name = "Player 1";
std::string p2Name = "Player 2";
float mX, mY;

//Variables for menu
int menuSelect = 0;
SDL_Rect menuButton1 = {90,200,300,100};
SDL_Rect menuButton2 = {90, 320, 300, 100};
SDL_Rect menuButton3 = {90,440,300,100};
SDL_Surface* buttonUSurface = nullptr;
SDL_Surface* buttonSSurface = nullptr;

//Variables for Main Game
SDL_Surface* bgSurface = nullptr;
SDL_Surface* redSurface = nullptr;
SDL_Surface* blackSurface = nullptr;
const int p1X1 = RECTXDISTANCE + TILEXOFFSET;
const int p1X2 = p1X1 + TILEWIDTH + TILEINTEROFFSET;
const int p2X1 = p1X2 + TILEWIDTH+  TILEXOFFSET + TEXTWIDTH + TILEXOFFSET;
const int p2X2 = p2X1 + TILEWIDTH + TILEINTEROFFSET;
struct Player p1 = {"Player 1", 1 , 1 , redSurface};
struct Player p2 = {"Player 2", 1 , 1, blackSurface};
SDL_Rect p1piece1 = {p1X1, 640-(TILEOFFSET*p1.y1)+6, 40,40};
SDL_Rect p1piece2 =  {p1X2, 640-(TILEOFFSET*p1.y2)+6, 40,40};
SDL_Rect p2piece1 =  {p2X1, 640-(TILEOFFSET*p2.y1)+6, 40,40};
SDL_Rect p2piece2 =  {p2X2, 640-(TILEOFFSET*p2.y2)+6, 40,40};

SDL_Rect diceRect = {20,130,100,100};
SDL_Surface* d1Surface = nullptr;
SDL_Surface* d2Surface = nullptr;
SDL_Surface* d3Surface = nullptr;
SDL_Surface* d4Surface = nullptr;
float dPos = 0;
//Mix_Music* bgMusic = nullptr;
//SDL_AudioDeviceID gameAudioId = 0;


enum InGameState gameState = DICE;
enum OverallState state = MENU;

bool init() {
  bool success = true;
  if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
    SDL_Log("Cant init SDL");
    success = false;
    std::cout << "L SDL" << std::endl ;

  }else {
    gameWindow = SDL_CreateWindow("AQAdo", winWidth, winHeight, 0);
    if (gameWindow == nullptr) {
      SDL_Log("Cant make window");
      success = false;
      std::cout << "L window"<< std::endl;
    } else {
      winSurface = SDL_GetWindowSurface(gameWindow);
      //Set up Audio
      /*SDL_AudioSpec auSpec;
      SDL_zero(auSpec);
      auSpec.format = SDL_AUDIO_F32;
      auSpec.channels = 2;
      auSpec.freq = 44100;
      gameAudioId = SDL_OpenAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &auSpec);
      if (gameAudioId == 0) {
        std::cout << "Audio no worky :(";
        success = false;
      }else {
        if (Mix_OpenAudio(gameAudioId, &auSpec) == false) {
          std::cout << "L Sdl_mixer";
          std::cout << SDL_GetError();
          success = false;
        }
      }*/
      
    }
  }


  return success;

}


bool loadImage(std::string imgPath, SDL_Surface** surface) {
  if (*surface != nullptr) {
    SDL_DestroySurface(*surface);
    *surface = nullptr;
  }
  std::string imagePath = imgPath;
  bool success = true;
  *surface = SDL_LoadBMP(imagePath.c_str());
  if (*surface == nullptr) {
    SDL_Log("Can't load image");
    success = false;
    std::cout << "Cant load img with path:" << imagePath << std::endl;

  }
  
  return success;
}

/*bool loadMusic(std::string filename, Mix_Music** song) {
  bool success = true;
  *song = Mix_LoadMUS(filename.c_str());
  if(*song == nullptr) {
    std::cout << "Cant load song with file name: " << filename;
    std::cout << SDL_GetError();
    success = false;
  }
  return success;
}*/


bool destroyImage(SDL_Surface** surface) {
  SDL_DestroySurface(*surface);
  surface = nullptr;
  return true;
}

/*bool destroyMusic(Mix_Music** song) {
  Mix_FreeMusic(*song);
  *song = nullptr;
  return true;
}*/


bool close() {
  //destroyMusic(&bgMusic);


  destroyImage(&winSurface);
  destroyImage(&bgSurface);
  destroyImage(&redSurface);
  destroyImage(&blackSurface);
  destroyImage(&buttonUSurface);
  destroyImage(&buttonSSurface);
  destroyImage(&d1Surface);
  destroyImage(&d2Surface);
  destroyImage(&d3Surface);
  destroyImage(&d4Surface);
  SDL_DestroyWindow(gameWindow);
  //Mix_CloseAudio();
  //`SDL_CloseAudioDevice( gameAudioId );
  //gameAudioId = 0;
  gameWindow = nullptr;
  winSurface = nullptr;
  return true;
}

bool isColliding(SDL_Rect checkRect, int x, int y) {
  if ((checkRect.x <= x && checkRect.x+checkRect.w >= x) && (checkRect.y <= y && checkRect.y+checkRect.h >= y)) {
    return true;
  } else {
    return false;
  }
}

bool canMove(int dice, int pos1, int pos2, int pieceMoving) {
  int tempPos = pieceMoving == 0 ? pos1 : pos2;
  int otherPiecePos = pieceMoving == 0 ? pos2 : pos1;
  int actualDice = dice == 3 ? -1 : dice + 1;
  
  if (tempPos + actualDice == otherPiecePos && otherPiecePos != 1 && otherPiecePos != 5 && otherPiecePos != 11 ) {
    return false;
  } else if (actualDice == -1 and tempPos == 1) {
    return false;
  } else {
    return true;
  }
}


int main() {
  init();
  loadImage("menu.bmp", &bgSurface);
  loadImage("button_u.bmp", &buttonUSurface);
  loadImage("button_s.bmp", &buttonSSurface);
  loadImage("redChar.bmp", &redSurface);
  loadImage("blackChar.bmp", &blackSurface);
  loadImage("d1.bmp",&d1Surface);
  loadImage("d2.bmp",&d2Surface);
  loadImage("d3.bmp",&d3Surface);
  loadImage("d4.bmp",&d4Surface);
  SDL_Surface* Die[4] = {d1Surface,d2Surface,d3Surface,d4Surface};
  p1.colour = redSurface;
  p2.colour = blackSurface; 
  bool quit = false;
  bool isSpinning = false; 
  int numSpins = (rand() % 201) + 100  ;
  std::cout << numSpins;
  int countedSpins = 0;
  bool alreadySpun = false;
  int turn = 0;
  // seed the randomness
  srand(time(NULL));
  //loadMusic("bgMusic.wav", &bgMusic);
  
  //Mix_PlayMusic(bgMusic, -1);
  //SDL_GetError();
  //
  //
  // Movement of pieces Variables
  int startPos = NULL;
  int endPos = NULL;
  int pieceMoving = NULL;


  SDL_Event e;
  //std::cout << surfaces[0];
  while (quit == false) {
    Uint64 start = SDL_GetPerformanceCounter();

    SDL_FillSurfaceRect(winSurface, nullptr, SDL_MapSurfaceRGB(winSurface, 255, 255, 255));
    SDL_BlitSurface(bgSurface, nullptr, winSurface, nullptr);
     
    switch (state)
    {
    case MENU:
      SDL_GetMouseState(&mX,&mY);
       
      if (isColliding(menuButton1, mX,mY)) {
        menuSelect = 1;
      }else if (isColliding(menuButton2,mX,mY)) {
        menuSelect = 2;
      }else if (isColliding(menuButton3,mX,mY)) {
        menuSelect = 3;
      }else {
        menuSelect = 0;
      }
      SDL_BlitSurface(menuSelect == 1 ? buttonSSurface : buttonUSurface, nullptr, winSurface, &menuButton1);
      SDL_BlitSurface(menuSelect == 2 ? buttonSSurface : buttonUSurface, nullptr, winSurface, &menuButton2);
      SDL_BlitSurface(menuSelect == 3 ? buttonSSurface : buttonUSurface, nullptr, winSurface, &menuButton3);


      while (SDL_PollEvent(&e)) {
        if (e.type == SDL_EVENT_QUIT) {
          quit = true;
        }
        else if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
          if (e.button.button == SDL_BUTTON_LEFT) {
            switch (menuSelect) {
              case 1:
                state = GAME;
                loadImage("bg.bmp", &bgSurface);
                break;
              case 2:
                //ChooseNames
                break;
              case 3:
                quit=true;
                break;
            }
          }
        }
      }
      break;
    case PNAME:
      break;
    case GAME:

      SDL_BlitSurface(bgSurface, nullptr, winSurface, nullptr);
      SDL_BlitSurface(p1.colour, nullptr, winSurface, &p1piece1 );
      SDL_BlitSurface(p1.colour, nullptr, winSurface, &p1piece2 );
      SDL_BlitSurface(p2.colour, nullptr, winSurface, &p2piece1 );
      SDL_BlitSurface(p2.colour, nullptr, winSurface, &p2piece2 );
      SDL_BlitSurface(Die[(int)std::floor(dPos)],nullptr,winSurface,&diceRect);
      switch (gameState) {
        case DICE:
          SDL_GetMouseState(&mX,&mY);
          
          
          while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) {
              quit = true;
            }
            else if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
              if (e.button.button == SDL_BUTTON_LEFT && isColliding(diceRect,mX,mY) && !isSpinning && !alreadySpun) { 
                isSpinning = true;
                alreadySpun = true;

              }
            }
          }

          
          if (isSpinning) {
            dPos = dPos >= 3.99 ? 0 : dPos + 0.10;
            countedSpins += 1;
            
          }

          if (countedSpins == numSpins ) {
            isSpinning = false;
            numSpins = (rand() % 201) + 100  ;
            countedSpins = 0;
            gameState = turn % 2 == true ? P1TURN : P2TURN;
          }

          break;
        case P1TURN:
          SDL_GetMouseState(&mX, &mY);
          while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) {
              quit = true;
            }
            else if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
              if (e.button.button == SDL_BUTTON_LEFT) {
                if(isColliding(p1piece1,mX,mY)) {
                  bool move = canMove((int)std::floor(dPos), p1.y1,p1.y2, 0  );    
                  if (move) {
                    gameState = MOVING;
                    pieceMoving = 0;
                    endPos = p1.y1 + ((int)std::floor(dPos) ? (int)std::floor(dPos) : -1);

                  }
                } else if(isColliding(p1piece2,mX,mY)) {
                  bool move = canMove((int)std::floor(dPos), p1.y2,p1.y1, 0  );    
                  if (move) {
                    gameState = MOVING;
                    pieceMoving = 1;
                    endPos = p1.y2 + ((int)std::floor(dPos) ? (int)std::floor(dPos) : -1);
                  }
                }
              }
            }
          }
 
          break;
        case P2TURN:
          SDL_GetMouseState(&mX, &mY);
          while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) {
              quit = true;
            }
            else if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
              if (e.button.button == SDL_BUTTON_LEFT) {
                if(isColliding(p2piece1,mX,mY)) {
                  bool move = canMove((int)std::floor(dPos), p2.y1,p2.y2, 0  );    
                  if (move) {
                    gameState = MOVING;
                    pieceMoving = 0;
                    endPos = p2.y1 + ((int)std::floor(dPos) ? (int)std::floor(dPos) : -1);
                  }
                } else if(isColliding(p2piece2,mX,mY)) {
                  bool move = canMove((int)std::floor(dPos), p2.y2,p2.y1, 0  );    
                  if (move) {
                    gameState = MOVING;
                    pieceMoving = 0;
                    endPos = p2.y2 + ((int)std::floor(dPos) ? (int)std::floor(dPos) : -1);
                  }
                }
              }
            }
          }

          break;
        case MOVING:
          while(SDL_PollEvent(&e)) {
            if(e.type == SDL_EVENT_QUIT) {
              quit = true;
            }
          }



      } 


      


       break;
    
    default:
      break;
    }
    

    SDL_UpdateWindowSurface(gameWindow);
    Uint64 end = SDL_GetPerformanceCounter();

	  float elapsedMS = (end - start) / (float)SDL_GetPerformanceFrequency() * 1000.0f;

	  // Cap to 60 FPS
	  SDL_Delay(std::floor(16.666f - elapsedMS));


  }
  close();
  return 0;
  
}


