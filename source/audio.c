/*

Thanks https://github.com/BlyZeDev for most of this code :)

*/ 

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include <3ds.h>
#include <citro2d.h>
#include <citro3d.h>

#include "audio.h"

sound sfxWing = { NULL, 0, SOUND_FORMAT_16BIT | SOUND_ONE_SHOT, 8, false };
sound sfxSwooshing = { NULL, 0, SOUND_FORMAT_16BIT | SOUND_ONE_SHOT, 9, false };
sound sfxPoint = { NULL, 0, SOUND_FORMAT_16BIT | SOUND_ONE_SHOT, 10, false };
sound sfxHit = { NULL, 0, SOUND_FORMAT_16BIT | SOUND_ONE_SHOT, 11, false };
sound sfxDie = { NULL, 0, SOUND_FORMAT_16BIT | SOUND_ONE_SHOT, 12, false };

void loadSound(char* path, sound* soundPtr)
{
	FILE* file = fopen(path, "rb");
	fseek(file, 0, SEEK_END);
	soundPtr->size = ftell(file);
	fseek(file, 0, SEEK_SET);

	soundPtr->buffer = linearAlloc(soundPtr->size);
    fread(soundPtr->buffer, 1, soundPtr->size, file);

	fclose(file);
}

void stopSound(sound* soundPtr)
{
	csndExecCmds(true);
	CSND_SetPlayState(soundPtr->channel, 0);
	csndExecCmds(true);
}

void playSound(sound* soundPtr)
{
	// stopSound(soundPtr);

	csndPlaySound(soundPtr->channel, soundPtr->flags, 48000, 1, 0, soundPtr->buffer, soundPtr->buffer, soundPtr->size);
	soundPtr->played = true;
}

void freeSound(sound* soundPtr)
{
	stopSound(soundPtr);

	memset(soundPtr->buffer, 0, soundPtr->size);
	GSPGPU_FlushDataCache(soundPtr->buffer, soundPtr->size);
	linearFree(soundPtr->buffer);
	soundPtr->buffer = NULL;
}

void initSfx(){
	loadSound("romfs:/sfx/die.bin", &sfxDie);
	loadSound("romfs:/sfx/hit.bin", &sfxHit);
	loadSound("romfs:/sfx/point.bin", &sfxPoint);
	loadSound("romfs:/sfx/swooshing.bin", &sfxSwooshing);
	loadSound("romfs:/sfx/wing.bin", &sfxWing);
}