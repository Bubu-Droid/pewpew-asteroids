#include "asteroids.h"
#include "raylib.h"
#include "raymath.h"
#include <array>
#include <print>
#include <random>

#define NEARBLACK CLITERAL(Color){15, 15, 15, 255}

std::random_device rd;
std::mt19937 gen(rd());

const int screenWidth = 600;
const int screenHeight = 600;
const Vector2 screenCenter = {screenWidth / 2.0f, screenHeight / 2.0f};

const int MAX_ASTEROIDS = 64;
const float ASTEROID_RANDOM_ANGLE = 30 * DEG2RAD;
static bool showCone = true;

const int AST_VEL_MIN = 100;
const int AST_VEL_MAX = 300;

const std::array<AsteroidSize, 3> AST_SIZE_ARR = {
    ASTEROID_SMALL, ASTEROID_MEDIUM, ASTEROID_LARGE};

Vector2 line0[2];
Vector2 line1[2];

std::uniform_int_distribution<int> indexChoice(0, 2);
std::uniform_int_distribution<int> velChoice(AST_VEL_MIN, AST_VEL_MAX);
std::uniform_real_distribution<float> angleNoise(-ASTEROID_RANDOM_ANGLE,
                                                 ASTEROID_RANDOM_ANGLE);

std::array<Asteroid, MAX_ASTEROIDS> asteroids{};

void UpdateDrawFrame(void);

int main() {
  InitWindow(screenWidth, screenHeight, "Raylib Asteroids");

  while (!WindowShouldClose()) {
    UpdateDrawFrame();
  }

  CloseWindow();
}

void UpdateDrawFrame(void) {

  float frametime = GetFrameTime();

  for (auto &asteroid : asteroids) {
    UpdateAsteroid(asteroid, frametime);
  }

  if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
    AddAsteroid(GetMousePosition(), AST_SIZE_ARR[indexChoice(gen)]);
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

  EndDrawing();
}

void DrawAsteroid(Asteroid &asteroid) {
  DrawPolyLines(asteroid.position, 3, 16 * asteroid.asteroidSize,
                asteroid.rotationAngle, WHITE);
}

void AddAsteroid(Vector2 position, AsteroidSize size) {
  bool created = false;

  for (auto &asteroid : asteroids) {
    if (asteroid.active) {
      continue;
    }

    Vector2 velocity = Vector2Subtract(screenCenter, position);
    Vector2 normVelocity = Vector2Normalize(velocity);
    velocity = Vector2Scale(normVelocity, velChoice(gen));

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

    velocity = Vector2Rotate(velocity, angleNoise(gen));

    asteroid = CreateAsteroid(position, size, velocity);
    created = true;
    break;
  }

  if (!created) {
    TraceLog(LOG_ERROR, "Failed to create an asteroid because there was no "
                        "inactive spots in the array!");
  }
}
