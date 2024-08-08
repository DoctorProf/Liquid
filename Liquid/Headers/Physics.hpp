#pragma once
#include <iostream>
#include "Data.hpp"

namespace physics 
{
	extern Vector2f gravity;
	void applyGravity(Particle& particle);
	void applyAirFriction(Particle& particle);
	//void satisfyConstraints(Particle& particle, std::vector<Particle>& particles);
	void collisionWithBoundaries(Particle& particle, float& width, float& height);
	void collisionParticles(Particle& particle, std::vector<Particle>& particles);
}