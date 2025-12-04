#pragma once
#include <SFML/Graphics.hpp>
#include <algorithm>

const int TILE_SIZE = 32;
const int GRID_WIDTH = 50;
const int GRID_HEIGHT = 50;
const int GRID_OFFSET = 1;

enum class TileType { Empty, Obstacle };

class GridMap
{
public:
	TileType grid[GRID_WIDTH][GRID_HEIGHT] = { TileType::Empty };

	void draw(sf::RenderWindow& window, float zoomFactor)
	{
		sf::RectangleShape tile(sf::Vector2f(TILE_SIZE * zoomFactor - GRID_OFFSET, TILE_SIZE * zoomFactor - GRID_OFFSET));
		for (int y = 0; y < GRID_HEIGHT; ++y)
		{
			for (int x = 0; x < GRID_WIDTH; ++x)
			{
				tile.setPosition(sf::Vector2f(float(x * TILE_SIZE) * zoomFactor, float(y * TILE_SIZE) * zoomFactor));
				tile.setFillColor(grid[x][y] == TileType::Obstacle ? sf::Color(100, 100, 100) : sf::Color(30, 30, 30));
				window.draw(tile);
			}
		}
	}
	GridMap()
	{
		for (int y = 0; y < GRID_HEIGHT; ++y)
			for (int x = 0; x < GRID_WIDTH; ++x)
				grid[x][y] = TileType::Empty;
	}

	static bool InBounds(int x, int y)
	{
		return x >= 0 && x < GRID_WIDTH && y >= 0 && y < GRID_HEIGHT;
	}
};