//entity.cpp
#include "defs.hpp"

void Entity::draw(){
	// we need to ensure the hp bar stays relatively small, when compared to the entity size
	if(iframe>0){
		DrawRectangle(pos.x, pos.y + size.y+10, hp,10, {40,60,200,255});
		DrawRectangle(pos.x, pos.y, size.x,size.y, color);
		DrawText("invulnerable",pos.x,pos.y,20,BLUE);

	}else{
		DrawRectangle(pos.x, pos.y + size.y+10, hp,10, {40,60,200,255});
		DrawRectangle(pos.x, pos.y, size.x,size.y, color);
	}
}

