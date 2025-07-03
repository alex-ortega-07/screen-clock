#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <functional>

#include "Button.h"

class Grid {
public:
	Grid(int x, int y, int w, int h);

	void setWidth(int w);
	void setHeight(int h);

	int updateDistribution(SDL_Renderer* renderer);

	void setMargin(int m);
	void setHidden(bool showBts);

	int add(Button& bt);
	int removeLast();

	std::vector<std::reference_wrapper<Button>> getBtList();

private:
	int posx, posy;
	int width, height;

	int margin;

	bool hidden = false;

	std::vector<std::reference_wrapper<Button>> btList;
};
