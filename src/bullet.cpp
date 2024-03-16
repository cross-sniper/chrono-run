#include "defs.hpp"
#ifndef BULLET
#define BULLET
class Bullet {
private:
  Vector2 position;
  Vector2 speed;
  float radius;
  Color color;
  int hp = 120;

public:
  const char *type = "Normal";
  int pierce = 0;
  // Constructor
  void init(Vector2 startPos, Vector2 bulletSpeed, float bulletRadius,
            Color bulletColor) {
    this->position = startPos;
    this->speed = bulletSpeed;
    this->radius = bulletRadius;
    this->color = bulletColor;
  }
  int getHp() { return hp; };
  // Update bullet position based on its speed
  void move() {
    position.x += speed.x * 5;
    position.y += speed.y * 5;
    hp--;
  }

  // Draw bullet on screen
  void draw() { DrawCircle(position.x, position.y, radius, color); }

  // Get bullet position
  Vector2 getPosition() const { return position; }

  // Get bullet radius
  float getRadius() const { return radius; }

  // Check if bullet is outside the screen bounds
  bool isOutsideBounds(int screenWidth, int screenHeight) const {
    return (position.x < 0 || position.x > screenWidth || position.y < 0 ||
            position.y > screenHeight);
  }

  // Check if bullet collides with a rectangular entity
  bool collidesWithEntity(const Entity &entity) const {
    Rectangle bulletRect = {position.x - radius, position.y - radius,
                            radius * 2, radius * 2};
    Rectangle entityRect = {entity.pos.x, entity.pos.y, entity.size.x,
                            entity.size.y};
    return CheckCollisionRecs(bulletRect, entityRect);
  }
};

class ExplosiveBullet : public Bullet {
  Vector2 position;
  Vector2 speed;
  int hp = 200;
  double radius;
  Color color;

public:
  const char *type = "Explosive";
  void init(Vector2 startPos, Vector2 bulletSpeed, float bulletRadius,
            Color bulletColor) {
    this->position = startPos;
    this->speed = bulletSpeed;
    this->radius = bulletRadius;
    this->color = bulletColor;
  }
  // Update bullet position based on its speed
  void move() {
    position.x += speed.x * 5;
    position.y += speed.y * 5;
    if (speed.x * 0.5 > 0)
      speed.x *= 0.5;
    if (speed.y * 0.5 > 0)
      speed.y *= 0.5;

    if (hp - 1 == 0) {
      explode();
    }
    hp--;
  }
  void explode() {
    // Define the explosion radius (you may adjust this value as needed)
    float explosionRadius = radius * 30;

    // Iterate through all enemies
    for (Entity &enemy : Entities) {
      // Calculate the distance between the explosion position and the enemy's
      // position
      float distance = sqrt(pow(enemy.pos.x - position.x, 2) +
                            pow(enemy.pos.y - position.y, 2));

      // If the enemy is within the explosion radius, "kill" it
      if (distance <= explosionRadius) {
        // You might implement some logic here to remove or "kill" the enemy
        enemy.hp = 0; // For example, set enemy's HP to 0 to simulate killing
      }
    }
  }
  bool collidesWithEntity(const Entity &entity) const { return false; }
};

Bullet DefaultBullet;
Bullet ExplosiveBullet;

#endif
