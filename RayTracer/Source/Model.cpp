#include "Model.h"
#include "Framebuffer.h"
#include "Camera.h"
#include "Triangle.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "Sphere.h"

void Model::Update()
{
	for (size_t i = 0; i < m_local_vertices.size(); i++)
	{
		m_vertices[i] = m_transform * glm::vec4{ m_local_vertices[i], 1 };
	}


	m_center = glm::vec3{ 0 };
	for (auto& vertex : m_vertices)
	{
		m_center += vertex;
	}
	m_center /= (float)m_vertices.size();

	m_radius = 0;
	for (auto& vertex : m_vertices)
	{
		float radius =glm::length(vertex - m_center);
		m_radius = glm::max(radius, m_radius); //<use glm::max of the radius and m_radius>
	}
}

void Model::Draw(Framebuffer& framebuffer, const glm::mat4& model, const Camera& camera)
{

}

bool Model::Load(const std::string& filename)
{
	// open file using ifstream (input file stream)
	std::ifstream stream(filename);

	// check if stream is_open
	if (!stream.is_open())
	{
		std::cerr << "File did not open: " + __LINE__;
		return false;
	}

	Vertices_t vertices;
	std::string line;
	while (std::getline(stream, line))
	{
		// read in vertex positions
		// https://cplusplus.com/reference/string/string/substr/
		if (line.substr().find("v ") == 0)
		{
			// read position of vertex
			std::istringstream sstream{ line.substr(2) };
			glm::vec3 position;

			//Takes the position values from the stringstream
			sstream >> position.x;
			sstream >> position.y;
			sstream >> position.z;

			vertices.push_back(position);
		}

		// read in faces (triangles)
		else if (line.substr().find("f ") == 0)
		{
			// read face (triangle), index of vertex position in vertices array [1, 2, 3]
			std::istringstream sstream{ line.substr(2) };
			std::string str;
			// https://en.cppreference.com/w/cpp/string/basic_string/getline
			while (std::getline(sstream, str, ' '))
			{
				std::istringstream sstream(str);
				std::string indexString;

				// read each triangle index (position, normal, uv)
				size_t i = 0;
				unsigned int index[3] = { 0, 0, 0 }; // 0 = position, 1 = normal, 2 = uv
				while (std::getline(sstream, indexString, '/'))
				{
					if (!indexString.empty())
					{
						std::istringstream indexStream{ indexString };
						indexStream >> index[i];
					}
					i++;
				}

				// check if index 0 (position) is valid
				if (index[0] != 0)
				{
					// get vertex at index position
					// index is 1 based, need to subtract one for array
					glm::vec3 position = vertices[index[0] - 1];

					// TODO: add position to m_vertices
					m_local_vertices.push_back(position);
				}
			}
		}
	}

	m_vertices.resize(m_local_vertices.size());
	stream.close();

	return true;
}

bool Model::Hit(const ray_t& ray, raycastHit_t& raycastHit, float minDistance, float maxDistance)
{
	//check for bounding sphere raycast
	float t;
	if (!Sphere::Raycast(ray, m_center, m_radius, minDistance, maxDistance, t))
	{
		return false;
	}

	// check cast ray with mesh triangles 
	for (size_t i = 0; i < m_vertices.size(); i += 3)
	{
		float t;
		if (Triangle::Raycast(ray, m_vertices[i], m_vertices[i + 1], m_vertices[i + 2], minDistance, maxDistance, t))
		{
			glm::vec3 edge1 = m_vertices[i + 1] - m_vertices[i];
			glm::vec3 edge2 = m_vertices[i + 2] - m_vertices[i];

			// set raycast hit
			raycastHit.distance = t;
			raycastHit.point = ray.At(t);
			raycastHit.normal = glm::normalize(Cross(edge1, edge2));
			raycastHit.material = GetMaterial();

			return true;
		}
	}
	return false;
}