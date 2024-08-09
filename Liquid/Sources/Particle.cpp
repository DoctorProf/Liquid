#include "../Headers/Particle.hpp"

Particle::Particle(Vector2f position, float radius)
{
	this->position = position;
	this->positionPrevious = {};
	this->radius = radius;
	this->force = {};
	this->velocity = {};
}
void Particle::setRadius(float radius)
{
	this->radius = radius;
}
float Particle::getRadius()
{
	return radius;
}
void Particle::setPosition(Vector2f position)
{
	this->position = position;
}
Vector2f Particle::getPosition()
{
	return position;
}
void Particle::setForce(Vector2f force) 
{
	this->force = force;
}
Vector2f Particle::getForce()
{
	return force;
}
void Particle::setVelocity(Vector2f velocity)
{
	this->velocity = velocity;
}
Vector2f Particle::getVelocity()
{
	return velocity;
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