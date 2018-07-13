// ConsoleApplication1.cpp : Defines the entry point for the console application.
//


#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "CustomMath.h"
#include <iostream>
#include "Player.h"
#include "ResourceHolder.h"
#include "Textures.h"

sf::CircleShape innercirclecreation() {
	auto innercirclecolor = sf::Color(112, 54, 14, 255);
	sf::CircleShape innercircle(200.f);
	innercircle.setFillColor(innercirclecolor);
	innercircle.setPointCount(60);
	return innercircle;
}

int main()
{
	ResourceHolder <sf::Texture, Textures::ID> rh;
	Player player(rh); 
	sf::Vector2f viewPos(0, 0);
	sf::RenderWindow window(sf::VideoMode(800, 640), "SFML works!");
	sf::View view(sf::FloatRect(viewPos.x, viewPos.y, 800, 640));
	sf::CircleShape innercircle = innercirclecreation();
	innercircle.setPosition(view.getCenter().x - (innercircle.getGlobalBounds().width / 2), view.getCenter().y - (innercircle.getGlobalBounds().height / 2));
	sf::Vector2f playerPosition(view.getCenter().x, view.getCenter().y);
	player.setPosition(playerPosition);
	window.setView(view);
	window.setFramerateLimit(30);

	while (window.isOpen())
	{
		player.move();
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			player.changeDirection((sf::Vector2f) sf::Mouse::getPosition(window));
		}
		window.setView(view);
		//window.draw(background);
		window.draw(innercircle);
		window.draw(player.getSprite());
		window.display();
		window.clear();
	}
}

