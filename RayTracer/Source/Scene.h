#pragma once
#include "Sphere.h"
#include <vector>
#include <memory>

class Scene
{
public:
	Scene() = default;

	void Update();

	void Render(class Framebuffer& buffer, const class Camera& camera, int numSamples = 10, int depth = 5);
	void AddObject(std::unique_ptr<SceneObject> object) { m_objects.push_back(std::move(object)); }

	friend class Tracer;

private:
	std::vector<std::unique_ptr<SceneObject>> m_objects;
	
};