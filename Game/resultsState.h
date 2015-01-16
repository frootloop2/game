#pragma once

#include "State.h"
#include "controlConfigState.h"

class ResultsState : public State {
public:
	ResultsState(std::string winner);
	~ResultsState() {}
	State* update(sf::RenderWindow *window);

	sf::Event event;
	sf::Font font;
	sf::Text text;
};