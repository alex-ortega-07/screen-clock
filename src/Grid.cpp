#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <functional>

#include <iostream>

#include "Grid.h"
#include "Button.h"

Grid::Grid(int x, int y, int w, int h)
	:posx(x), posy(y), width(w), height(h) {}

void Grid::setWidth(int w) {
	this->width = w;
}

void Grid::setHeight(int h) {
	this->height = h;
}

int Grid::updateDistribution(SDL_Renderer* renderer) {
	if (btList.empty()) return 0;

	int startingPosx = 0;
	int totalWidth = 0;
	int totalWidthBts = 0;

	int numberBtsPerRow = 1;
	int numRow = 0;

	std::vector<Button> btsToPut;
	std::vector<std::vector<std::reference_wrapper<Button>>> rowBts;

	for (int i = 0; i < btList.size(); i++) {
		if ((totalWidthBts + btList.at(i).get().getWidth() + this->margin * i) > this->width) break;

		totalWidthBts += btList.at(i).get().getWidth();
		btsToPut.push_back(btList.at(i));
		numberBtsPerRow = btsToPut.size();
	}

	if (btList.size() % numberBtsPerRow == 0) numRow = btList.size() / numberBtsPerRow;

	else numRow = btList.size() / numberBtsPerRow + 1;

	int counter = 0;
	bool flag = false;
	std::vector<std::reference_wrapper<Button>> row;
	for (int i = 0; i < numRow; i++){
		for (int j = 0; j < numberBtsPerRow; j++) {
			if (counter == btList.size()) {
				flag = true;
				break;
			}

			row.push_back(btList.at(counter++));
		}

		rowBts.push_back(row);
		row.clear();

		if (flag) break;
	}

	flag = false;
	counter = 0;
	int startingPosy = 0;
	
	int totalHeightBt = 0;
	int totalHeight = 0;
	int maxBtHeight = rowBts.at(0).at(0).get().getHeight();
	for (int i = 0; i < rowBts.size(); i++){
		for (int j = 0; j < numberBtsPerRow; j++) {
			if (counter == btList.size()) {
				flag = true;
				break;
			}

			if (rowBts.at(i).at(j).get().getHeight() > maxBtHeight) maxBtHeight = rowBts.at(i).at(j).get().getHeight();
			counter++;
		}

		totalHeightBt += maxBtHeight;
		if (flag) break;
	}

	if (this->hidden) return 0;

	flag = false;
	counter = 0;

	totalWidth = totalWidthBts + (rowBts.at(0).size() - 1) * this->margin;
	totalHeight = totalHeightBt + (rowBts.size() - 1) * this->margin;

	startingPosx = this->posx + (this->width - totalWidth) / 2;
	startingPosy = this->posy + (this->height - totalHeight) / 2;
	for (int i = 0; i < numRow; i++) {
		for (int j = 0; j < numberBtsPerRow; j++) {
			if (counter == btList.size()) {
				flag = true;
				break;
			}
			int btPosx = startingPosx + j * rowBts.at(i).at(j).get().getWidth() + this->margin * j;
			int btPosy = startingPosy + i * rowBts.at(i).at(j).get().getHeight() + this->margin * i;
			
			
			rowBts.at(i).at(j).get().setPosx(btPosx);
			rowBts.at(i).at(j).get().setPosy(btPosy);
			rowBts.at(i).at(j).get().create(renderer);
			counter++;
		}

		if (flag) break;
	}

	return 1;
}

void Grid::setMargin(int m) {
	this->margin = m;
}

int Grid::add(Button& bt) {
	btList.push_back(bt);
	return 1;
}

int Grid::removeLast() {
	if (btList.empty()) return 0;

	btList.pop_back();
	return 1;
}

std::vector<std::reference_wrapper<Button>> Grid::getBtList() {
	return this->btList;
}

void Grid::setHidden(bool set) {
	for (int i = 0; i < btList.size(); i++) {
		btList.at(i).get().setHidden(set);
	}

	this->hidden = set;
}

int Grid::isHidden() {
	for (int i = 0; i < btList.size(); i++) {
		if (btList.at(i).get().isHidden()) return 1;
	}

	return 0;
}
