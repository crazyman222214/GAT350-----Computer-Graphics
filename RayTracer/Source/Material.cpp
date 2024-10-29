#include "Material.h"
#include "Random.h"
bool Lambertian::Scatter(const ray_t& ray, const raycastHit_t& raycastHit, color3_t& attenuation, ray_t& scatter)
{
    scatter.origin = raycastHit.point;
    scatter.direction = raycastHit.normal + randomOnUnitSphere();
    

    attenuation = m_albedo;

    return true;
}

bool Metal::Scatter(const ray_t& ray, const raycastHit_t& raycastHit, color3_t& attenuation, ray_t& scatter)
{
    glm::vec3 reflected = Reflect(raycastHit.point, raycastHit.normal);//<Reflect() function with raycast hit direction and normal>

        // set scattered ray from reflected ray + random point in sphere (fuzz = 0 no randomness, fuzz = 1 random reflected)
        // a mirror has a fuzz value of 0 and a diffused metal surface a higher value
        scatter = ray_t{ raycastHit.point, reflected + (randomOnUnitSphere() * m_fuzz) };
        attenuation = m_albedo;

        // check that reflected ray is going away from surface normal (dot product > 0)
        return DotProduct(scatter.direction, raycastHit.normal) > 0; //<dot product of scattered ray direction and raycast hit normal> > 0;
}


