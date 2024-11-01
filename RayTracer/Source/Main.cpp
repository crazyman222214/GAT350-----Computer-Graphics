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
#include "Triangle.h"

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

    std::shared_ptr<Material> sphereMaterial = std::make_shared<Emissive>(color3_t{1, 1, 0}, 20);
    std::unique_ptr<Sphere> sphere = std::make_unique<Sphere>(glm::vec3{0, 0, 0}, 2.0f, sphereMaterial);
    //scene.AddObject(std::move(sphere));

    std::shared_ptr<Material> planeMaterial = std::make_shared<Lambertian>(color3_t{ 0.59f, 0.59f, 0.66f });
    std::unique_ptr<Plane> plane = std::make_unique<Plane>(glm::vec3{ 0, 15, 0 }, glm::vec3{ 0, 1, 0.15f }, planeMaterial);

    std::shared_ptr<Material> gray = std::make_shared<Metal>(color3_t{ 0.5f }, 0.5f);
    std::shared_ptr<Material> red = std::make_shared<Dielectric>(color3_t{ 1, 1, 1 }, 1);
    std::shared_ptr<Material> green = std::make_shared<Dielectric>(color3_t{ 1, 1, 1 }, 1.333f);
    std::shared_ptr<Material> blue = std::make_shared<Lambertian>(color3_t{ 0, 0, 1 });

    std::vector<std::shared_ptr<Material>> materials;

    materials.push_back(red);
    materials.push_back(gray);
    materials.push_back(blue);
    materials.push_back(green);

    auto model = std::make_unique<Model>(blue);
    model->Load("teapot.obj");
    scene.AddObject(std::move(model));


    for (int i = 0; i < 00; i++)
    {
        auto object = std::make_unique<Sphere>(random(glm::vec3{ -10 }, glm::vec3{ 10 }), randomf(2), materials[random(materials.size())]);
        auto triangle = std::make_unique<Triangle>(random(glm::vec3{ -5 }, glm::vec3{5}), random(glm::vec3{ -5 }, glm::vec3{ 5 }), random(glm::vec3{ -5 }, glm::vec3{ 5 }), blue);
        scene.AddObject(std::move(object));
        scene.AddObject(std::move(triangle));
    }

    auto object = std::make_unique<Sphere>(glm::vec3{0, 5, 5}, 2.5f, gray);
    auto object1 = std::make_unique<Sphere>(glm::vec3{5, 0, 0}, 2.5f, blue);
    auto object2 = std::make_unique<Sphere>(glm::vec3{-5, 0, 0}, 2.5f, red);

    auto triangle = std::make_unique<Triangle>(glm::vec3{ 0, 0, 0 }, glm::vec3{ 0, -5, 0 }, glm::vec3{5, 0, 0}, blue);
    //scene.AddObject(std::move(triangle));
   

    //scene.AddObject(std::move(object));
    //scene.AddObject(std::move(object1));
    //scene.AddObject(std::move(object2));

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
    scene.Render(buffer, camera, 10, 10);
    buffer.Update();

    while (!renderer->IsQuit())
    {
        time.Tick();

        renderer->CheckForEvents();
        //tracer.Render(buffer, camera);
        renderer->CopyFramebuffer(buffer);

        renderer->EndFrame();
    }

    return 0;
}