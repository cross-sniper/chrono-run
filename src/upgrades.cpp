#include "bullet.cpp"

Upgrade hpUp = {
	"hp Upgrade - 20 points",
	[](){
		if(GameVars.points >= 20){
			if(ImGui::Button("Upgrade max hp")){
				player.maxHp += 30;
				GameVars.points-=20;
			}
		}
	}
};
Upgrade attackSpeedUp = {
	"attack speed Upgrade - 30 points",
	[](){
		if(GameVars.points >= 30 and player.reloadTime*0.4 > 0.3){
			if(ImGui::Button("Upgrade reload Time")){
				player.reloadTime*=0.4;
				GameVars.points-=30;
			}
		}
	}
};

Upgrade moreBullets = {
	"more bullets - 40 points",
	[](){
		if(GameVars.points >= 40 and player.bullets*2 < 20){
			if(ImGui::Button("2 times more bullets")){
				player.bullets*=2;
				GameVars.points-=40;
			}
		}
	}
};

Upgrade addPierce = {
	"more piere - 10 points",
	[](){
		if(GameVars.points >= 10 and DefaultBullet.pierce < 10){
			if(ImGui::Button("add more piercing")){
				DefaultBullet.pierce++;
				GameVars.points-=10;
			}
		}
	}
};

void init(){
	Upgrades.push_back(hpUp);
	Upgrades.push_back(attackSpeedUp);
	Upgrades.push_back(moreBullets);
	Upgrades.push_back(addPierce);
}
