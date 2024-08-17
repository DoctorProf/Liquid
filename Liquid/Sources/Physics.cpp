#include "../Headers/Physics.hpp"

Vector2f physics::gravity = Vector2f(0, 5);
std::vector<Particle*> physics::particles;
float physics::width = VideoMode::getDesktopMode().width;
float physics::height = VideoMode::getDesktopMode().height;
float physics::radius = 4.f;
Grid physics::grid = Grid(width / (radius * 2), height / (radius * 2));
std::vector<Material*> physics::materials;

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

        float margin = radius * 2;
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
        float margin = radius * 2;
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
bool physics::collide(Vector2f positionFirst, Vector2f positionSecond)
{
    if (positionFirst == positionSecond) return false;
    return data::distance(positionFirst, positionSecond) < 2.f * radius;
}
void physics::solveCollide(int firstIndex, int secondIndex)
{
    Vector2f direction = particles[firstIndex]->position - particles[secondIndex]->position;
    direction /= data::lengthVector(direction);
    float distance = data::distance(particles[firstIndex]->position, particles[secondIndex]->position);

    float minDistance = radius * 2.f;
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
        Vector2i position = Vector2i(particles[i]->position / (radius * 2));
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
                if (collide(particles[particleIndex]->position, particles[otherParticleIndex]->position))
                {
                    solveCollide(particleIndex, otherParticleIndex);
                }
            }
        }
    }
}
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
void checkCollisionParticlesMaterial() 
{

}
void physics::findCollisionMaterial()
{
    for (int i = 0; i < materials.size(); ++i)
    {
        Vector2f materialPosition = materials[i]->position;
        Vector2i cellPosition = Vector2i(materials[i]->position / (radius * 2));
        for (int di = -1; di <= 1; ++di)
        {
            for (int dj = -1; dj <= 1; ++dj)
            {
                if ((cellPosition.y + di < 0 || cellPosition.y + di > grid.height - 1) || (cellPosition.x + dj < 0 || cellPosition.x + dj > grid.width - 1)) continue;
                auto& currentCell = grid.cells[cellPosition.y + di][cellPosition.x + dj];

                for (int j = 0; j < currentCell->particlesIndex.size(); ++j)
                {
                    Vector2f partilcePosition = particles[currentCell->particlesIndex[j]]->position;
                    int particleIndex = currentCell->particlesIndex[j];
                    if (collide(materialPosition, partilcePosition))
                    {
                        Vector2f direction = materialPosition - partilcePosition;
                        direction /= data::lengthVector(direction);
                        float distance = data::distance(materialPosition, partilcePosition);

                        float minDistance = radius * 2.f;
                        float c = (minDistance - distance);
                        Vector2f p = -c * direction * 0.1f;
                        particles[particleIndex]->move(p);
                    }
                }
            }
        } 
    }
}