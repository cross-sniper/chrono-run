#include "defs.hpp"

void Entity::draw() {
    // Calculate the ratio of current HP to max HP
    float hpRatio = static_cast<float>(hp) / maxHp;

    // Calculate the length of the HP bar relative to the entity size
    float hpBarLength = hpRatio * size.x;

    if (iframe > 0) {
        DrawRectangle(pos.x, pos.y + size.y + 10, hpBarLength, 10, {40, 60, 200, 255});
        DrawRectangle(pos.x, pos.y, size.x, size.y, color);
        DrawText("invulnerable", pos.x, pos.y, 20, BLUE);
    } else {
        DrawRectangle(pos.x, pos.y + size.y + 10, hpBarLength, 10, {40, 60, 200, 255});
        DrawRectangle(pos.x, pos.y, size.x, size.y, color);
    }
}
