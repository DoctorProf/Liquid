#include "../Headers/Data.hpp"

boost::random::mt19937 data::rng(time(NULL));
float data::PI = 3.141592;
Time data::delta = seconds(1.f / 20.f);

float data::distance(Vector2f vec1, Vector2f vec2)
{
	float dx = vec1.x - vec2.x;
	float dy = vec1.y - vec2.y;
	return std::sqrt(dx * dx + dy * dy);
}
int data::generateNumber(int min, int max)
{
	boost::random::uniform_real_distribution<> dist(min, max);
	return dist(rng);
}
float data::lengthVector(Vector2f vector)
{
	return sqrt(pow(vector.x, 2) + pow(vector.y, 2));
}