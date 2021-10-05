//---------------------------------------------------------
// file:    enemy_management_system.h
//
// author:  Joe Centeno
//
// email:   joe.centeno@digipen.edu
//
// brief:   header file: enemy_management_system vars
//
// documentation link:
// https://inside.digipen.edu/main/GSDP:GAM100/CProcessing
//
// Copyright ?2021 DigiPen, All rights reserved.
//---------------------------------------------------------

#include "cprocessing.h"

#pragma once


#define MAX_OBJECTS 50

void enemy_management_system_init(void);
void enemy_management_system_update(void);
void enemy_management_system_exit(void);

void enemyWaveSystem(void);

int creatSharkEnemy();


CP_Vector enemySpawnPos;
CP_Vector left;
float enemySpeed;
int enemylimitPos;

CP_Sound hurt1;

CP_Image enemySprite;

int enemyWaveLimit;
float timeLimit;
