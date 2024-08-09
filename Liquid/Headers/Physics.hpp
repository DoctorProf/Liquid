#pragma once
#include <iostream>
#include "Data.hpp"

namespace physics 
{
	extern Vector2f gravity;
	void applyGravity(std::vector<Particle>& particles);
	void applyAirFriction(std::vector<Particle>& particles);
	void updateDerivatives(std::vector<Particle>& particles);
	void collisionWithBoundaries(std::vector<Particle>& particles, float& width, float& height);
	void collisionParticles(std::vector<Particle>& particles);
	void resetDerivatives(std::vector<Particle>& particles);
}