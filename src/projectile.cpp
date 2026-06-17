#include "projectile.h"
#include "constants.h"
#include "raylib.h"
#include "raymath.h"

#include <cmath>
#include <memory>

static const Rectangle SCREEN_RECT = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

Projectile CreateProjectile(Vector2 &position, float &rotation) {
  std::unique_ptr<Projectile> projectile =
      std::make_unique<Projectile>(true, position, rotation);

  return *projectile;
}

void UpdateProjectile(Projectile &projectile, float &frametime, float &time) {
  if (time > projectile.creationTime + PROJ_LIFETIME) {
    projectile.active = false;
    return;
  }
  if (!projectile.active) {
    return;
  }

  if (!CheckCollisionRecs(SCREEN_RECT,
                          {projectile.position.x, projectile.position.y,
                           PROJ_THICKNESS, PROJ_LENGTH})) {
    projectile.active = false;
    return;
  }

  Vector2 projectileDir = Vector2(PROJ_SPEED * std::cos(projectile.rotation),
                                  PROJ_SPEED * std::sin(projectile.rotation));

  projectile.position =
      Vector2Add(projectile.position, Vector2Scale(projectileDir, frametime));
}

void DrawProjectile(Projectile &projectile) {
  if (!projectile.active) {
    return;
  }

  Rectangle rect = {projectile.position.x, projectile.position.y,
                    PROJ_THICKNESS, PROJ_LENGTH};
  Vector2 origin = {rect.width / 2, rect.height / 2};
  DrawRectanglePro(rect, origin, 90 + projectile.rotation * RAD2DEG,
                   PROJECTILE_COLOR);
}

void AddProjectile(std::array<Projectile, MAX_PROJECTILE> &projectiles,
                   Vector2 position, float rotation) {

  for (auto &projectile : projectiles) {
    if (projectile.active) {
      continue;
    }

    projectile = CreateProjectile(position, rotation);
    return;
  }

  TraceLog(LOG_ERROR, "Failed to create an projectile because there was no "
                      "inactive spot in the array!");
}
