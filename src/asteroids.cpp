#include "asteroids.h"
#include "raymath.h"
#include <memory>

Asteroid CreateAsteroid(Vector2 position, Vector2 velocity, AsteroidSize size,
                        float rotationAngle, float rotationSpeed) {
  std::unique_ptr<Asteroid> Ast = std::make_unique<Asteroid>(
      true, position, velocity, size, rotationAngle, rotationSpeed);
  return *Ast;
}

void UpdateAsteroid(Asteroid &asteroid, float &frametime, float &time) {
  if (time > asteroid.creationTime + AST_LIFETIME) {
    asteroid.active = false;
  }

  if (!asteroid.active) {
    return;
  }

  asteroid.position =
      Vector2Add(asteroid.position, Vector2Scale(asteroid.velocity, frametime));
  asteroid.rotationAngle += asteroid.rotationSpeed * frametime;
}
