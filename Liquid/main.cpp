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
void simulation(std::vector<Particle>& water, bool& running, bool& pause, float& width, float& height)
{
	Clock logic;
	Time accumulate = Time::Zero;

	while (running) 
	{
		accumulate += logic.restart();
		if (accumulate >= data::delta)
		{
			accumulate -= data::delta;
			if (!pause)
			{
				physics::applyGravity(water);
				physics::updateDerivatives(water);
				physics::collisionWithBoundaries(water, width, height);
				physics::collisionParticles(water);
				physics::resetDerivatives(water);
			}
		}
	}
}
int main()
{
	ContextSettings contextSettings;
	//contextSettings.antialiasingLevel = 1;
	//VideoMode videoMode = VideoMode::getDesktopMode();
	VideoMode videoMode = VideoMode(600, 600);
	RenderWindow window(videoMode, "", Style::Default, contextSettings);
	//window.setVerticalSyncEnabled(true);
	//Font font;
	//font.loadFromFile("C:/Windows/Fonts/Arial.ttf");
	Texture waterTexture;
	waterTexture.loadFromFile("Resources/water.png");
	Clock logic;
	Time accumulate = Time::Zero;
	bool running = true;
	bool pause = false;

	float width = videoMode.width;
	float height = videoMode.height;

	std::vector<Particle> water;

	std::thread threadSumulation(&simulation, std::ref(water), std::ref(running), std::ref(pause), std::ref(width), std::ref(height));
	threadSumulation.detach();

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
		if (accumulate >= data::delta)
		{
			accumulate -= data::delta;
			if (Mouse::isButtonPressed(Mouse::Left))
			{
				Vector2f mousePosition = Vector2f(Mouse::getPosition(window));
				if (mousePosition.x > 0 && mousePosition.x < width && mousePosition.y > 0 && mousePosition.y < height) 
				{
					int radius = 10;
					Vector2f position = mousePosition + Vector2f(data::generateNumber(-radius, radius), data::generateNumber(-radius, radius));
					Water particle(position, 10.f);
					water.push_back(particle);
				}
			}
			if (Mouse::isButtonPressed(Mouse::Right))
			{
				Vector2f mousePosition = Vector2f(Mouse::getPosition(window));
				if (mousePosition.x > 0 && mousePosition.x < width && mousePosition.y > 0 && mousePosition.y < height)
				{
					Concurrency::parallel_for(0, (int)water.size(), [&](int i)
					{
						Vector2f vector = water[i].position - mousePosition;
						vector = vector / data::lengthVector(vector);
						float distance = data::distance(mousePosition, water[i].position);
						water[i].force += vector * (float)(pow(10, 5) / distance);
					});
				}
			}
		}
		
		window.clear(Color::Black);
		window.draw(createGrid(10, width, height));

		for (size_t i = 0; i < water.size(); i++)
		{
			float radius = water[i].radius;
			Vector2f position = water[i].position;
			Vector2u sizeTexture = waterTexture.getSize();
			Sprite sprite;
			sprite.setTexture(waterTexture);
			sprite.setScale(radius / sizeTexture.x, radius / sizeTexture.y);
			sprite.setOrigin(radius / 2, radius / 2);
			sprite.setPosition(position);
			window.draw(sprite);
		}
		window.setTitle(std::to_string(water.size()));
		window.display();
	}
	return 0;
}