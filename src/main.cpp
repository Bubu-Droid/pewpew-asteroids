#include <algorithm>
#define PLAYER_SCORE_ON 1
#define ASTEROIDS_ON 1
#define PLAYER_ON 1
#define PROJECTILE_ON 1

#include <array>
#include <chrono>
#include <cmath>
#include <format>
#include <memory>
#include <random>
#include <string>

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
std::uniform_real_distribution<float>
    randRotationSpeed(AST_ROT_SPEED_MIN, AST_ROT_SPEED_MAX);
std::uniform_real_distribution<float>
    randAngleNoise(-ASTEROID_RANDOM_ANGLE, ASTEROID_RANDOM_ANGLE);

const std::array<AsteroidSize, 3> AST_SIZE_ARR =
    {ASTEROID_SMALL, ASTEROID_MEDIUM, ASTEROID_LARGE};
std::array<Asteroid, MAX_ASTEROIDS> asteroids {};
float lastAstCreationTime = -1.0f;

std::array<Vector2, 2> line0;
std::array<Vector2, 2> line1;
#endif

#if PROJECTILE_ON == 1
std::array<Projectile, MAX_PROJECTILE> projectiles {};
float lastProjCreationTime = -1.0f;
#endif

static void UpdateDrawFrame(Player& player);
static void UpdateEndFrame(Player& player);
static Texture2D playerTexture;
static Texture2D backTexture;

static bool gameEnd = false;

static int playerScore = 0;
static int duration = 0;
static auto startPoint = std::chrono::high_resolution_clock::now();
auto start = std::chrono::time_point_cast<std::chrono::milliseconds>(startPoint)
                 .time_since_epoch()
                 .count();

int main() {
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Raylib Asteroids");

  static std::unique_ptr<Player> player = std::make_unique<Player>();

  backTexture = LoadTexture("../textures/back.png");
  playerTexture = LoadTexture("../textures/ship.png");

  while (!WindowShouldClose()) {
    DrawTexture(backTexture, 0, 0, WHITE);
    if (!gameEnd) {
      UpdateDrawFrame(*player);
    } else {
      UpdateEndFrame(*player);
    }
  }

  CloseWindow();
}

static void UpdateEndFrame(Player& player) {
  BeginDrawing();
  ClearBackground(NEARBLACK);

  float panelWidth = 340;
  float panelHeight = 300;
  float panelX = (SCREEN_WIDTH - panelWidth) / 2.0f;
  float panelY = (SCREEN_HEIGHT - panelHeight) / 2.0f;

  DrawRectangle(panelX, panelY, panelWidth, panelHeight, Fade(BLACK, 0.6f));

  std::string titleText = "LMAO DED!";
  int titleFontSize = 40;
  int titleWidth = MeasureText(titleText.c_str(), titleFontSize);
  DrawText(
      titleText.c_str(),
      (SCREEN_WIDTH - titleWidth) / 2,
      panelY + 30,
      titleFontSize,
      RED
  );

  int scoreFontSize = 20;
  std::string scoreStr = std::format("Score: {}", playerScore);
  int scoreWidth = MeasureText(scoreStr.c_str(), scoreFontSize);
  DrawText(
      scoreStr.c_str(),
      (SCREEN_WIDTH - scoreWidth) / 2,
      panelY + 100,
      scoreFontSize,
      WHITE
  );

  int timeFontSize = 20;
  std::string timeStr = std::format("Time: {}s", duration);
  int timeWidth = MeasureText(timeStr.c_str(), timeFontSize);
  DrawText(
      timeStr.c_str(),
      (SCREEN_WIDTH - timeWidth) / 2,
      panelY + 140,
      timeFontSize,
      WHITE
  );

  float btnWidth = 180;
  float btnHeight = 50;
  float btnX = (SCREEN_WIDTH - btnWidth) / 2;
  float btnY = panelY + 210;

  DrawRectangle(btnX, btnY, btnWidth, btnHeight, LIGHTGRAY);

  const char* btnText = "Play Again";
  int btnFontSize = 20;
  int btnTextWidth = MeasureText(btnText, btnFontSize);
  float btnTextX = btnX + (btnWidth - btnTextWidth) / 2.0f;
  float btnTextY = btnY + (btnHeight - btnFontSize) / 2.0f;

  DrawText(btnText, btnTextX, btnTextY, btnFontSize, BLACK);

  Rectangle btnCollisionRect = {btnX, btnY, btnWidth, btnHeight};

  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)
      && CheckCollisionPointRec(GetMousePosition(), btnCollisionRect)) {
    playerScore = 0;
    duration = 0;
    std::fill(asteroids.begin(), asteroids.end(), Asteroid());
    std::fill(projectiles.begin(), projectiles.end(), Projectile());
    player = *std::make_unique<Player>();
    startPoint = std::chrono::high_resolution_clock::now();
    start = std::chrono::time_point_cast<std::chrono::milliseconds>(startPoint)
                .time_since_epoch()
                .count();
    gameEnd = false;
  }

  EndDrawing();
}

static void UpdateDrawFrame(Player& player) {
  float frametime = GetFrameTime();
  float time = GetTime();
  Vector2 mousePos = GetMousePosition();

  for (auto& asteroid : asteroids) {
    if (asteroid.active
        && CheckCollisionCircles(
            asteroid.position,
            16 * asteroid.asteroidSize,
            player.position,
            PLAYER_RADIUS
        )) {
      gameEnd = true;
    }
  }

  for (auto& asteroid : asteroids) {
    if (asteroid.active) {
      float asteroidRadius = 16 * asteroid.asteroidSize;
      for (auto& projectile : projectiles) {
        if (projectile.active) {
          float projRadius =
              std::sqrt(std::pow(PROJ_THICKNESS, 2) + std::pow(PROJ_LENGTH, 2));
          if (CheckCollisionCircles(
                  projectile.position,
                  projRadius,
                  asteroid.position,
                  asteroidRadius
              )) {
            if (asteroid.asteroidSize == ASTEROID_LARGE) {
              playerScore += 1;
            } else if (asteroid.asteroidSize == ASTEROID_MEDIUM) {
              playerScore += 2;
            } else if (asteroid.asteroidSize == ASTEROID_SMALL) {
              playerScore += 3;
            }
            asteroid.active = false;
            projectile.active = false;
          }
        }
      }
    }
  }

#if ASTEROIDS_ON == 1
  for (auto& asteroid : asteroids) {
    UpdateAsteroid(asteroid, frametime, time);
  }

  if (time > lastAstCreationTime + AST_CREATION_DELAY) {
    AddAsteroid(
        asteroids,
        GetNextAsteroidPosition(randDirection(gen)),
        AST_SIZE_ARR[randIndexChoice(gen)],
        randVelocity(gen),
        randAngleNoise(gen),
        randDirection(gen),
        randRotationSpeed(gen),
        line0,
        line1
    );
    lastAstCreationTime = time;
  }
#endif

#if PROJECTILE_ON == 1
  for (auto& projectile : projectiles) {
    UpdateProjectile(projectile, frametime, time);

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)
        && time > lastProjCreationTime + PROJ_CREATION_DELAY) {
      float rotation = -Vector2Angle(player.facingDirection, Vector2(1, 0));
      AddProjectile(projectiles, player.position, rotation);
      lastProjCreationTime = time;
    }
  }
#endif

#if PLAYER_SCORE_ON == 1
  auto endTimePoint = std::chrono::high_resolution_clock::now();
  auto end =
      std::chrono::time_point_cast<std::chrono::milliseconds>(endTimePoint)
          .time_since_epoch()
          .count();

  duration = static_cast<int>(end - start) * 0.001f;
  int seconds = duration % 60;
  int minutes = duration / 60;

  std::string scoreString;

  if (playerScore < 10) {
    scoreString = std::format("00{} PTS", playerScore);
  } else if (playerScore < 100) {
    scoreString = std::format("0{} PTS", playerScore);
  } else {
    scoreString = std::format("{} PTS", playerScore);
  }

  std::string timeString;

  if (seconds < 10) {
    timeString = std::format("{}:0{}", minutes, seconds);
  } else {
    timeString = std::format("{}:{}", minutes, seconds);
  }
#endif

#if PLAYER_ON == 1
  UpdatePlayer(player, mousePos, frametime);
#endif

  BeginDrawing();
  ClearBackground(NEARBLACK);

#if ASTEROIDS_ON == 1
  for (auto& asteroid : asteroids) {
    DrawAsteroid(asteroid);
  }

  if (showCone && debug) {
    DrawLineV(line0[0], line0[1], RED);
    DrawLineV(line1[0], line1[1], BLUE);
  }
#endif

#if PROJECTILE_ON == 1
  for (auto& projectile : projectiles) {
    DrawProjectile(projectile);
  }
#endif

#if PLAYER_ON == 1
  DrawPlayer(player, playerTexture);
#endif

#if PROJECTILE_ON == 1
  if (showAmmo) {
    int ammoCount = 0;
    for (auto& projectile : projectiles) {
      if (projectile.active)
        ammoCount++;
    }

    std::string ammoStr = std::format("AMMO: {}", MAX_PROJECTILE - ammoCount);
    int ammoWidth = MeasureText(ammoStr.c_str(), 20);

    DrawRectangle(10, 10, ammoWidth + 24, 36, Fade(BLACK, 0.6f));
    DrawText(ammoStr.c_str(), 22, 18, 20, SKYBLUE);
  }
#endif

#if PLAYER_SCORE_ON == 1
  int scoreWidth = MeasureText(scoreString.c_str(), 20);
  int scoreX = (SCREEN_WIDTH - (scoreWidth + 24)) / 2;

  DrawRectangle(scoreX, 10, scoreWidth + 24, 36, Fade(BLACK, 0.6f));
  DrawText(scoreString.c_str(), scoreX + 12, 18, 20, GOLD);

  int timeWidth = MeasureText(timeString.c_str(), 20);
  int timeX = SCREEN_WIDTH - timeWidth - 34;

  DrawRectangle(timeX, 10, timeWidth + 24, 36, Fade(BLACK, 0.6f));
  DrawText(timeString.c_str(), timeX + 12, 18, 20, LIGHTGRAY);
#endif

#if ASTEROIDS_ON == 1
  if (showAstCount && debug) {
    int astCount = 0;
    for (auto& asteroid : asteroids) {
      if (asteroid.active)
        astCount++;
    }

    std::string astStr = std::format("ASTEROIDS: {}", astCount);
    int astWidth = MeasureText(astStr.c_str(), 20);
    int astX = SCREEN_WIDTH - astWidth - 34;

    DrawRectangle(astX, 56, astWidth + 24, 36, Fade(BLACK, 0.6f));
    DrawText(astStr.c_str(), astX + 12, 64, 20, ORANGE);
  }
#endif

#if PLAYER_ON == 1
  if (showPlayerStats && debug) {
    float playerSpeed = Vector2Length(player.velocity);
    float playerDir =
        90 - Vector2Angle(player.facingDirection, Vector2(0, 1)) * RAD2DEG;
    Vector2 playerPos = player.position;

    std::string speedStr = std::format("SPEED: {:.1f}", playerSpeed);
    std::string dirStr = std::format("DIRECTION: {:.1f}", playerDir);
    std::string posStr =
        std::format("POSITION: ({:.1f}, {:.1f})", playerPos.x, playerPos.y);

    int w1 = MeasureText(speedStr.c_str(), 20);
    int w2 = MeasureText(dirStr.c_str(), 20);
    int w3 = MeasureText(posStr.c_str(), 20);

    int maxW = w1;
    if (w2 > maxW)
      maxW = w2;
    if (w3 > maxW)
      maxW = w3;

    int statsHeight = 100;
    int statsY = SCREEN_HEIGHT - statsHeight - 10;

    DrawRectangle(10, statsY, maxW + 24, statsHeight, Fade(BLACK, 0.6f));
    DrawText(speedStr.c_str(), 22, statsY + 14, 20, LIME);
    DrawText(dirStr.c_str(), 22, statsY + 40, 20, LIME);
    DrawText(posStr.c_str(), 22, statsY + 66, 20, LIME);
  }
#endif

  EndDrawing();
}
