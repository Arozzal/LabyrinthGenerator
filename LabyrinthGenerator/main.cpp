#include "DungeonMap.hpp"

#include <iostream>

float getFrameTime() {

	static sf::Clock clock;
	sf::Time time = clock.getElapsedTime();
	clock.restart();
	return time.asSeconds();
	
}


sf::Image generateFloorMetaData(sf::Image& img) {
	sf::Image newImg;
	newImg.create(img.getSize().x, img.getSize().y);

	for (int x = 0; x < img.getSize().x; x++) {
		for (int y = 0; y < img.getSize().y; y++) {
			int tilechanche = rand() % 1000;

			if (tilechanche > 100) {
				newImg.setPixel(x, y, sf::Color(0, 0, 0, 0));
			}
			else if (tilechanche > 20) {
				newImg.setPixel(x, y, sf::Color(0, 0, 0, 1));
			}
			else if (tilechanche > 5) {
				newImg.setPixel(x, y, sf::Color(0, 0, 0, 2));
			}
			else{
				newImg.setPixel(x, y, sf::Color(0, 0, 0, 3));
			}
		}
	}

	return newImg;
}




int main() {
	sf::RenderWindow window(sf::VideoMode(1080, 720), "W A S D Camera Scrolling", sf::Style::Default);
	DungeonMap dungeon;
	dungeon.DEAD_AND_REMOVING = 300;
	dungeon.CHEST_CHANCHE = 20;
	dungeon.DOOR_CHANCHE = 10;
	dungeon.ROOM_COUNT = 40;


	dungeon.generate();

	sf::Image map = dungeon.getImage();
	sf::Texture floortilesTex;
	floortilesTex.loadFromFile("tileset/floortile.png");
	sf::Sprite floortilesSpr;
	floortilesSpr.setTexture(floortilesTex);

	sf::Image metaMap = generateFloorMetaData(map);

	
	sf::Event event;
	sf::View view;
	float viewposx = 0;
	float viewposy = 0;
	float viewspeed = 128;
	
	while (true) {
		window.pollEvent(event);

		float elapsedTime = getFrameTime();


		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
			viewposx -= viewspeed * elapsedTime;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
			viewposx += viewspeed * elapsedTime;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
			viewposy -= viewspeed * elapsedTime;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
			viewposy += viewspeed * elapsedTime;
		}

		window.clear(sf::Color::Black);

		// Initialize the view to a rectangle located at (100, 100) and with a size of 400x200
		view.reset(sf::FloatRect(viewposx, viewposy, window.getSize().x / 4, window.getSize().y / 4));

		// Set its target viewport to be half of the window
		//view.setViewport(sf::FloatRect(0.f, 0.f, 0.5f, 1.f));
		// Apply it
		window.setView(view);
		// Render stuff

		for (int x = 0; x < map.getSize().x; x++) {
			for (int y = 0; y < map.getSize().y; y++) {
				if (map.getPixel(x, y) == sf::Color::Yellow || map.getPixel(x, y) == sf::Color::Blue) {
					
					int tilechanche = metaMap.getPixel(x, y).a;
					
					if (tilechanche == 0) {
						floortilesSpr.setTextureRect({ 0, 0, 16, 16 });
					}
					else if (tilechanche == 1) {
						floortilesSpr.setTextureRect({ 0, 16, 16, 16 });
					}
					else if (tilechanche == 2) {
						floortilesSpr.setTextureRect({ 0, 32, 16, 16 });
					}
					else {
						floortilesSpr.setTextureRect({ 0, 48, 16, 16 });
					}
					
					

					floortilesSpr.setPosition(x * 16, y * 16);
					window.draw(floortilesSpr);

					unsigned char pos = ((dungeon.getBlock(x, y - 1) != sf::Color::Yellow) << 0) |
										((dungeon.getBlock(x + 1, y) != sf::Color::Yellow) << 1) |
										((dungeon.getBlock(x, y + 1) != sf::Color::Yellow) << 2) |
										((dungeon.getBlock(x - 1, y) != sf::Color::Yellow) << 3);



					floortilesSpr.setPosition(x * 16, y * 16);
					window.draw(floortilesSpr);

				}

				else if (map.getPixel(x, y) == sf::Color::Cyan) {
					floortilesSpr.setTextureRect({ 16, 0, 16, 16 });
					floortilesSpr.setPosition(x * 16, y * 16);
					window.draw(floortilesSpr);
				}
				else if (map.getPixel(x, y) == sf::Color::Black) {
					
					floortilesSpr.setTextureRect({ 0, 0, 16, 16 });
					floortilesSpr.setPosition(x * 16, y * 16);
					window.draw(floortilesSpr);

					


					floortilesSpr.setTextureRect({ 32, 0, 16, 16 });
					
					


					floortilesSpr.setPosition(x * 16, y * 16);
					window.draw(floortilesSpr);
				}

			}
		}
			
		window.setView(window.getDefaultView());





		
		
		window.display();
	}
}
