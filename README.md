## About the project
It's simple Conway's game of life simulator in 64x64 grid, like many others out there. I made it out of my other project right [here](https://github.com/Blayung/ca-physics).
## Controls
**Space ->** Pause / unpause  
**LMB ->** Make cell alive  
**RMB ->** Make cell dead  
**R ->** Reset the grid  
**Q/Escape ->** Quit  
## Compiling
Clone the repo:  
`git clone https://github.com/Blayung/game-of-life.git`  
`cd game-of-life`  

Install sdl2 and sdl2 image (using pacman for example):  
`sudo pacman -S sdl2`  
`sudo pacman -S sdl2_image`  

Compile the program:  
`g++ main.cpp -lSDL2 -lSDL2_image`  

Run the program:  
`./a.out`
