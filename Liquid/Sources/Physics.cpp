#include "../Headers/Physics.hpp"

Vector2f physics::gravity = Vector2f(0, 200);

void physics::applyGravity(std::vector<Particle>& particles)
{
    for (size_t i = 0; i < particles.size(); i++)
    {
        particles[i].setForce(particles[i].getForce() + gravity);
    }
}
void physics::applyAirFriction(std::vector<Particle>& particles)
{
    float friction = 0.6f;
    for (size_t i = 0; i < particles.size(); i++)
    {
        particles[i].setForce(particles[i].getForce() - particles[i].getVelocity() * friction);
    }
}
void physics::updateDerivatives(std::vector<Particle>& particles)
{
    for (size_t i = 0; i < particles.size(); i++)
    {
        particles[i].updateDerivatives(data::delta.asSeconds());
    }
}
void physics::collisionWithBoundaries(std::vector<Particle>& particles, float& width, float& height)
{
    for (size_t i = 0; i < particles.size(); i++)
    {
        if (!data::collisionSquare(particles[i], Vector2f(0, 0), Vector2f(width, height)))
        {
            Vector2f position = particles[i].getPosition();
            Vector2f positionPointScreen;

            if (position.x > width) positionPointScreen = Vector2f(width, position.y);
            else if (position.x < 0) positionPointScreen = Vector2f(0, position.y);
            else if (position.y > height) positionPointScreen = Vector2f(position.x, height);
            else if (position.y < 0) positionPointScreen = Vector2f(position.x, 0);

            Vector2f direction = positionPointScreen - position;
            direction = direction / data::lengthVector(direction);
            float distance = data::distance(position, positionPointScreen);
            float radius = particles[i].getRadius();
            Vector2f p = (distance)*direction * 0.5f;

            particles[i].move(p);
        }
    }
}
void physics::collisionParticles(std::vector<Particle>& particles)
{
    for (Particle& particle : particles)
    {
        Vector2f step;
        for (Particle& otherParticle : particles)
        {
            if (particle == otherParticle) continue;

            Vector2f direction = particle.getPosition() - otherParticle.getPosition();
            direction = direction / data::lengthVector(direction);
            float distance = data::distance(particle.getPosition(), otherParticle.getPosition());

            double minDistance = particle.getRadius() + otherParticle.getRadius();
            if (distance < minDistance)
            {
                float c = minDistance - distance;
                Vector2f p = -c * direction * 0.2f;
                step += -p;
                //particle.move(-p);
                otherParticle.move(p);
            }
        }
        particle.move(step);
    }
}
void physics::resetDerivatives(std::vector<Particle>& particles)
{
    for (size_t i = 0; i < particles.size(); i++)
    {
        particles[i].resetDerivatives(data::delta.asSeconds());
    }
}