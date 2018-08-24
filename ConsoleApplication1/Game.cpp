// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "Game.h"
#include "Fireball.h"
#include <vector>
#define GLOBAL_COOLDOWN 0.5
sf::CircleShape Game::innercirclecreation() {
	auto innercirclecolor = sf::Color(112, 54, 14, 255);
	sf::CircleShape innercircle(200.f);
	innercircle.setFillColor(innercirclecolor);
	innercircle.setPointCount(60);
	return innercircle;
}


void Game::loadTextures()
{
	rh.load(Textures::Player, "C:/Users/frede/Desktop/Images/topdownzombie.png");
	rh.load(Textures::Fireball, "C:/Users/frede/Desktop/Images/fireball.png");
}

Game::Game() : viewPos(0,0), windowRect(viewPos.x, viewPos.y, windowWidth, windowHeight)
{
	
	loadTextures();
	sf::Clock globalCooldownClock;
	float elapsedtime = 0;
	Player player(rh);
	sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "SFML works!");
	sf::View view(windowRect);
	sf::CircleShape innercircle = innercirclecreation();
	innercircle.setPosition(view.getCenter().x - (innercircle.getGlobalBounds().width / 2), view.getCenter().y - (innercircle.getGlobalBounds().height / 2));
	sf::Vector2f playerPosition(view.getCenter().x, view.getCenter().y);
	player.setPosition(playerPosition);
	window.setView(view);
	window.setFramerateLimit(30);
	std::vector<Ability*> activeAbilities;
	activeAbilities.reserve(100000);

	//intersection test
	Player p2(rh);
	Fireball fb(rh,&p2);
	fb.setPosition(sf::Vector2f(400,30));
	fb.changeDirectionToPoint(playerPosition);
	fb.startMoving(playerPosition);
	activeAbilities.push_back(&fb);

	while (window.isOpen())
	{
		sf::Vector2f pointClicked = (sf::Vector2f) sf::Mouse::getPosition(window);
		sf::Event event;
		player.moveInDirection();
		player.handleHitStun(player.getMovementSpeed());
		player.setBoundingShape();
		player.stopAtDestination();
		sf::Shape* mrs = player.getBoundingShape(); 
		/*sf::CircleShape mrs;
		sf::Vector2f circlepos = sf::Vector2f(player.getSprite().getPosition().x - (player.getSprite().getTextureRect().width / 5 / 3.5), player.getSprite().getPosition().y - (player.getSprite().getTextureRect().height / 5 / 2.9));
		float circleradius = 20;
		mrs.setPosition(circlepos);
		
		mrs.setRadius(20);*/
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			if (player.getHitState() == Player::NOTHIT) {
				player.changeDirectionToPoint(pointClicked);
				player.startMoving(pointClicked);
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
				if (globalCooldownClock.getElapsedTime().asSeconds() > GLOBAL_COOLDOWN && player.getHitState() == Player::NOTHIT) {
					Fireball* fb = new Fireball(rh,&player);
					fb->setPosition(player.getSprite().getPosition());
					fb->changeDirectionToPoint(pointClicked);
					fb->startMoving(pointClicked);
					//Fix later
					for (auto i = 0; i < 2; i++) {
						fb->moveInDirection();
					}
					globalCooldownClock.restart();
					activeAbilities.push_back(fb);
				}
		}
		window.setView(view);
		window.draw(innercircle);
		//std::cout << activeAbilities.size();
		for (Ability* ability : activeAbilities) {
			if (windowRect.contains(ability->getSprite().getPosition())) {
				ability->setBoundingShape();
				sf::Shape *fbrs = ability->getBoundingShape();
			   if (CustomMath::circleIntersectionWithRotatedRectangle(mrs->getPosition(), player.getPlayerCircleRadius(),fbrs->getGlobalBounds(),fbrs->getRotation())) {
					sf::Vector2f unitVectorDirection = CustomMath::normalizeUnitVector(player.getSprite().getPosition(), ability->getSprite().getPosition());
					sf::Vector2f unitVectorOppositeDirection = sf::Vector2f(-unitVectorDirection.x, -unitVectorDirection.y);
					if (&player != ability->getOwner()) {
						player.isHit(unitVectorOppositeDirection);
					}
				}

				// CHECK INTERSECTION MED CIRKEL!
				ability->moveInDirection();
				window.draw(*fbrs);
				window.draw(ability->getSprite());
				//std::cout << a->getSprite().getPosition().y;
				//Husk!! mange abilities = mange checks. 
			}
		}
	//	std::cout << activeAbilities.size() << "\n";
		window.draw(*mrs);
		window.draw(player.getSprite());

		window.display();
		window.clear();
	}
	
}

