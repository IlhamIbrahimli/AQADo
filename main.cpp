
#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <string>



const int winWidth = 480;
const int winHeight = 640;
SDL_Surface* surfaces[20];
SDL_Window* gameWindow = nullptr;
SDL_Surface* winSurface = nullptr;
SDL_Surface* bgSurface = nullptr;
SDL_Surface* redSurface = nullptr;
SDL_Surface* blackSurface = nullptr;
SDL_Surface* buttonUSurface = nullptr;
SDL_Surface* buttonSSurface = nullptr;
std::string p1Name = "Player 1";
std::string p2Name = "Player 2";
int mX, mY;
int menuSelect = 0;
SDL_Rect menuButton1 = {90,200,0,0};
SDL_Rect menuButton2 = {90, 320, 0, 0};
SDL_Rect menuButton3 = {90,440,0,0};





enum OverallState {
  MENU,
  PNAME,
  GAME
};


enum OverallState state = MENU;

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
  for (int j = 0; j < 20; j++) {
    if (surfaces[j] == nullptr) {
      surfaces[j] = *surface;
    }
  }
  return success;
}


bool destroyImage(SDL_Surface** surface) {
  for (int i = 0; i <20; i++) {
    if (*surface == surfaces[i] ) {
      surfaces[i] == nullptr;
    }
  }

  SDL_DestroySurface(*surface);
  surface = nullptr;
  return true;
}




bool close() {
  for (int i = 0; i < 20; i++) {
    if (surfaces[i] != nullptr) {
      destroyImage(&surfaces[i]);
    }
  }
  std::cout << surfaces;
  SDL_DestroyWindow(gameWindow);
  gameWindow = nullptr;
  winSurface = nullptr;
  return true;
}

bool isColliding(SDL_Rect checkRect, int x, int y) {
  if ((checkRect.x <= x && checkRect.x+w >= x) && (checkRect.y <= y && checkRect.y+h >= y)) {
    return true;
  } else {
    return false;
  }
}


int main() {
  init();
  loadImage("menu.bmp", &bgSurface);
  loadImage("button_u.bmp", &buttonUSurface);
  loadImage("button_s.bmp", &buttonSSurface);

  bool quit = false;
  
  
  
  SDL_Event e;
  //std::cout << surfaces[0];
  while (quit == false) {
    SDL_FillSurfaceRect(winSurface, nullptr, SDL_MapSurfaceRGB(winSurface, 255, 255, 255));
    SDL_BlitSurface(bgSurface, nullptr, winSurface, nullptr);

    switch (state)
    {
    case MENU:
      SDL_GetMouseState(&mX,&mY);
      if (isColliding(menuButton1, mX,mY)) {

      }else if (isColliding(menuButton2,mX,mY)) {
        
      }else if (isColliding(menuButton3,mX,mY)) {
        
      }
      while (SDL_PollEvent(&e)) {
        if (e.type == SDL_EVENT_QUIT) {
          quit = true;
        }
        else if (e.type == SDL_EVENT_KEY_DOWN) {
          
        }
      }
      break;
    case PNAME:
      break;
    case GAME:

      while (SDL_PollEvent(&e)) {
        if (e.type == SDL_EVENT_QUIT) {
          quit = true;
        }
        else if (e.type == SDL_EVENT_KEY_DOWN) {
          
        }
      }
      break;
    
    default:
      break;
    }
    

    SDL_UpdateWindowSurface(gameWindow);
  }
  close();
  return 0;
  
}


