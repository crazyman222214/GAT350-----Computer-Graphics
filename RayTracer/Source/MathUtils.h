#pragma once
#include <glm/glm.hpp>
#define FLT_EPSILON 1.192092896e-07F 

template<typename T>
inline T Lerp(const T& a, const T& b, float t)
{
	return static_cast<T>(a + (t * (b - a)));
}

inline void QuadraticPoint(int x1, int y1, int x2, int y2, int x3, int y3, float t, int& x, int& y)
{
	float one_minus_t = 1 - t;
	float a1 = one_minus_t * one_minus_t;
	float b1 = 2 * one_minus_t * t;
	float c1 = t * t;

	x = (int)(a1 * x1 + b1 * x2 + c1 * x3);
	y = (int)(a1 * y1 + b1 * y2 + c1 * y3);
}

inline void CubicPoint(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, float t, int& x, int& y)
{
	float one_minus_t = 1 - t;
	float a = one_minus_t * one_minus_t * one_minus_t;
	float b = 3 * (one_minus_t * one_minus_t) * t;
	float c = 3 * (one_minus_t) * t*t;
	float d = t*t*t;

	x = (int)(a * x1 + b * x2 + c * x3 + d * x4);
	y = (int)(a * y1 + b * y2 + c * y3 + d * y4);
}

template<typename T>
inline T Clamp(const T& value, const T& min, const T& max)
{
	return (value > max) ? max : (value < min) ? min : value;
}

inline glm::vec3 Cross(const glm::vec3& v1, const glm::vec3& v2)
{
	glm::vec3 result;

	result.x = v1.y * v2.z - v2.y * v1.z;
	result.y = v1.x * v2.z - v2.x * v1.z;
	result.z = v1.x * v2.y - v2.x * v1.y;

	return result;
}


inline bool Approximately(float value1, float value2)
{
	// check if the difference between the values is less than epsilon
	return (std::fabs(value1 - value2) < FLT_EPSILON);
}

inline float DotProduct(const glm::vec3& vec1, const glm::vec3& vec2)
{
	return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z;
}

inline float Angle(const glm::vec3& vec1, const glm::vec3& vec2)
{
	return glm::acos(DotProduct(glm::normalize(vec1), glm::normalize(vec2)));
}

inline glm::vec3 Reflect(const glm::vec3& i, const glm::vec3& n)
{
	return i - (n * DotProduct(n, i)) * 2.0f;

}

inline bool Refract(const glm::vec3& i, const glm::vec3& n, float ri, glm::vec3& refract)
{
	glm::vec3 ni = glm::normalize(i);
	float cosine =  DotProduct(ni, n);


	float discriminant = 1 - (ri * ri) * (1 - cosine * cosine);	
	if (discriminant > 0)
	{
		//nonsense math
		refract = ri * (ni - (n * cosine)) - (n * std::sqrt(discriminant));
		return true;
	}

	return false;
}

inline float Schlick(float cosine, float index)
{
	// Step 1: Calculate the base reflectance at zero incidence (angle = 0)
	// This is the reflection coefficient when the light hits the surface straight on
	float r0 = (1.0f - index) / (1.0f + index);
	r0 = r0 * r0;

	// Step 2: Use Schlick's approximation to adjust reflectance based on angle
	// Schlick’s approximation gives the probability of reflection at an angle `cosine`
	// It interpolates between `r0` and 1, with stronger reflection at glancing angles
	return r0 + (1.0f - r0) * (float)std::pow((1.0f - cosine), 5);
}