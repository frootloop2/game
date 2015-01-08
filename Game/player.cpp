#include "player.h"

Player::Player(float startX, float startY, Type type, sf::Color color, int joyNum, sf::Joystick::Axis xAxis, sf::Joystick::Axis yAxis, int dashButton) {
	spawnPosition.x = startX;
	spawnPosition.y = startY;
	position.x = startX;
	position.y = startY;
	lineType = type;
	teamColor = color;

	circle.setRadius(radius);
	circle.setFillColor(sf::Color::Transparent);
	circle.setOutlineColor(color);
	circle.setOutlineThickness(-1);
	circle.setOrigin(circle.getRadius(), circle.getRadius());
	
	joystickNum = joyNum;
	joystickXAxis = xAxis;
	joystickYAxis = yAxis;
	joystickDashButton = dashButton;
	score = 0;

	updateLines();
}

void Player::update(sf::RenderWindow *window) {
	float posX = sf::Joystick::getAxisPosition(joystickNum, joystickXAxis) / 100;
	float posY = sf::Joystick::getAxisPosition(joystickNum, joystickYAxis) / 100;
	
	// SFML reads the GCN -> USB converter's Y axis and U axis upside down for some unknown reason. They're fine on other software...
	sf::Joystick::Identification info = sf::Joystick::getIdentification(joystickNum);
	if (info.vendorId == 10395) {
		posY = joystickYAxis == sf::Joystick::Y || joystickYAxis == sf::Joystick::U ? -posY : posY;
		posX = joystickXAxis == sf::Joystick::Y || joystickXAxis == sf::Joystick::U ? -posX : posX;
	}

	// we want to move at the same speed regardless of direction so we need to calculate the proper speed when moving diagonally
	// right 100 + up 100 is different than upright 100 because triangles.
	// NOTE: this math is only correct when moving at a perfect diagonal. If analog movement is added this code will have to change.
	float dirMoveSpeed = posX * posY != 0 ? sqrt(0.5f * moveSpeed * moveSpeed) : moveSpeed;

	position.x = std::max(0.0f, std::min(position.x + dirMoveSpeed * posX, (float)window->getSize().x));
	position.y = std::max(0.0f, std::min(position.y + dirMoveSpeed * posY, (float)window->getSize().y));

	updateLines();
}

void Player::updateLines() {
	// updates line positions given player position. Ideally they would just be updated by the delta on the position.
	if (lineType == Type::DIAGONAL) {
		// we want to have the same arm length regardless of direction so we need to calculate the proper arm length
		// right 100 + up 100 is different than upright 100 because triangles.
		float diagonalArmLength = sqrt(0.5f * armLength * armLength);
		linePositions[0][0] = position + sf::Vector2f(-diagonalArmLength, -diagonalArmLength);
		linePositions[0][1] = position + sf::Vector2f(diagonalArmLength, diagonalArmLength);
		linePositions[1][0] = position + sf::Vector2f(-diagonalArmLength, diagonalArmLength);
		linePositions[1][1] = position + sf::Vector2f(diagonalArmLength, -diagonalArmLength);
	} else if (lineType == Type::CARDINAL) {
		linePositions[0][0] = position + sf::Vector2f(-armLength, 0.0f);
		linePositions[0][1] = position + sf::Vector2f(armLength, 0.0f);
		linePositions[1][0] = position + sf::Vector2f(0.0f, armLength);
		linePositions[1][1] = position + sf::Vector2f(0.0f, -armLength);
	}
}

bool Player::hits(Player otherPlayer) {
	if (teamColor == otherPlayer.teamColor) {
		return false;
	}

	sf::Vector2f distanceVector;
	float distance;
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			distanceVector = otherPlayer.position - linePositions[i][j];
			distance = sqrt(distanceVector.x * distanceVector.x + distanceVector.y * distanceVector.y);
			if (distance < otherPlayer.radius) {
				return true;
			}
		}
	}
	return false;
}

bool Player::collects(Item item) {
	sf::Vector2f distanceVector = position - item.position;
	float distance = sqrt(distanceVector.x * distanceVector.x + distanceVector.y * distanceVector.y);
	return !item.used && distance < item.radius + radius;
}

void Player::draw(sf::RenderWindow *window) {
	// game coordinates has origin at bottom left with positive directions being up and right.
	// window coordinates has origin at top left with positive directions being down and right.
	// this code does the translation.

	// position SFML graphics stuff based on game data
	circle.setPosition(position.x, window->getSize().y - position.y);
	lines[0][0].position.x = linePositions[0][0].x;
	lines[0][0].position.y = window->getSize().y - linePositions[0][0].y;
	lines[0][1].position.x = linePositions[0][1].x;
	lines[0][1].position.y = window->getSize().y - linePositions[0][1].y;
	lines[1][0].position.x = linePositions[1][0].x;
	lines[1][0].position.y = window->getSize().y - linePositions[1][0].y;
	lines[1][1].position.x = linePositions[1][1].x;
	lines[1][1].position.y = window->getSize().y - linePositions[1][1].y;

	window->draw(lines[0], 2, sf::Lines);
	window->draw(lines[1], 2, sf::Lines);
	window->draw(circle);
}