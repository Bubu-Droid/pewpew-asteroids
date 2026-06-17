#pragma once

#include <array>

#include "raylib.h"

#define PROJECTILE_COLOR CLITERAL(Color) {245, 201, 207, 255}

const float PROJ_SPEED = 800.0f;
const float PROJ_LIFETIME = 4.0f;

const float PROJ_THICKNESS = 10.0f;
const float PROJ_LENGTH = 30.0f;

const int MAX_PROJECTILE = 1;
const float PROJ_CREATION_DELAY = 0.25f;

struct Projectile {
  bool active;

  Vector2 position;
  float rotation;

  float creationTime;

  Projectile() : active(false) {}

  Projectile(bool active, Vector2& position, float& rotation) :
      active(active),
      position(position),
      rotation(rotation),
      creationTime(GetTime()) {}

  ~Projectile() {
    TraceLog(LOG_INFO, "Projectile destroyed!");
  }
};

Projectile CreateProjectile(Vector2& position, float& rotation);
void UpdateProjectile(Projectile& projectile, float& frametime, float& time);
void DrawProjectile(Projectile& projectile);
void AddProjectile(
    std::array<Projectile, MAX_PROJECTILE>& projectiles,
    Vector2 position,
    float rotation
);
