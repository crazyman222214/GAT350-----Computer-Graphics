#include "Renderer.h"
#include "Framebuffer.h"
#include "Image.h"
#include "PostProcess.h"
#include "Color.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SDL.h>
#include <iostream>

int main(int argc, char* argv[])
{
    Renderer* renderer = new Renderer();

    renderer->Initialize();
    renderer->CreateWindow("Comp Graphics", 600, 800);

    Framebuffer buffer(*renderer, renderer->GetWidth(), renderer->GetHeight());

    Image image;
    image.Load("scene.jpg");

    Image imageAlt;
    imageAlt.Load("colors.png");
    PostProcess::Alpha(imageAlt.m_buffer, 128);

    //OpenGL Math
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));


    // create renderer
    while (!renderer->IsQuit())
    {
        renderer->CheckForEvents();
        buffer.Clear({255,255,255,255});

        buffer.DrawImage(-10, -20, image);

        for (int i = 0; i < 10; i++)
        {
            int x1 = rand() % 800;
            int y1 = rand() % 600;

            int x3 = 100;
            int y3 = 400;
            
            //buffer.DrawTriangle(x1, y1, x2, y2, x3, y3, color_t{(Uint8)(rand() % 255), (Uint8)(rand() % 255), (Uint8)(rand() % 255), 255});
            /*buffer.DrawCircle(200, 400, 10, {255, 0, 0, 255});
            buffer.DrawCircle(200, 400, 50, {255, 255, 0, 255});
            buffer.DrawCircle(400, 400, 100, {0, 0, 255, 255});

            buffer.DrawTriangle(100, 400, 300, 400, 200, 300, { 255, 255,255,255 });

            buffer.DrawLine(-10, -50, 900, 700, {255, 255, 255, 255});*/


            
        }
        //buffer.DrawLinearCurve(200, 100, 100, 200, {255, 0, 0, 255});
        int mx, my;
        SDL_GetMouseState(&mx, &my);
        ////buffer.DrawQuadraticCurve(200, 300, mx, my, 400, 300, {255, 0, 0, 255});
        //buffer.DrawCubicCurve(200, 300, mx, my, 400, 500, 450, 300, {255, 0, 0, 255});

        SetBlendMode(BlendMode::Additive);
        buffer.DrawImage(mx, my, imageAlt);

        #pragma region PostProcess
        //PostProcess::Invert(buffer.m_buffer);
        //PostProcess::Monochrome(buffer.m_buffer);
        //PostProcess::Brightness(buffer.m_buffer, 40);
        //PostProcess::ColorBalance(buffer.m_buffer, 70, 50, 50);
        //PostProcess::Noise(buffer.m_buffer, 30);
        //PostProcess::Threshold(buffer.m_buffer, 200);
        //PostProcess::Posterize(buffer.m_buffer, 10);

        //PostProcess::BoxBlur(buffer.m_buffer, buffer.m_width, buffer.m_height);
        //PostProcess::GaussianBlur(buffer.m_buffer, buffer.m_width, buffer.m_height);
        //PostProcess::Sharpen(buffer.m_buffer, buffer.m_width, buffer.m_height);
        //PostProcess::Edge(buffer.m_buffer, buffer.m_width, buffer.m_height, 10);
        //PostProcess::Emboss(buffer.m_buffer, buffer.m_width, buffer.m_height);
        #pragma endregion
        buffer.Update();
        renderer->CopyFramebuffer(buffer);
        //renderer = buffer;

        renderer->EndFrame();
    }

    return 0;
}