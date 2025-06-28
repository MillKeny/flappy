#ifndef SPRITES_H
#define SPRITES_H

extern C2D_SpriteSheet spriteSheet;
extern C2D_Sprite bg, bird[3], fl, pl[4], go, ok, tap, gr, smenu, smenu2, news, exittext, logo, menubttn, clear, credit;
extern C2D_Sprite digits[10];
extern C2D_Sprite medals[4];

void initSprites();
void yhyn();

float random_number(float min, float max);
void generatePlumb(int i, bool init);
void showDigits(int nmber, int x, int y, float size);
float InOutEase(float t);
bool checkTouch(C2D_Sprite sprite, float offset, bool held);

#endif