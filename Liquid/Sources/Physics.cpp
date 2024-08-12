#include "../Headers/Physics.hpp"

Vector2f physics::gravity = Vector2f(0, 10);
std::vector<Particle*> physics::particles;
float physics::width = VideoMode::getDesktopMode().width;
float physics::height = VideoMode::getDesktopMode().height;
float physics::radius = 5.f;
Grid physics::grid = Grid(width / (radius * 2), height / (radius * 2));

void physics::applyGravity()
{
    for (int i = 0; i < particles.size(); ++i)
    {
        particles[i]->force += gravity;
    }
}
void physics::updateDerivatives(float delta)
{
    for (int i = 0; i < particles.size(); ++i)
    {
        particles[i]->force += gravity;
        particles[i]->force -= particles[i]->velocity * 0.1f;
        particles[i]->updateDerivatives(delta);

        float margin = particles[i]->radius * 2;
        if (particles[i]->position.x > width - margin) particles[i]->position.x = width - margin;
        else if (particles[i]->position.x < margin) particles[i]->position.x = margin;
        if (particles[i]->position.y > height - margin)  particles[i]->position.y = height - margin;
        else if (particles[i]->position.y < margin) particles[i]->position.y = margin;
    }
}
void physics::collisionWithBoundaries()
{
    for (int i = 0; i < particles.size(); ++i)
    {
        float margin = particles[i]->radius * 2;
        if (particles[i]->position.x > width - margin) particles[i]->position.x = width - margin;
        else if (particles[i]->position.x < margin) particles[i]->position.x = margin;
        if (particles[i]->position.y > height - margin)  particles[i]->position.y = height - margin;
        else if (particles[i]->position.y < margin) particles[i]->position.y = margin;
    }
}
void physics::resetDerivatives()
{
    for (int i = 0; i < particles.size(); ++i)
    {
        particles[i]->resetDerivatives(data::delta.asSeconds());
    }
}
bool physics::collide(int firstIndex, int secondIndex)
{
    if (firstIndex == secondIndex) return false;
    return data::distance(particles[firstIndex]->position, particles[secondIndex]->position) < particles[firstIndex]->radius + particles[secondIndex]->radius;
}
void physics::solveCollide(int firstIndex, int secondIndex)
{
    Vector2f direction = particles[firstIndex]->position - particles[secondIndex]->position;
    direction = direction / data::lengthVector(direction);
    float distance = data::distance(particles[firstIndex]->position, particles[secondIndex]->position);

    double minDistance = particles[firstIndex]->radius + particles[secondIndex]->radius;
    float c = (minDistance - distance);
    Vector2f p = -c * direction * 0.05f;
    particles[firstIndex]->move(-p);
    particles[secondIndex]->move(p);
}
void physics::checkCells()
{
    grid.clear();
    for (size_t i = 0; i < particles.size(); ++i)
    {
        Vector2i position = Vector2i(particles[i]->position / (particles[i]->radius * 2));
        if (position.x >= 0 && position.x < grid.width && position.y >= 0 && position.y < grid.height) grid.cells[position.y][position.x]->particlesIndex.push_back(i);
    }
}
void physics::checkCellsCollision(Cell*& cell1, Cell*& cell2)
{
    for (int& particleIndex : cell1->particlesIndex)
    {
        for (int& otherParticleIndex : cell2->particlesIndex)
        {
            if (particleIndex != otherParticleIndex)
            {
                if (collide(particleIndex, otherParticleIndex)) 
                {
                    solveCollide(particleIndex, otherParticleIndex);
                }
            }
        }
    }
}
//void physics::findCollisionGrid()
//{
//    for (int i = 0; i < grid.height; ++i)
//    {
//        for (int j = 0; j < grid.width; ++j)
//        {
//            auto& currentCell = grid.cells[i][j];
//            for (int di = -1; di <= 1; ++di)
//            {
//                for (int dj = -1; dj <= 1; ++dj)
//                {
//                    if ((i + di < 0 || i + di > grid.height - 1) || (j + dj < 0 || j + dj > grid.width - 1)) continue;
//                    auto& otherCell = grid.cells[i + di][j + dj];
//                    checkCellsCollision(currentCell, otherCell);
//                }
//            }
//        }
//    }
//}
void physics::findCollisionGrid()
{
    Concurrency::parallel_for(0, (int)grid.height, [&](int i)
        {
            Concurrency::parallel_for(0, (int)grid.width, [&](int j)
                {
                    auto& currentCell = grid.cells[i][j];
                    for (int di = -1; di <= 1; ++di)
                    {
                        for (int dj = -1; dj <= 1; ++dj)
                        {
                            if ((i + di < 0 || i + di > grid.height - 1) || (j + dj < 0 || j + dj > grid.width - 1)) continue;
                            auto& otherCell = grid.cells[i + di][j + dj];
                            checkCellsCollision(currentCell, otherCell);
                        }
                    }
                });
        });
}