
#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <string>



const int winWidth = 480;
const int winHeight = 640;

SDL_Window* gameWindow = nullptr;
SDL_Surface* winSurface = nullptr;
SDL_Surface* bgSurface = nullptr;
SDL_Surface* redSurface = nullptr;
SDL_Surface* blackSurface = nullptr;
std::string p1Name = "Player 1";
std::string p2Name = "Player 2";

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

bool close() {
  SDL_DestroySurface(bgSurface);
  bgSurface = nullptr;
  SDL_DestroyWindow(gameWindow);
  gameWindow = nullptr;
  winSurface = nullptr;
  return true;
}

int main() {
  init();
  loadImage("bg.bmp", &bgSurface);
  bool quit = false;
  SDL_Event e;
  while (quit == false) {
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_EVENT_QUIT) {
        quit = true;
      }
    }

    SDL_FillSurfaceRect(winSurface, nullptr, SDL_MapSurfaceRGB(winSurface, rand() % 256, rand() % 256, rand() % 256));
//    SDL_BlitSurface(bgSurface, nullptr, winSurface, nullptr);
    SDL_UpdateWindowSurface(gameWindow);
  }
  close();
  return 0;
  
}


