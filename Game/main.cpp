#include <SFML/Graphics.hpp>
#include <Windows.h>
#include <stdlib.h>
#include "State.h"
#include "controlConfigState.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow) {
	sf::RenderWindow window(sf::VideoMode(900, 900), "Game", sf::Style::None);
	window.setFramerateLimit(60);

	State *state = new ControlConfigState();

	while (window.isOpen()) {
		if (window.hasFocus() && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
			window.close();
		}

		window.clear();
		state = state->update(&window);
		window.display();
	}
	return 0;
}