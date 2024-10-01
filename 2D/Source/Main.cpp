#include "Renderer.h"

#include <SDL.h>
#include <iostream>

int main(int argc, char* argv[])
{
    Renderer* renderer = new Renderer();

    renderer->Initialize();
    renderer->CreateWindow("Game Fr", 600, 800);

    // create renderer

    while (true)
    {
        // clear screen
        SDL_SetRenderDrawColor(renderer->renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer->renderer);

        // show screen
        SDL_RenderPresent(renderer->renderer);
    }

    return 0;
}