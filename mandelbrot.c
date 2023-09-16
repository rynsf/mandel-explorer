#include <stdio.h>
#include <SDL2/SDL.h>
#include <complex.h>

#define SIZEX 1400
#define SIZEY 1000

SDL_Window* win;
SDL_Renderer* rend;
double inty;
double intx;
double complex c;

int isMandelbrot(double complex C) {
    double complex z = 0;
    for(int n = 0; n < 255; n++) {
        z = (z*z) + C;
        if(creal(z) > 2 || creal(z) < -2 || cimag(z) > 2 || cimag(z) < -2) {
            return n;
        }
    }
    return 0;
}

int DrawMandelbrot() {
    int iter;
    for(int y = 0; y < SIZEY; y++) {
        for(int x = 0; x < SIZEX; x++) {
            iter = isMandelbrot(c);
            SDL_SetRenderDrawColor(rend, iter, iter, iter, 255);
            SDL_RenderDrawPoint(rend, x, y);
            c = (creal(c)+intx) + (cimag(c) * I);
        }
        c = -2 + ((cimag(c)-inty) * I);
    }
}

int Init() {
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Failed to init SDL: %s", SDL_GetError());
        return -1;
    }

    win = SDL_CreateWindow("Mandelbrot set", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SIZEX, SIZEY, 0);
    if(win == NULL) {
        printf("Failed to create SDL Window: %s", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    rend = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(rend == NULL) {
        printf("Failed to Create Renderer: %s", SDL_GetError());
        SDL_DestroyWindow(win);
        SDL_Quit();
        return -1;
    }
    return 0;
}

int main(int argc, char** argv) {
    inty = 4.0 / (2*SIZEY);
    intx = 4.0 / (2*SIZEY);
    c = (intx*-SIZEX/2) + (inty * SIZEY/2) * I;

    if(Init() != 0) {
        return -1;
    }
    SDL_RenderClear(rend);
    SDL_RenderPresent(rend);

    int loop = 1;
    int rendered = 0;
    while(loop) {
        SDL_Event event;
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) {
                loop = 0;
            }
        }

        if(!rendered) {
            DrawMandelbrot();
            SDL_RenderPresent(rend);
            rendered = 1;
        }
        SDL_Delay(20);
    }

    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
