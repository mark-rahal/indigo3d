#pragma once
#include "Window.h"
#include "Graphics.h"

class Game {
private:
	Window window;
	Graphics gfx;
	void render();
public:
	Game();
	~Game();
	int run();
};