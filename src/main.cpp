#include <cmath>
#define ASTEROIDS_ON 1
#define PLAYER_ON 1
#define PROJECTILE_ON 1

#include <array>
#include <format>
#include <memory>
#include <print>
#include <random>

#include "asteroids.h"
#include "constants.h"
#include "player.h"
#include "projectile.h"
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

#if PROJECTILE_ON == 1
std::array<Projectile, MAX_PROJECTILE> projectiles{};
float lastProjCreationTime = -1.0f;
#endif

static void UpdateDrawFrame(Player &player);
static Texture2D playerTexture;

int main() {
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Raylib Asteroids");

  static std::unique_ptr<Player> player = std::make_unique<Player>();
  playerTexture =
      LoadTexture("/home/bubu/Desktop/asteroids/resources/textures/ship.png");

  while (!WindowShouldClose()) {
    UpdateDrawFrame(*player);
  }

  CloseWindow();
}

static void UpdateDrawFrame(Player &player) {
  float frametime = GetFrameTime();
  float time = GetTime();
  Vector2 mousePos = GetMousePosition();

  for (auto &asteroid : asteroids) {
    if (asteroid.active) {
      float asteroidRadius = 16 * asteroid.asteroidSize;
      for (auto &projectile : projectiles) {
        if (projectile.active) {
          float projRadius =
              std::sqrt(std::pow(PROJ_THICKNESS, 2) + std::pow(PROJ_LENGTH, 2));
          if (CheckCollisionCircles(projectile.position, projRadius,
                                    asteroid.position, asteroidRadius)) {
            asteroid.active = false;
            projectile.active = false;
          }
        }
      }
    }
  }

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
#if PROJECTILE_ON == 1
  for (auto &projectile : projectiles) {
    UpdateProjectile(projectile, frametime, time);

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
        time > lastProjCreationTime + PROJ_CREATION_DELAY) {
      float rotation = -Vector2Angle(player.facingDirection, Vector2(1, 0));
      AddProjectile(projectiles, player.position, rotation);
      lastProjCreationTime = time;
    }
  }

  if (showAmmo) {

    int ammoCount = 0;

    for (auto &projectile : projectiles) {
      if (projectile.active) {
        ammoCount++;
      }
    }

    DrawRectangle(8, 10, 107, 37, Fade(BLACK, 0.6f));
    DrawText(std::format("AMMO: {}", MAX_PROJECTILE - ammoCount).c_str(), 20,
             20, 20, WHITE);
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

  if (showCone && debug) {
    DrawLineV(line0[0], line0[1], RED);
    DrawLineV(line1[0], line1[1], BLUE);
  }

  int astCount = 0;
  if (showAstCount && debug) {
    for (auto &asteroid : asteroids) {
      if (asteroid.active) {
        astCount++;
      }
    }
    DrawRectangle(SCREEN_WIDTH - 180, 10, 170, 37, Fade(BLACK, 0.6f));
    DrawText(std::format("ASTEROIDS: {}", astCount).c_str(), SCREEN_WIDTH - 170,
             20, 20, WHITE);
  }
#endif
#if PROJECTILE_ON == 1
  for (auto &projectile : projectiles) {
    DrawProjectile(projectile);
  }
#endif
#if PLAYER_ON == 1
  DrawPlayer(player, playerTexture);

  if (showPlayerStats && debug) {
    float playerSpeed = Vector2Length(player.velocity);
    float playerDir =
        90 - Vector2Angle(player.facingDirection, Vector2(0, 1)) * RAD2DEG;
    Vector2 playerPos = player.position;

    DrawRectangle(10, SCREEN_HEIGHT - 90, 360, 80, Fade(BLACK, 0.6f));
    DrawText(std::format("SPEED: {}", playerSpeed).c_str(), 20,
             SCREEN_HEIGHT - 85, 20, WHITE);
    DrawText(std::format("DIRECTION: {}", playerDir).c_str(), 20,
             SCREEN_HEIGHT - 60, 20, WHITE);
    DrawText(
        std::format("POSITION: ({}, {})", playerPos.x, playerPos.y).c_str(), 20,
        SCREEN_HEIGHT - 35, 20, WHITE);
  }
#endif

  EndDrawing();
}
