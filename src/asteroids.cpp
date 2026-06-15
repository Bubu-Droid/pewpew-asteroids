#include "asteroids.h"
#include "raylib.h"
#include "raymath.h"
#include <memory>
#include <random>

Asteroid CreateAsteroid(Vector2 position, AsteroidSize size, Vector2 velocity) {
  std::random_device rd;
  std::mt19937 gen(rd());

  std::uniform_real_distribution<float> randRotationAngle(0.0f, 360.0f);
  std::uniform_real_distribution<float> randRotationSpeed(AST_ROT_SPEED_MIN,
                                                          AST_ROT_SPEED_MAX);

  std::unique_ptr<Asteroid> Ast =
      std::make_unique<Asteroid>(true, position, size, randRotationAngle(gen),
                                 randRotationSpeed(gen), velocity);
  return *Ast;
}

void UpdateAsteroid(Asteroid &asteroid, float &frametime) {
  if (!asteroid.active) {
    return;
  }

  asteroid.position =
      Vector2Add(asteroid.position, Vector2Scale(asteroid.velocity, frametime));
  asteroid.rotationAngle += asteroid.rotationSpeed * frametime;
}
