#pragma once

#include "State.h"
#include "menuState.h"

class ResultsState : public State {
public:
	ResultsState(std::string winner);
	~ResultsState() {}
	State* update(sf::RenderWindow *window);

	sf::Font font;
	sf::Text text;
};