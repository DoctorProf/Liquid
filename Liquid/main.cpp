#include <iostream>
#include "Headers/Physics.hpp"
#include <SFML/GpuPreference.hpp>

#define SFML_DEFINE_DISCRETE_GPU_PREFERENCE

int main()
{
	VideoMode videoMode = VideoMode::getDesktopMode();
	RenderWindow window(videoMode, "", Style::Fullscreen);

	Font font;
	font.loadFromFile("C://Windows/Fonts/Arial.ttf");
	Text countParticles;
	countParticles.setCharacterSize(16);
	countParticles.setFillColor(Color::White);
	countParticles.setFont(font);
	countParticles.setPosition(10, 10);

	bool running = true;
	bool pause = false;
	bool spawn = true;
	bool draw = false;
	Vector2f lastMousePosition;

	VertexArray water;
	water.setPrimitiveType(Points);

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
			if (event.type == Event::KeyPressed && event.key.code == Keyboard::S)
			{
				spawn = !spawn;
			}
			if (event.type == Event::KeyPressed && event.key.code == Keyboard::D)
			{
				physics::materials.clear();
			}
			if (event.type == Event::MouseButtonPressed && event.key.code == Mouse::Left && !spawn)
			{
				Vector2f mousePosition;
				if (!draw) 
				{
					mousePosition = Vector2f(Mouse::getPosition(window));
					lastMousePosition = mousePosition;
					draw = true;
				}
				else 
				{
					mousePosition = Vector2f(Mouse::getPosition(window));
					int count = data::distance(mousePosition, lastMousePosition) / (physics::radius);
					Vector2f direction = mousePosition - lastMousePosition;
					direction /= data::lengthVector(direction);
					for (int i = 0; i < count; ++i) 
					{
						physics::materials.push_back(new Material(lastMousePosition));
						lastMousePosition += physics::radius * direction;
					}
					draw = false;
				}
			}
		}
		if (Mouse::isButtonPressed(Mouse::Left) && spawn)
		{
			Vector2f mousePosition = Vector2f(Mouse::getPosition(window));
			if (mousePosition.x > 0 && mousePosition.x < physics::width && mousePosition.y > 0 && mousePosition.y < physics::height)
			{
				float radius = physics::radius;
				for (int i = 0; i < 2; ++i) 
				{
					Vector2f position = mousePosition + Vector2f(data::generateNumber(-radius, radius), data::generateNumber(-radius, radius));
					Particle* particle = new Particle(position);
					physics::particles.push_back(particle);
				}
			}
		}
		if (Mouse::isButtonPressed(Mouse::Right))
		{
			Vector2f mousePosition = Vector2f(Mouse::getPosition(window));
			if (mousePosition.x > 0 && mousePosition.x < physics::width && mousePosition.y > 0 && mousePosition.y < physics::height)
			{
				for (int i = 0; i < physics::particles.size(); ++i)
				{
					Vector2f vector = physics::particles[i]->position - mousePosition;
					vector = vector / data::lengthVector(vector);
					float distance = data::distance(mousePosition, physics::particles[i]->position);
					physics::particles[i]->force += vector * (float)(pow(10, 3) / distance);
				}
			}
		}

		if (!pause)
		{
			physics::updateDerivatives(data::delta.asSeconds());
			physics::checkCells();
			physics::findCollisionGrid();
			physics::findCollisionMaterial();
			physics::resetDerivatives();
		}
		
		window.clear(Color::Black);
		for (int i = 0; i < physics::materials.size(); ++i)
		{
			float radius = physics::radius;
			CircleShape material(radius);
			material.setFillColor(Color(128, 128, 128));
			material.setOrigin(radius, radius);
			material.setPosition(physics::materials[i]->position);
			window.draw(material);
		}
		water.clear();
		for (int i = 0; i < physics::particles.size(); ++i)
		{
			Vertex point;
			point.position = physics::particles[i]->position;
			point.color = Color::White;
			water.append(point);
		}
		window.draw(water);
		
		countParticles.setString(std::to_string(physics::particles.size()));
		window.draw(countParticles);
		window.display();
	}
	return 0;
}