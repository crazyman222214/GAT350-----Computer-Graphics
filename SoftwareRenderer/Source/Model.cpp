#include "Model.h"
#include "Framebuffer.h"
#include "Shader.h"

#include <iostream>
#include <fstream>
#include <sstream>
/// <summary>
/// Convert point from model space to world space
/// </summary>
/// <param name="framebuffer">The buffer we are drawing to</param>
/// <param name="model">the model matrix that holds the points in model space</param>
void Model::Draw()
{

	Shader::Draw(m_vb);

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

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
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
		//normals
		else if (line.substr().find("vn ") == 0)
		{
			// read normal of vertex
			std::istringstream sstream{ line.substr(3) };
			glm::vec3 normal;

			//Takes the position values from the stringstream
			sstream >> normal.x;
			sstream >> normal.y;
			sstream >> normal.z;

			normals.push_back(normal);
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

				// check if index 0 (position) && index 1 (normal) is valid
				if (index[0])
				{
					// get vertex at index position
					// index is 1 based, need to subtract one for array

					// TODO: add position to m_vertices
					vertex_t vertex;
					vertex.position = vertices[index[0] - 1];
					vertex.normal = (index[2]) ? normals[index[2] - 1] : glm::vec3{ 1 };
					
					m_vb.push_back(vertex);
				}
			}
		}
	}

	stream.close();

	return true;
}
