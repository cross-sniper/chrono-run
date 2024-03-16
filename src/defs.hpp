#ifndef DEFS_HPP
#define DEFS_HPP
#include "imgui/rlImGui.cpp"
#include "timer.cpp"
#include <cmath>
#include <imgui.h>
#include <raylib.h>
#include <vector>
typedef struct Entity {
  Vector2 pos;  // entity position
  Color color;  // entity color
  Vector2 size; // entity size
  double reloadTime = 30;
  double bullets = 1;
  double hp = 20;           // entity health
  double resistance = 1.5f; // entity armor
  double damage = 10;       // entity damage
  int iframe = 0;
  int iframeMax = 240; // 240 frames without taking damage
  double maxHp = 20;
  void draw();
} Entity;
Entity player;
bool operator==(const Entity &lhs, const Entity &rhs) {
  // Compare the relevant attributes of the Entity objects
  return lhs.pos.x == rhs.pos.x && lhs.pos.y == rhs.pos.y &&
         lhs.color.r == rhs.color.r && lhs.color.g == rhs.color.g &&
         lhs.color.b == rhs.color.b && lhs.size.x == rhs.size.x &&
         lhs.size.y == rhs.size.y && lhs.hp == rhs.hp &&
         lhs.resistance == rhs.resistance && lhs.damage == rhs.damage &&
         lhs.iframe == rhs.iframe && lhs.iframeMax == rhs.iframeMax;
}

struct _GameVars {
public:
  int points;
  int tries;
  int maxScore;
};
_GameVars GameVars; // why not?

typedef struct Upgrade {
  const char *name;
  void (*draw)();
  // Add more properties as needed
} Upgrade;
std::vector<Entity> Entities;

std::vector<Upgrade> Upgrades;
#endif
