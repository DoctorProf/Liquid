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
	Text speed;
	speed.setCharacterSize(16);
	speed.setFillColor(Color::White);
	speed.setFont(font);
	Texture waterTexture;
	waterTexture.loadFromFile("Resources/water.png");
	bool running = true;
	bool pause = false;

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
		}
		if (Mouse::isButtonPressed(Mouse::Left))
		{
			Vector2f mousePosition = Vector2f(Mouse::getPosition(window));
			if (mousePosition.x > 0 && mousePosition.x < physics::width && mousePosition.y > 0 && mousePosition.y < physics::height)
			{
				float radius = physics::radius;
				for (int i = 0; i < 10; ++i) 
				{
					Vector2f position = mousePosition + Vector2f(data::generateNumber(-radius, radius), data::generateNumber(-radius, radius));
					Particle* particle = new Particle(position, radius);
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
					physics::particles[i]->force += vector * (float)(pow(10, 4) / distance);
				}
			}
		}

		if (!pause)
		{
			//physics::applyGravity();
			physics::updateDerivatives(data::delta.asSeconds());
			//physics::collisionWithBoundaries();
			physics::checkCells();
			physics::findCollisionGrid();
			physics::resetDerivatives();
		}
		
		window.clear(Color::Black);

		//for (int i = 0; i < physics::particles.size(); ++i)
		//{
		//	float radius = physics::radius;
		//	Vector2f position = physics::particles[i]->position;
		//	Vector2u sizeTexture = waterTexture.getSize();
		//	Sprite sprite;
		//	sprite.setTexture(waterTexture);
		//	sprite.setScale(radius * 2 / sizeTexture.x, radius * 2 / sizeTexture.y);
		//	sprite.setOrigin(radius, radius);
		//	sprite.setPosition(position);
		//	window.draw(sprite);
		//	//speed.setPosition(physics::particles[i]->position + Vector2f(radius, -radius));
		//	//speed.setString(std::to_string(data::lengthVector(physics::particles[i]->velocity)));
		//	//window.draw(speed);
		//}
		water.clear();
		for (int i = 0; i < physics::particles.size(); i++)
		{
			Vertex point;
			point.position = physics::particles[i]->position;
			point.color = Color::White;
			water.append(point);
		}
		window.draw(water);
		window.setTitle(std::to_string(physics::particles.size()));
		window.display();
	}
	return 0;
}