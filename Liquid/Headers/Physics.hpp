#pragma once
#include <iostream>
#include "Data.hpp"
#include "Grid.hpp"
#include <thread>
#include <ppl.h>
#include "Material.hpp"

namespace physics 
{
	extern Vector2f gravity;
	extern std::vector<Particle*> particles;
	extern float width;
	extern float height;
	extern float radius;
	extern Grid grid;
	extern std::vector<Material*> materials;

	void applyGravity();
	void updateDerivatives(float delta);
	void collisionWithBoundaries();
	void resetDerivatives();

	bool collide(Vector2f positionFirst, Vector2f positionSecond);
	void solveCollide(int firstIndex, int secondIndex);
	void checkCells();
	void findCollisionGrid();
	void checkCellsCollision(Cell*& cell1, Cell*& cell2);
	void findCollisionMaterial();
}