#include "asteroids.h"
#include "raylib.h"
#include <memory>
#include <random>

Asteroid CreateAsteroid(const Vector2 &position, const AsteroidSize &size,
                        const Vector2 &velocity) {
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

void UpdateAsteroid(Asteroid &asteroid, float frametime) {
  asteroid.position =
      Vector2(asteroid.position.x + asteroid.velocity.x * frametime,
              asteroid.position.y + asteroid.position.y * frametime);

  asteroid.rotationAngle += asteroid.rotationSpeed * frametime;
}
