#ifndef SPRITES_CPP
#define SPRITES_CPP
#include <raylib.h>
Texture2D bg;
void initSprites(){
	Image img = LoadImage("images/bg.png");
	bg = LoadTextureFromImage(img);
}

void unload(){
	UnloadTexture(bg);
}
#endif
