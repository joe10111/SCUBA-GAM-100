//---------------------------------------------------------
// file:    object_management_system.c
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

#include "cprocessing.h"              /* C-Processing          */
#include "object_mangement_system.h"  /* Enemy Mangment Header */
#include "common.h"                   /* Common Header         */

int hit = 0;          /* Hit Bool    */
int hitTimer = 0;     /* Hit Timer   */
int HealthBlock = 0;  /* HealthBlock */
int HealthBlock1 = 0; /* HealthBlock */

int spawnedShark, initFunc, updateFunc, waveSystemFunc = 0;

float timer = 0;

float shellTimer = 0;

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
} enemyShark, jellyFish;

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
} shell;

enemyShark sharkList[MAX_SHARKS] = {0}; /* Setting Shark Object to a Struct The size of Max Objects */
shell shellList[MAX_SHELLS] = { 0 };    /* Setting Shark Object to a Struct The size of Max Objects */
jellyFish JellyFishList[MAX_JELLYFISH] = { 0 };

/*           |Enemy Managment Init|           */
  /* Enemy Mangment System Init Function  */
void object_management_system_init(void)
{
	CP_System_SetFrameRate(60.0f);                            /* Setting Frames            */
	enemylimitPos = -100;                                     /* Enmey Position            */
	enemySprite = CP_Image_Load("./Assets/shark300.png");     /* Enemy Sprite              */

	jellyFishSprite = CP_Image_Load("./Assets/jellyfish1.png");
	enemySpeed = -15;                                         /* Enemy Speed               */
	enemyWaveLimit = 1;                                       /* Enemy Wave Spawning Limit */                                       /* Time Limit                */
	initFunc = 1;                                             /* De-Bug Point              */
	hurt1 = CP_Sound_Load("./Assets/Health/enemyHitSound.wav");
	hitshell = CP_Sound_Load("./Assets/Health/shell.mp3");    /* shell sound effect        */
	hitjellyfish = CP_Sound_Load("./Assets/Health/jellyfish.mp3");    /* shell sound effect        */
	gameoversound = CP_Sound_Load("./Assets/Gameover/end.mp3"); /*game over sound          */
	timeLimit = 4.0f;
	sharkSize = CP_Vector_Set(200, 60);
	jellyFishSize = CP_Vector_Set(70, 95);
	shellSize = CP_Vector_Set(30, 30);
	soundTrack = CP_Sound_LoadMusic("./Assets/Music/Scuba_MusicV4.wav");
	CP_Sound_PlayAdvanced(soundTrack, 0.5f, 1, 1, 0);

err = fopen_s(&shellCountFile, "./Files/shells.txt", "r");
	fgets(shellCount, 5, shellCountFile);
	sscanf_s(shellCount, "%d", &shellC);
	fclose(shellCountFile);}

/*           |Enemy Managment Update|           */
  /* Enemy Mangment System Update Function  */
void object_management_system_update(void)
{


	if ((int)getPlayerDist() % 300 == 0 && (int)getPlayerDist() != 0)
	{
		printf("Time Limit: %f \n", timeLimit);
		if (timeLimit <= 1.0f)
		{
			timeLimit = 1.0f;
		}
		else 
		{
			timeLimit -= 0.06f;
		}
		if (enemySpeed <= -20)
			enemySpeed = -20;
		else if (enemySpeed >= 0.5)
			enemySpeed -= 1;
	}
	/* Timer For Waves */
	timer += 1 * CP_System_GetDt();
	shellTimer += 1 * CP_System_GetDt();
	
	/* De-Bug Console Key */
	if (CP_Input_KeyTriggered(KEY_R))
		CP_System_ShowConsole();
	//printf("|  Time : %i |  Frames : %i  |  Frames Per Sec : %i  |  Init: %i  Update: %i WaveSystem: %i Shark Spawned: %i |\n", 
		   //(int)timer, (int)CP_System_GetFrameCount(), (int)CP_System_GetFrameRate(), initFunc, updateFunc, waveSystemFunc, spawnedShark);
	
	/* Setting Enemy Spawn Position Randomly Every Frame */
	enemySpawnPos = CP_Vector_Set(2000, CP_Random_RangeFloat(50.0f, (float)CP_System_GetWindowHeight() - 50.0f));
	
	jellyFishSpawnPos = CP_Vector_Set(2000, CP_Random_RangeFloat(50.0f, (float)CP_System_GetWindowHeight() - 50.0f));
	
	enemyWaveSystem(); /* Calling Wave System */
	shellSpawning();

	for (int i = 40; i < MAX_JELLYFISH; ++i) /* Loop Over All Jelly in Wave */
	{
		if (JellyFishList[i].isInUse == 1 && JellyFishList[i].spriteAlpha != 0)
		{
			JellyFishList[i].position.x += (enemySpeed / 5);
			
			if(randomFloat > 50.0f)
			JellyFishList[i].position.y += (enemySpeed / 5);
			else
			JellyFishList[i].position.y -= (enemySpeed/ 5);
	
			
			CP_Settings_ImageMode(CP_POSITION_CENTER);
			CP_Image_Draw(JellyFishList[i].sprite, JellyFishList[i].position.x, JellyFishList[i].position.y, 100, 120, JellyFishList[i].spriteAlpha);
			if (JellyFishList[i].position.x < enemylimitPos)
			{
				JellyFishList[i].isInUse = 0;
			}
			if (collision_enemy_sub(JellyFishList[i].position.x, JellyFishList[i].position.y, jellyFishSize) == 1 && JellyFishList[i].spriteAlpha != 0)
			{
				JellyFishList[i].isInUse = 0;
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

					CP_Sound_PlayAdvanced(hitjellyfish, .8f, 1.0f, 0, CP_SOUND_GROUP_6);
				}
			}
		}
	}


	for (int i = 20; i < MAX_SHELLS; ++i) /* Loop Over All Shells in Wave */
	{
		if (shellList[i].isInUse == 1 && shellList[i].spriteAlpha != 0)
		{
			shellList[i].position.x += -2;
			CP_Settings_ImageMode(CP_POSITION_CENTER);
			CP_Image_Draw(shellList[i].sprite, shellList[i].position.x, shellList[i].position.y, 50, 50, shellList[i].spriteAlpha);
			if (shellList[i].position.x < enemylimitPos)
			{
				shellList[i].isInUse = 0;
			}
			if (collision_enemy_sub(shellList[i].position.x, shellList[i].position.y, shellSize) == 1 && shellList[i].spriteAlpha != 0)
			{
				shellList[i].isInUse = 0;

				shellC += 1;
				sprintf_s(shellCount, _countof(shellCount), "%d", shellC);
				printf("Score: %i \n", shellC);
				CP_Sound_PlayAdvanced(hitshell, .8f, 1.0f, 0, CP_SOUND_GROUP_6);
			}
		}
		
	}


	for (int i = 1; i < MAX_SHARKS; ++i) /* Loop Over All Sharks in Wave */
	{
		if (sharkList[i].isInUse == 1 && sharkList[i].spriteAlpha != 0)
		{
			//Move
			sharkList[i].position.x += enemySpeed;
			//Draw
			CP_Settings_ImageMode(CP_POSITION_CENTER);
			CP_Image_Draw(sharkList[i].sprite, sharkList[i].position.x, sharkList[i].position.y, 210, 120, sharkList[i].spriteAlpha);

			// caution bubble
			if (sharkList[i].position.x > (float)CP_System_GetWindowWidth() + 50)
			{
				CP_Settings_Stroke(CP_Color_Create(255, 255, 255, 255));
				CP_Settings_Fill(CP_Color_Create(0, 0, 0, 0));
				drawBubbleWarning((float)CP_System_GetWindowWidth() - 50, sharkList[i].position.y);
			}
			//Logic for enemy bounds
			if (sharkList[i].position.x < enemylimitPos)
			{
				sharkList[i].isInUse = 0;
			}

			//Collistion Logic
			if (collision_enemy_sub(sharkList[i].position.x, sharkList[i].position.y, sharkSize) == 1 && sharkList[i].spriteAlpha != 0)
			{
				sharkList[i].isInUse = 0;

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

					CP_Sound_PlayAdvanced(hurt1, .8f, 1, 0, CP_SOUND_GROUP_6);
				}
			}
			/*Health system limit*/
		}
	}
	if (HealthBlock > HealthBlock1)
	{
		CP_Settings_Fill(CP_Color_Create(200, 0, 0, 255));
		CP_Graphics_DrawRect(800 - (50 * (float)(HealthBlock))*3, 20, 50 * (float)(HealthBlock)*3, 50);
		CP_Settings_Fill(CP_Color_Create(0, 0, 0, 0));
		if (HealthBlock == 3)
		{
			err = fopen_s(&shellCountFile, "./Files/shells.txt", "w");
			fputs(shellCount, shellCountFile);
			fclose(shellCountFile);

			int tempHS;
			int tempB;
			sscanf_s(highScore, "%d", &tempHS);
			sscanf_s(buffer, "%d", &tempB);
			if (tempHS < tempB)
			{
				sprintf_s(highScore, _countof(highScore), "%d", tempB);
				err = fopen_s(&highScoreFile, "./Files/score.txt", "w");
				fputs(highScore, highScoreFile);
				fclose(highScoreFile);
			}

		  CP_Sound_StopAll();

		  CP_Sound_PlayAdvanced(gameoversound, .8f, 1.0f, 0, CP_SOUND_GROUP_9);
			CP_Engine_SetNextGameStateForced(gamestate_submarine_init, gamestate_gameover_update, gamestate_submarine_exit);
			HealthBlock = 0;
		}
	}
   if (updateFunc == 0)
    updateFunc = 1;
}

void object_management_system_exit(void)
{
	timer = 0;
	enemySpeed = -15;                                        
	enemyWaveLimit = 1;                                     
	timeLimit = 4.0f;
	CP_Sound_Free(soundTrack);
}

/*    |Create Shark Enemy|    */
  /* Spawns A Shark Enemy */
int creatSharkEnemy()
{
	for (int i = 0; i < MAX_SHARKS; ++i)
	{
		if (sharkList[i].isInUse == 0)
		{
			sharkList[i].isInUse = 1;
			sharkList[i].position = enemySpawnPos;

			return i;
		}

		sharkList[i].sprite = enemySprite;
		sharkList[i].spriteAlpha = 255;
		sharkList[i].hitPoints = 100;
	}
	spawnedShark = 1;
	return 0;
}

int spawnShell()
{
	for (int i = 20; i < MAX_SHELLS; ++i)
	{
			if (shellList[i].isInUse == 0)
			{
				shellList[i].isInUse = 1;
				shellList[i].position = enemySpawnPos;

				return i;
			}
			shellList[i].sprite = shellSprite;

			shellList[i].spriteAlpha = 255;
			shellList[i].hitPoints = 100;
	  }
  return 0;
}

int creatJellyFishEnemy()
{
	for (int i = 40; i < MAX_JELLYFISH; ++i)
	{
		if (JellyFishList[i].isInUse == 0)
		{
			JellyFishList[i].isInUse = 1;
			JellyFishList[i].position = jellyFishSpawnPos;

			return i;
		}

		JellyFishList[i].sprite = jellyFishSprite;
		JellyFishList[i].spriteAlpha = 255;
		JellyFishList[i].hitPoints = 100;
	}
	return 0;
}

/*    |Enemy Wave System|        */
  /* Spawns Waves on a Timer */
void enemyWaveSystem(void)
{
	if (timer > timeLimit)
	{
		creatJellyFishEnemy();
		creatSharkEnemy();
		randomFloat = CP_Random_RangeFloat(0, 100);
		timer = 0;
		++enemyWaveLimit;
	}
	waveSystemFunc = 1;
}

void shellSpawning(void)
{
	for (int j = 0; j <= 3; j++)
	{
		if (shellTimer > timeLimit)
		{
			spawnShell();
			shellTimer = 0;
		}
	}
}


