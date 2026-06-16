#pragma once

#include "constants.h"
#include "raylib.h"

static const float PLAYER_MAX_SPEED = 600.0f;
static const float PLAYER_MASS = 200.0f;
static const float PLAYER_ACCEL = 1200.0f;
static const float PLAYER_DECEL = 400.0f;

static const int EDGE_PADDING = 10;

static const int TOP_EDGE_PADDING = EDGE_PADDING;
static const int LEFT_EDGE_PADDING = EDGE_PADDING;
static const int BOTTOM_EDGE_PADDING = SCREEN_WIDTH - EDGE_PADDING;
static const int RIGHT_EDGE_PADDING = SCREEN_HEIGHT - EDGE_PADDING;

struct Player {
  Vector2 position;
  Vector2 velocity;
  Vector2 facingDirection;

  Player()
      : position(SCREEN_CENTER), velocity(0), facingDirection(Vector2(0, 1)) {}
};

void UpdatePlayer(Player &player, Vector2 &mousePos, float &frametime);
void DrawPlayer(Player &player, Texture2D playerTexture);
