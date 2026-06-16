#include "player.h"
#include "raymath.h"
#include <print>

void UpdatePlayer(Player &player, Vector2 &mousePos, float &frametime) {
  int xIn = (int)IsKeyDown(KEY_D) - (int)IsKeyDown(KEY_A);
  int yIn = (int)IsKeyDown(KEY_S) - (int)IsKeyDown(KEY_W);

  Vector2 motionVector = Vector2Normalize(Vector2(xIn, yIn));

  if (motionVector == Vector2(0, 0) && player.velocity != Vector2(0, 0)) {
    Vector2 decelVector = Vector2Normalize(Vector2Scale(player.velocity, -1));

    decelVector = Vector2Scale(decelVector, PLAYER_DECEL);

    player.velocity =
        Vector2Add(player.velocity, Vector2Scale(decelVector, frametime));

  } else {
    Vector2 accelVector = Vector2Scale(motionVector, PLAYER_ACCEL);

    player.velocity =
        Vector2Add(player.velocity, Vector2Scale(accelVector, frametime));

    float velMag = Vector2Length(player.velocity);

    if (velMag > PLAYER_MAX_SPEED) {
      player.velocity =
          Vector2Scale(player.velocity, PLAYER_MAX_SPEED / velMag);
    }

    if (velMag < 0.01f) {
      player.velocity = Vector2(0, 0);
    }
  }

  player.position =
      Vector2Add(player.position, Vector2Scale(player.velocity, frametime));

  if (player.position.x < LEFT_EDGE_PADDING) {
    player.position.x = RIGHT_EDGE_PADDING;
  } else if (player.position.x > RIGHT_EDGE_PADDING) {
    player.position.x = LEFT_EDGE_PADDING;
  } else if (player.position.y < TOP_EDGE_PADDING) {
    player.position.y = BOTTOM_EDGE_PADDING;
  } else if (player.position.y > BOTTOM_EDGE_PADDING) {
    player.position.y = TOP_EDGE_PADDING;
  }

  player.facingDirection = Vector2Subtract(mousePos, player.position);
}

void DrawPlayer(Player &player) {
  float rotation =
      90 - Vector2Angle(player.facingDirection, Vector2(0, 1)) * RAD2DEG;
  DrawPoly(player.position, 3, 16, rotation, WHITE);
}
