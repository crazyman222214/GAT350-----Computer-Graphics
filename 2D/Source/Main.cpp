#include "Renderer.h"
#include "Framebuffer.h"
#include <SDL.h>
#include <iostream>

int main(int argc, char* argv[])
{
    Renderer* renderer = new Renderer();

    renderer->Initialize();
    renderer->CreateWindow("Game Fr", 600, 800);

    Framebuffer buffer(*renderer, renderer->GetWidth(), renderer->GetHeight());

    // create renderer
    while (!renderer->IsQuit())
    {
        renderer->CheckForEvents();



        buffer.Clear({0,0,0,255});
        /*for (int i = 0; i < 100; i++)
        {
            int x = rand() % 800;
            int y = rand() % 600;
            buffer.DrawPoint(x, y, { 255,255,255,255 });

        }*/

        buffer.DrawRect(100, 100, 100, 50, {255, 255, 255,255});

        buffer.Update();

        renderer->CopyFramebuffer(buffer);
        //renderer = buffer;

        renderer->EndFrame();
    }

    return 0;
}