#include "Framebuffer.h"
#include "Renderer.h"
#include <iostream>

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
	if (x >= m_width || x < 0 || y >= m_height || y < 0) return;
	m_buffer[x + (y*m_width)] = color;
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

	std::cout << "initial 1st point is at (" << x1 << ", " << y1 << ")\n";
	std::cout << "initial 2nd point is at (" << x2 << ", " << y2 << ")\n";

	CohenSutherlandClipDetection(x1, y1, x2, y2);
	//Verifying the Clip Detection works :0
	std::cout << "new 1st point is at (" << x1 << ", " << y1 << ")\n";
	std::cout << "new 2nd point is at (" << x2 << ", " << y2 << ")\n";

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

			//Since our y=0 is at the top, y=0
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




