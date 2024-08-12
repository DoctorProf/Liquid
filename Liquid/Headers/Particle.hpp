#pragma once
#include "Data.hpp"


class Particle 
{
public:
	Particle(Vector2f position, float radius);
	void updateDerivatives(float deltaTime);
	void resetDerivatives(float deltaTime);
	void move(Vector2f vector);
	bool operator==(const Particle& other) const;
	bool operator!=(const Particle& other) const;

	Vector2f position;
	Vector2f positionPrevious;
	Vector2f force;
	Vector2f velocity;
	float radius;
};