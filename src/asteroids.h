#pragma once

#include "raylib.h"

enum AsteroidSize : int {
  ASTEROID_SMALL = 1,
  ASTEROID_MEDIUM = 2,
  ASTEROID_LARGE = 4,
};

struct Asteroid {
  bool active;
  Vector2 position;
  AsteroidSize asteroidSize;
  float rotationAngle;
  float rotationSpeed;
  Vector2 velocity;

  Asteroid() : active(false) {}

  Asteroid(bool active, Vector2 position, AsteroidSize asteroidSize,
           float rotationAngle, float rotationSpeed, Vector2 velocity)
      : active(active), position(position), asteroidSize(asteroidSize),
        rotationAngle(rotationAngle), rotationSpeed(rotationSpeed),
        velocity(velocity) {}
};

const float AST_ROT_SPEED_MIN = 5.0f;
const float AST_ROT_SPEED_MAX = 240.0f;

Asteroid CreateAsteroid(Vector2 position, AsteroidSize size, Vector2 velocity);

void UpdateAsteroid(Asteroid &asteroid, float &frametime);
void DrawAsteroid(Asteroid &asteroid);
void AddAsteroid(Vector2 position, AsteroidSize size);
