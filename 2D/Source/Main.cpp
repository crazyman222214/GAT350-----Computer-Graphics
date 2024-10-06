#include "Renderer.h"
#include "Framebuffer.h"
#include <SDL.h>
#include <iostream>

int main(int argc, char* argv[])
{
    Renderer* renderer = new Renderer();

    renderer->Initialize();
    renderer->CreateWindow("Comp Graphics", 600, 800);

    Framebuffer buffer(*renderer, renderer->GetWidth(), renderer->GetHeight());

    // create renderer
    while (!renderer->IsQuit())
    {
        renderer->CheckForEvents();



        buffer.Clear({0,0,0,255});
        //for (int i = 0; i < 10; i++)
        {
            int x1 = 300;
            int y1 = 300;

            int x2 = 200;
            int y2 = 200;

            int x3 = 100;
            int y3 = 400;

            //buffer.DrawTriangle(x1, y1, x2, y2, x3, y3, color_t{(Uint8)(rand() % 255), (Uint8)(rand() % 255), (Uint8)(rand() % 255), 255});
            buffer.DrawCircle(200, 400, 10, {255, 0, 0, 255});
            buffer.DrawCircle(200, 400, 50, {255, 255, 0, 255});
            buffer.DrawCircle(400, 400, 100, {0, 0, 255, 255});

            buffer.DrawLine(-10, -50, 900, 700, {255, 255, 255, 255});
        }

        buffer.DrawRect(100, 100, 100, 50, {255, 255, 255,255});
        buffer.Update();
        renderer->CopyFramebuffer(buffer);
        //renderer = buffer;

        renderer->EndFrame();
    }

    return 0;
}