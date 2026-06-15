#include "asteroids.h"
#include "raylib.h"
#include <array>

const int screenWidth = 600;
const int screenHeight = 600;
const Vector2 screenSize = {screenWidth, screenHeight};

const int MAX_ASTEROIDS = 64;

const std::array<Asteroid, MAX_ASTEROIDS> asteroids{};

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

  BeginDrawing();
  ClearBackground(RAYWHITE);
  EndDrawing();
}
