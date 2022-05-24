#pragma once
#include <SFML/Graphics.hpp>
#include "general.h"

const float START_SIZE = 10.0f;
const unsigned short DRAWING = 12;

enum class Cell
{
	Empty,
	Wall
};

enum class Direction 
{
	LEFT,
	RIGHT,
	UP,
	DOWN
};

class Player 
{
public:
	Player(sf::Vector2f position)
	{
		angle = 0;
		 
		delta_position.x = cos(angle) * 3;
		delta_position.y = sin(angle) * 3;

		this->position = position;
		shape.setPosition(position.x, position.y);
		shape.setRadius(START_SIZE);
		shape.setFillColor(sf::Color::Red);
	}
	Player()
	{
		position.x = 100;
		position.y = 100;

		angle = 0;

		delta_position.x = cos(angle) * 3;
		delta_position.y = sin(angle) * 3;

		shape.setPosition(position.x, position.y);
		shape.setRadius(START_SIZE);
		shape.setFillColor(sf::Color::Red);
	}

	void draw(sf::RenderWindow& window);
	void move(const Direction direction, const std::array<std::array<Cell, MAP_WIDTH>, MAP_HEIGHT>& map);
	void draw_map(const std::array<std::array<Cell, MAP_WIDTH>, MAP_HEIGHT>& map, sf::RenderWindow& window);
	void draw_rays(const std::array<std::array<Cell, MAP_WIDTH>, MAP_HEIGHT>& map, sf::RenderWindow& window) const;
	void set_direction(const sf::Vector2i &mouse);
	double distance_to(const sf::Vector2f &point) const;
private:
	bool check_map_collision(const sf::Vector2f& curr_position, const std::array<std::array<Cell, MAP_WIDTH>, MAP_HEIGHT>& map) const;
private:
	sf::Vector2f position;
	sf::Vector2f delta_position;
	sf::CircleShape shape;

	float angle;
};
