#include <SDL.h>

#include "viewport.h"

void Viewport::update(std::vector<SDL_Event>& eventList) {
	// always call this, because it reports relativity to the last
	// time this was called rather than the last tick. if we don't,
	// we get big jumps from mouse movement between the last middle
	// mouse click and the current one.
	int relativeX;
	int relativeY;
	SDL_GetRelativeMouseState(&relativeX, &relativeY);

	for (auto& event : eventList) {
		switch (event.type) {
		case SDL_KEYDOWN:
			handleKeyDown(event.key.keysym.sym);
			break;

		case SDL_KEYUP:
			handleKeyUp(event.key.keysym.sym);
			break;

		case SDL_MOUSEBUTTONDOWN:
			handleMouseDown(event.button.button);
			break;

		case SDL_MOUSEBUTTONUP:
			handleMouseUp(event.button.button);
			break;

		default:
			break;
		}
	}

	if (mouseMoving) {
		handleMouseMoving(relativeX, relativeY);
	}
	else if (zooming) {
		handleZoom(relativeY);
	}
	else {
		handleKeyboardMoving();
	}

	// keep viewport boundaries enforced
	performBoundaryContainment();
}

void Viewport::handleMouseDown(int button) {
	if (button == SDL_BUTTON_MIDDLE) {
		mouseMoving = true;
	}

	if (button == SDL_BUTTON_RIGHT) {
		zooming = true;
	}
}

void Viewport::handleMouseUp(int button) {
	if (button == SDL_BUTTON_MIDDLE) {
		mouseMoving = false;
	}

	if (button == SDL_BUTTON_RIGHT) {
		zooming = false;
	}
}

void Viewport::handleKeyDown(SDL_Keycode key) {
	switch (key) {
	case SDLK_a:
		movingLeft = true;
		break;

	case SDLK_w:
		movingUp = true;
		break;

	case SDLK_d:
		movingRight = true;
		break;

	case SDLK_s:
		movingDown = true;
		break;

	default:
		break;
	}
}

void Viewport::handleKeyUp(SDL_Keycode key) {
	switch (key) {
	case SDLK_a:
		movingLeft = false;
		break;

	case SDLK_w:
		movingUp = false;
		break;

	case SDLK_d:
		movingRight = false;
		break;

	case SDLK_s:
		movingDown = false;
		break;

	default:
		break;
	}
}

void Viewport::handleKeyboardMoving() {
	if (movingDown) {
		topLeft.y += MOVE_SPEED * scaleFactor;
	}

	if (movingUp) {
		topLeft.y -= MOVE_SPEED * scaleFactor;
	}

	if (movingLeft) {
		topLeft.x -= MOVE_SPEED * scaleFactor;
	}

	if (movingRight) {
		topLeft.x += MOVE_SPEED * scaleFactor;
	}
}

void Viewport::handleZoom(int relativeY) {
	// recall that negative y = mouse moving "up"
	int x;
	int y;
	SDL_GetMouseState(&x, &y);

	if (relativeY > 0) {
		scale(SCALE_UP_FACTOR, Vector(x, y));
	}
	else if (relativeY < 0) {
		scale(SCALE_DOWN_FACTOR, Vector(x, y));
	}
}

void Viewport::handleMouseMoving(int relativeX, int relativeY) {
	// SDL_MOUSEMOTION is completely fucking borked, so we do it here manually
	// moving the mouse right pushes the viewport left
	// moving the mouse up pushes the viewport down
	topLeft.x -= relativeX * scaleFactor;
	topLeft.y -= relativeY * scaleFactor;
}

void Viewport::scale(float factor, Vector target) {
	// when scaling UP, the transform scales down by the inverse
	// since the relative unit coordinates are now 'smaller' in
	// a bigger space
	scaleFactor *= factor;
	height = height * factor;
	width = width * factor;
	transform = transform * (1.0 / factor);
}

Vector Viewport::bottomLeft() {
	Vector result;
	
	result.x = topLeft.x;
	result.y = topLeft.y + height;
	
	return result;
}

Vector Viewport::bottomRight() {
	Vector result;

	result.x = topLeft.x + width;
	result.y = topLeft.y + height;

	return result;
}

Vector Viewport::topRight() {
	Vector result;

	result.x = topLeft.x + width;
	result.y = topLeft.y;

	return result;
}

double Viewport::size() {
	return width * height;
}

bool Viewport::isTooFarDown() {
	return bottomLeft().y > WORLD_BOUNDARY_DOWNY;
}

bool Viewport::isTooFarLeft() {
	return topLeft.x < WORLD_BOUNDARY_LEFTX;
}

bool Viewport::isTooFarRight() {
	return topRight().x > WORLD_BOUNDARY_RIGHTX;
}

bool Viewport::isTooFarUp() {
	return topLeft.y < WORLD_BOUNDARY_UPY;
}

bool Viewport::isTooSmall() {
	return (width * height) < minSize;
}

bool Viewport::isTooBig() {
	return (width * height) > WORLD_SIZE;
}

void Viewport::performBoundaryContainment() {
	// if the viewport extends beyond the boundaries in any direction,
	// move it back in the opposite direction sufficiently to be contained
	// within the valid zone
	if (isTooFarLeft()) {
		topLeft.x += (WORLD_BOUNDARY_LEFTX - topLeft.x);
	}

	if (isTooFarUp()) {
		topLeft.y += (WORLD_BOUNDARY_UPY - topLeft.y);
	}

	if (isTooFarDown()) {
		topLeft.y -= (bottomLeft().y - WORLD_BOUNDARY_DOWNY);
	}

	if (isTooFarRight()) {
		topLeft.x -= (topRight().x - WORLD_BOUNDARY_RIGHTX);
	}

	// we're now 'contained', but we may still be out-of-zone because of
	// zoom scaling, so check again and scale until we're good to go
	if (isTooSmall()) {
		scale(static_cast<double> (minSize) / size(), Vector(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2));
	}

	if (isTooBig()) {
		scale(static_cast<double> (WORLD_SIZE) / size(), Vector(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2));
	}
}
