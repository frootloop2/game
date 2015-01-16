#include "gameplayState.h"

GameplayState::GameplayState(int player1JoystickId, sf::Joystick::Axis p1x, sf::Joystick::Axis p1y, int p1dashButton, int player2JoystickId, sf::Joystick::Axis p2x, sf::Joystick::Axis p2y, int p2dashButton) {
	// hard coded controllers + controls for now
	sf::Joystick::update();
	players.push_back(Player(450.0f, 180.0f, Player::Type::CARDINAL, sf::Color::Red, player1JoystickId, p1x, p1y, 5));
	players.push_back(Player(729.0f, 720.0f, Player::Type::DIAGONAL, sf::Color::Cyan, player2JoystickId, p2x, p2y, 1));

	items.push_back(Item(180.0f, 720.0f, sf::Color::Green));
	items.push_back(Item(720.0f, 180.0f, sf::Color::Green));

	font.loadFromFile("calibri.ttf");

	scoreText.setFont(font);
	updateScoreText();

	roundStartText.setFont(font);
	roundStartText.setCharacterSize(256);
	roundStartText.setString("GO");
	roundStartText.setPosition(285.0f, 255.0f);

	roundStartCount = roundStartLength;
}

void GameplayState::updateScoreText() {
	std::string score = std::to_string(players[0].score);
	for (int i = 1; i < (int)players.size(); i++) {
		score = score + " - " + std::to_string(players[i].score);
	}
	scoreText.setString(score);
}

void GameplayState::resetPosition() {
	for (int i = 0; i < (int)items.size(); i++) {
		items[i].used = false;
	}
	for (int i = 0; i < (int)players.size(); i++) {
		players[i].lineType = players[i].lineType == Player::Type::CARDINAL ? Player::Type::DIAGONAL : Player::Type::CARDINAL;
		players[i].updateLines();
		players[i].position.x = players[i].spawnPosition.x;
		players[i].position.y = players[i].spawnPosition.y;
		players[i].updateLines();
	}
}

State* GameplayState::update(sf::RenderWindow *window) {
	// only read input and run game logic during rounds
	if (roundStartCount < 0 && roundWonCount < 0) {
		// input
		{
			sf::Joystick::update();
			for (int i = 0; i < (int)players.size(); i++) {
				players[i].update(window);
			}
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
							updateScoreText();
							roundWonCount = roundWonLength;
						}
					}
				}
			}
		}
	}

	// ticks
	{
		if (roundStartCount >= 0) {
			roundStartCount--;
		}

		if (roundWonCount >= 0) {
			roundWonCount--;
			if (roundWonCount < 0) {
				for (int i = 0; i < (int)players.size(); i++) {
					if (players[i].score >= winScore) {
						return new ResultsState("Player " + std::to_string(i + 1));
					}
				}
				resetPosition();
				roundStartCount = roundStartLength;
			}
		}
	}

	// render
	{
		for (int i = 0; i < (int)items.size(); i++) {
			items[i].draw(window);
		}
		for (int i = 0; i < (int)players.size(); i++) {
			players[i].draw(window);
		}
		window->draw(scoreText);

		if (roundStartCount > 0) {
			window->draw(roundStartText);
		}
	}

	return this;
}