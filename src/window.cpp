// window.cpp
#include "defs.hpp"
void drawWindows() {
  rlImGuiBegin();
  ImGui::Begin("info");
#ifdef DEBUG_BUILD
  // Display player information
  ImGui::Text("Player Position: (%.1f, %.1f)", player.pos.x, player.pos.y);
  ImGui::Text("Player Health: %.1f", player.hp);
  ImGui::Text("Player Iframe: %d", player.iframe);
  ImGui::Text("Player bullets: %f", player.bullets);
#endif
  ImGui::Text("You can quit by pressing \"ESC\"");
  ImGui::Text("chrono-danger is still in development");
  for (Upgrade up : Upgrades) {
    up.draw();
  }
  ImGui::End();
  rlImGuiEnd();
}
