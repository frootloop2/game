#pragma once

#include "State.h"
#include "gameplayState.h"

class ControlConfigState : public State {
	public:
		ControlConfigState();
		~ControlConfigState() {}
		State* update(sf::RenderWindow *window);

		sf::Event event;
		sf::Font font;
		sf::Text text;

		sf::Joystick::Axis *axisToSet;

		sf::Joystick::Axis p1x;
		sf::Joystick::Axis p1y;
		int p1dashButton = -1;

		sf::Joystick::Axis p2x;
		sf::Joystick::Axis p2y;
		int p2dashButton = -1;

		int player1JoystickId;
		int player2JoystickId;

};