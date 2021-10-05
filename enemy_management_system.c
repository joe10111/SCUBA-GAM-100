//---------------------------------------------------------
// file:    enemy_management_system.c
//
// author:  Collin Thomas
//          Joe Centeno
//          Jennifer Inn
//          Xiao Ke Xiong
//
// email:   collin.thomas@digipen.edu
//          joe.centeno@digipen.edu
//          hannah.i@digipen.edu
//          xiaoke.xiong@digipen.edu
//
// brief:   
//
// documentation link:
// https://inside.digipen.edu/main/GSDP:GAM100/CProcessing
//
// Copyright ?2021 DigiPen, All rights reserved.
//---------------------------------------------------------

#include "cprocessing.h"              /* C-Processing          */
#include "enemy_management_system.h"  /* Enemy Mangment Header */
#include "common.h"                   /* Common Header         */

int hit = 0;      /* Hit Bool  */
int hitTimer = 0; /* Hit Timer */
int HealthBlock = 0;  /* HealthBlock */
int HealthBlock1 = 0; /* HealthBlock */

int spawnedShark, initFunc, updateFunc, waveSystemFunc = 0;

float timer = 0;

/* Enemy Shark Struct */
typedef struct
{
	int isInUse;          /* InuseBool          */
	CP_Vector position;   /* Position of Enemy  */
	float rotation;       /* Rotation of Enmey  */
	CP_Vector velocity;   /* Velcoity of Enmey  */
	CP_Image sprite;      /* Image of Enmey     */
	CP_Vector spriteSize; /* Sprite Size Vector */
	int spriteAlpha;      /* Sprite Alpha       */
	int hitPoints;        /* Health             */
} enemyShark;

enemyShark objectList[MAX_OBJECTS]; /* Setting Shark Object to a Struct The size of Max Objects */

/*           |Enemy Managment Init|           */
  /* Enemy Mangment System Init Function  */
void enemy_management_system_init(void)
{
	CP_System_SetFrameRate(60.0f);                            /* Setting Frames            */
	memset(objectList, 0, sizeof(enemyShark) * MAX_OBJECTS);  /* Setting Each Object to 0  */
	enemylimitPos = -100;                                     /* Enmey Position            */
	enemySprite = CP_Image_Load("./Assets/shark300.png");     /* Enemy Sprite              */
	enemySpeed = -5;                                          /* Enemy Speed               */
	enemyWaveLimit = 1;                                       /* Enemy Wave Spawning Limit */
	timeLimit = 1.0f;                                        /* Time Limit                */
	initFunc = 1;                                             /* De-Bug Point              */
	hurt1 = CP_Sound_Load("./Assets/Health/HURT.mp3");        /* Hurt sound effect      */
}

/*           |Enemy Managment Update|           */
  /* Enemy Mangment System Update Function  */
void enemy_management_system_update(void)
{
	/* Timer For Waves */
	timer += 1 * CP_System_GetDt();
	/* De-Bug Console Key */
	if (CP_Input_KeyTriggered(KEY_R))
		CP_System_ShowConsole();
	printf("|  Time : %i |  Frames : %i  |  Frames Per Sec : %i  |  Init: %i  Update: %i WaveSystem: %i Shark Spawned: %i |\n", 
		   (int)timer, (int)CP_System_GetFrameCount(), (int)CP_System_GetFrameRate(), initFunc, updateFunc, waveSystemFunc, spawnedShark);
	
	/* Setting Enemy Spawn Position Randomly Every Frame */
	enemySpawnPos = CP_Vector_Set(2000, CP_Random_RangeFloat(50.0f, (float)CP_System_GetWindowHeight() - 50.0f));

	enemyWaveSystem(); /* Calling Wave System */


	for (int i = 1; i < enemyWaveLimit; ++i) /* Loop Over All Enemys in Wave */
	{
		if (objectList[i].isInUse == 1 && objectList[i].spriteAlpha != 0)
		{
			//Move
			objectList[i].position.x += enemySpeed;
			//Draw
			CP_Settings_ImageMode(CP_POSITION_CENTER);
			CP_Image_Draw(objectList[i].sprite, objectList[i].position.x, objectList[i].position.y, 210, 120, objectList[i].spriteAlpha);

			// caution bubble
			if (objectList[i].position.x > (float)CP_System_GetWindowWidth() + 50)
			{
				CP_Settings_Stroke(CP_Color_Create(255, 255, 255, 255));
				CP_Settings_Fill(CP_Color_Create(0, 0, 0, 0));
				drawBubbleWarning((float)CP_System_GetWindowWidth() - 50, objectList[i].position.y);
			}
			//Logic for enemy bounds
			if (objectList[i].position.x < enemylimitPos)
			{
				objectList[i].isInUse = 0;
			}

			//Collistion Logic
			if (collision_enemy_sub(objectList[i].position.x, objectList[i].position.y) == 1 && objectList[i].spriteAlpha != 0)
			{
				objectList[i].isInUse = 0;

				if (hit == 0)
				{
					hit = 1;
				}
				//If collide
				if (hit == 1)
				{
					HealthBlock = HealthBlock + 1;
					CP_Settings_Fill(CP_Color_Create(173, 0, 0, 100));
					CP_Graphics_DrawRect(0, 0, (float)CP_System_GetWindowWidth(), (float)CP_System_GetWindowHeight());
					CP_Sound_PlayAdvanced(hurt1, 1.5f, 1, 0, CP_SOUND_GROUP_6);
				}
			}
			/*Health system limit*/
			if (HealthBlock > HealthBlock1)
			{
				CP_Settings_Fill(CP_Color_Create(200, 0, 0, 255));
				CP_Graphics_DrawRect(170, 80, 50 * (float)(HealthBlock), 50);
				CP_Settings_Fill(CP_Color_Create(0, 0, 0, 0));
				if (HealthBlock == 7)
				{
				    CP_Sound_StopAll();
					CP_Engine_SetNextGameStateForced(gamestate_submarine_init, gamestate_gameover_update, gamestate_submarine_exit);
					HealthBlock = 0;
				}
			}
		}
	}
   if (updateFunc == 0)
    updateFunc = 1;
}

void enemy_management_system_exit(void)
{
	/* Free System When Close */
}

/*    |Create Shark Enemy|    */
  /* Spawns A Shark Enemy */
int creatSharkEnemy()
{
	for (int i = 0; i < enemyWaveLimit; ++i)
	{
		if (objectList[i].isInUse == 0)
		{
			objectList[i].isInUse = 1;
			objectList[i].position = enemySpawnPos;

			return i;
		}

		objectList[i].sprite = enemySprite;
		objectList[i].spriteAlpha = 255;
		objectList[i].hitPoints = 100;
	}
	spawnedShark = 1;
	return 0;
}

/*    |Enemy Wave System|        */
  /* Spawns Waves on a Timer */
void enemyWaveSystem(void)
{
	if (timer > timeLimit)
	{
		creatSharkEnemy();
		timer = 0;
		++enemyWaveLimit;
	}
	waveSystemFunc = 1;
}
