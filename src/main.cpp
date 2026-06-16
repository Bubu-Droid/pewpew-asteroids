#define ASTEROIDS_ON 1
#define PLAYER_ON 1

#include <array>
#include <format>
#include <memory>
#include <random>

#include "asteroids.h"
#include "constants.h"
#include "player.h"
#include "raylib.h"
#include "raymath.h"

#if ASTEROIDS_ON == 1
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

#endif

static void UpdateDrawFrame(Player &player);

int main() {
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Raylib Asteroids");

  std::unique_ptr<Player> player = std::make_unique<Player>();

  while (!WindowShouldClose()) {
    UpdateDrawFrame(*player);
  }

  CloseWindow();
}

static void UpdateDrawFrame(Player &player) {
  float frametime = GetFrameTime();
  float time = GetTime();
  Vector2 mousePos = GetMousePosition();

#if ASTEROIDS_ON == 1
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
#endif
#if PLAYER_ON == 1
  UpdatePlayer(player, mousePos, frametime);
#endif

  BeginDrawing();
  ClearBackground(NEARBLACK);

#if ASTEROIDS_ON == 1
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
    DrawRectangle(10, 10, 260, 52, Fade(BLACK, 0.6f));
    DrawText(std::format("ASTEROIDS: {}", count).c_str(), 20, 20, 32, WHITE);
  }
#endif
#if PLAYER_ON == 1
  DrawPlayer(player);

  if (showPlayerStats) {
    float playerSpeed = Vector2Length(player.velocity);
    float playerDir =
        90 - Vector2Angle(player.facingDirection, Vector2(0, 1)) * RAD2DEG;
    Vector2 playerPos = player.position;

    DrawRectangle(10, SCREEN_HEIGHT - 90, 350, 80, Fade(BLACK, 0.6f));
    DrawText(std::format("Speed: {}", playerSpeed).c_str(), 20,
             SCREEN_HEIGHT - 85, 20, WHITE);
    DrawText(std::format("Direction: {}", playerDir).c_str(), 20,
             SCREEN_HEIGHT - 60, 20, WHITE);
    DrawText(
        std::format("Position: ({}, {})", playerPos.x, playerPos.y).c_str(), 20,
        SCREEN_HEIGHT - 35, 20, WHITE);
  }
#endif

  EndDrawing();
}
