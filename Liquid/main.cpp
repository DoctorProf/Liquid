#include <iostream>
#include "Headers/Physics.hpp"

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
int main()
{
	ContextSettings contextSettings;
	contextSettings.antialiasingLevel = 8;
	//VideoMode videoMode = VideoMode::getDesktopMode();
	VideoMode videoMode = VideoMode(600, 600);
	RenderWindow window(videoMode, "", Style::Default, contextSettings);

	Clock logic;
	Time accumulate = Time::Zero;
	Time delta = seconds(1.0f / 30.0f);
	bool running = true;
	bool pause = false;

	float width = videoMode.width;
	float height = videoMode.height;

	std::vector<Particle> water;

	Clock debug;

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
				water.clear();
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
		accumulate += logic.restart();
		if (accumulate >= delta) 
		{
			accumulate -= delta;
			if (Mouse::isButtonPressed(Mouse::Left))
			{
				Vector2f mousePosition = Vector2f(Mouse::getPosition(window));
				int radius = 20;
				Vector2f position = mousePosition + Vector2f(data::generateNumber(-radius, radius), data::generateNumber(-radius, radius));
				Water particle(position, 15.f);
				water.push_back(particle);
			}
			if (!pause) 
			{
				debug.restart();
				for (size_t i = 0; i < water.size(); i++)
				{
					physics::applyGravity(water[i]);
					physics::applyAirFriction(water[i]);
					water[i].updateDerivatives(delta.asSeconds());
					physics::collisionWithBoundaries(water[i], width, height);
					physics::collisionParticles(water[i], water);
					water[i].resetDerivatives(delta.asSeconds());
				}
				std::cout << water.size() << " Time " << debug.restart().asMilliseconds() << std::endl;
			}	
		}
		
		window.clear(Color::Black);
		window.draw(createGrid(10, width, height));

		for (size_t i = 0; i < water.size(); i++)
		{
			float radius = water[i].getRadius();
			Vector2f position = water[i].getPosition();
			CircleShape particleDraw(radius);
			particleDraw.setOrigin(radius, radius);
			particleDraw.setPosition(position);
			particleDraw.setFillColor(Color::Blue);
			window.draw(particleDraw);
		}
		window.setTitle(std::to_string(water.size()));
		window.display();
	}
	return 0;
}