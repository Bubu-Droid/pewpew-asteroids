#pragma once

#include "raylib.h"

#define NEARBLACK CLITERAL(Color){15, 15, 15, 255}

static const int SCREEN_WIDTH = 600;
static const int SCREEN_HEIGHT = 600;
static const Vector2 SCREEN_CENTER = {SCREEN_WIDTH / 2.0f,
                                      SCREEN_HEIGHT / 2.0f};

static bool debug = false;

static bool showCone = true;
static bool showAstCount = true;

static bool showPlayerStats = true;

static bool showAmmo = true;
