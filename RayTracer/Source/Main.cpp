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
#include "Tracer.h"
#include "Scene.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SDL.h>
#include <iostream>
#include "Plane.h"
#include "Random.h"

int main(int argc, char* argv[])
{
    srand((unsigned int)time(NULL));
    Time time;

    Renderer* renderer = new Renderer();
    renderer->Initialize();
    renderer->CreateWindow("Comp Graphics", 600, 800);

    Framebuffer buffer(*renderer, renderer->GetWidth(), renderer->GetHeight());

    Camera camera(70.0f, buffer.m_width / (float)buffer.m_height);
    camera.SetView({ 0, 0, -20 }, glm::vec3{0});

    Scene scene;

    std::shared_ptr<Material> sphereMaterial = std::make_shared<Lambertian>(color3_t{1, 1, 0});
    std::unique_ptr<Sphere> sphere = std::make_unique<Sphere>(glm::vec3{0, 0, 0}, 2.0f, sphereMaterial);
    scene.AddObject(std::move(sphere));

    std::shared_ptr<Material> planeMaterial = std::make_shared<Lambertian>(color3_t{ 0.59f, 0.59f, 0.66f });
    std::unique_ptr<Plane> plane = std::make_unique<Plane>(glm::vec3{ 0, 5, 0 }, glm::vec3{ 0, 1, 0.15f }, planeMaterial);

    std::shared_ptr<Material> gray = std::make_shared<Metal>(color3_t{ 0.5f }, 1);
    std::shared_ptr<Material> red = std::make_shared<Lambertian>(color3_t{ 1, 0, 0 });
    std::shared_ptr<Material> blue = std::make_shared<Metal>(color3_t{ 0, 0, 1 }, 0.5f);

    std::vector<std::shared_ptr<Material>> materials;

    materials.push_back(red);
    materials.push_back(gray);
    materials.push_back(blue);

    for (int i = 0; i < 30; i++)
    {
        auto object = std::make_unique<Sphere>(random(glm::vec3{ -15 }, glm::vec3{ 15 }), randomf(2), materials[random(materials.size())]);
        scene.AddObject(std::move(object));
    }

    scene.AddObject(std::move(plane));

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

    SetBlendMode(BlendMode::Normal);
    //OpenGL Math


    // create renderer
    while (!renderer->IsQuit())
    {
        time.Tick();

        renderer->CheckForEvents();

        buffer.Clear(ColorConvert(color4_t{0, 0, 0, 1}));
        scene.Render(buffer, camera, 100, 10);
        //tracer.Render(buffer, camera);
        buffer.Update();
        renderer->CopyFramebuffer(buffer);

        renderer->EndFrame();
    }

    return 0;
}