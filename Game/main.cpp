#include <SFML/Graphics.hpp>
#include <Windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include "player.h"
#include "item.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow) {
	sf::RenderWindow window(sf::VideoMode(900, 900), "Game", sf::Style::Close);
	window.setFramerateLimit(60);

	enum GameState {
		MAINMENU,
		GAMEPLAY,
		ROUNDWON,
		ROUNDSTART,
		GAMEWON
	};

	// general resources
	int winScore = 10;
	int gameState = GameState::MAINMENU;
	sf::Font font;
	if (!font.loadFromFile("calibri.ttf")) {
		return -1;
	}
	
	// MAINMENU resources
	sf::Text mainMenuText;
	{
		mainMenuText.setFont(font);
		mainMenuText.setString("Press Enter to start");
		mainMenuText.setPosition((float)window.getSize().x / 10, (float)window.getSize().y / 2);
	}

	// GAMEPLAY resources
	std::string score;
	sf::Text scoreText;
	sf::Vertex gridlines[18][2];
	std::vector<Player> players;
	std::vector<Item> items;
	bool playerHit;
	{	
		// lame procedural gridlines just do a bg eventually I guess
		for (int i = 0; i < 9; i++) {
			gridlines[i][0] = sf::Vertex(sf::Vector2f(window.getSize().x * (i + 1.0f) / 10.0f, 0.0f), sf::Color(63, 63, 63));
			gridlines[i][1] = sf::Vertex(sf::Vector2f(window.getSize().x * (i + 1.0f) / 10.0f, (float)window.getSize().y), sf::Color(63, 63, 63));
		}
		gridlines[4][0].color = sf::Color(127, 127, 127);
		gridlines[4][1].color = sf::Color(127, 127, 127);
		for (int i = 9; i < 18; i++) {
			gridlines[i][0] = sf::Vertex(sf::Vector2f(0.0f, window.getSize().y * (i - 8.0f) / 10.0f), sf::Color(63, 63, 63));
			gridlines[i][1] = sf::Vertex(sf::Vector2f((float)window.getSize().x, window.getSize().y * (i - 8.0f) / 10.0f), sf::Color(63, 63, 63));
		}
		gridlines[13][0].color = sf::Color(127, 127, 127);
		gridlines[13][1].color = sf::Color(127, 127, 127);

		// hard coded controllers + controls for now
		players.push_back(Player(180.0f, 180.0f, Player::Type::CARDINAL, sf::Color::Red, 0, sf::Joystick::X, sf::Joystick::Y, 5));
		players.push_back(Player(720.0f, 720.0f, Player::Type::DIAGONAL, sf::Color::Blue, 1, sf::Joystick::PovX, sf::Joystick::PovY, 1));

		items.push_back(Item(180.0f, 720.0f, sf::Color::Green));
		items.push_back(Item(720.0f, 180.0f, sf::Color::Green));

		playerHit = false;

		// set the string to display
		scoreText.setFont(font);
		scoreText.setPosition(5.0f, 0.0f);
		score = std::to_string(players[0].score);
		for (int i = 1; i < (int)players.size(); i++) {
			score = score + " - " + std::to_string(players[i].score);
		}
		scoreText.setString(score);
	}

	// ROUNDWON resources
	int maxRoundWonFrames;
	int roundWonFrameCount;
	{
		maxRoundWonFrames = 30;
		roundWonFrameCount = maxRoundWonFrames;
	}

	// ROUNDSTART resources
	int maxRoundStartFrames;
	int roundStartFrameCount;
	sf::Text roundStartText;
	{
		maxRoundStartFrames = 30;
		roundStartFrameCount = maxRoundStartFrames;
		roundStartText.setFont(font);
		roundStartText.setCharacterSize(256);
		roundStartText.setString("GO");
		roundStartText.setPosition(285.0f, 255.0f);
	}

	// GAMEWON resources
	sf::Text winnerText;
	{
		winnerText.setFont(font);
		winnerText.setPosition((float)window.getSize().x / 10, (float)window.getSize().y / 2);
	}

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
				window.close();
			}
		}

		switch (gameState) {
			case GameState::MAINMENU:
				// input
				{
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) {
						gameState = GameState::ROUNDSTART;
					}
				}
				// render
				{
					window.clear();
					window.draw(mainMenuText);
					window.display();
				}
				break;
			case GameState::ROUNDWON:
				// count frames
				roundWonFrameCount--;
				if (roundWonFrameCount <= 0) {
					roundWonFrameCount = maxRoundWonFrames;
					gameState = GameState::ROUNDSTART;
					// reset field
					for (int i = 0; i < (int)players.size(); i++) {
						players[i].position.x = players[i].spawnPosition.x;
						players[i].position.y = players[i].spawnPosition.y;
						players[i].lineType = players[i].lineType == Player::Type::CARDINAL ? Player::Type::DIAGONAL : Player::Type::CARDINAL;
						players[i].updateLines();
					}
					for (int i = 0; i < (int)items.size(); i++) {
						items[i].used = false;
					}
					for (int i = 0; i < (int)players.size(); i++) {
						if (players[i].score >= winScore) {
							gameState = GameState::GAMEWON;
							winnerText.setString("Player " + std::to_string(i + 1) + " wins\nPress Space to continue");
						}
					}
				}
				// render
				{
					window.clear();

					for (int i = 0; i < 18; i++) {
						window.draw(gridlines[i], 2, sf::Lines);
					}
					for (int i = 0; i < (int)items.size(); i++) {
						items[i].draw(&window);
					}
					for (int i = 0; i < (int)players.size(); i++) {
						players[i].draw(&window);
					}
					window.draw(scoreText);
					
					window.display();
				}
				break;
			case GameState::ROUNDSTART:
				// count frames
				roundStartFrameCount--;
				if (roundStartFrameCount <= 0) {
					roundStartFrameCount = maxRoundStartFrames;
					gameState = GameState::GAMEPLAY;

				}
				// render
				{
					window.clear();

					for (int i = 0; i < 18; i++) {
						window.draw(gridlines[i], 2, sf::Lines);
					}
					for (int i = 0; i < (int)items.size(); i++) {
						items[i].draw(&window);
					}
					for (int i = 0; i < (int)players.size(); i++) {
						players[i].draw(&window);
					}
					window.draw(scoreText);

					window.draw(roundStartText);

					window.display();
				}
				break;
			case GameState::GAMEWON:
				// input
				{
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
						for (int i = 0; i < (int)players.size(); i++) {
							players[i].score = 0;
						}
						// set the string to display
						score = std::to_string(players[0].score);
						for (int i = 1; i < (int)players.size(); i++) {
							score = score + " - " + std::to_string(players[i].score);
						}
						scoreText.setString(score);
						gameState = GameState::MAINMENU; 

					}
				}

				// render
				{
					window.clear();
					window.draw(winnerText);
					window.draw(scoreText);
					window.display();
				}
				break;
			case GameState::GAMEPLAY:
				// input
				for (int i = 0; i < (int)players.size(); i++) {
					players[i].update(&window);
				}

				// collision resolution
				{
					// player collecting item
					for (int i = 0; i < (int)items.size(); i++) {
						for (int j = 0; j < (int)players.size(); j++) {
							if (players[j].collects(items[i])) {
								items[i].used = true;
								for (int k = 0; k < (int)players.size(); k++) {
									players[k].lineType = players[k].lineType == Player::Type::CARDINAL ? Player::Type::DIAGONAL : Player::Type::CARDINAL;
									players[k].updateLines();
								}
							}
						}
					}

					// player hitting player
					for (int i = 0; i < (int)players.size(); i++) {
						for (int j = 0; j < (int)players.size(); j++) {
							if (i != j) {
								if (players[i].hits(players[j])) {
									// don't break yet, need to see if other players got hit this frame (Double KO)
									players[i].score++;
									// set the string to display
									score = std::to_string(players[0].score);
									for (int i = 1; i < (int)players.size(); i++) {
										score = score + " - " + std::to_string(players[i].score);
									}
									scoreText.setString(score);
									gameState = GameState::ROUNDWON;
								}
							}
						}
					}
				}

				// render
				{
					window.clear();

					for (int i = 0; i < 18; i++) {
						window.draw(gridlines[i], 2, sf::Lines);
					}
					for (int i = 0; i < (int)items.size(); i++) {
						items[i].draw(&window);
					}
					for (int i = 0; i < (int)players.size(); i++) {
						players[i].draw(&window);
					}
					window.draw(scoreText);

					window.display();
				}
				break;
		}
	}
	return 0;
}