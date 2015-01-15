#pragma once

#include "State.h"
#include "gameplayState.h"

class MenuState : public State {
	public:
		MenuState();
		~MenuState() {}
		State* update(sf::RenderWindow *window);

		sf::Font font;
		sf::Text text;
};