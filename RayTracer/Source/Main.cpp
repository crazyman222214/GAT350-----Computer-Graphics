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
#include "Plane.h"
#include "Random.h"
#include "Triangle.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SDL.h>
#include <iostream>


void InitCornellBox(Scene& scene)
{
    /*One light source in the center of a white ceiling
      A green right wall
      A red left wall
      A white back wall
      A white floor
    */

    std::shared_ptr<Material> white = std::make_shared<Lambertian>(color3_t{ 1, 1, 1});
    std::shared_ptr<Material> red = std::make_shared<Lambertian>(color3_t{ 1, 0, 0});
    std::shared_ptr<Material> green = std::make_shared<Lambertian>(color3_t{ 0, 1, 0});
    std::shared_ptr<Material> lightMaterial = std::make_shared<Emissive>(color3_t{ 1, 1, 1}, 3.0f);
    std::shared_ptr<Material> metalMaterial = std::make_shared<Dielectric>(color3_t{ 0.5f}, 1.333f);
    

    std::unique_ptr<Plane> floor = std::make_unique<Plane>(white, Transform{ glm::vec3{0, 0, 0}, glm::vec3{0, 0, 0} });
    std::unique_ptr<Plane> ceiling = std::make_unique<Plane>(white, Transform{ glm::vec3{0, 30, 0}, glm::vec3{0, 0, 0} });
    std::unique_ptr<Plane> backWall = std::make_unique<Plane>(white, Transform{ glm::vec3{0, 0, 10}, glm::vec3{0, -90, 90} });
    std::unique_ptr<Plane> leftWall = std::make_unique<Plane>(red, Transform{ glm::vec3{-30, 0, 0}, glm::vec3{0, -25, 90} });
    std::unique_ptr<Plane> rightWall = std::make_unique<Plane>(green, Transform{ glm::vec3{30, 0, 0}, glm::vec3{0, 25, 90} });

    std::unique_ptr<Model> light = std::make_unique<Model>(lightMaterial, Transform{ glm::vec3{0, 29, -10}, glm::vec3{0, 0, 0}, glm::vec3{10} });
    light->Load("Models/quad.obj");

    std::unique_ptr<Model> cube = std::make_unique<Model>(metalMaterial, Transform{ glm::vec3{-4.5f, 2.75f, -10.0f}, glm::vec3{0, 0, 0}, glm::vec3{7.5f, 12.5f, 3.0f} });
    cube->Load("Models/cube.obj");


    std::unique_ptr<Model> cube2 = std::make_unique<Model>(white, Transform{ glm::vec3{-12.0f, 2.75f, -14.0f}, glm::vec3{0, -15, 0}, glm::vec3{5} });
    cube2->Load("Models/cube.obj");

    std::unique_ptr<Sphere> sphere = std::make_unique<Sphere>(white, 5, Transform{ glm::vec3{5, 4, -12}, glm::vec3{0, 0, 0} });


    scene.AddObject(std::move(floor));
    scene.AddObject(std::move(ceiling));
    scene.AddObject(std::move(backWall));
    scene.AddObject(std::move(leftWall));
    scene.AddObject(std::move(rightWall));

    scene.AddObject(std::move(light));
    scene.AddObject(std::move(cube));
    scene.AddObject(std::move(cube2));
    scene.AddObject(std::move(sphere));
}


void InitScene(Scene& scene)
{
    std::shared_ptr<Material> planeMaterial = std::make_shared<Lambertian>(color3_t{ 0.59f, 0.59f, 0.66f });
    std::unique_ptr<Plane> plane = std::make_unique<Plane>(planeMaterial, Transform{ glm::vec3{0, -5, 0}, glm::vec3{0, 0, 0} });

    std::shared_ptr<Material> gray = std::make_shared<Metal>(color3_t{ 0.5f }, 0.5f);
    std::shared_ptr<Material> red = std::make_shared<Dielectric>(color3_t{ 1, 1, 1 }, 1);
    std::shared_ptr<Material> green = std::make_shared<Dielectric>(color3_t{ 1, 1, 1 }, 1.333f);
    std::shared_ptr<Material> blue = std::make_shared<Lambertian>(color3_t{ 0, 0, 1 });

    std::vector<std::shared_ptr<Material>> materials;

    materials.push_back(red);
    materials.push_back(gray);
    materials.push_back(blue);
    materials.push_back(green);

    auto model = std::make_unique<Model>(blue, Transform{ glm::vec3{0, 0, 2}, glm::vec3{0, 40, 0}, glm::vec3{4} });
    model->Load("Models/cube.obj");
    scene.AddObject(std::move(model));


    for (int i = 0; i < 00; i++)
    {
        auto object = std::make_unique<Sphere>(materials[random(materials.size())], randomf(2), Transform{ random(glm::vec3{-10}, glm::vec3{10}) });
        //auto triangle = std::make_unique<Triangle>(random(glm::vec3{ -5 }, glm::vec3{5}), random(glm::vec3{ -5 }, glm::vec3{ 5 }), random(glm::vec3{ -5 }, glm::vec3{ 5 }), blue);
        scene.AddObject(std::move(object));
        //scene.AddObject(std::move(triangle));
    }

    scene.AddObject(std::move(plane));
}

void InitScene01(Scene& scene, Camera& camera)
{
    camera.SetFOV(20.0f);
    camera.SetView({ 13, 2, 3 }, { 0, 0, 0 });

    auto ground_material = std::make_shared<Lambertian>(color3_t(0.5f));
    scene.AddObject(std::make_unique<Plane>(ground_material, Transform{ glm::vec3{ 0 } }));

    for (int a = -20; a < 20; a++) {
        for (int b = -20; b < 20; b++) {
            auto choose_mat = randomf();
            glm::vec3 center(a + 0.9 * randomf(), 0.2, b + 0.9 * randomf());

            if ((center - glm::vec3(4, 0.2, 0)).length() > 0.9) {
                std::shared_ptr<Material> sphere_material;

                if (choose_mat < 0.5) {
                    // diffuse
                    auto albedo = HSVtoRGB({ randomf(0, 360), 1.0f, 1.0f });
                    sphere_material = std::make_shared<Lambertian>(albedo);
                    scene.AddObject(std::make_unique<Sphere>(sphere_material, 0.2f, Transform{ center }));
                }
                else if (choose_mat < 0.90) {
                    // metal
                    auto albedo = HSVtoRGB({ randomf(0, 360), 1.0f, 1.0f });
                    auto fuzz = randomf(0.0f, 0.5f);
                    sphere_material = std::make_shared<Metal>(albedo, fuzz);
                    scene.AddObject(std::make_unique<Sphere>(sphere_material, 0.2f, Transform{ center }));
                }
                else {
                    // glass
                    sphere_material = std::make_shared<Dielectric>(color3_t{ 1 }, 1.5f);
                    scene.AddObject(std::make_unique<Sphere>(sphere_material, 0.2f, Transform{ center }));
                }
            }
        }
    }

    auto material1 = std::make_shared<Dielectric>(color3_t{ 1 }, 1.5f);
    scene.AddObject(std::make_unique<Sphere>(material1, 1.0f, Transform{ glm::vec3{ 0, 1, 0 } }));

    auto material2 = std::make_shared<Lambertian>(color3_t(0.4f, 0.2f, 0.1f));
    scene.AddObject(std::make_unique<Sphere>(material2, 1.0f, Transform{ glm::vec3{ -4, 1, 0 } }));

    auto material3 = std::make_shared<Metal>(color3_t(0.7f, 0.6f, 0.5f), 0.0f);
    scene.AddObject(std::make_unique<Sphere>(material3, 1.0f, Transform{ glm::vec3{ 4, 1, 0 } }));
}

int main(int argc, char* argv[])
{   
    srand((unsigned int)time(NULL));
    Time time;

    Renderer* renderer = new Renderer();
    renderer->Initialize();
    renderer->CreateWindow("Comp Graphics", 600, 800);

    Framebuffer buffer(*renderer, renderer->GetWidth(), renderer->GetHeight());

    Camera camera(70.0f, buffer.m_width / (float)buffer.m_height);
    //camera.SetView({ 0, 10, -40 }, glm::vec3{0, 8, 0});

    Scene scene;

    
    SetBlendMode(BlendMode::Normal);
    //OpenGL Math
    InitScene01(scene, camera);
    //InitCornellBox(scene);

    // create renderer
    scene.Update();
    scene.Render(buffer, camera, 150, 35);
    //PostProcess::Sharpen(buffer.m_buffer, buffer.m_width, buffer.m_height);
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
