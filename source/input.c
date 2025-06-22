#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <citro2d.h>

u32 kDown = 0;
u32 kHeld = 0;
u32 kUp = 0;

touchPosition touch;
bool jumpButtons;

void updateInput()
{
    hidScanInput();

    kDown = hidKeysDown();
    kHeld = hidKeysHeld();
    kUp = hidKeysUp();

    jumpButtons = kDown & KEY_B || kDown & KEY_UP || kDown & KEY_DOWN || kDown & KEY_LEFT || kDown & KEY_RIGHT;

    hidTouchRead(&touch);
}