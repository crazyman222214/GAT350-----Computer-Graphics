#include "VertexShader.h"
VertexShader::uniforms_t VertexShader::uniforms =
{
	glm::mat4{1}, //model
	glm::mat4{1}, //view
	glm::mat4{1}, //projection
	color3_t{1}

};

void VertexShader::Process(const vertex_t& ivertex, vertex_output_t& overtex)
{
    glm::mat4 mvp = uniforms.projection * uniforms.view * uniforms.model;
    overtex.position = mvp * glm::vec4{ ivertex.position, 1 };
    glm::mat4 mv = uniforms.view * uniforms.model;
    overtex.normal = glm::vec3{ glm::normalize(glm::mat3{ mv } *ivertex.normal) };

    glm::vec3 light_dir;
    
    float intensity = 1;
    glm::vec4 light_pos = uniforms.view * glm::vec4{ uniforms.light.position, 1 };

    if (uniforms.light.lightType == light_type_t::POINT)
    {
        glm::vec3 vposition = mv * glm::vec4{ ivertex.position, 1 };
        light_dir = glm::normalize(light_pos - overtex.position); // normalize light direction

        intensity = std::max(glm::dot(light_dir, overtex.normal), 0.0f); // Clamped so the lowest is 0
    }
    else if (uniforms.light.lightType == light_type_t::DIRECTIONAL)
    {
       
        light_dir = glm::normalize(uniforms.view * glm::vec4{ -uniforms.light.direction, 0 }); //flipping the normalized light direction

        intensity = std::max(glm::dot(light_dir, overtex.normal), 0.0f); // Clamped so the lowest is 0
    }

    color3_t diffuse = uniforms.light.color * intensity;
    overtex.color = uniforms.ambient + diffuse; 
}


