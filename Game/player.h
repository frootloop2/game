#pragma once

#include "SFML\Graphics.hpp"
#include "item.h"
#include <math.h> 

class Player {
	public:
		enum Type { CARDINAL, DIAGONAL};
		
		Player(float startX, float startY, Type type, sf::Color color, int joyNum, sf::Joystick::Axis xAxis, sf::Joystick::Axis yAxis, int dashButton);
		void update(sf::RenderWindow *window);
		void updateLines();
		void draw(sf::RenderWindow *window);
		bool hits(Player otherPlayer);
		bool collects(Item item);
		Type lineType;

		float moveSpeed = 15.0f;
		float armLength = 150.0f;
		float radius = 75.0f;

		int score;

		sf::Color teamColor;
		sf::Vector2f spawnPosition;
		sf::Vector2f position;
		sf::Vector2f linePositions[2][2];

		sf::Vertex lines[2][2];
		sf::CircleShape circle;

		int joystickNum;
		sf::Joystick::Axis joystickXAxis;
		sf::Joystick::Axis joystickYAxis;
		int joystickDashButton;
};