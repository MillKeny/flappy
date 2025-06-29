#include <citro2d.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "input.h"

#define WIDTH 400
#define BOTTOM_WIDTH 320
#define HEIGHT 240

C2D_SpriteSheet spriteSheet;
C2D_Sprite bg, bird[3], fl, pl[4], go, ok, tap, gr, smenu, smenu2, news, exittext, logo, menubttn, clear, credit;
C2D_Sprite digits[10];
C2D_Sprite medals[4];

float random_number(float min, float max) {
    float difference = (max - min);
    return (((float)(difference + 1)/RAND_MAX) * rand() + min);
}

int hole = 100;
int pos = 50;

void generatePlumb(int i, bool init) {
	int second = 0;
	if(init == true) second = (i/2)*150;
	
	if (i % 2 == 0) {
		hole = random_number(90, 130);
		pos = random_number(-65, 40);
		C2D_SpriteSetPos(&pl[i], WIDTH-hole-pos, HEIGHT+52+second);
	}
	else {
		C2D_SpriteSetPos(&pl[i], 55+hole-pos, HEIGHT+52+second);
	}
}

void showDigits(int nmber, int x, int y, float size){
	int score10 = nmber / 10;
	int score1 = nmber - score10 * 10;

	C2D_SpriteSetScale(&digits[score1], size, size);
	C2D_SpriteSetScale(&digits[score10], size, size);

	C2D_SpriteSetCenter(&digits[score1], 0, 0.5f);
	C2D_SpriteSetPos(&digits[score1], x, y+(size-1));
	C2D_DrawSprite(&digits[score1]);

	C2D_SpriteSetCenter(&digits[score10], 0.5f, 0.5f);
	C2D_SpriteSetPos(&digits[score10], x, y-(6*size)-(size-1));
	C2D_DrawSprite(&digits[score10]);
}

float InOutEase(float t)
{
    if(t <= 0.5f)
        return 2.0f * t * t;
    t -= 0.5f;
    return 2.0f * t * (1.0f - t) + 0.5f;
}

bool checkTouch(C2D_Sprite sprite, float offset, bool held) {
	if (touch.px > sprite.params.pos.x+offset-sprite.params.pos.h/2 &&
		touch.px < sprite.params.pos.x-offset+sprite.params.pos.h/2 &&
		touch.py > sprite.params.pos.y+offset-sprite.params.pos.w/2 &&
		touch.py < sprite.params.pos.y-offset+sprite.params.pos.w/2
	) {
		if (!held) {
			if (kDown & KEY_TOUCH) return true;
		} else return true;
	}
	
	return false;
}

void initSprites(){
	// Init Medals Sprites
	spriteSheet = C2D_SpriteSheetLoad("romfs:/gfx/medals.t3x");
	if (!spriteSheet) svcBreak(USERBREAK_PANIC);

	for (size_t i = 0; i < 4; i++){
		C2D_SpriteFromSheet(&medals[i], spriteSheet, i);
		C2D_SpriteSetCenter(&medals[i], 0.5f, 0);
		C2D_SpriteSetScale(&medals[i], 2, 2);
		C2D_SpriteSetRotationDegrees(&medals[i], 90);
	}

	// Init Digits
    spriteSheet = C2D_SpriteSheetLoad("romfs:/gfx/digits.t3x");
	if (!spriteSheet) svcBreak(USERBREAK_PANIC);

	for (size_t i = 0; i < 10; i++){
		C2D_SpriteFromSheet(&digits[i], spriteSheet, i);
		C2D_SpriteSetRotationDegrees(&digits[i], 90);
	}

	// Init Sprites
	spriteSheet = C2D_SpriteSheetLoad("romfs:/gfx/sprites.t3x");
	if (!spriteSheet) svcBreak(USERBREAK_PANIC);

	// Background
	C2D_SpriteFromSheet(&bg, spriteSheet, 0);
	C2D_SpriteSetCenter(&bg, 0, 0);
	C2D_SpriteSetPos(&bg, WIDTH, 0);
	C2D_SpriteSetRotationDegrees(&bg, 90);
	C2D_SpriteSetScale(&bg, 2, 2);

	// Game Over Sign
	C2D_SpriteFromSheet(&go, spriteSheet, 7);
	C2D_SpriteSetScale(&go, 2, 2);
	C2D_SpriteSetCenter(&go, 0.5f, 0.5f);
	C2D_SpriteSetRotationDegrees(&go, 90);

	// Get Ready Sign
	C2D_SpriteFromSheet(&gr, spriteSheet, 12);
	C2D_SpriteSetPos(&gr, 246, HEIGHT/2);
	C2D_SpriteSetScale(&gr, 2, 2);
	C2D_SpriteSetCenter(&gr, 0.5f, 0.5f);
	C2D_SpriteSetRotationDegrees(&gr, 90);

	// Bird
	for (size_t i = 0; i < 3; i++){
		C2D_SpriteFromSheet(&bird[i], spriteSheet, i+1);
		C2D_SpriteSetCenter(&bird[i], 0.45f, 0.5f);
		C2D_SpriteSetRotationDegrees(&bird[i], 90);
		C2D_SpriteSetScale(&bird[i], 2, 2);
	}

	// Floor
	C2D_SpriteFromSheet(&fl, spriteSheet, 6);
	C2D_SpriteSetCenter(&fl, 0, 0);
	C2D_SpriteSetPos(&fl, 56, 0);
	C2D_SpriteSetRotationDegrees(&fl, 90);

	// Plumbs
	for (size_t i = 0; i < 4; i++){
		if (i % 2 == 0) {
			C2D_SpriteFromSheet(&pl[i], spriteSheet, 4);
			C2D_SpriteSetCenter(&pl[i], 1, 1);
		}
		else {
			C2D_SpriteFromSheet(&pl[i], spriteSheet, 5);
			C2D_SpriteSetCenter(&pl[i], 1, 0);
		}

		C2D_SpriteSetScale(&pl[i], 2, 2);
		C2D_SpriteSetRotationDegrees(&pl[i], 90);
		generatePlumb(i, true);
	}

	// OK Button
	C2D_SpriteFromSheet(&ok, spriteSheet, 8);
	C2D_SpriteSetScale(&ok, 2, 2);
	C2D_SpriteSetCenter(&ok, 0.5f, 0.5f);
	C2D_SpriteSetPos(&ok, 75, HEIGHT/2);
	C2D_SpriteSetRotationDegrees(&ok, 90);

	// Menu Button
	C2D_SpriteFromSheet(&menubttn, spriteSheet, 17);
	C2D_SpriteSetScale(&menubttn, 2, 2);
	C2D_SpriteSetCenter(&menubttn, 0.5f, 0.5f);
	C2D_SpriteSetPos(&menubttn, 75, HEIGHT/2);
	C2D_SpriteSetRotationDegrees(&menubttn, 90);

	// Tap Sign
	C2D_SpriteFromSheet(&tap, spriteSheet, 9);
	C2D_SpriteSetScale(&tap, 2, 2);
	C2D_SpriteSetCenter(&tap, 0.5f, 0.5f);
	C2D_SpriteSetPos(&tap, BOTTOM_WIDTH/2, HEIGHT/2);
	C2D_SpriteSetRotationDegrees(&tap, 90);

	// Score Menu
	C2D_SpriteFromSheet(&smenu, spriteSheet, 10);
	C2D_SpriteSetScale(&smenu, 2, 2);
	C2D_SpriteSetCenter(&smenu, 0.5f, 0.5f);
	C2D_SpriteSetRotationDegrees(&smenu, 90);

	// Score Menu 2
	C2D_SpriteFromSheet(&smenu2, spriteSheet, 14);
	C2D_SpriteSetPos(&smenu2, 120, HEIGHT/2);
	C2D_SpriteSetScale(&smenu2, 2, 2);
	C2D_SpriteSetCenter(&smenu2, 0.5f, 0.5f);
	C2D_SpriteSetRotationDegrees(&smenu2, 90);

	// New Best Sign
	C2D_SpriteFromSheet(&news, spriteSheet, 11);
	C2D_SpriteSetPos(&news, 151, 156);
	C2D_SpriteSetScale(&news, 2, 2);
	C2D_SpriteSetCenter(&news, 0.5f, 0.5f);
	C2D_SpriteSetRotationDegrees(&news, 90);

	// to Exit Sign
	C2D_SpriteFromSheet(&exittext, spriteSheet, 13);
	C2D_SpriteSetPos(&exittext, 13, HEIGHT/2);
	C2D_SpriteSetScale(&exittext, 2, 2);
	C2D_SpriteSetCenter(&exittext, 0.5f, 0.5f);
	C2D_SpriteSetRotationDegrees(&exittext, 90);

	// Flappy Bird Logo
	C2D_SpriteFromSheet(&logo, spriteSheet, 15);
	C2D_SpriteSetPos(&logo, 286, HEIGHT/2);
	C2D_SpriteSetScale(&logo, 2, 2);
	C2D_SpriteSetCenter(&logo, 0.5f, 0.5f);
	C2D_SpriteSetRotationDegrees(&logo, 90);

	// Clear Save Data Button
	C2D_SpriteFromSheet(&clear, spriteSheet, 18);
	C2D_SpriteSetPos(&clear, BOTTOM_WIDTH/2, HEIGHT/2);
	C2D_SpriteSetScale(&clear, 2, 2);
	C2D_SpriteSetCenter(&clear, 0.5f, 0.5f);
	C2D_SpriteSetRotationDegrees(&clear, 90);

	// Credits
	C2D_SpriteFromSheet(&credit, spriteSheet, 19);
	C2D_SpriteSetPos(&credit, WIDTH-45, HEIGHT/2);
	C2D_SpriteSetScale(&credit, 2, 2);
	C2D_SpriteSetCenter(&credit, 0.5f, 0.5f);
	C2D_SpriteSetRotationDegrees(&credit, 90);
}

void superBird() {
	spriteSheet = C2D_SpriteSheetLoad("romfs:/gfx/sprites.t3x");
	if (!spriteSheet) svcBreak(USERBREAK_PANIC);
	
	for (size_t i = 0; i < 3; i++){
		C2D_SpriteFromSheet(&bird[i], spriteSheet, i+20);
		C2D_SpriteSetCenter(&bird[i], 0.45f, 0.5f);
		C2D_SpriteSetRotationDegrees(&bird[i], 90);
		C2D_SpriteSetScale(&bird[i], 2, 2);
	}
}

void initNightSprites(){
	spriteSheet = C2D_SpriteSheetLoad("romfs:/gfx/night.t3x");
	if (!spriteSheet) svcBreak(USERBREAK_PANIC);

	// Background Night
	C2D_SpriteFromSheet(&bg, spriteSheet, 3);
	C2D_SpriteSetCenter(&bg, 0, 0);
	C2D_SpriteSetPos(&bg, WIDTH, 0);
	C2D_SpriteSetRotationDegrees(&bg, 90);
	C2D_SpriteSetScale(&bg, 2, 2);

	// Bird Night
	for (size_t i = 0; i < 3; i++){
		C2D_SpriteFromSheet(&bird[i], spriteSheet, i);
		C2D_SpriteSetCenter(&bird[i], 0.45f, 0.5f);
		C2D_SpriteSetRotationDegrees(&bird[i], 90);
		C2D_SpriteSetScale(&bird[i], 2, 2);
	}
}

void yhyn() {
	// Ը
	spriteSheet = C2D_SpriteSheetLoad("romfs:/gfx/sprites.t3x");
	if (!spriteSheet) svcBreak(USERBREAK_PANIC);

	for (size_t i = 0; i < 3; i++){
		C2D_SpriteFromSheet(&bird[i], spriteSheet, 16);
		C2D_SpriteSetCenter(&bird[i], 0.5f, 0.5f);
		C2D_SpriteSetRotationDegrees(&bird[i], 90);
		C2D_SpriteSetScale(&bird[i], 0.5f, 0.5f);
	}
}