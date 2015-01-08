#include "item.h"

Item::Item(float startX, float startY, sf::Color color) {
	used = false;

	position.x = startX;
	position.y = startY;
	
	circle.setRadius(radius);
	circle.setFillColor(sf::Color::Transparent);
	circle.setOutlineColor(color);
	circle.setOutlineThickness(-1);
	circle.setOrigin(circle.getRadius(), circle.getRadius());
}

void Item::draw(sf::RenderWindow *window) {
	if (!used) {
		circle.setPosition(position.x, window->getSize().y - position.y);
		window->draw(circle);
	}
}