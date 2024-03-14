// main.cpp
#include "defs.hpp"
#include "entity.cpp"
#include "window.cpp"
#include <algorithm>
#include <raylib.h>
#include <vector>
Camera2D cam;

std::vector<Entity> Entities;
#include "bullet.cpp" // Include the Bullet class file

std::vector<Bullet> bullets; // Vector to store active bullets

void postUpdate() {
  std::vector<Entity> toDelete;

  // Check enemies with zero or negative health
  for (Entity &enemy : Entities) {
    if (enemy.hp <= 0) {
      toDelete.push_back(enemy);
    }
  }

  // Delete enemies marked for deletion
  for (const Entity &enemy : toDelete) {
    auto it = std::find(Entities.begin(), Entities.end(), enemy);
    if (it != Entities.end()) {
      Entities.erase(it);
    }
  }

  // Check if player's health is zero or below
  if (player.hp <= 0) {
    Entities.clear(); // Remove all remaining enemies
    if (GameVars.points > GameVars.maxScore)
      GameVars.maxScore = GameVars.points;
    GameVars.points = 0;
    player.hp = 30; // Reset player health
    GameVars.tries += 1;
    // Other game reset logic...
  }
}

void attack() {
  // Check if the player pressed the attack key (Spacebar)

  // Calculate direction vector from player to mouse cursor
  Vector2 direction = GetMousePosition();
  direction.x -= player.pos.x + cam.offset.x - (float)GetScreenWidth() / 2;
  direction.y -= player.pos.y + cam.offset.y - (float)GetScreenHeight() / 2;

  // Normalize direction vector
  float length = sqrt(direction.x * direction.x + direction.y * direction.y);
  direction.x /= length;
  direction.y /= length;

  // Calculate bullet position
  Vector2 bulletStartPosition = {player.pos.x + direction.x * player.size.x,
                                 player.pos.y + direction.y * player.size.y};

  // Create a bullet with the calculated position and direction
  Bullet bullet(bulletStartPosition, direction, 5.0f, WHITE);
  bullets.push_back(bullet);
}

void checkCollisions() {
  if (player.iframe > 0)
    return; // dont even loop over the enemies, since we are on iframes
  // Check collision between player and enemies
  for (Entity &enemy : Entities) {
    if (CheckCollisionRecs(
            {player.pos.x, player.pos.y, player.size.x, player.size.y},
            {enemy.pos.x, enemy.pos.y, enemy.size.x, enemy.size.y})) {
      // Collision detected, decrease player health
      player.hp -= enemy.damage - player.resistance;
      player.iframe = player.iframeMax;
      break; // prevents the player from dying in the first 10 seconds
    }
  }
}

void update() {
  if (IsKeyDown(KEY_W)) {
    player.pos.y -= 5;
  }
  if (IsKeyDown(KEY_S)) {
    player.pos.y += 5;
  }
  if (IsKeyDown(KEY_A)) {
    player.pos.x -= 5;
  }
  if (IsKeyDown(KEY_D)) {
    player.pos.x += 5;
  }
  if (IsKeyPressed(KEY_SPACE))
    attack();

  // Adjust the speed at which enemies move towards the player
  float enemySpeed = 3.5f; // You can adjust this value

  for (Entity &enemy : Entities) {
    // Calculate direction vector from enemy to player
    Vector2 direction = {player.pos.x - enemy.pos.x,
                         player.pos.y - enemy.pos.y};

    // Normalize direction vector
    float length = sqrt(direction.x * direction.x + direction.y * direction.y);
    direction.x /= length;
    direction.y /= length;
    // Move enemy towards the player
    enemy.pos.x += direction.x * enemySpeed;
    enemy.pos.y += direction.y * enemySpeed;
  }

  // Update and draw all active bullets
  for (auto it = bullets.begin(); it != bullets.end(); /* No increment */) {
    it->move();
    it->draw();

    // Check if bullet is outside bounds or collided with an enemy
    if (it->getHp() < 0) {
      it = bullets.erase(it); // Remove bullet if outside bounds
    } else {
      bool collided = false;
      for (Entity &enemy : Entities) {
        if (it->collidesWithEntity(enemy)) {
          collided = true;
          // Reduce enemy's health
          enemy.hp -= player.damage - enemy.resistance;
          GameVars.points += GetRandomValue(10, 20);
          break;
        }
      }
      if (collided) {
        it = bullets.erase(it); // Remove bullet if it collided with an enemy
      } else {
        ++it; // Move to the next bullet
      }
    }
  }
  checkCollisions();
  postUpdate();
}
void Spawn() {
  if (Entities.size() < 50) {
    // Randomly spawn enemies within the visible screen area
    Entity enemy = {
        {(float)GetRandomValue(cam.target.x - cam.offset.x,
                               cam.target.x + cam.offset.x),
         (float)GetRandomValue(cam.target.y - cam.offset.y,
                               cam.target.y + cam.offset.y)},
        RED,
        {20, 20} // Size of the enemy
    };
    Entities.push_back(enemy);
  }
}

int main() {
  GameVars.maxScore = 0;
  GameVars.tries = 0;
  GameVars.points = 0;
  // fullscreen
  InitWindow(0, 0, "chrono-danger v1");
  rlImGuiSetup(true);
  SetTargetFPS(60);
  // Initialize camera properties
  cam.target = (Vector2){0, 0}; // Initial target position
  cam.offset = (Vector2){0, 0}; // Initial offset
  cam.rotation = 0;             // Initial rotation
  cam.zoom = 1.0f;              // Initial zoom level
  Timer spawnTimer = Timer();

  spawnTimer.Interval([&]() { Spawn(); }, 2000);

  player = {{0, 0}, WHITE, {30, 30}, 30, 3, 4};
  while (not WindowShouldClose()) {
    cam.target = player.pos;
    cam.offset = {(float)GetScreenWidth() / 2,
                  (float)GetScreenHeight() /
                      2}; // Center the camera on the screen

    BeginDrawing();
    ClearBackground(BLACK);
    DrawFPS(0, 0);
    DrawText(TextFormat("Score: %i", GameVars.points), 0, 20, 20, WHITE);
    DrawText(TextFormat("Max Score: %i", GameVars.maxScore), 0, 40, 20, WHITE);
    DrawText(TextFormat("Runs: %i", GameVars.tries), 0, 60, 20, WHITE);
    BeginMode2D(cam);
    DrawCircle(GetMousePosition().x - cam.offset.x,
               GetMousePosition().y - cam.offset.y, 20, {150, 150, 150, 255});
    update();
    if (player.iframe > 0)
      player.iframe--;
    for (Entity enemy : Entities) {
      enemy.draw();
    }
    player.draw();
    EndMode2D();
    drawWindows();
    EndDrawing();
  }
  CloseWindow();
  return 0;
}
