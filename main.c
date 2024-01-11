#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <math.h>

#define PI 3.14159265
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

typedef struct{
  int alive;
  double time;
  double angle;
}Objects;

Objects objects[15];
TTF_Font *font = NULL;
SDL_Color font_color = {255,255,255};
int current_angle=0;

void calculateProjectilePosition(double initialVelocity, double launchAngle, double gravity, double time, double* x, double* y) {
    double angleInRadians = launchAngle * (PI / 180.0);
    *x = initialVelocity * cos(angleInRadians) * time;
    *y = initialVelocity * sin(angleInRadians) * time  - (gravity/2) * pow(time, 2);
}

void add_object(){
  for(int i=0;i<15;i++){
    if (!objects[i].alive){
      objects[i].time=0;
      objects[i].alive=1;
      objects[i].angle = (double)current_angle;
     // SDL_Log("angle : %f\nMouse X : %d\nMouse Y : %d" , objects[i].angle-360.0 , mouseX , mouseY);
      break;
    }
  }
}

int main() {
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        SDL_Log("SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

    window = SDL_CreateWindow("Projectile Motion", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        SDL_Log("SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        SDL_Log(" SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

    TTF_Init();
    font = TTF_OpenFont("arial.ttf" , 25);

    double initialVelocity = 80.0;
    double launchAngle = 45.0;
    double gravity = 9.8;
    double timeStep = 0.1;

    SDL_Event e;
    int quit = 0;
    double time = 0;
    SDL_Texture *font_texture=NULL;
    
    SDL_Surface *side_surface = IMG_Load("side.png");
    SDL_Texture *side_texture = SDL_CreateTextureFromSurface(renderer , side_surface);
    SDL_FreeSurface(side_surface);

    SDL_Rect font_rect;
    font_rect.x = 0;
    font_rect.y = 0;
    font_rect.w = 800;
    font_rect.h = 64;
    
    SDL_Rect side_rect;
    side_rect.x = 0;
    side_rect.y = SCREEN_HEIGHT-200;
    side_rect.w = 8;
    side_rect.h = 400;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            }
            switch(e.type){
              case SDL_KEYDOWN: 
                switch(e.key.keysym.scancode){
                  case SDL_SCANCODE_SPACE: add_object();break;
                  default: break;
                };break;
              default: break;
            }
                                      
        }
        int mouseX , mouseY;
        SDL_GetMouseState(&mouseX , &mouseY);
        current_angle = 360-(atan2(mouseY - SCREEN_HEIGHT , mouseX) * 58);
        current_angle -= 360;
        char buffer[1024];
        snprintf(buffer , sizeof(buffer)-1 , "Sudut (titik poin = koordinat mouse) : %d" , current_angle);
        SDL_Surface *surface = TTF_RenderText_Solid(font , buffer , font_color);
        SDL_DestroyTexture(font_texture);
        font_texture = SDL_CreateTextureFromSurface(renderer , surface);
        SDL_FreeSurface(surface);

        for(int i=0;i<15;i++){
          double x, y;
          if (objects[i].alive){
            objects[i].time += timeStep;
            calculateProjectilePosition(initialVelocity, objects[i].angle, gravity, objects[i].time, &x, &y);
            SDL_Rect rect;
            rect.x = (int)x;
            rect.y = SCREEN_HEIGHT-(int)y;
            rect.w = 32;
            rect.h = 32;
          //  SDL_Log("x : %d" , (int)rect.x);
           // SDL_Log("y : %d\ntime : %f\n" , (int)rect.y , objects[i].time);
            int r,g,b;
            r = rand()%255;
            g = rand()%255;
            b = rand()%255;
            SDL_SetRenderDrawColor(renderer, r, g, b, 0);
            SDL_RenderDrawRect(renderer , &rect);
            if (rect.y > SCREEN_HEIGHT){
              objects[i].alive=0;
            }
          }
          SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        }
        SDL_RenderCopyEx(renderer , side_texture , NULL , &side_rect , 90.0-(double)current_angle , NULL , SDL_FLIP_VERTICAL);
     //   SDL_RenderCopy(renderer , side_texture , NULL  ,&side_rect);
        SDL_RenderCopy(renderer , font_texture , NULL , &font_rect);
        SDL_RenderPresent(renderer);
        SDL_RenderClear(renderer);
        SDL_Delay(1000/60);
    }

    // Cleanup
    SDL_DestroyTexture(side_texture);
    SDL_DestroyTexture(font_texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();

    return 0;
}

