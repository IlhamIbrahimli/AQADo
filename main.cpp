
#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <string>

#define TILEOFFSET 72+50
#define TILEXOFFSET 13
#define TILEWIDTH 40
#define TILEINTEROFFSET 12
#define RECTXDISTANCE 139
#define TEXTWIDTH 84

struct Player {
  std::string name;
  int y1;
  int y2;
  
  SDL_Surface* colour;

  
};

enum State {
  DICE,
  P1TURN,
  P2TURN,
  MOVING
};

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

const int winWidth = 480;
const int winHeight = 640;
const int p1X1 = RECTXDISTANCE + TILEXOFFSET;
const int p1X2 = p1X1 + TILEWIDTH + TILEINTEROFFSET;
const int p2X1 = p1X2 + TILEWIDTH+  TILEXOFFSET + TEXTWIDTH + TILEXOFFSET;
const int p2X2 = p2X1 + TILEWIDTH + TILEINTEROFFSET;
SDL_Window* gameWindow = nullptr;
SDL_Surface* winSurface = nullptr;
SDL_Surface* bgSurface = nullptr;
SDL_Surface* redSurface = nullptr;
SDL_Surface* blackSurface = nullptr;
struct Player p1 = {"Player 1", 1 , 1 , redSurface};
struct Player p2 = {"Player 2", 1 , 1, blackSurface};
SDL_Rect p1piece1 = {p1X1, 640-(TILEOFFSET*p1.y1)+6, 40,40};
SDL_Rect p1piece2 =  {p1X2, 640-(TILEOFFSET*p1.y2)+6, 40,40};
SDL_Rect p2piece1 =  {p2X1, 640-(TILEOFFSET*p2.y1)+6, 40,40};
SDL_Rect p2piece2 =  {p2X2, 640-(TILEOFFSET*p2.y2)+6, 40,40};
float dicePos = 0;



bool init() {
  bool success = true;
  if (!SDL_Init(SDL_INIT_VIDEO)) {
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
    }
    SDL_SetRenderVSync( winSurface, 1 );
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


bool destroyImage(SDL_Surface** surface) {
  SDL_DestroySurface(*surface);
  surface = nullptr;
  return true;
}




bool close() {
  destroyImage(&winSurface);
  destroyImage(&bgSurface);
  destroyImage(&redSurface);
  destroyImage(&blackSurface);
  SDL_DestroyWindow(gameWindow);

  gameWindow = nullptr;
  winSurface = nullptr;
  return true;
}

int main() {
  init();
  loadImage("bg.bmp", &bgSurface);
  loadImage("redChar.bmp", &redSurface);
  loadImage("blackChar.bmp", &blackSurface);

  p1.colour = redSurface;
  p2.colour = blackSurface;
  bool quit = false;
  bool skib = false;
  SDL_Event e;
  //std::cout << surfaces[0];
  while (quit == false) {
    SDL_FillSurfaceRect(winSurface, nullptr, SDL_MapSurfaceRGB(winSurface, 255, 255, 255));
    SDL_BlitSurface(bgSurface, nullptr, winSurface, nullptr);
    SDL_BlitSurface(p1.colour, nullptr, winSurface, &p1piece1 );
    SDL_BlitSurface(p1.colour, nullptr, winSurface, &p1piece2 );
    SDL_BlitSurface(p2.colour, nullptr, winSurface, &p2piece1 );
    SDL_BlitSurface(p2.colour, nullptr, winSurface, &p2piece2 );

    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_EVENT_QUIT) {
        quit = true;
      }
      else if (e.type == SDL_EVENT_KEY_DOWN) {
        p1.y1 = p1.y1 < 11 ? p1.y1 + 1 : p1.y1;
        p1piece1.y = 640-(TILEOFFSET*p1.y1)+6;
      }
    }

    SDL_UpdateWindowSurface(gameWindow);
  }
  close();
  return 0;
  
}


