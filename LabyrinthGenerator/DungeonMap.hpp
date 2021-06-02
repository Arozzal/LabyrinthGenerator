#pragma once
#include <SFML\Graphics.hpp>
#include <stack>


class DungeonMap {



	int PXS = 32;
	int LSX = 1920 * 0.75 / 8;
	int LSY = 1080 * 0.75 / 8;
	int PXSX = PXS * LSX;
	int PXSY = PXS * LSY;


public:

	int ROOM_COUNT = 100;
	int DOOR_CHANCHE = 100;
	int CHEST_CHANCHE = 20;
	int DEAD_AND_REMOVING = 4;

	int spawnx = 0;
	int spawny = 0;

private:

	sf::Color* laby;
	sf::Image canvas;

	enum DIR {
		LEFT,
		RIGHT,
		UP,
		DOWN
	};

public:

	void setBlock(int x, int y, sf::Color col) {
		if (x >= LSX || y >= LSY || x < 0 || y < 0)
			return;

		laby[y * LSX + x] = col;
		canvas.setPixel(x, y, col);
	}

	sf::Color getBlock(int x, int y) {
		if (x < 0 || y < 0 || x >= LSX || y >= LSY)
			return sf::Color::Transparent;

		return laby[y * LSX + x];
	}

	DungeonMap() {
		laby = new sf::Color[LSX * LSY];
		for (int i = 0; i < LSX * LSY; i++) {
			laby[i] = sf::Color::Black;
		}
	}

	sf::Image getImage() {
		return canvas;
	}


	int generate()
	{

		srand(time(0));
		canvas.create(LSX, LSY);


		///////////////////
		//Room Generation//
		///////////////////
		for (int i = 0; i < ROOM_COUNT; i++) {
			int x = rand() % LSX;
			int y = rand() % LSY;
			if (x % 2 == 0)
				x++;
			if (y % 2 == 0)
				y++;

			sf::Color color(rand() % 255, rand() % 255, rand() % 255, 255);
			int xsize = (rand() % 13) + 5;
			int ysize = (rand() % 13) + 5;

			if (xsize % 2 == 0)
				xsize++;

			if (ysize % 2 == 0)
				ysize++;

			for (int xx = x; xx < x + xsize; xx++) {
				if (xx + 1 > LSX) {
					continue;
				}
				for (int yy = y; yy < y + ysize; yy++) {
					if (yy + 1 > LSY) {
						continue;
					}
					setBlock(xx, yy, color);
					if (xx == x || yy == y || yy == y + ysize - 1 || xx == x + xsize - 1 || xx == 0 || yy == 0 || xx == LSX - 1 || yy == LSY - 1)
						setBlock(xx, yy, sf::Color::Black);
				}
			}
		}


		////////////////////////
		//Labyrinth Generation//
		////////////////////////
		int iterCounter = 0;

	looping:
		int posx, posy;
		int counter = 0;

		bool found = false;
		bool firstIter = true;
		std::stack<int> lastPoses;
		lastPoses.push(1);
		lastPoses.push(1);
		while (true) {

			counter++;
			posx = (rand() % (LSX - 1)) + 1;
			posy = (rand() % (LSY - 1)) + 1;
			if (posx % 2 != 0)
				posx--;
			if (posy % 2 != 0)
				posy--;



			if (getBlock(posx, posy) == sf::Color::Black) {
				while (true) {
					setBlock(posx, posy, sf::Color::Yellow);
					std::vector<DIR> dirselect;

					if (firstIter) {
						firstIter = false;
						spawnx = posx;
						spawny = posy;
					}

					if (getBlock(posx + 2, posy) == sf::Color::Black && getBlock(posx + 1, posy) == sf::Color::Black) { dirselect.push_back(RIGHT); }
					if (getBlock(posx - 2, posy) == sf::Color::Black && getBlock(posx - 1, posy) == sf::Color::Black) { dirselect.push_back(LEFT); }
					if (getBlock(posx, posy + 2) == sf::Color::Black && getBlock(posx, posy + 1) == sf::Color::Black) { dirselect.push_back(DOWN); }
					if (getBlock(posx, posy - 2) == sf::Color::Black && getBlock(posx, posy - 1) == sf::Color::Black) { dirselect.push_back(UP); }

					if (lastPoses.size() < 2) {
						iterCounter++;
						lastPoses.empty();
						if (iterCounter >= 10000) {
							goto end;
						}

						goto looping;
					}

					if (dirselect.size() <= 0) {
						posy = lastPoses.top();
						lastPoses.pop();
						posx = lastPoses.top();
						lastPoses.pop();
						dirselect.clear();
						continue;
					}

					lastPoses.push(posx);
					lastPoses.push(posy);

					int dir = rand() % dirselect.size();

					if (dirselect.size() == 1)
						dir = 0;



					switch (dirselect[dir]) {
					case RIGHT:
						setBlock(posx + 1, posy, sf::Color::Yellow);
						posx += 2;
						break;
					case LEFT:
						setBlock(posx - 1, posy, sf::Color::Yellow);
						posx -= 2;
						break;
					case DOWN:
						setBlock(posx, posy + 1, sf::Color::Yellow);
						posy += 2;
						break;
					case UP:
						setBlock(posx, posy - 1, sf::Color::Yellow);
						posy -= 2;
						break;
					}

				}
			}
			if (counter > 1000 || found)
				break;
		}
	end:

		//////////////////
		//Door Placement//
		//////////////////
		std::vector<int> blocksaves;
		for (int x = 0; x < LSX; x++) {
			for (int y = 0; y < LSY; y++) {
				if (getBlock(x, y) == sf::Color::Black) {
					if (getBlock(x - 1, y) == sf::Color::Transparent || getBlock(x + 1, y) == sf::Color::Transparent || getBlock(x, y - 1) == sf::Color::Transparent || getBlock(x, y + 1) == sf::Color::Transparent) {
						continue;
					}
					if (getBlock(x - 1, y) != sf::Color::Black && getBlock(x - 1, y) != sf::Color::Yellow ||
						getBlock(x + 1, y) != sf::Color::Black && getBlock(x + 1, y) != sf::Color::Yellow ||
						getBlock(x, y + 1) != sf::Color::Black && getBlock(x, y + 1) != sf::Color::Yellow ||
						getBlock(x, y - 1) != sf::Color::Black && getBlock(x, y - 1) != sf::Color::Yellow) {
						if (getBlock(x - 1, y) != getBlock(x + 1, y) || getBlock(x, y - 1) != getBlock(x, y + 1)) {
							if (((getBlock(x + 1, y) == sf::Color::Black && getBlock(x - 1, y) == sf::Color::Black) && (
								getBlock(x, y + 1) != sf::Color::Black && getBlock(x, y - 1) != sf::Color::Black)) ||
								((getBlock(x + 1, y) != sf::Color::Black && getBlock(x - 1, y) != sf::Color::Black) && (
									getBlock(x, y + 1) == sf::Color::Black && getBlock(x, y - 1) == sf::Color::Black))) {

								blocksaves.push_back(x);
								blocksaves.push_back(y);
							}
						}
					}



				}
			}
		}

		for (int i = 0; i < blocksaves.size(); i += 2) {
			int x = blocksaves[i];
			int y = blocksaves[i + 1];

			if (rand() % DOOR_CHANCHE == 1)
				if (getBlock(x - 1, y) != sf::Color::Magenta &&
					getBlock(x + 1, y) != sf::Color::Magenta &&
					getBlock(x, y + 1) != sf::Color::Magenta &&
					getBlock(x, y - 1) != sf::Color::Magenta)
					setBlock(blocksaves[i], blocksaves[i + 1], sf::Color::Red);
		}

		////////////////////
		//Chest Generation//
		////////////////////
		for (int x = 0; x < LSX; x++) {
			for (int y = 0; y < LSY; y++) {
				sf::Color col = getBlock(x, y);
				if (getBlock(x - 1, y) == col &&
					getBlock(x + 1, y) == col &&
					getBlock(x, y + 1) == col &&
					getBlock(x, y - 1) == col)
					if (rand() % CHEST_CHANCHE == 1)
						setBlock(x, y, sf::Color::Blue);
			}
		}

		//////////////////////
		//Dead End Removeing//
		//////////////////////
		for (int i = 0; i < DEAD_AND_REMOVING; i++) {
			for (int x = 0; x < LSX; x++) {
				for (int y = 0; y < LSY; y++) {
					int yellowCount = 0;
					int blackCount = 0;
					if (getBlock(x + 1, y) == sf::Color::Black) { blackCount++; }
					if (getBlock(x - 1, y) == sf::Color::Black) { blackCount++; }
					if (getBlock(x, y + 1) == sf::Color::Black) { blackCount++; }
					if (getBlock(x, y - 1) == sf::Color::Black) { blackCount++; }

					if (getBlock(x + 1, y) == sf::Color::Yellow) { yellowCount++; }
					if (getBlock(x - 1, y) == sf::Color::Yellow) { yellowCount++; }
					if (getBlock(x, y + 1) == sf::Color::Yellow) { yellowCount++; }
					if (getBlock(x, y - 1) == sf::Color::Yellow) { yellowCount++; }

					if (yellowCount == 1 && blackCount == 3)
						setBlock(x, y, sf::Color::Black);
				}
			}
		}

		//////////////////
		//Color Clearing//
		//////////////////
		for (int x = 0; x < LSX; x++) {
			for (int y = 0; y < LSY; y++) {
				if (getBlock(x, y) != sf::Color::Black && getBlock(x, y) != sf::Color::Yellow && getBlock(x, y) != sf::Color::Blue && getBlock(x, y) != sf::Color::Cyan)
					setBlock(x, y, sf::Color::Yellow);
			}
		}


		setBlock(spawnx, spawny, sf::Color::Red);



		return 0;
	}
};