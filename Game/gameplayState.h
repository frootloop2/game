#pragma once
#include "State.h"
#include "player.h"
#include "item.h"
#include "resultsState.h"

class GameplayState : public State {
	public:
		GameplayState(int player1JoystickId, sf::Joystick::Axis p1x, sf::Joystick::Axis p1y, int p1dashButton, int player2JoystickId, sf::Joystick::Axis p2x, sf::Joystick::Axis p2y, int p2dashButton);
		~GameplayState() {}
		State* update(sf::RenderWindow *window);
		void updateScoreText();
		void resetPosition();

		sf::Font font;
		sf::Text scoreText;
		std::vector<Player> players;
		std::vector<Item> items;
		bool playerHit = false;

		int winScore = 2;
		int roundWonLength = 30;
		int roundWonCount = -1;
		int roundStartLength = 30;
		int roundStartCount = -1;

		sf::Text roundStartText;
};