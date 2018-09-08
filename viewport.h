#pragma once

#include <vector>

#include "common.h"
#include "math.h"
#include "entity.h"

class Viewport {
public:
	// tracks the visible portion of the world
	// altered by player input (WASD)
	Viewport() : topLeft(0, 0), minSize(static_cast<double>(INIT_VIEWPORT_SIZE) * .25), transform(1, 0, 0, 1) {}

	// width and height can change with zoom
	double width = INIT_VIEWPORT_WIDTH;
	double height = INIT_VIEWPORT_HEIGHT;
	double minSize;
	double scaleFactor = 1.0;
	Vector topLeft;
	Vector mousePosition;
	TransformMatrix transform;

	bool zooming = false;
	bool mouseMoving = false;
	bool movingLeft = false;
	bool movingRight = false;
	bool movingUp = false;
	bool movingDown = false;

	void update(std::vector<SDL_Event>& eventList);

	// derived coordinate properties (we only directly change topleft)
	Vector bottomLeft();
	Vector bottomRight();
	Vector topRight();
	double size();

	// update subfunctions
	void handleZoom(int relativeY);
	void handleMouseMoving(int relativeX, int relativeY);
	void handleKeyboardMoving();
	void handleKeyDown(SDL_Keycode key);
	void handleKeyUp(SDL_Keycode key);
	void handleMouseDown(int button);
	void handleMouseUp(int button);

	// "zoom" functions increase the size of the viewport
	void scale(float factor, Vector target);

	// basic rendering API
	bool isEntityInViewport(Entity* entity);
	Vector computeScreenspaceCoordinates(Vector position, float parallaxCoefficient, bool applyTransform = true);

	// viewport boundary containment, performed each tick
	void performBoundaryContainment();
	bool isTooBig();
	bool isTooSmall();
	bool isTooFarLeft();
	bool isTooFarRight();
	bool isTooFarDown();
	bool isTooFarUp();
};
