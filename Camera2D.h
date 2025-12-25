#pragma once
#include <SFML/Graphics.hpp>

class Camera2D
{
public:
	Camera2D(float width, float height);

	void handleEvent(const sf::Event& event, const sf::RenderWindow& window);
	void update(float deltaTime, const sf::RenderWindow& window);

	void apply(sf::RenderWindow& window);
	sf::View& getView();

private:
	sf::View view;

	sf::Vector2f baseSize;
	float cameraSpeed = 1000.f;

	float zoomLevel = 1.f;
	float zoomSpeed = 0.1f;
	float minZoom = 0.5f;
	float maxZoom = 2.f;


	bool isDragging = false;
	sf::Vector2f lastMouseWorldPos;
private:
	float clampf(float value, float min, float max);
};