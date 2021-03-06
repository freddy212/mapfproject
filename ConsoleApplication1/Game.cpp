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
	innercircle.setOrigin(sf::Vector2f(innercircle.getGlobalBounds().width / 2, (innercircle.getGlobalBounds().height / 2)));
	return innercircle;
}

void Game::loadFonts() {
	fh.load(Fonts::orangeJuice, "C:/Users/frede/Desktop/Fonts/orange juice 2.0.ttf");
}

void Game::loadTextures()
{
	rh.load(Textures::Player, "C:/Users/frede/Desktop/Images/topdownzombie.png");
	rh.load(Textures::Fireball, "C:/Users/frede/Desktop/Images/fireball.png");
	rh.load(Textures::Sartan, "C:/Users/frede/Desktop/Images/bandanabird.png");
}

Game::Game() : viewPos(0,0), windowRect(viewPos.x, viewPos.y, windowWidth, windowHeight)
{
	loadFonts();
	loadTextures();
	sf::Text healthText;
	healthText.setFont(fh.get(Fonts::orangeJuice));
	//healthText.setFillColor(sf::Color::Blue);
	healthText.setCharacterSize(30);
	sf::Clock globalCooldownClock;
	float elapsedtime = 0;
	Player player(rh,1);
	sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "SFML works!");
	sf::View view(windowRect);
	sf::CircleShape innercircle = innercirclecreation();
	//sf::View view(sf::FloatRect(windowWidth / 2, windowHeight / 2, windowWidth, windowHeight));
	window.setView(view);
	window.setFramerateLimit(30);
	//innercircle.setPosition(view.getCenter().x - (innercircle.getGlobalBounds().width / 2), view.getCenter().y - (innercircle.getGlobalBounds().height / 2));
	innercircle.setPosition(view.getCenter().x, view.getCenter().y);
	sf::Vector2f playerPosition(view.getCenter().x, view.getCenter().y);
	player.setPosition(playerPosition);
	//player.getBoundingShape()->setOrigin(player.getSprite().getOrigin());
	std::vector<Projectile*> activeAbilities;
	//activeAbilities.reserve(100000);

	//intersection test
	Sartan s(rh, 3);
	Player p2(rh,2);
	Fireball fb(rh,p2.getID());
	fb.setPosition(sf::Vector2f(400,30));
	fb.changeDirectionToPoint(playerPosition);
	fb.startMoving(playerPosition);
	activeAbilities.push_back(&fb);

	while (window.isOpen())
	{
		if (!CustomMath::circleContainsPoint(innercircle, player.getSprite().getPosition())) {
			player.outsideBounds();
		}
		view.setCenter(player.getSprite().getPosition());
		sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
		sf::Vector2f pointClicked = window.mapPixelToCoords(pixelPos);
		sf::Event event;
		player.moveInDirection();
		player.handleHitStun();
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
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && player.getHitState() == Player::NOTHIT) {
				player.changeDirectionToPoint(pointClicked);
				player.startMoving(pointClicked);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
				if (globalCooldownClock.getElapsedTime().asSeconds() > GLOBAL_COOLDOWN && player.getHitState() == Character::NOTHIT) {
					Fireball* fb = new Fireball(rh,player.getID());
					player.shootProjectile(fb, pointClicked);
					globalCooldownClock.restart();
					activeAbilities.push_back(fb);
				}
		}
		window.setView(view);
		window.draw(innercircle);
		//std::cout << activeAbilities.size();
		auto projectileit = std::begin(activeAbilities);
		while (projectileit != std::end(activeAbilities)) {
		//	if (windowRect.contains(ability->getSprite().getPosition())) {
				(*projectileit)->setBoundingShape();
				sf::Shape *fbrs = (*projectileit)->getBoundingShape();
			   if (CustomMath::circleIntersectionWithRotatedRectangle(sf::Vector2f(mrs->getPosition().x - player.getPlayerCircleRadius(), mrs->getPosition().y - player.getPlayerCircleRadius()), player.getPlayerCircleRadius(),fbrs->getGlobalBounds(),fbrs->getRotation())) {
					sf::Vector2f unitVectorDirection = CustomMath::normalizeUnitVector(player.getSprite().getPosition(), (*projectileit)->getSprite().getPosition());
					sf::Vector2f unitVectorOppositeDirection = sf::Vector2f(-unitVectorDirection.x, -unitVectorDirection.y);
					if (player.getID() != (*projectileit)->getOwnerID()) {
						player.isHit(unitVectorOppositeDirection);
						projectileit = activeAbilities.erase(projectileit);
						continue;
					}
				}

				// CHECK INTERSECTION MED CIRKEL!
				(*projectileit)->moveInDirection();
				window.draw(*fbrs);
				window.draw((*projectileit)->getSprite());
				//std::cout << (*projectileit)->getSprite().getPosition().y << "\n";
				projectileit++;
				//Husk!! mange abilities = mange checks. 
			//}
		}

		healthText.setString("Health :" + std::to_string(player.getHealth()));
		healthText.setPosition((sf::Vector2f) window.mapPixelToCoords(sf::Vector2i(0, 0)));
		window.draw(healthText);
		window.draw(*mrs);;
		window.draw(s.getSprite());
		window.draw(player.getSprite());
		window.draw(*(s.getBoundingShape()));

		window.display();
		window.clear();
	}
	
}

