#include "Camera2D.h"

Camera2D::Camera2D(float width, float height)
{
	baseSize = { width, height };
	view.setSize(width, height);
	view.setCenter(width / 2, height / 2);
}

void Camera2D::handleEvent(const sf::Event& event, const sf::RenderWindow& window)
{
    // ZOOM
    if (event.type == sf::Event::MouseWheelScrolled)
    {
        sf::Vector2f beforeZoomWorldPos =
            window.mapPixelToCoords(
                sf::Mouse::getPosition(window),
                view
            );

        float zoomFactor = (event.mouseWheelScroll.delta > 0) ? -1.f : 1.f;

        zoomLevel += zoomFactor * zoomSpeed * zoomLevel;
        zoomLevel = clampf(zoomLevel, minZoom, maxZoom);

        view.setSize(
            baseSize.x * zoomLevel,
            baseSize.y * zoomLevel
        );

        sf::Vector2f afterZoomWorldPos =
            window.mapPixelToCoords(
                sf::Mouse::getPosition(window),
                view
            );

        view.move(beforeZoomWorldPos - afterZoomWorldPos);

        lastMouseWorldPos = afterZoomWorldPos;
    }

    // START DRAG
    if (event.type == sf::Event::MouseButtonPressed &&
        event.mouseButton.button == sf::Mouse::Middle)
    {
        isDragging = true;

        lastMouseWorldPos =
            window.mapPixelToCoords(
                { event.mouseButton.x, event.mouseButton.y },
                view
            );
    }

    // STOP DRAG
    if (event.type == sf::Event::MouseButtonReleased &&
        event.mouseButton.button == sf::Mouse::Middle)
    {
        isDragging = false;
    }

    // DRAG MOVE
    if (event.type == sf::Event::MouseMoved && isDragging)
    {
        sf::Vector2f currentMouseWorldPos =
            window.mapPixelToCoords(
                { event.mouseMove.x, event.mouseMove.y },
                view
            );

        sf::Vector2f delta = lastMouseWorldPos - currentMouseWorldPos;
        view.move(delta);

        lastMouseWorldPos = currentMouseWorldPos;
    }
}

void Camera2D::update(float deltaTime, const sf::RenderWindow& window)
{
	// KEYBOARD MOVEMENT
	sf::Vector2f move(0.f, 0.f);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		move.y -= cameraSpeed * deltaTime;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		move.y += cameraSpeed * deltaTime;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		move.x -= cameraSpeed * deltaTime;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		move.x += cameraSpeed * deltaTime;

	view.move(move);
}

void Camera2D::apply(sf::RenderWindow& window)
{
	window.setView(view);
}

void Camera2D::clamp(const sf::FloatRect& mapBounds)
{
    //
}

sf::View& Camera2D::getView()
{
	return view;
}

float Camera2D::clampf(float value, float min, float max)
{
	if (value < min)
		return min;
	if (value > max)
		return max;
	return value;
}

