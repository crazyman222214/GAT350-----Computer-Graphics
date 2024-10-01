#pragma once
#include <SDL.h>
#include <string>
class Renderer
{
public:
	bool Initialize();
	bool CreateWindow(std::string name, int height, int width);
	void Shutdown();

	SDL_Window* window{ nullptr };
	SDL_Renderer* renderer{ nullptr };
};