#pragma once

#include "raylib.h"

#define NEARBLACK CLITERAL(Color){15, 15, 15, 255}

static const int screenWidth = 600;
static const int screenHeight = 600;
static const Vector2 screenCenter = {screenWidth / 2.0f, screenHeight / 2.0f};

static const int MAX_ASTEROIDS = 64;
static const float ASTEROID_RANDOM_ANGLE = 20 * DEG2RAD;
static const int AST_SPAWN_PADDING = 128;

static bool showCone = true;
static bool showAstCount = true;
