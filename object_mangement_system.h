//---------------------------------------------------------
// file:    object_management_system.h
//
// author: Primary: Joe Centeno
//         Secondary: 
//          Xiao Ke Xiong
//          Jennifer Inn
//
// email:  Primary: joe.centeno@digipen.edu
//         Secondary:
//          xiaoke.xiong@digipen.edu
//          hannah.i@digipen.edu
//
// All content © 2021 DigiPen (USA) Corporation,
// all rights reserved.
//---------------------------------------------------------

#include "cprocessing.h"

#pragma once


#define MAX_OBJECTS 80
#define MAX_SHARKS 20
#define MAX_JELLYFISH 60
#define MAX_SHELLS 40


void enemyWaveSystem(void);

int creatSharkEnemy();

int score;

CP_Vector enemySpawnPos;
CP_Vector shellSpawnPos;
CP_Vector jellyFishSpawnPos;
CP_Vector left;

CP_Vector sharkSize;
CP_Vector jellyFishSize;
CP_Vector shellSize;

float enemySpeed;
int enemylimitPos;

float randomFloat;

CP_Sound hurt1;
CP_Sound hitshell;
CP_Sound hitjellyfish;
CP_Sound gameoversound;
CP_Sound soundTrack;

CP_Image enemySprite;
CP_Image shellSprite;
CP_Image jellyFishSprite;

int enemyWaveLimit;
float timeLimit;
