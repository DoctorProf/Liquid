#pragma once
#include "Cell.hpp"

class Grid
{
public:
	Grid(float width, float height,	float sizeCell);
	void clear();

	float width;
	float height;
	float sizeCell;
	std::vector<std::vector<Cell*>> cells;
};