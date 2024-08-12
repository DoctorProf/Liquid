#pragma once
#include "Cell.hpp"

class Grid
{
public:
	Grid(float width, float height);
	void clear();

	float width;
	float height;
	std::vector<std::vector<Cell*>> cells;
};