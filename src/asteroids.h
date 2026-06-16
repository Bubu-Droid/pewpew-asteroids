#pragma once

#include "raylib.h"

enum AsteroidSize : int {
  ASTEROID_SMALL = 1,
  ASTEROID_MEDIUM = 2,
  ASTEROID_LARGE = 4,
};

static const float AST_ROT_SPEED_MIN = 5.0f;
static const float AST_ROT_SPEED_MAX = 240.0f;

static const float AST_CREATION_DELAY = 0.5f;
static const float AST_LIFETIME = 8.0f;

static const int AST_VEL_MIN = 100;
static const int AST_VEL_MAX = 300;

struct Asteroid {
  bool active;
  Vector2 position;
  Vector2 velocity;

  AsteroidSize asteroidSize;
  float rotationAngle;
  float rotationSpeed;

  float creationTime;

  Asteroid() : active(false) {}

  Asteroid(bool active, Vector2 position, Vector2 velocity,
           AsteroidSize asteroidSize, float rotationAngle, float rotationSpeed)
      : active(active), position(position), velocity(velocity),
        asteroidSize(asteroidSize), rotationAngle(rotationAngle),
        rotationSpeed(rotationSpeed), creationTime(GetTime()) {}
};

Asteroid CreateAsteroid(Vector2 position, Vector2 velocity, AsteroidSize size,
                        float rotationAngle, float rotationSpeed);
void UpdateAsteroid(Asteroid &asteroid, float &frametime, float &time);
