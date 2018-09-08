#pragma once

// general settings
static const int SCREEN_WIDTH = 1920;
static const int SCREEN_HEIGHT = 1080;
static const int SCREEN_SIZE = SCREEN_WIDTH * SCREEN_HEIGHT;

// viewport aspect ratio matches the screen
static const int INIT_VIEWPORT_WIDTH = SCREEN_WIDTH;
static const int INIT_VIEWPORT_HEIGHT = SCREEN_HEIGHT;
static const int INIT_VIEWPORT_SIZE = INIT_VIEWPORT_WIDTH * INIT_VIEWPORT_HEIGHT;

// to avoid having to do maths around aspect ratios,
// set world dimensions as multiples of screen dimentions
static const int WORLD_HEIGHT = SCREEN_HEIGHT * 8;
static const int WORLD_WIDTH = SCREEN_WIDTH * 8;
static const int WORLD_SIZE = WORLD_HEIGHT * WORLD_WIDTH;

static const int WORLD_BOUNDARY_LEFTX = (WORLD_WIDTH / 2) * -1;
static const int WORLD_BOUNDARY_RIGHTX = WORLD_WIDTH / 2;
static const int WORLD_BOUNDARY_UPY = (WORLD_HEIGHT / 2) * -1;
static const int WORLD_BOUNDARY_DOWNY = WORLD_HEIGHT / 2;

// interaction settings
static const double SCALE_DOWN_FACTOR = .98;
static const double SCALE_UP_FACTOR = 1.02;
static const double MOVE_SPEED = 5;
