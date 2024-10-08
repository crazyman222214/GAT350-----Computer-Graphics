#include "Renderer.h"
#include "Framebuffer.h"
#include "Image.h"
#include <SDL.h>
#include <iostream>

int main(int argc, char* argv[])
{
    Renderer* renderer = new Renderer();

    renderer->Initialize();
    renderer->CreateWindow("Comp Graphics", 600, 800);

    Framebuffer buffer(*renderer, renderer->GetWidth(), renderer->GetHeight());

    Image image;
    image.Load("images.png");

    // create renderer
    while (!renderer->IsQuit())
    {
        renderer->CheckForEvents();
        buffer.Clear({255,255,255,255});
        for (int i = 0; i < 10; i++)
        {
            int x1 = rand() % 600;
            int y1 = rand() % 400;

            int x3 = 100;
            int y3 = 400;

            //buffer.DrawTriangle(x1, y1, x2, y2, x3, y3, color_t{(Uint8)(rand() % 255), (Uint8)(rand() % 255), (Uint8)(rand() % 255), 255});
            /*buffer.DrawCircle(200, 400, 10, {255, 0, 0, 255});
            buffer.DrawCircle(200, 400, 50, {255, 255, 0, 255});
            buffer.DrawCircle(400, 400, 100, {0, 0, 255, 255});

            buffer.DrawTriangle(100, 400, 300, 400, 200, 300, { 255, 255,255,255 });

            buffer.DrawLine(-10, -50, 900, 700, {255, 255, 255, 255});*/

            buffer.DrawImage(x1, y1, image);
            
        }
        //buffer.DrawLinearCurve(200, 100, 100, 200, {255, 0, 0, 255});
        int mx, my;
        SDL_GetMouseState(&mx, &my);
        //buffer.DrawQuadraticCurve(200, 300, mx, my, 400, 300, {255, 0, 0, 255});
        buffer.DrawCubicCurve(200, 300, mx, my, 400, 500, 450, 300, {255, 0, 0, 255});

        //buffer.DrawRect(100, 100, 100, 50, {255, 255, 255,255});
        buffer.Update();
        renderer->CopyFramebuffer(buffer);
        //renderer = buffer;

        renderer->EndFrame();
    }

    return 0;
}