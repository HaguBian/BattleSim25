#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <optional>

#include "GridMap.h"
#include "Unit.h"

//
const int WINDOW_WIDTH = 1920;
const int WINDOW_HEIGHT = 1080;

const float ZOOM_STEP = 0.1f;
const float MIN_ZOOM = 0.5f;
const float MAX_ZOOM = 3.f;

const float CAMERA_SPEED = 10.f;
const float CAMERA_MARGIN = 100.f;



int main()
{
	// Create the main window
	sf::RenderWindow window(sf::VideoMode({ WINDOW_WIDTH, WINDOW_HEIGHT }), "BattleSim25");

	// Load a sprite to display

	// Create a graphical text to display
	sf::Font font;
	if (!font.openFromFile("resources/fonts/Orbitron-VariableFont_wght.ttf"))
	{

        std::cerr << "Could not load font\n";
		return -1;
	}
	sf::Text text(font, "BattleSim25", 50);

	// Load a music to play

	// Play the music

	// GridMap
	GridMap gridMap;

	// Teams
	std::vector<Unit> team0, team1;
	
	for (int i = 0; i < 10; ++i) team0.emplace_back(i + 2, 1, 0);
	for (int i = 0; i < 5; i++) team1.emplace_back(i + 2, 3, 1);

	// Create camera view 
	sf::View camera(sf::FloatRect(sf::Vector2f(0.f, 0.f), sf::Vector2f(float(WINDOW_WIDTH), float(WINDOW_HEIGHT))));
	window.setView(camera);

	// Camera movement variables
	bool dragging = false;
	sf::Vector2i lastMousePos;
	float zoomLevel = 1.f;

	// Start the game loop
	while (window.isOpen())
	{
		// Process events
		while (const std::optional event = window.pollEvent())
		{
			// Close window: exit
			if (event->is<sf::Event::Closed>()) window.close();

			// Middle mouse drag
            if (event->is<sf::Event::MouseButtonPressed>() &&
				event->getIf<sf::Event::MouseButtonPressed>()->button == sf::Mouse::Button::Middle)

			{
				dragging = true;
				lastMousePos = sf::Mouse::getPosition(window);
			}
			if (event->is<sf::Event::MouseButtonReleased>() &&
				event->getIf<sf::Event::MouseButtonReleased>()->button == sf::Mouse::Button::Middle)
			{
				dragging = false;
			}
			if (event->is<sf::Event::MouseMoved>() && dragging)
			{
				sf::Vector2i currentMousePos = sf::Mouse::getPosition(window);
				sf::Vector2f delta = window.mapPixelToCoords(lastMousePos) - window.mapPixelToCoords(currentMousePos);
				camera.move(delta);
				window.setView(camera);
				lastMousePos = currentMousePos;
			}

			// Mouse wheel zoom
			if (event->is<sf::Event::MouseWheelScrolled>())
			{
				auto wheel = event->getIf<sf::Event::MouseWheelScrolled>();
				zoomLevel += wheel->delta * ZOOM_STEP;
				zoomLevel = std::clamp(zoomLevel, MIN_ZOOM, MAX_ZOOM);
			}
		}

		// Hold LShift to move camera faster
		float cameraSpeed = CAMERA_SPEED;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift))
			cameraSpeed *= 10.f;

		// WASD / Arrow keys
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
			camera.move(sf::Vector2f(0.f, -cameraSpeed));
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
			camera.move(sf::Vector2f(0.f, cameraSpeed));
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
			camera.move(sf::Vector2f(-cameraSpeed, 0.f));
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
			camera.move(sf::Vector2f(cameraSpeed, 0.f));
		
		// Clamp camera to map bounds
		sf::Vector2f camPos = camera.getCenter();
		float halfWinW = WINDOW_WIDTH / 2.f;
		float halfWinH = WINDOW_HEIGHT / 2.f;

		float maxX = std::max(halfWinW, float(GRID_WIDTH * TILE_SIZE) - halfWinW);
		float minX = std::min(halfWinW, maxX);
		float maxY = std::max(halfWinH, float(GRID_HEIGHT * TILE_SIZE) - halfWinH);
		float minY = std::min(halfWinH, maxY);

		camPos.x = std::clamp(camPos.x, minX - CAMERA_MARGIN, maxX + CAMERA_MARGIN);
		camPos.y = std::clamp(camPos.y, minY - CAMERA_MARGIN, maxY + CAMERA_MARGIN);
		camera.setCenter(camPos);

		window.setView(camera);

		// Clear screen
		window.clear(sf::Color::Black);

		// Draw the grid map
		gridMap.draw(window, zoomLevel);

		// Draw the units
		for (auto& unit : team0) unit.Draw(window, zoomLevel);
		for (auto& unit : team1) unit.Draw(window, zoomLevel);

		// Draw the sprite
		// window.draw(sprite);

		// Draw the text
		//window.draw(text);

		// Update the window
		window.display();

	}

	return 0;
}