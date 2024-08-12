#pragma once
#include <iostream>
#include "Data.hpp"
#include "Grid.hpp"
#include <thread>
#include <ppl.h>

namespace physics 
{
	extern Vector2f gravity;
	extern std::vector<Particle*> particles;
	extern float width;
	extern float height;
	extern float radius;
	extern Grid grid;

	void applyGravity();
	void updateDerivatives(float delta);
	void collisionWithBoundaries();
	void resetDerivatives();

	bool collide(int firstIndex, int secondIndex);
	void solveCollide(int firstIndex, int secondIndex);
	void checkCells();
	void findCollisionGrid();
	void checkCellsCollision(Cell*& cell1, Cell*& cell2);
	//void threadFindCollision();
}