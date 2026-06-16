#include "asteroids.h"
#include "constants.h"
#include "raymath.h"
#include <array>
#include <cmath>
#include <format>
// #include <print>
#include <random>

std::random_device rd;
std::mt19937 gen(rd());

std::uniform_int_distribution<int> randIndexChoice(0, 2);
std::uniform_int_distribution<int> randDirection(0, 360);
std::uniform_int_distribution<int> randVelocity(AST_VEL_MIN, AST_VEL_MAX);
std::uniform_real_distribution<float> randRotationSpeed(AST_ROT_SPEED_MIN,
                                                        AST_ROT_SPEED_MAX);
std::uniform_real_distribution<float> randAngleNoise(-ASTEROID_RANDOM_ANGLE,
                                                     ASTEROID_RANDOM_ANGLE);
std::array<Asteroid, MAX_ASTEROIDS> asteroids{};

float lastAstCreationTime = -1.0f;

const std::array<AsteroidSize, 3> AST_SIZE_ARR = {
    ASTEROID_SMALL, ASTEROID_MEDIUM, ASTEROID_LARGE};

Vector2 line0[2];
Vector2 line1[2];

void UpdateDrawFrame();
Vector2 GetNextAsteroidPosition();
void AddAsteroid(Vector2 position, AsteroidSize size);
void DrawAsteroid(Asteroid &asteroid);

int main() {
  InitWindow(screenWidth, screenHeight, "Raylib Asteroids");

  while (!WindowShouldClose()) {
    UpdateDrawFrame();
  }

  CloseWindow();
}

void UpdateDrawFrame() {

  float frametime = GetFrameTime();
  float time = GetTime();

  for (auto &asteroid : asteroids) {
    UpdateAsteroid(asteroid, frametime, time);
  }

  if (time > lastAstCreationTime + AST_CREATION_DELAY) {
    AddAsteroid(GetNextAsteroidPosition(), AST_SIZE_ARR[randIndexChoice(gen)]);
    lastAstCreationTime = time;
  }

  BeginDrawing();
  ClearBackground(NEARBLACK);

  for (auto &asteroid : asteroids) {
    DrawAsteroid(asteroid);
  }

  if (showCone) {
    DrawLineV(line0[0], line0[1], RED);
    DrawLineV(line1[0], line1[1], BLUE);
  }

  int count = 0;
  if (showAstCount) {
    for (auto &asteroid : asteroids) {
      if (asteroid.active) {
        count++;
      }
    }
    DrawRectangle(10, 10, 100, 52, Fade(BLACK, 0.0f));
    DrawText(std::format("ASTEROIDS: {}", count).c_str(), 20, 20, 32, WHITE);
  }

  EndDrawing();
}

void DrawAsteroid(Asteroid &asteroid) {
  if (asteroid.active) {
    DrawPolyLines(asteroid.position, 3, 16 * asteroid.asteroidSize,
                  asteroid.rotationAngle, WHITE);
  }
}

void AddAsteroid(Vector2 position, AsteroidSize size) {
  bool created = false;

  for (auto &asteroid : asteroids) {
    if (asteroid.active) {
      continue;
    }

    Vector2 velocity = Vector2Subtract(screenCenter, position);
    Vector2 normVelocity = Vector2Normalize(velocity);
    velocity = Vector2Scale(normVelocity, randVelocity(gen));

    if (showCone) {
      line0[0] = position;
      line1[0] = position;

      line0[1] =
          Vector2Add(position, Vector2Rotate(Vector2Scale(normVelocity, 1000),
                                             -ASTEROID_RANDOM_ANGLE));
      line1[1] =
          Vector2Add(position, Vector2Rotate(Vector2Scale(normVelocity, 1000),
                                             ASTEROID_RANDOM_ANGLE));
    }

    velocity = Vector2Rotate(velocity, randAngleNoise(gen));

    asteroid = CreateAsteroid(position, velocity, size, randDirection(gen),
                              randRotationSpeed(gen));
    created = true;
    break;
  }

  if (!created) {
    TraceLog(LOG_ERROR, "Failed to create an asteroid because there was no "
                        "inactive spots in the array!");
  }
}

Vector2 GetNextAsteroidPosition() {
  int astDir = randDirection(gen);
  float screenRadius = std::sqrt(std::pow((screenHeight / 2.0f), 2) +
                                 std::pow((screenWidth / 2.0f), 2));
  Vector2 newAstPos = Vector2Rotate(Vector2(1, 0), astDir);
  newAstPos = Vector2Scale(newAstPos, (screenRadius + AST_SPAWN_PADDING));
  newAstPos = Vector2Add(newAstPos, screenCenter);
  return newAstPos;
}
