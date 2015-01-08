#pragma once

#include "SFML\Graphics.hpp"

class Item {
	public:
		Item(float startX, float startY, sf::Color color);
		void draw(sf::RenderWindow *window);

		float radius = 30;

		bool used; // used item is invisible and non-interactive for rest of round.

		sf::Vector2f position;
		sf::CircleShape circle;
};