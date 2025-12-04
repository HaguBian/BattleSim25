#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>

#include <iostream>
#include <optional>

#include "GridMap.h"
#include "Unit.h"
#include "Faction.h"
#include "UI.h"
#include "AIController.h"
#include "FactionMan.h"

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
#pragma region HiddenCode
	// Initialize
	InitializeUI();
	sf::Time turnDelay = sf::seconds(0.01f);

	// Create the main window
	sf::RenderWindow window(sf::VideoMode({ WINDOW_WIDTH, WINDOW_HEIGHT }), "BattleSim25");

	// Load a sprite to display

	// Create a graphical text to display

	// Load a music to play

	// Play the music

	// GridMap
	GridMap gridMap;
#pragma endregion

	// Teams (vector of Unit pointers)
	UnitTemplate newbieTempl = {
		"Newbie", "Newbie",
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
	};

	Faction* f0 = new Faction(0, "Blue Team");
	Faction* f1 = new Faction(1, "Red Team");
	Faction* f2 = new Faction(2, "Green Team");
	Faction* f3 = new Faction(3, "Yellow Team");
	std::vector<Faction*> factions = { f0, f1, f2, f3 };
	f0->aiController = new AIController(f0);
	f1->aiController = new AIController(f1);
	f2->aiController = new AIController(f2);
	f3->aiController = new AIController(f3);

	auto spawnUnits = [&](Faction* fac, int startX, int startY)
		{
			int count = 0;
			for (int i = 0; i < 5; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					if (count >= 20) return;

					int x = startX + i;
					int y = startY + j;

					Unit* u = new Unit(x, y, fac->factionID, newbieTempl);
					u->faction = fac;
					fac->AddUnit(u);

					count++;
				}
			}
		};

	spawnUnits(f0, 5, 5);
	spawnUnits(f1, 45 - 7, 5);
	spawnUnits(f2, 5, 45 - 7);
	spawnUnits(f3, 45 - 7, 45 - 7);
	FactionManager factionManager;
	for (auto* f : factions)
		factionManager.AddFaction(f);

	factionManager.BuildTurnOrder();


	// Selected unit info panelz
	Unit* selectedUnit = nullptr;
	bool found = false;
		
	// Create camera view 
	sf::View camera(sf::FloatRect(sf::Vector2f(0.f, 0.f), sf::Vector2f(float(WINDOW_WIDTH), float(WINDOW_HEIGHT))));
	window.setView(camera);

	// Camera movement variables
	bool dragging = false;
	sf::Vector2i lastMousePos;
	float zoomFactor = 1.f;
	sf::Clock turnClock;
	// Start the game loop
	while (window.isOpen())
	{
		// Process events
		while (const std::optional event = window.pollEvent())
		{
			// Close window: exit
			if (event->is<sf::Event::Closed>()) window.close();

			// Left mouse click - select unit
			if (event->is<sf::Event::MouseButtonPressed>() &&
				event->getIf<sf::Event::MouseButtonPressed>()->button == sf::Mouse::Button::Left)
			{
				sf::Vector2i mousePos = sf::Mouse::getPosition(window);
				sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);

				selectedUnit = nullptr; // Deselect first
				found = false;

				float cellSize = TILE_SIZE * zoomFactor;
				for (auto& faction : factionManager.factions)
				{
					for (auto* u : faction->units)
					{
						sf::FloatRect box(
							sf::Vector2f(u->gridX * cellSize ,  u->gridY * cellSize), 
							sf::Vector2f(cellSize, cellSize ));

						if (box.contains(worldPos))
						{
							selectedUnit = u;
							found = true;
						}
					}
					if (found) break;
				}
			}
#pragma region mouse draft and zoom
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
				zoomFactor += wheel->delta * ZOOM_STEP;
				zoomFactor = std::clamp(zoomFactor, MIN_ZOOM, MAX_ZOOM);
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
#pragma endregion		
		
		// Clear screen
		window.clear(sf::Color::Black);

		// Draw the grid map
		gridMap.draw(window, zoomFactor);

		// Draw units
		for (auto& faction : factionManager.factions)
		{
			faction->DrawVision(window, zoomFactor);
			for (auto& u : faction->units)
			{
				u->Draw(window, zoomFactor);
			}
		}

		// Draw selected unit info panel
		window.setView(window.getDefaultView());
		DrawUnitInfoPanel(window, selectedUnit);

		// Draw the sprite
		// window.draw(sprite);

		// Draw the text
		//window.draw(text);

		if (turnClock.getElapsedTime() >= turnDelay)
		{
			factionManager.NextTurn();
			turnClock.restart();
		}

		// Update the window
		window.display();

	}

	// Cleanup
	for (auto& faction : factions)
		delete faction;

	return 0;
}