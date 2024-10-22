#include "Renderer.h"
#include "Framebuffer.h"
#include "Image.h"
#include "PostProcess.h"
#include "Color.h"
#include "Transform.h"
#include "ETime.h"
#include "Input.h"
#include "Model.h"
#include "Camera.h"
#include "Actor.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SDL.h>
#include <iostream>

int main(int argc, char* argv[])
{
    Time time;
    Input input;
    Image image;
    Image imageAlt;


    Renderer* renderer = new Renderer();
    renderer->Initialize();
    renderer->CreateWindow("Comp Graphics", 600, 800);

    input.Initialize();

    image.Load("scene.jpg");

    Camera camera(renderer->GetWidth(), renderer->GetHeight());
    camera.SetView(glm::vec3{ 0, 40, -50 }, glm::vec3{ 1 });
    camera.SetProjection(120.0f, 800.0f / 600.0f, 0.1f, 200.0f);
    Transform cameraTransform{ {0, 0, -20}};


    Framebuffer buffer(*renderer, renderer->GetWidth(), renderer->GetHeight());

    /*Verticies_t verticies
    { 
        {-5, 5, 0}, 
        {5, 5, 0},
        {-5, -5, 0},
        {-10, 0, 0},
        {-5, 0, 0},
        {-10, -5, 0 }
    };*/
    //Model model{ verticies, {0, 255, 0, 255} };
    Transform gunTransform{ {0, 0, 0}, glm::vec3{0, 90, 0}, glm::vec3{3} };
    Transform teacupTransform{ {20, 0, 0}, glm::vec3{0, 0, 0}, glm::vec3{3} };
    Transform benzTransform{ {20, 0, 0}, glm::vec3{0, 0, 0}, glm::vec3{3} };

    std::vector<std::unique_ptr<Actor>> actors;

    //Model gunModel;
    auto gunModel = std::make_shared<Model>();
    gunModel->Load("Pistol_02.obj");
    gunModel->SetColor({ 0, 255, 0, 255 });
    Actor gunActor(gunTransform, gunModel);

    Model teacupModel;
    teacupModel.Load("teapot.obj");
    teacupModel.SetColor({ 0, 0, 255, 255 });


    SetBlendMode(BlendMode::Normal);
    //OpenGL Math
    

    // create renderer
    while (!renderer->IsQuit())
    {
        time.Tick();
        input.Update();

        renderer->CheckForEvents();

        //Sets bg color
        buffer.Clear({128,128,128,255});
        //Allows Alpha
        PostProcess::Alpha(imageAlt.m_buffer, 128);

        //Draws background Image
        SetBlendMode(BlendMode::Normal);
        //buffer.DrawImage(-10, -20, image);

        int mx, my;
        SDL_GetMouseState(&mx, &my);

        SetBlendMode(BlendMode::Alpha);
        //buffer.DrawImage(mx, my, imageAlt);

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


        if (input.GetMouseButtonDown(2))
        {
            input.SetRelativeMode(true);

            glm::vec3 direction{ 0 };
            int rotation = 0;

            if (input.GetKeyDown(SDL_SCANCODE_RIGHT)) direction.x = 1;
            if (input.GetKeyDown(SDL_SCANCODE_LEFT)) direction.x = -1;
            if (input.GetKeyDown(SDL_SCANCODE_UP)) direction.y = -1;
            if (input.GetKeyDown(SDL_SCANCODE_DOWN)) direction.y = 1;

            if (input.GetKeyDown(SDL_SCANCODE_W)) direction.z = -1;
            if (input.GetKeyDown(SDL_SCANCODE_S)) direction.z = 1;

            if (input.GetKeyDown(SDL_SCANCODE_Q)) rotation = 1;
            if (input.GetKeyDown(SDL_SCANCODE_E)) rotation = -1;


            glm::vec3 offset = cameraTransform.GetMatrix() * glm::vec4{ direction, 0 };
            cameraTransform.position += offset * 100.0f * time.GetDeltaTime();
            //cameraTransform.rotation.x += rotation * 360.0f * time.GetDeltaTime();
            cameraTransform.rotation.y = input.GetMousePosition().x * 0.1f;
            cameraTransform.rotation.x = input.GetMousePosition().y * 0.1f;
        }
        else
        {
            input.SetRelativeMode(false);
        }
        camera.SetView(cameraTransform.position, cameraTransform.position + cameraTransform.GetForward());
        
        //transform.rotation.z += rotation * 90.0f * time.GetDeltaTime();
        
        //teacupModel.Draw(buffer, teacupTransform.GetMatrix(), camera);
        //benzModel.Draw(buffer, benzTransform.GetMatrix(), camera);
        gunActor.Draw(buffer, camera);
        
        buffer.Update();
        renderer->CopyFramebuffer(buffer);


        //buffer.DrawLinearCurve(200, 100, 100, 200, {255, 0, 0, 255});
        ////buffer.DrawQuadraticCurve(200, 300, mx, my, 400, 300, {255, 0, 0, 255});
        //buffer.DrawCubicCurve(200, 300, mx, my, 400, 500, 450, 300, {255, 0, 0, 255});
        //renderer = buffer;

        

        renderer->EndFrame();
    }

    return 0;
}