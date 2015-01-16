#pragma once

#include "SFML\Graphics.hpp"

class State {
	public: 
		virtual ~State() {}
		virtual State* update(sf::RenderWindow *window) { return this; }
};