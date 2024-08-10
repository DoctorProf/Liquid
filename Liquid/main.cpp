#include <iostream>
#include "Headers/Physics.hpp"
#include <thread>
#include <SFML/GpuPreference.hpp>

#define SFML_DEFINE_DISCRETE_GPU_PREFERENCE

VertexArray createGrid(int radius, float& width, float& height)
{
	VertexArray grid;
	grid.setPrimitiveType(Lines);

	int windowX = width;
	int windowY = height;
	int radiusCell = radius * 2;

	for (int i = 0; i <= windowX; i += radiusCell)
	{
		Vertex point;
		point.color = Color::Color(40, 40, 40);

		point.position = Vector2f(i, 0);

		grid.append(point);

		point.position = Vector2f(i, windowY);

		grid.append(point);
	}
	for (int i = 0; i <= windowY; i += radiusCell)
	{
		Vertex point;
		point.color = Color::Color(40, 40, 40);

		point.position = Vector2f(0, i);

		grid.append(point);

		point.position = Vector2f(windowX, i);

		grid.append(point);
	}
	return grid;
}
//void simulation(std::vector<Particle*>& particles, Grid& grid, bool& running, bool& pause, float& width, float& height)
//{
//	Clock logic;
//	Time accumulate = Time::Zero;
//
//	while (running) 
//	{
//		accumulate += logic.restart();
//		if (accumulate >= data::delta)
//		{
//			accumulate -= data::delta;
//			if (!pause)
//			{
//				physics::applyGravity();
//				physics::updateDerivatives();
//				physics::collisionWithBoundaries(width, height);
//				physics::checkCells(grid);
//				physics::findCollisionGrid(grid);
//				physics::resetDerivatives();
//			}
//		}
//	}
//}
int main()
{
	VideoMode videoMode = VideoMode::getDesktopMode();
	RenderWindow window(videoMode, "", Style::Fullscreen);

	Texture waterTexture;
	waterTexture.loadFromFile("Resources/water.png");
	bool running = true;
	bool pause = false;

	physics::width = videoMode.width;
	physics::height = videoMode.height;

	float radius = 5.f;

	Grid grid(physics::width / (radius * 2), physics::height / (radius * 2), radius * 2);

	//std::thread threadSumulation(&simulation, std::ref(particles), std::ref(grid), std::ref(running), std::ref(pause), std::ref(width), std::ref(height));
	//threadSumulation.detach();

	while (running) 
	{
		Event event;
		while (window.pollEvent(event)) 
		{
			if (event.type == Event::Closed) 
			{
				running = false;
			}
			if (event.type == Event::KeyPressed && event.key.code == Keyboard::C) 
			{
				physics::particles.clear();
			}
			if (event.type == Event::KeyPressed && event.key.code == Keyboard::Space)
			{
				pause = !pause;
			}
			if (event.type == Event::KeyPressed && event.key.code == Keyboard::G)
			{
				physics::gravity = -physics::gravity;
			}
		}
		if (Mouse::isButtonPressed(Mouse::Left))
		{
			Vector2f mousePosition = Vector2f(Mouse::getPosition(window));
			if (mousePosition.x > 0 && mousePosition.x < physics::width && mousePosition.y > 0 && mousePosition.y < physics::height)
			{
				Vector2f position = mousePosition + Vector2f(data::generateNumber(-radius, radius), data::generateNumber(-radius, radius));
				Particle* particle = new Particle(position, radius);
				physics::particles.push_back(particle);
;
			}
		}
		if (Mouse::isButtonPressed(Mouse::Right))
		{
			Vector2f mousePosition = Vector2f(Mouse::getPosition(window));
			if (mousePosition.x > 0 && mousePosition.x < physics::width && mousePosition.y > 0 && mousePosition.y < physics::height)
			{
				for (int i = 0; i < physics::particles.size(); i++)
				{
					Vector2f vector = physics::particles[i]->position - mousePosition;
					vector = vector / data::lengthVector(vector);
					float distance = data::distance(mousePosition, physics::particles[i]->position);
					physics::particles[i]->force += vector * (float)(pow(10, 4) / distance);
				}
			}
		}
		
		if (!pause)
		{
			physics::applyGravity();
			physics::updateDerivatives(data::delta.asSeconds());
			physics::collisionWithBoundaries();
			physics::checkCells(grid);
			physics::findCollisionGrid(grid);
			physics::resetDerivatives();
		}
		
		window.clear(Color::Black);
		//window.draw(createGrid(radius, physics::width, physics::height));

		for (int i = 0; i < physics::particles.size(); i++)
		{
			Vector2f position = physics::particles[i]->position;
			Vector2u sizeTexture = waterTexture.getSize();
			Sprite sprite;
			sprite.setTexture(waterTexture);
			sprite.setScale(radius * 2 / sizeTexture.x, radius * 2 / sizeTexture.y);
			sprite.setOrigin(radius, radius);
			sprite.setPosition(position);
			window.draw(sprite);
		}
		window.setTitle(std::to_string(physics::particles.size()));
		window.display();
	}
	return 0;
}