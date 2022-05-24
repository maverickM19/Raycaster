#include <array>
#include <cmath>
#include "player.h"

void Player::draw(sf::RenderWindow& window)
{
	shape.setPosition(position.x - START_SIZE, position.y - START_SIZE);
	window.draw(shape);
}

void Player::draw_rays(const std::array<std::array<Cell, MAP_WIDTH>, MAP_HEIGHT>& map, sf::RenderWindow& window) const
{
	sf::Vector2f position_ray, next_position_ray, position_map, horisontal_ray;
	float angle_ray, tangens;
	float degree = convert_radians_to_degrees(angle) - 55;

	if (degree < 0)
	{
		degree += 360;
	}
	else if (degree > 360) 
	{
		degree -= 360;
	}

	for (size_t i = 0; i < 110 * 2; i++)
	{
		angle_ray = convert_degrees_to_radians(degree);

		tangens = (-1) / tan(angle_ray);

		if (angle_ray > PI)		//looking UP
		{
			position_ray.y = std::floor(position.y / CELL_SIZE) * CELL_SIZE;
			position_ray.x = (position.y - position_ray.y) * tangens + position.x;
			next_position_ray.y = -CELL_SIZE;
			next_position_ray.x = -next_position_ray.y * tangens;
		}
		else if (angle_ray < PI)	//looking DOWN
		{
			position_ray.y = std::ceil(position.y / CELL_SIZE) * CELL_SIZE;
			position_ray.x = (position.y - position_ray.y) * tangens + position.x;
			next_position_ray.y = CELL_SIZE;
			next_position_ray.x = -next_position_ray.y * tangens;
		}

		for (size_t i = 0; i < DRAWING; i++)
		{
			position_map.x = std::ceil(position_ray.x / CELL_SIZE) - 1;
			position_map.y = (angle_ray > PI) ? (position_ray.y / CELL_SIZE - 1) : (position_ray.y / CELL_SIZE);

			if (((position_map.x >= 0 && position_map.x < MAP_WIDTH) && (position_map.y >= 0 && position_map.y < MAP_HEIGHT))
				&& (map[position_map.y][position_map.x] == Cell::Wall))
			{
				break;
			}

			position_ray += next_position_ray;
		}

		horisontal_ray = position_ray;	//save horisontal line

		tangens = -tan(angle_ray);

		if (angle_ray > PI / 2 && angle_ray < (3 * PI / 2)) // looking LEFT
		{
			position_ray.x = std::floor(position.x / CELL_SIZE) * CELL_SIZE;
			position_ray.y = (position.x - position_ray.x) * tangens + position.y;
			next_position_ray.x = -CELL_SIZE;
			next_position_ray.y = -next_position_ray.x * tangens;
		}
		else if (angle_ray < PI / 2 || angle_ray >(3 * PI / 2))	//looking RIGHT
		{
			position_ray.x = std::ceil(position.x / CELL_SIZE) * CELL_SIZE;
			position_ray.y = (position.x - position_ray.x) * tangens + position.y;
			next_position_ray.x = CELL_SIZE;
			next_position_ray.y = -next_position_ray.x * tangens;
		}

		for (size_t i = 0; i < DRAWING; i++)
		{
			position_map.y = std::ceil(position_ray.y / CELL_SIZE) - 1;
			position_map.x = (angle_ray < PI / 2 || angle_ray >(3 * PI / 2)) ? (position_ray.x / CELL_SIZE) : (position_ray.x / CELL_SIZE - 1);

			if (((position_map.x >= 0 && position_map.x < MAP_WIDTH) && (position_map.y >= 0 && position_map.y < MAP_HEIGHT))
				&& (map[position_map.y][position_map.x] == Cell::Wall))
			{
				break;
			}

			position_ray += next_position_ray;
		}
		
		if (distance_to(horisontal_ray) < distance_to(position_ray))	//choose the shortest line
		{
			position_ray = horisontal_ray;
		}

		sf::Vertex direction[] = { sf::Vertex(sf::Vector2f(position.x, position.y), sf::Color::White),
			sf::Vertex(sf::Vector2f(position_ray.x, position_ray.y), sf::Color::White) };

		window.draw(direction, 2, sf::Lines);

		degree += 0.5;

		if (degree < 0)
		{
			degree += 360;
		}
		else if (degree > 360)
		{
			degree -= 360;
		}
	}
}

void Player::move(const Direction direction, const std::array<std::array<Cell, MAP_WIDTH>, MAP_HEIGHT>& map)
{
	switch (direction)
	{
	case Direction::LEFT:
		angle -= 0.05;

		if (angle < 0)
		{
			angle += 2 * PI;
		}

		delta_position.x = cos(angle) * 3;
		delta_position.y = sin(angle) * 3;

		break;
	case Direction::RIGHT:
		angle += 0.05;

		if (angle > 2 * PI)
		{
			angle -= 2 * PI;
		}

		delta_position.x = cos(angle) * 3;
		delta_position.y = sin(angle) * 3;

		break;
	case Direction::UP:
		if (check_map_collision(sf::Vector2f(position + delta_position), map))
		{
			position.x += delta_position.x;
			position.y += delta_position.y;
		}
		else if (check_map_collision(sf::Vector2f(position.x + delta_position.x, position.y), map)) 
		{
			position.x += delta_position.x;
		}
		else if (check_map_collision(sf::Vector2f(position.x, position.y + delta_position.y), map))
		{
			
			position.y += delta_position.y;
		}

		break;
	case Direction::DOWN:
		if (check_map_collision(sf::Vector2f(position - delta_position), map))
		{
			position.x -= delta_position.x;
			position.y -= delta_position.y;
		}
		else if (check_map_collision(sf::Vector2f(position.x - delta_position.x, position.y), map))
		{
			position.x -= delta_position.x;
		}
		else if (check_map_collision(sf::Vector2f(position.x, position.y - delta_position.y), map))
		{
			position.y -= delta_position.y;
		}

		break;
	default:
		break;
	}
}

bool Player::check_map_collision(const sf::Vector2f &curr_position, const std::array<std::array<Cell, MAP_WIDTH>, MAP_HEIGHT>& map) const
{
	sf::Vector2f map_position;

	for (size_t i = 0; i < 4; i++)
	{
		switch (i)
		{
		case 0:
		{
			map_position.x = (floor((curr_position.x - START_SIZE) / 64));
			map_position.y = (floor((curr_position.y - START_SIZE) / 64));

			break;
		}
		case 1:
		{
			map_position.x = (floor((curr_position.x - START_SIZE) / 64));
			map_position.y = (floor((curr_position.y + START_SIZE) / 64));

			break;
		}
		case 2:
		{
			map_position.x = (floor((curr_position.x + START_SIZE) / 64));
			map_position.y = (floor((curr_position.y - START_SIZE) / 64));

			break;
		}
		case 3:
		{
			map_position.x = (floor((curr_position.x + START_SIZE) / 64));
			map_position.y = (floor((curr_position.y + START_SIZE) / 64));
			break;
		}
		}

		if ((map[map_position.y][map_position.x] == Cell::Wall))
		{
			return false;
		}
	}

	return true;
}

void Player::draw_map(const std::array<std::array<Cell, MAP_WIDTH>, MAP_HEIGHT> &map, sf::RenderWindow& window)
{
	sf::Color dark_gray(105, 105, 105, 230);

	for (size_t i = 0; i < MAP_HEIGHT; i++)
	{
		for (size_t j = 0; j < MAP_WIDTH; j++)
		{
			sf::RectangleShape cell_shape;
			cell_shape.setPosition(sf::Vector2f(j * CELL_SIZE, i * CELL_SIZE));
			cell_shape.setSize(sf::Vector2f(CELL_SIZE - 1, CELL_SIZE - 1));

			switch (map[i][j])
			{
			case Cell::Empty:
				cell_shape.setFillColor(sf::Color::Black);
				window.draw(cell_shape);
				break;
			case Cell::Wall:
				cell_shape.setFillColor(dark_gray);
				window.draw(cell_shape);
				break;
			default:
				break;
			}
		}
	}
}

double Player::distance_to(const sf::Vector2f &point) const
{
	return(sqrt((position.x - point.x) * (position.x - point.x) + (position.y - point.y) * (position.y - point.y)));
}

void Player::set_direction(const sf::Vector2i& mouse) 
{
	angle = -atan2(mouse.x - position.x, mouse.y - position.y) + PI/2;

	delta_position.x = cos(angle) * 5;
	delta_position.y = sin(angle) * 5;
}

float convert_degrees_to_radians(const float degree)
{
	return (degree * ONE_DEGREE_TO_RADIAN);
}

float convert_radians_to_degrees(const float radian)
{
	return (radian * RADIAN);
}
