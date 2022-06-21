#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

struct Cell{
    bool state;
    bool toFlip;
    SDL_Surface *image;
    SDL_Rect rect;
};

int main(int argv, char* args[]){
    //Init
    if(SDL_Init(SDL_INIT_EVERYTHING)>0){
        std::cout<<"SDL Error: "<<std::endl<<SDL_GetError()<<std::endl;
    }

    SDL_Window *window=SDL_CreateWindow("Physics simulation",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,640,750,SDL_WINDOW_OPENGL);
    if(window==NULL){
        std::cout<<"SDL Error: "<<std::endl<<SDL_GetError()<<std::endl;
    }
    SDL_Surface *screen=SDL_GetWindowSurface(window);

    //Pre-loop stuff
    Cell grid[64][64];
    for(int i=0;i<64;i++){
        for(int j=0;j<64;j++){
            grid[i][j].state=false;
            grid[i][j].image=SDL_CreateRGBSurface(0,10,10,32,0,0,0,0);
            grid[i][j].rect=grid[i][j].image->clip_rect;
            SDL_FillRect(grid[i][j].image,NULL,SDL_MapRGB(screen->format,255,255,255));
            grid[i][j].rect.x=i*10;
            grid[i][j].rect.y=j*10;
        }
    }

    SDL_Surface *uiImage=IMG_Load("ui.png");
    SDL_Rect uiRect=uiImage->clip_rect;
    uiRect.x=0;
    uiRect.y=640;

    SDL_Surface *uiPauseImage=SDL_CreateRGBSurface(0,9,17,32,0,0,0,0);
    SDL_Rect uiPauseRect=uiPauseImage->clip_rect;
    SDL_FillRect(uiPauseImage,NULL,SDL_MapRGB(screen->format,255,255,255));
    uiPauseRect.x=510;

    bool isLeftMouseButtonDown=false;
    bool isRightMouseButtonDown=false;
    int mouseX, mouseY;

    int howManyAround;
    int paused=true;

    Uint32 frameStartTick;
    SDL_Event event;
    bool running=true;

    //Main Loop
    while(running){
        frameStartTick=SDL_GetTicks();

        //Event loop
        while(SDL_PollEvent(&event)){
            if(event.type==SDL_QUIT){
                running=false;
                break;
            }
            else if(event.type==SDL_MOUSEBUTTONDOWN){
                if(event.button.button==SDL_BUTTON_LEFT)
                isLeftMouseButtonDown=true;
                else if(event.button.button==SDL_BUTTON_RIGHT)
                isRightMouseButtonDown=true;
            }
            else if(event.type==SDL_MOUSEBUTTONUP){
                if(event.button.button==SDL_BUTTON_LEFT)
                isLeftMouseButtonDown=false;
                else if(event.button.button==SDL_BUTTON_RIGHT)
                isRightMouseButtonDown=false;
            }
            else if(event.type==SDL_KEYDOWN){
                if(event.key.keysym.sym==SDLK_r){
                    for(int i=0;i<64;i++){
                        for(int j=0;j<64;j++){
                            grid[i][j].state=false;
                        }
                    }
                }
                else if(event.key.keysym.sym==SDLK_q||event.key.keysym.sym==SDLK_ESCAPE){
                    running=false;
                    break;
                }
                else if(event.key.keysym.sym==SDLK_SPACE){
                    if(paused){
                        paused=false;
                    }else{
                        paused=true;
                    }
                }
            }
        }

        //Mouse events
        SDL_PumpEvents();
        SDL_GetMouseState(&mouseX,&mouseY);
        if(isLeftMouseButtonDown&&mouseY<640&&mouseY>0&&mouseX<640&&mouseX>0){
            grid[mouseX/10][mouseY/10].state=true;
        }else if(isRightMouseButtonDown&&mouseY<640&&mouseY>0&&mouseX<640&&mouseX>0){
            grid[mouseX/10][mouseY/10].state=false;
        }
    
        //Every-frame stuff
        SDL_FillRect(screen,NULL,SDL_MapRGB(screen->format, 0,0,0));

        //Grid changes
        for(int i=0;i<64;i++){
            for(int j=0;j<64;j++){
                grid[i][j].toFlip=false;
            }
        }

        if(paused!=true){
            for(int i=0;i<64;i++){
                for(int j=0;j<64;j++){
                    howManyAround=0;
                    if(grid[i+1][j].state&&i!=63) howManyAround++;
                    if(grid[i-1][j].state&&i!=0) howManyAround++;
                    if(grid[i][j+1].state&&j!=63) howManyAround++;
                    if(grid[i][j-1].state&&j!=0) howManyAround++;
                    if(grid[i+1][j+1].state&&i!=63&&j!=63) howManyAround++;
                    if(grid[i-1][j-1].state&&i!=0&&j!=0) howManyAround++;
                    if(grid[i-1][j+1].state&&i!=0&&j!=63) howManyAround++;
                    if(grid[i+1][j-1].state&&i!=63&&j!=0) howManyAround++;

                    if(howManyAround==3&&grid[i][j].state==false){
                        grid[i][j].toFlip=true;
                    }else if(howManyAround!=2&&howManyAround!=3&&grid[i][j].state){
                        grid[i][j].toFlip=true;
                    }
                }
            }
        }

        for(int i=0;i<64;i++){
            for(int j=0;j<64;j++){
                if(grid[i][j].toFlip){
                    if(grid[i][j].state){
                        grid[i][j].state=false;
                    }else{
                        grid[i][j].state=true;
                    }
                }
            }
        }

        //Cells rendering
        for(int i=0;i<64;i++){
            for(int j=0;j<64;j++){
                if(grid[i][j].state){
                    SDL_BlitSurface(grid[i][j].image,NULL,screen,&grid[i][j].rect);
                }
            }
        }

        //UI
        SDL_BlitSurface(uiImage,NULL,screen,&uiRect);
        if(paused){
            uiPauseRect.y=651;
        }else{
            uiPauseRect.y=672;
        }
        SDL_BlitSurface(uiPauseImage,NULL,screen,&uiPauseRect);

        if((1000/16)>SDL_GetTicks()-frameStartTick) SDL_Delay(1000/16-(SDL_GetTicks()-frameStartTick));
        SDL_UpdateWindowSurface(window);
    }

    SDL_FreeSurface(screen);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
