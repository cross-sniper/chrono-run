#include "defs.hpp"

class Bullet {
private:
    Vector2 position;
    Vector2 speed;
    float radius;
    Color color;
    int hp = 120;

public:
    // Constructor
    Bullet(Vector2 startPos, Vector2 bulletSpeed, float bulletRadius, Color bulletColor)
        : position(startPos), speed(bulletSpeed), radius(bulletRadius), color(bulletColor) {}
    int getHp(){return hp;};
    // Update bullet position based on its speed
    void move() {
        position.x += speed.x * 5;
        position.y += speed.y * 5;
        hp--;
    }

    // Draw bullet on screen
    void draw() {
        DrawCircle(position.x, position.y, radius, color);
    }

    // Get bullet position
    Vector2 getPosition() const {
        return position;
    }

    // Get bullet radius
    float getRadius() const {
        return radius;
    }

    // Check if bullet is outside the screen bounds
    bool isOutsideBounds(int screenWidth, int screenHeight) const {
        return (position.x < 0 || position.x > screenWidth || position.y < 0 || position.y > screenHeight);
    }

    // Check if bullet collides with a rectangular entity
    bool collidesWithEntity(const Entity& entity) const {
        Rectangle bulletRect = { position.x - radius, position.y - radius, radius * 2, radius * 2 };
        Rectangle entityRect = { entity.pos.x, entity.pos.y, entity.size.x, entity.size.y };
        return CheckCollisionRecs(bulletRect, entityRect);
    }
};
