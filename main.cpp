#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <chrono>
#include <array>
#include "player.h"

std::array<std::array<Cell, MAP_WIDTH>, MAP_HEIGHT> get_map()
{
	std::fstream is(PATH_MAP);

	std::array<std::array<Cell, MAP_WIDTH>, MAP_HEIGHT> input_map = {};
	char value;

	for (size_t i = 0; i < MAP_HEIGHT; i++)
	{
		for (size_t j = 0; j < MAP_WIDTH; j++)
		{
			is >> value;
			switch (value)
			{
			case '0':
				input_map[i][j] = Cell::Empty;
				break;
			case '1':
				input_map[i][j] = Cell::Wall;
				break;
			default:
				break;
			}
		}
	}

	return input_map;
}

int main() 
{
	sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Raycaster!");
	Player player(sf::Vector2f(100, 100));

	std::chrono::time_point<std::chrono::steady_clock> previous_time;
	unsigned int lag = 0;

	std::array<std::array<Cell, MAP_WIDTH>, MAP_HEIGHT> map{};

	map = get_map();

	while (window.isOpen())
	{
		sf::Event event;

		auto delta_time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - previous_time).count();
		lag += delta_time;
		previous_time += std::chrono::microseconds(delta_time);

		while (FRAME_DURATION <= lag)
		{
			lag -= FRAME_DURATION;

			if (FRAME_DURATION > lag)
			{
				while (window.pollEvent(event))
				{
					if (event.type == sf::Event::Closed)
						window.close();

					if (event.type == sf::Event::MouseMoved) 
					{
						player.set_direction(sf::Mouse::getPosition(window));
					}

					if (event.type == sf::Event::KeyPressed)
					{
						if (event.key.code == sf::Keyboard::Key::A)
						{
							player.move(Direction::LEFT, map);
						}
						else if (event.key.code == sf::Keyboard::Key::D)
						{
							player.move(Direction::RIGHT, map);
						}
						else if (event.key.code == sf::Keyboard::Key::W)
						{
							player.move(Direction::UP, map);
						}
						else if (event.key.code == sf::Keyboard::Key::S)
						{
							player.move(Direction::DOWN, map);
						}
					}
				}
			}
		}

		player.draw_map(map, window);
		player.draw(window);
		player.draw_rays(map, window);

		window.display();
		window.clear(sf::Color(0, 0, 0, 0));
	}

	return 0;
}
