// main.cpp
#include "defs.hpp"
#include "entity.cpp"
#include "window.cpp"
#include "upgrades.cpp"
#include <algorithm>
#include <imgui.h>
#include <raylib.h>
#include <vector>
#include "bullet.cpp" // Include the Bullet class file
#include "sprites.cpp"
Camera2D cam;

std::vector<Entity> Entities;

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
    player.hp = player.maxHp; // Reset player health
    GameVars.tries += 1;
    // Other game reset logic...
  }
}

// todo: modify this to allow future upgrades, like a shotgun upgrade
void attack() {
    // Check if the player pressed the attack key (Spacebar)

    // Get the position of the mouse cursor in world coordinates
    Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), cam);

    // Calculate direction vector from player to mouse cursor
    Vector2 direction = { mouseWorldPos.x - player.pos.x, mouseWorldPos.y - player.pos.y };

    // Normalize direction vector
    float length = sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length > 0) {
        direction.x /= length;
        direction.y /= length;
    }

    // Calculate bullet position
    Vector2 bulletStartPosition = { player.pos.x + direction.x * player.size.x,
                                    player.pos.y + direction.y * player.size.y };

    // Spread the bullets if required
    if (player.bullets > 1) {
        float angleIncrement = 20.0f / (player.bullets - 1);
        float currentAngle = -20.0f / 2.0f;
        for (int i = 0; i < player.bullets; ++i) {
            // Calculate direction for each bullet based on spread angle
            Vector2 bulletDirection = { direction.x * cos(DEG2RAD * currentAngle) - direction.y * sin(DEG2RAD * currentAngle),
                                        direction.x * sin(DEG2RAD * currentAngle) + direction.y * cos(DEG2RAD * currentAngle) };

            // Create a bullet with the calculated position and direction
            Bullet bullet=DefaultBullet;
            bullet.init(bulletStartPosition, bulletDirection, 5.0f, WHITE);
            bullets.push_back(bullet);

            // Increment angle for the next bullet
            currentAngle += angleIncrement;
        }
    } else {
        // Create a single bullet if no spread
        Bullet bullet = DefaultBullet;
        bullet.init(bulletStartPosition, direction, 5.0f, WHITE);
        bullets.push_back(bullet);
    }
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
	float player_speed = 3.0f;
    if (IsKeyDown(KEY_W)) {
        player.pos.y -= player_speed;
        if (player.pos.y < -2000) player.pos.y = -2000; // Constrain within bounds
    }
    if (IsKeyDown(KEY_S)) {
        player.pos.y += player_speed;
        if (player.pos.y > 2000) player.pos.y = 2000; // Constrain within bounds
    }
    if (IsKeyDown(KEY_A)) {
        player.pos.x -= player_speed;
        if (player.pos.x < -2000) player.pos.x = -2000; // Constrain within bounds
    }
    if (IsKeyDown(KEY_D)) {
        player.pos.x += player_speed;
        if (player.pos.x > 2000) player.pos.x = 2000; // Constrain within bounds
    }

  // Adjust the speed at which enemies move towards the player
  float enemySpeed = 2.5f; // You can adjust this value

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
        if(it->pierce >= 1){
          it->pierce--;
          ++it;
        }
        else{
          it = bullets.erase(it); // Remove bullet if it collided with an enemy
        }
      } else {
        ++it; // Move to the next bullet
      }
    }
  }
  checkCollisions();
  postUpdate();
}

// Function to calculate distance between two points
float CalculateDistance(float x1, float y1, float x2, float y2) {
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

void Spawn() {
    const int maxEnemies = 50;
    const float minDistanceToPlayer = 300.0f; // Minimum distance between enemy and player

    if (Entities.size() < maxEnemies) {
        float x, y;
        do {
            x = (float)GetRandomValue(cam.target.x - cam.offset.x, cam.target.x + cam.offset.x);
            y = (float)GetRandomValue(cam.target.y - cam.offset.y, cam.target.y + cam.offset.y);
        } while (CalculateDistance(player.pos.x, player.pos.y, x, y) < minDistanceToPlayer); // Ensure minimum distance from player

        Entity enemy = {
            {x, y},
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
  init();
  initSprites();

  rlImGuiSetup(true);
  SetTargetFPS(60);
  // Initialize camera properties
  cam.target = (Vector2){0, 0}; // Initial target position
  cam.offset = (Vector2){0, 0}; // Initial offset
  cam.rotation = 0;             // Initial rotation
  cam.zoom = 1.0f;              // Initial zoom level
  Timer spawnTimer = Timer();
  Timer shotTime = Timer();
  shotTime.setTimeout(player.reloadTime * 1000);
  spawnTimer.setTimeout(3000);// 3 seconds
  spawnTimer.Interval([&]() { Spawn(); });
  shotTime.Interval([&]() { attack(); });

  player = {{0, 0}, WHITE, {30, 30}, 3 ,0, 30};
  while (not WindowShouldClose()) {
    cam.target = player.pos;
    cam.offset = {(float)GetScreenWidth() / 2,
                  (float)GetScreenHeight() /
                      2}; // Center the camera on the screen
    shotTime.setTimeout(player.reloadTime * 1000);
    BeginDrawing();
    ClearBackground(BLACK);
    BeginMode2D(cam);
    DrawTexture(bg, -(int)(GetScreenWidth() / 2),-(int)(GetScreenHeight()/2),WHITE);
    update();
    if (player.iframe > 0)
      player.iframe--;
    for (Entity enemy : Entities) {
      enemy.draw();
    }
    player.draw();
    Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), cam);
    DrawCircle(mouseWorldPos.x, mouseWorldPos.y, 10, {255,255,255,100});

    EndMode2D();
    DrawFPS(0, 0);
    DrawText(TextFormat("Score: %i", GameVars.points), 0, 20, 20, WHITE);
    DrawText(TextFormat("Max Score: %i", GameVars.maxScore), 0, 40, 20, WHITE);
    DrawText(TextFormat("Runs: %i", GameVars.tries), 0, 60, 20, WHITE);
    drawWindows();
    EndDrawing();
  }
  unload();
  CloseWindow();
  return 0;
}
