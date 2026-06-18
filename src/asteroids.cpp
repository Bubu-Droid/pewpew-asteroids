#include "asteroids.h"

#include "constants.h"
#include "raymath.h"

Asteroid CreateAsteroid(
    Vector2 position,
    Vector2 velocity,
    AsteroidSize size,
    float rotationAngle,
    float rotationSpeed
) {
  Asteroid Ast =
      Asteroid(true, position, velocity, size, rotationAngle, rotationSpeed);
  return Ast;
}

void UpdateAsteroid(Asteroid& asteroid, float& frametime, float& time) {
  if (time > asteroid.creationTime + AST_LIFETIME) {
    asteroid.active = false;
    return;
  }

  if (!asteroid.active) {
    return;
  }

  asteroid.position =
      Vector2Add(asteroid.position, Vector2Scale(asteroid.velocity, frametime));
  asteroid.rotationAngle += asteroid.rotationSpeed * frametime;
}

void DrawAsteroid(Asteroid& asteroid) {
  if (asteroid.active) {
    DrawPolyLines(
        asteroid.position,
        6,
        16 * asteroid.asteroidSize,
        asteroid.rotationAngle,
        WHITE
    );
  }
}

Vector2 GetNextAsteroidPosition(int astDir) {
  float screenRadius = std::sqrt(
      std::pow((SCREEN_HEIGHT / 2.0f), 2) + std::pow((SCREEN_WIDTH / 2.0f), 2)
  );
  Vector2 newAstPos = Vector2Rotate(Vector2(1, 0), astDir);
  newAstPos = Vector2Scale(newAstPos, (screenRadius + AST_SPAWN_PADDING));
  newAstPos = Vector2Add(newAstPos, SCREEN_CENTER);
  return newAstPos;
}

void AddAsteroid(
    std::array<Asteroid, MAX_ASTEROIDS>& asteroids,
    Vector2 position,
    AsteroidSize size,
    int randVelocity,
    float randAngleNoise,
    int randDirection,
    float randRotationSpeed,
    std::array<Vector2, 2>& line0,
    std::array<Vector2, 2>& line1
) {
  for (auto& asteroid : asteroids) {
    if (asteroid.active) {
      continue;
    }

    Vector2 velocity = Vector2Subtract(SCREEN_CENTER, position);
    Vector2 normVelocity = Vector2Normalize(velocity);
    velocity = Vector2Scale(normVelocity, randVelocity);

    if (showCone) {
      line0[0] = position;
      line1[0] = position;

      line0[1] = Vector2Add(
          position,
          Vector2Rotate(
              Vector2Scale(normVelocity, 1000),
              -ASTEROID_RANDOM_ANGLE
          )
      );
      line1[1] = Vector2Add(
          position,
          Vector2Rotate(Vector2Scale(normVelocity, 1000), ASTEROID_RANDOM_ANGLE)
      );
    }

    velocity = Vector2Rotate(velocity, randAngleNoise);

    asteroid = CreateAsteroid(
        position,
        velocity,
        size,
        randDirection,
        randRotationSpeed
    );
    return;
  }

  TraceLog(
      LOG_ERROR,
      "Failed to create an asteroid because there was no "
      "inactive spot in the array!"
  );
}
