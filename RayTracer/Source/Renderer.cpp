#include "Renderer.h"
#include "Framebuffer.h"
#include <iostream>

bool Renderer::Initialize()
{
    // initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "Error initializing SDL: " << SDL_GetError() << std::endl;
        return false;
    }

    return true;
}

bool Renderer::CreateWindow(std::string name, int height, int width)
{
    // create window
    // returns pointer to window if successful or nullptr if failed
    m_height = height;
    m_width = width;
    window = SDL_CreateWindow(name.c_str(),
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        m_width, m_height,
        SDL_WINDOW_SHOWN);
    if (window == nullptr)
    {
        std::cerr << "Error creating SDL window: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);
    return true;
}

void Renderer::Shutdown()
{
}

void Renderer::BeginFrame()
{
    // clear screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
}

void Renderer::EndFrame()
{
    // show screen
    SDL_RenderPresent(renderer);
}

void Renderer::CheckForEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            quit = true;
        }
        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
        {
            quit = true;
        }
    }
}

void Renderer::CopyFramebuffer(const Framebuffer& framebuffer)
{
    SDL_RenderCopy(renderer, framebuffer.m_texture, NULL, NULL);
}

void Renderer::operator=(const Framebuffer& framebuffer)
{
    SDL_RenderCopy(renderer, framebuffer.m_texture, NULL, NULL);
}
