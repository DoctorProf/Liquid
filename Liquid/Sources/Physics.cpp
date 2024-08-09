#include "../Headers/Physics.hpp"

Vector2f physics::gravity = Vector2f(0, 200);

void physics::applyGravity(std::vector<Particle>& particles)
{
    Concurrency::parallel_for(0, (int)particles.size(), [&](int i)
        {
            particles[i].force += gravity;
        });
}
//void physics::applyAirFriction(std::vector<Particle>& particles)
//{
//    float friction = 0.6f;
//    Concurrency::parallel_for(0, (int)particles.size(), [&](int i)
//        {
//            particles[i].force -= particles[i].velocity * friction;
//        });
//
//}
void physics::updateDerivatives(std::vector<Particle>& particles)
{
    Concurrency::parallel_for(0, (int)particles.size(), [&](int i)
        {
            particles[i].updateDerivatives(data::delta.asSeconds());
        });
}
void physics::collisionWithBoundaries(std::vector<Particle>& particles, float& width, float& height)
{
    Concurrency::parallel_for(0, (int)particles.size(), [&](int i)
        {
            if (!data::collisionSquare(particles[i], Vector2f(0, 0), Vector2f(width, height)))
            {
                Vector2f position = particles[i].position;
                Vector2f positionPointScreen;

                if (position.x > width) positionPointScreen = Vector2f(width, position.y);
                else if (position.x < 0) positionPointScreen = Vector2f(0, position.y);
                else if (position.y > height) positionPointScreen = Vector2f(position.x, height);
                else if (position.y < 0) positionPointScreen = Vector2f(position.x, 0);

                Vector2f direction = positionPointScreen - position;
                direction = direction / data::lengthVector(direction);
                float distance = data::distance(position, positionPointScreen);
                float radius = particles[i].radius;
                Vector2f p = (distance)*direction * 0.5f;

                particles[i].move(p);
            }
        });
}
void physics::collisionParticles(std::vector<Particle>& particles)
{
    Concurrency::parallel_for(0, (int)particles.size(), [&](int i)
        {
            Vector2f step;
            Concurrency::parallel_for(i + 1, (int)particles.size(), [&](int j)
                {
                    if (particles[i] != particles[j]) 
                    {
                        Vector2f direction = particles[i].position - particles[j].position;
                        direction = direction / data::lengthVector(direction);
                        float distance = data::distance(particles[i].position, particles[j].position);

                        double minDistance = particles[i].radius + particles[j].radius;
                        if (distance < minDistance)
                        {
                            float c = minDistance - distance;
                            Vector2f p = -c * direction * 0.2f;
                            step += -p;
                            particles[j].move(p);
                        }
                    }
                });
            particles[i].move(step);
        });
   /* for (size_t i = 0; i < particles.size(); i++)
    {
        Vector2f step;
        for (size_t j = i + 1; j < particles.size(); j++)
        {
            if (particles[i] == particles[j]) continue;
            Vector2f direction = particles[i].position - particles[j].position;
            direction = direction / data::lengthVector(direction);
            float distance = data::distance(particles[i].position, particles[j].position);

            double minDistance = particles[i].radius + particles[j].radius;
            if (distance < minDistance)
            {
                float c = minDistance - distance;
                Vector2f p = -c * direction * 0.2f;
                step += -p;
                particles[j].move(p);
            }
        }
        particles[i].move(step);
    }*/
}
void physics::resetDerivatives(std::vector<Particle>& particles)
{
    Concurrency::parallel_for(0, (int)particles.size(), [&](int i)
        {
            particles[i].resetDerivatives(data::delta.asSeconds());
        });
}