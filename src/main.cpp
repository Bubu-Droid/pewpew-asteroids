#include "asteroids.h"
#include "constants.h"

#include <array>
#include <format>
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

const std::array<AsteroidSize, 3> AST_SIZE_ARR = {
    ASTEROID_SMALL, ASTEROID_MEDIUM, ASTEROID_LARGE};
std::array<Asteroid, MAX_ASTEROIDS> asteroids{};
float lastAstCreationTime = -1.0f;

std::array<Vector2, 2> line0;
std::array<Vector2, 2> line1;

static void UpdateDrawFrame();
void DrawAsteroid(Asteroid &asteroid);
Vector2 GetNextAsteroidPosition(int astDir);
void AddAsteroid(std::array<Asteroid, MAX_ASTEROIDS> &asteroids,
                 Vector2 position, AsteroidSize size, int randVelocity,
                 float randAngleNoise, int randDirection,
                 float randRotationSpeed, std::array<Vector2, 2> &line0,
                 std::array<Vector2, 2> &line1);

int main() {
  InitWindow(screenWidth, screenHeight, "Raylib Asteroids");

  while (!WindowShouldClose()) {
    UpdateDrawFrame();
  }

  CloseWindow();
}

static void UpdateDrawFrame() {
  float frametime = GetFrameTime();
  float time = GetTime();

  for (auto &asteroid : asteroids) {
    UpdateAsteroid(asteroid, frametime, time);
  }

  if (time > lastAstCreationTime + AST_CREATION_DELAY) {
    AddAsteroid(asteroids, GetNextAsteroidPosition(randDirection(gen)),
                AST_SIZE_ARR[randIndexChoice(gen)], randVelocity(gen),
                randAngleNoise(gen), randDirection(gen), randRotationSpeed(gen),
                line0, line1);
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
