#pragma once
#include <boost/random.hpp>
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace boost;

namespace data 
{
	extern random::mt19937 rng;
	extern float PI;
	extern Time delta;

	float distance(Vector2f vec1, Vector2f vec2);
	float generateNumber(float min, float max);
	float lengthVector(Vector2f vector);
}