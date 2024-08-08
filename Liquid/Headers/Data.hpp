#pragma once
#include <boost/random.hpp>
#include "Particle.hpp"
#include "Water.hpp"

using namespace boost;

namespace data 
{
	extern random::mt19937 rng;
	extern float PI;

	float distance(Vector2f vec1, Vector2f vec2);
	int generateNumber(int min, int max);
	float lengthVector(Vector2f vector);
	float collisionSquare(Particle particle, Vector2f position, Vector2f size);
}