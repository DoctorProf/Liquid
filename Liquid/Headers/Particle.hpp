#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Particle 
{
public:
	Particle(Vector2f position, float radius);
	void setRadius(float radius);
	float getRadius();
	void setPosition(Vector2f position);
	Vector2f getPosition();
	void setForce(Vector2f force);
	Vector2f getForce();
	void setVelocity(Vector2f velocity);
	Vector2f getVelocity();
	void updateDerivatives(float deltaTime);
	void resetDerivatives(float deltaTime);
	void move(Vector2f vector);
	void setBlock(bool block);
	bool operator==(const Particle& other) const;
protected:
	Vector2f position;
	Vector2f positionPrevious;
	Vector2f force;
	Vector2f velocity;
	float radius;
	bool block;
};