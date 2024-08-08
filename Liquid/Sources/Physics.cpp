#include "../Headers/Physics.hpp"

Vector2f physics::gravity = Vector2f(0, 1000);

void physics::applyGravity(Particle& particle)
{
	particle.setForce(particle.getForce() + gravity);
}
void physics::applyAirFriction(Particle& particle)
{
	float friction = 0.6f;
	particle.setForce(particle.getForce() - particle.getVelocity() * friction);
}
void physics::collisionWithBoundaries(Particle& particle, float& width, float& height)
{
	if (!data::collisionSquare(particle, Vector2f(0, 0), Vector2f(width, height)))
	{
        Vector2f position = particle.getPosition();
        Vector2f positionPointScreen;

        if (position.x > width) positionPointScreen = Vector2f(width, position.y);
        else if(position.x < 0) positionPointScreen = Vector2f(0, position.y);
        else if(position.y > height) positionPointScreen = Vector2f(position.x, height);
        else if (position.y < 0) positionPointScreen = Vector2f(position.x, 0);

        Vector2f direction = positionPointScreen - position;
        direction = direction / data::lengthVector(direction);
        float distance = data::distance(position , positionPointScreen);
        float radius = particle.getRadius();
        /*float c;
        if (distance > radius) c = distance - radius;
        else c = radius - distance;*/
        Vector2f p = (distance) * direction;

        particle.move(p);
	}
}
void physics::collisionParticles(Particle& particle, std::vector<Particle>& particles)
{
    Vector2f step;
    for (Particle otherParticle : particles)
    {
        if (particle == otherParticle) continue;

        Vector2f direction = particle.getPosition() - otherParticle.getPosition();
        direction = direction / data::lengthVector(direction);
        float distance = data::distance(particle.getPosition(), otherParticle.getPosition());

        double minDistance = particle.getRadius() + otherParticle.getRadius();
        if (distance < minDistance)
        {
            float c = minDistance - distance;
            Vector2f p = -c * direction;
            step += -p;
            //particle.move(-p);
            otherParticle.move(p);
        }
    }
    particle.move(step);
}