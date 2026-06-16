#pragma once

#include <array>

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

static const int MAX_ASTEROIDS = 64;
static const float ASTEROID_RANDOM_ANGLE = 20 * DEG2RAD;
static const int AST_SPAWN_PADDING = 128;

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
void DrawAsteroid(Asteroid &asteroid);
Vector2 GetNextAsteroidPosition(int astDir);
void AddAsteroid(std::array<Asteroid, MAX_ASTEROIDS> &asteroids,
                 Vector2 position, AsteroidSize size, int randVelocity,
                 float randAngleNoise, int randDirection,
                 float randRotationSpeed, std::array<Vector2, 2> &line0,
                 std::array<Vector2, 2> &line1);
