#include "../Headers/Particle.hpp"

Particle::Particle(Vector2f position)
{
	this->position = position;
	this->positionPrevious = position;
	this->force = Vector2f(0, 0);
	this->velocity = Vector2f(0, 0);
}
void Particle::updateDerivatives(float deltaTime)
{
	positionPrevious = position;
	velocity += force * deltaTime;
	position += velocity * deltaTime;
}
void Particle::resetDerivatives(float deltaTime)
{
	velocity = (position - positionPrevious) / deltaTime;
	force = Vector2f(0, 0);
}
void Particle::move(Vector2f vector)
{
	position += vector;
}
bool Particle::operator==(const Particle& other) const
{
	return position == other.position;
}
bool Particle::operator!=(const Particle& other) const
{
	return position != other.position;
}