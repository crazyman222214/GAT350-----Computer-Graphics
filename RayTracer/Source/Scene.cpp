#include "Scene.h"
#include "Framebuffer.h"
#include "Camera.h"
#include "Color.h"
#include "Tracer.h"

void Scene::Render(Framebuffer& buffer, const Camera& camera)
{
	for (int y = 0; y < buffer.m_height; y++)
	{
		for (int x = 0; x < buffer.m_width; x++)
		{
			glm::vec2 pixel{ x, y };
			glm::vec2 point = pixel / glm::vec2{buffer.m_width, buffer.m_height};
			point.y = 1 - point.y;

			ray_t ray = camera.GetRay(point);

			color3_t color = Tracer::Trace(*this, ray);

			buffer.DrawPoint(x, y, ColorConvert(color));
		}
	}
}
