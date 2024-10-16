#include "Framebuffer.h"
#include "Renderer.h"
#include "Image.h"
#include <iostream>
#include "Color.h"

Framebuffer::Framebuffer(const Renderer& renderer, int width, int height)
{
	m_width = width;
	m_height = height;
	m_pitch = width * sizeof(color_t);

	m_texture = SDL_CreateTexture(renderer.renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STATIC, width, height);
	if (!m_texture)
	{
		std::cerr << "Error initializing SDL: " << SDL_GetError() << std::endl;
	}
	m_buffer.resize(m_width * m_height);
}

Framebuffer::~Framebuffer()
{
	SDL_DestroyTexture(m_texture);
}

void Framebuffer::Update()
{
	SDL_UpdateTexture(m_texture, NULL, m_buffer.data(), m_pitch);
}

void Framebuffer::Clear(const color_t& color)
{
	std::fill(m_buffer.begin(), m_buffer.end(), color);
}

void Framebuffer::DrawPoint(int x, int y, const color_t& color)
{
	color_t& dest = m_buffer[x + (y * m_width)];
	dest = ColorBlend(color, dest);
}

void Framebuffer::DrawPointClip(int x, int y, const color_t& color)
{

	if (x >= m_width || x < 0 || y >= m_height || y < 0) return;
	color_t& dest = m_buffer[x + (y * m_width)];
	dest = ColorBlend(color, dest);
}

void Framebuffer::DrawRect(int x, int y, int height, int width, const color_t& color)
{
	if (x >= m_width || x + width < 0 || y >= m_height || y + height < 0) return;

	//Clamping the x and y values to the screen
	int x1 = std::max(x, 0);
	int x2 = std::min(x + width, m_width);
	int y1 = std::max(y, 0);
	int y2 = std::min(y + height, m_height);

	for (int h = y1; h < y2; h++)
	{
		int index = x1 + (h * m_width);
		std::fill(m_buffer.begin() + index, m_buffer.begin() + (index + x2 - x1), color);
		//for (int w = x1; w < x2; w++)
		//{
	//m_buffer[w + (h * m_width)] = color;
		//}
	}
}

void Framebuffer::DrawLineSlope(int x1, int y1, int x2, int y2, const color_t& color)
{
	int dx = x2 - x1;
	int dy = y2 - y1;

	
	if (dx == 0)
	{
		if (y1 > y2) std::swap(y1, y2);
		for (int y = y1; y < y2; y++)
		{
			m_buffer[x1 + y * m_width] = color;
		}
	}
	else
	{
		float m = (float)dy / dx;
		float b = y1 - (m * x1);

		if (std::abs(dx) > std::abs(dy))
		{
			for (int x = x1; x < x2; x++)
			{
				// y = mx + b
				int y = (int)round((m * x) + b);
				m_buffer[x + (y * m_width)] = color;
			}
		}
		else
		{
			for (int y = y1; y < y2; y++)
			{
				// x = (y - b) / m
				int x = (int)round((y - b) / m);
				m_buffer[x + (y * m_width)] = color;
			}
		}
	}
	

	

}

void Framebuffer::DrawLine(int x1, int y1, int x2, int y2, const color_t& color)
{

	CohenSutherlandClipDetection(x1, y1, x2, y2);

	int dy = y2 - y1;
	int dx = x2 - x1;

	bool steep = std::abs(dy) > std::abs(dx);

	if (steep)
	{
		std::swap(x1, y1);
		std::swap(x2, y2);
	}

	if (x1 > x2)
	{
		std::swap(x1, x2);
		std::swap(y1, y2);
	}

	dy = std::abs(y2 - y1);
	dx = x2 - x1;

	int error = dx / 2;
	int ystep = (y1 < y2) ? 1 : -1;

	for (int x = x1, y = y1; x <= x2; x++)
	{
		(steep) ? DrawPoint(y, x, color) : DrawPoint(x, y, color);
		error -= dy;
		if (error < 0)
		{
			y += ystep;
			error += dx;
		}
	}
}

void Framebuffer::DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, const color_t& color)
{
	DrawLine(x1, y1, x2, y2, color);
	DrawLine(x2, y2, x3, y3, color);
	DrawLine(x3, y3, x1, y1, color);
}

void Framebuffer::DrawCircle(int xc, int yc, int radius, const color_t& color)
{
	//Bresenham's Circle Algorithm
	//https://www.geeksforgeeks.org/bresenhams-circle-drawing-algorithm/

	int x = 0;
	int y = radius;
	int d = 3 - (2 * radius);

	CirclePoint(xc, yc, x, y, color);

	while (x <= y)
	{
		if (d < 0)
		{
			d += (4 * x) + 6;
		}
		else
		{
			d += 4 * (x - y) + 10;
			y--;
		}
		x++;

		CirclePoint(xc, yc, x, y, color);
	}
	
}

void Framebuffer::CirclePoint(int xc, int yc, int x, int y, const color_t& color)
{

	m_buffer[(xc + x) + ((yc + y) *m_width)] = color;
	m_buffer[(xc - x) + ((yc + y) *m_width)] = color;
	m_buffer[(xc + x) + ((yc - y) *m_width)] = color;
	m_buffer[(xc - x) + ((yc - y) *m_width)] = color;

	//Flip the x and the y values
	m_buffer[(xc + y) + ((yc + x) *m_width)] = color;
	m_buffer[(xc - y) + ((yc + x) *m_width)] = color;
	m_buffer[(xc + y) + ((yc - x) *m_width)] = color;
	m_buffer[(xc - y) + ((yc - x) *m_width)] = color;
}



int Framebuffer::ClippingRegionCode(int x, int y)
{
	//        T|B|R|L
	int code = 0000;

	//If memory was an issue, I wouldn't store these values but it increases readability
	short left = 1;
	short right = 2;
	short bottom = 4;
	short top = 8;

		// Bit OR operator
	if (x < 0) code |= left;
	if (x > m_width) code |= right;
	if (y < 0) code |= top;
	if (y > m_height) code |= bottom;

	return code;
}

//If I make the parameters references, I don't have to worry about returning or messing with the variables
void Framebuffer::CohenSutherlandClipDetection(int& x1, int& y1, int& x2, int& y2)
{
	//If memory was an issue, I wouldn't store these values but it increases readability
	short left = 1;
	short right = 2;
	short bottom = 4;
	short top = 8;

	int code1 = ClippingRegionCode(x1, y1);
	int code2 = ClippingRegionCode(x2, y2);
	bool accept = false;

	while (true)
	{
		//If the codes match have 0000, both endpoints are inside
		if (code1 == 0 && code2 == 0)
		{
			accept = true;
			break;
		}
		//If the codes are in the same region AND they are both not inside, we don't need to draw it
		else if (code1 & code2)
		{
			break;
		}
		//Otherwise, we cross regions and have to clip
		else
		{
			int codeOut;
			int x, y;

			//Finds the outside endpoint
			codeOut = (code1) ? code1 : code2;

			//Does some funny math to find the intersection with the borders
			if (codeOut & top)
			{
				x = x1 + (x2 - x1) * (0 - y1) / (y2 - y1);
				y = 0;
			}
			else if (codeOut & bottom)
			{
				x = x1 + (x2 - x1) * (m_height - y1) / (y2 - y1);
				y = m_height;
			}
			else if (codeOut & left)
			{
				y = y1 + (y2 - y1) * (0 - x1) / (x2 - x1);
				x = 0;
			}
			else if (codeOut & left)
			{
				y = y1 + (y2 - y1) * (m_width - x1) / (x2 - x1);
				x = m_width;
			}

			//Finds out which code it is correcting and generates a new clipping code and runs the algo again
			if (codeOut == code1)
			{
				x1 = x;
				y1 = y;
				code1 = ClippingRegionCode(x1, y1);
			}
			else if (codeOut == code2)
			{
				x2 = x;
				y2 = y;
				code2 = ClippingRegionCode(x2, y2);
			}
		}
	}
}

void Framebuffer::DrawLinearCurve(int x1, int y1, int x2, int y2, const color_t& color)
{
	float dt = 1.0f/10;
	float t = 0;
	for (int i = 0; i < 10; i++)
	{
		int sx1 = Lerp(x1, x2, t);
		int sy1 = Lerp(y1, y2, t);
		
		int sx2 = Lerp(x1, x2, t+dt);
		int sy2 = Lerp(y1, y2, t+dt);
		t += dt;

		DrawLine(sx1, sy1, sx2, sy2, color);
	}
	
}

void Framebuffer::DrawQuadraticCurve(int x1, int y1, int x2, int y2, int x3, int y3, const color_t& color)
{
	float dt = 1.0f / 10;
	float t = 0;
	for (int i = 0; i < 10; i++)
	{
		int sx1, sy1;
		QuadraticPoint(x1, y1, x2, y2, x3, y3, t, sx1, sy1);
		int sx2, sy2;
		QuadraticPoint(x1, y1, x2, y2, x3, y3, t+dt, sx2, sy2);
		
		t += dt;

		DrawLine(sx1, sy1, sx2, sy2, color);
	}
}

void Framebuffer::DrawCubicCurve(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, const color_t& color)
{
	float dt = 1.0f / 500;
	float t = 0;
	
	for (int i = 0; i < 500; i++)
	{
		int sx1, sy1;
		CubicPoint(x1, y1, x2, y2, x3, y3, x4, y4, t, sx1, sy1);
		int sx2, sy2;
		CubicPoint(x1, y1, x2, y2, x3, y3, x4, y4, t + dt, sx2, sy2);

		t += dt;

		DrawLine(sx1, sy1, sx2, sy2, color);
	}
}

void Framebuffer::DrawImage(int x, int y, const Image& image)
{
	// check if off-screen
	if (x + image.m_width < 0 || x > m_width || y + image.m_height < 0 || y > m_height) return;

	// iterate through image y
	for (int iy = 0; iy < image.m_height; iy++)
	{
		// set screen y 
		int sy = y + iy;
		// check if off-screen, don't draw if off-screen
		if (sy >= m_height || sy < 0) continue;

		// iterate through image x
		for (int ix = 0; ix < image.m_width; ix++)
		{
			// set screen x
			int sx = x + ix;
			// check if off-screen, don't draw if off-screen
			if (sx >= m_width || sx < 0) continue;

			// get image pixel color
			color_t color = image.m_buffer[ix + (iy * image.m_width)];
			// check alpha, if 0 don't draw
			if (color.a == 0) continue;
			// set buffer to color
			DrawPoint(sx, sy, color);
		}
	}
}




