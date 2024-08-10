#pragma once
#include <iostream>
#include "Data.hpp"
#include "Grid.hpp"

namespace physics 
{
	extern Vector2f gravity;
	extern std::vector<Particle*> particles;
	extern float width;
	extern float height;

	void applyGravity();
	void updateDerivatives(float delta);
	void collisionWithBoundaries();
	void resetDerivatives();

	bool collide(int firstIndex, int secondIndex);
	void solveCollide(int firstIndex, int secondIndex);

	void checkCells(Grid& grid);
	void findCollisionGrid(Grid& grid);
	void checkCellsCollision(Cell*& cell1, Cell*& cell2);
	void simulation(Grid& grid);
}