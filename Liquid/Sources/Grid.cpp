#include "../Headers/Grid.hpp"

Grid::Grid(float width, float height, float sizeCell) 
{
	this->width = width;
	this->height = height;
	this->sizeCell = sizeCell;
	for (size_t i = 0; i < height; i++) 
	{
		cells.push_back(std::vector<Cell*>());
		for (size_t j = 0; j < width; j++)
		{
			cells[i].push_back(new Cell());
		}
	}
}
void Grid::clear() 
{
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			cells[i][j]->particlesIndex.clear();
		}
	}
}