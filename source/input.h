#ifndef INPUT_H
#define INPUT_H

extern u32 kDown;
extern u32 kHeld;
extern u32 kUp;

extern bool jumpButtons;

extern touchPosition touch;

void updateInput();

#endif