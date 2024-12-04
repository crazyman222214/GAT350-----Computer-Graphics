#pragma once
#include "VertexShader.h"
#include "FragmentShader.h"

enum eFrontFace
{
	CW,
	CCW
};

enum eCullMode
{
	FRONT,
	BACK,
	NONE
};

class Shader
{
public:
	static void Draw(const vertexbuffer_t& vb);
	static bool ToScreen(const vertex_output_t& vertex, glm::vec2& screen);

public:
	static class Framebuffer* framebuffer;

	static eFrontFace front_face;
	static eCullMode cull_mode;
};

