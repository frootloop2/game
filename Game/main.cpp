#include <SFML/Graphics.hpp>
#include <Windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include "State.h"
#include "menuState.h"
#include "gameplayState.h"
#include "resultsState.h"
#include "player.h"
#include "item.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow) {
	sf::RenderWindow window(sf::VideoMode(900, 900), "Game", sf::Style::Close);
	window.setFramerateLimit(60);

	State *state = new MenuState();

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
				window.close();
			}
		}

		window.clear();
		state = state->update(&window);
		window.display();
	}
	return 0;
}