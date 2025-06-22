/*

Flappy Bird Remake for 3DS | v1.0
	(as first attempt to make homebrew game)

by MillKeny

Started 19.06.2025
Last Updated 23.06.2025

*/

#include <citro2d.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "fs.h"
#include "sprites.h"
#include "input.h"

#define WIDTH 400
#define BOTTOM_WIDTH 320
#define HEIGHT 240

#define GROUND 70

void setMedal(int score, int* medal){
		if (score == 99) *medal = 4;
		else if (score >= 75) *medal = 3;
		else if (score >= 50) *medal = 2;
		else if (score >= 25) *medal = 1;
		else if (score < 25) *medal = 0;
}

//---------------------------------------------------------------------------------
int main(int argc, char* argv[]) {
//---------------------------------------------------------------------------------
	romfsInit();
	gfxInitDefault();
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
	C2D_Prepare();

	initSd();
	initSprites();

	C3D_RenderTarget* top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
	C3D_RenderTarget* bottom = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);

	u32 clrClear = C2D_Color32(222, 215, 152, 255);

	int birdx = 50;
	int birdy = 226;

	int birdanim = 0; 
	int birdframe = 0;
	int birdanimstep = 0;
	int birdsanimspeed = 5;

	float vely = 0.0f;
	float g = 0.2f;
	float jumpStrength = 4.6f;
	bool birdDied = false;

	float speed = 2.0f;
	bool stopGame = false;
	bool inReady = true;
	bool inMenu = false;

	int score = 0;
	int best = readHighscore();
	int medal = 0;
	int bestmedal = 0;
	bool isScored = false;
	bool newBest = false;

	int fade = 255;
	float smenuanim = 0.5f;

	if (best > 99) yhyn();
	
//---------------------------------------------------------------------------------

	while (aptMainLoop())
	{
		speed = 2.0f+((float)score/45);

		updateInput();

		if (kDown & KEY_START)
			break;
		else if ((jumpButtons || kDown & KEY_TOUCH) && !stopGame)
			vely = -jumpStrength;

		if ((jumpButtons || kDown & KEY_TOUCH) && inReady && !checkTouch(menubttn, 0) && !inMenu) {
			inReady = false;
			smenuanim = 0.5f;
		}


		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);

// -----------------------------Top Screen-----------------------------------------

		C2D_TargetClear(top, clrClear);
		C2D_SceneBegin(top);

		if(stopGame) {
			if (!birdDied){
				vely = -jumpStrength/1.25f;
				birdDied = true;
			}
			C2D_Fade(C2D_Color32(255, 255, 255, fade));
		}
		
		C2D_DrawSprite(&bg);

		// Plumbs System, Collision check and Score System
		for (size_t i = 0; i < 4; i++){
			if(!stopGame && !inReady) C2D_SpriteMove(&pl[i], 0, -speed);
			C2D_DrawSprite(&pl[i]);

			if (pl[i].params.pos.y < 0) generatePlumb(i, false);

			if(
				(birdx+10 > pl[i].params.pos.y - 52) &&
				(birdx-10 < pl[i].params.pos.y)
			){
				if(i % 2 == 0){
					if (birdy+10 > pl[i].params.pos.x) stopGame = true;
				} else {
					if (birdy-10 < pl[i].params.pos.x) stopGame = true;
				}
			}

			if (birdx >= pl[i].params.pos.y - 26 && birdx < pl[i].params.pos.y && !isScored && score < 99 && !stopGame && !inReady) {
				score++;
				isScored = true;
			} else if (birdx > pl[i].params.pos.y+10 && isScored)
			{
				isScored = false;
			}

			// C2D_DrawCircleSolid(pl[i].params.pos.x, pl[i].params.pos.y+10, 0, 3, C2D_Color32(255, 0, 0, 255));
			
		}

		// Bird
		birdsanimspeed = 6-abs((int)vely);

		if (birdanimstep < birdsanimspeed) birdanimstep++;
		else birdanimstep = 1;

		if (birdanimstep % birdsanimspeed == 0){
			if (birdanim < 3 && !stopGame) birdanim++;
			else birdanim = 0;

			if (birdanim == 3) birdframe = 1;
			else birdframe = birdanim;
		}
		
		if(birdy > GROUND && !inReady){
			vely += g;
			birdy -= vely;

			C2D_SpriteSetRotationDegrees(&bird[birdframe], 90+vely*10);
		}

		if (inReady) {
			C2D_DrawSprite(&credit);
			
			C2D_SpriteSetRotationDegrees(&bird[birdframe], 90);
			vely = 0;

			C2D_DrawSprite(&logo);
			
			if (smenuanim < 1.0f) smenuanim += 0.014f;

			C2D_SpriteSetPos(&smenu2, (InOutEase(smenuanim)-0.5)*300-24, HEIGHT/2);
			C2D_DrawSprite(&smenu2);

			if (smenuanim >= 1.0f){
				C2D_SpriteSetPos(&medals[bestmedal-1], 137, 85);
				if (bestmedal > 0) C2D_DrawSprite(&medals[bestmedal-1]);

				showDigits(best, 117, 159, 1);
			}
		}

		C2D_SpriteSetPos(&bird[birdframe], birdy, birdx);
		C2D_DrawSprite(&bird[birdframe]);
		
		// C2D_DrawCircleSolid(birdy, birdx, 0, 3, C2D_Color32(255, 0, 0, 255));

		if (birdy <= GROUND) stopGame = true;

		// Floor Loop
		if(!stopGame) C2D_SpriteMove(&fl, 0, -speed);
		C2D_DrawSprite(&fl);

		if (fl.params.pos.y <= -HEIGHT) C2D_SpriteSetPos(&fl, 56, 0);

		// Show Score 
		if (!inReady) showDigits(score, 350, HEIGHT/2, 2);
		
		if(stopGame) {
			C2D_SpriteSetPos(&go, 290, HEIGHT/2);
			C2D_DrawSprite(&go);
		}


// -----------------------------Bottom Screen--------------------------------------


		C2D_TargetClear(bottom, clrClear);
		C2D_SceneBegin(bottom);
		C2D_Fade(C2D_Color32(255, 255, 255, 0));

		if (inReady) {
			setMedal(best, &bestmedal);

			if (!inMenu) {
				C2D_DrawSprite(&gr);
				C2D_DrawSprite(&menubttn);
			} else {
				C2D_DrawSprite(&ok);
				
				C2D_DrawSprite(&clear);

				if (checkTouch(clear, 5)) {
					writeHighscore((u32)0);
					best = readHighscore();
					setMedal(best, &bestmedal);
					inMenu = false;
				}
			}
		
		
			if (checkTouch(menubttn, 0) && kDown & KEY_TOUCH) {
				if (!inMenu) inMenu = true;
				else inMenu = false;
			}
		}

		if(!stopGame && !inMenu) {
			C2D_DrawSprite(&tap);
			fade = 255;
		}

		if(stopGame) {
			setMedal(score, &medal);

			int fadeSpeed = (256-(fade-10))/3;
			if (fade-fadeSpeed > 0) fade -= fadeSpeed;
			else fade = 0;

			if (smenuanim < 1.0f) smenuanim += 0.018f;

			C2D_SpriteSetPos(&go, 250, HEIGHT/2);
			C2D_DrawSprite(&go);

			if (score > best) {
				best = score;
				newBest = true;
				writeHighscore((u32)best);
			}

			C2D_SpriteSetPos(&smenu, (InOutEase(smenuanim)-0.5)*432-57, HEIGHT/2);
			C2D_DrawSprite(&smenu);

			if (smenuanim >= 1.0f){
				showDigits(score, 133+42, 200, 1);
				showDigits(best, 133, 200, 1);
				
				if (newBest) C2D_DrawSprite(&news);
				
				C2D_SpriteSetPos(&medals[medal-1], 174, 55);
				if (medal > 0) C2D_DrawSprite(&medals[medal-1]);

				C2D_DrawSprite(&ok);

				if (checkTouch(ok, 0) || jumpButtons
				) {
					for (size_t i = 0; i < 4; i++) generatePlumb(i, true);
					birdy = 226;
					score = 0;
					stopGame = false;
					inReady = true;
					newBest = false;
					isScored = false;
					smenuanim = 0.5f;
					birdDied = false;
				}
			}
		}

		C2D_DrawSprite(&exittext);

		C3D_FrameEnd(0);
	}
	
	C2D_SpriteSheetFree(spriteSheet);
	C2D_Fini();
	C3D_Fini();
	gfxExit();
	romfsExit();
	exitSd();

	return 0;
}