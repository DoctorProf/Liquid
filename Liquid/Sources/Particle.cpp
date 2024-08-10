#include "../Headers/Particle.hpp"

Particle::Particle(Vector2f position, float radius)
{
	this->position = position;
	this->positionPrevious = position;
	this->radius = radius;
	this->force = Vector2f(0, 0);
	this->velocity = Vector2f(0, 0);
}
void Particle::updateDerivatives(float deltaTime)
{
	positionPrevious = position;
	velocity += force;
	position += velocity * deltaTime;
}
void Particle::resetDerivatives(float deltaTime)
{
	velocity = (position - positionPrevious) / deltaTime;
	force = Vector2f(0, 0);
}
void Particle::move(Vector2f vector)
{
	positionPrevious = position;
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