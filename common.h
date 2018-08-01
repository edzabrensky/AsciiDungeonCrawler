#ifndef __COMMON_H__
#define __COMMON_H__
#include "nokia5110.h"
const  unsigned char NUM_MENU_CHOICE     = 4;
const  unsigned char NUM_COMBAT_CHOICE   = 3;
const  unsigned char NUM_REWARD_CARDS_CHOICE    = 4;
const  unsigned char NUM_HELP_PAGES      = 14;
const  uint32_t      EEPROM_BASE_ADDR     = 1000;
const  unsigned char MAX_ENEMIES 	     = 2;
const  unsigned char MAX_ENEMYHPBLINK    = 8*2;
const  unsigned char MAX_PLAYERHPBLINK   = 2*4;
const  unsigned char MAX_PLAYERPWRBLINK  = 2*4;
const  unsigned char MAX_PLAYERCOSTBLINK = 2*4;
const unsigned char  MAX_ENEMY_TURN_BLINK_TIME = 2*4; //should be even
const  unsigned char MAX_MS_COUNT        = 4; //Max monster state count MS is whether the enemy is offset by 1 or not
//HP
extern unsigned char HP;

//score
extern unsigned char score;

//high score
extern unsigned char highScore;

//used to tell the high score lcd to display the new high score message
extern unsigned char newHighScoreBool;

//deckSize
extern unsigned char deckSize;

//randseed
extern unsigned char randSeed;

//power - increased attack by this amount
extern unsigned char numPower;

extern unsigned char tmpPower;

//reward
extern unsigned char reward;

//attack
extern unsigned char attack;

//shield for player
extern unsigned char shield;

extern unsigned char tmpShield;

//card struct
typedef struct _card {
	char symbol;
	unsigned char magnitude;
} card;


// deck
extern card deck[99];

extern card hand[5];

extern card rewardCard[3];

extern unsigned char numCardsInHand;

//choice
extern unsigned char choice; //defines which selection is being made on screen 


//mode
extern unsigned char mode;	 //defines which screen we are currently on
							 // 0x00 = start screen
							 // 0x01 = menu screen
							 // 0x02 = high score screen
							 // 0x04 = combat choice screen
							 // 0x05 = defeat screen
							 // 0x06 = victory screen
							 // 0x08 = help screen
							 // 0x10 = combat screen
							 // 0x11 = monster select screen
							 // 0x12 = monster turn blnk


//enemy struct
typedef struct _enemy {
	char symbol;
	unsigned char intent; //0 for attack, 1 for defend
	unsigned char magnitude;
	unsigned char HP;
	unsigned char shield;
} enemy;

extern enemy enemies[2];

extern unsigned char numEnemies;

//extern unsigned char monsterTurn;

extern unsigned char turnDone;

extern unsigned char cost;

extern unsigned char blinkMonsterHP; //bool value that tells monster sm to start blinking

extern unsigned char blinkMonsterHPcounter;

extern unsigned char blinkPlayerHP;

extern unsigned char blinkPlayerHPcounter;

extern unsigned char blinkPlayerPower;

extern unsigned char blinkPlayerPowercounter;

extern unsigned char blinkPlayerCost;

extern unsigned char blinkPlayerCostcounter;

extern unsigned char enemyTurnBlinkCounter;

extern unsigned char enemyDefendingBool;

extern unsigned char enemyAttackingBool;

extern unsigned char enemyDefendingBool1;

extern unsigned char enemyAttackingBool1;

// typedef struct screen1 {
//     /* screen byte massive */
//     uint8_t screen[504];

//     /* cursor position */
//     uint8_t cursor_x;
//     uint8_t cursor_y;

// } screen; /*= {
//     .cursor_x = 0,
//     .cursor_y = 0
// };*/

// extern screen nokia_lcd;

 							 // 0x00 = start screen
							 // 0x01 = menu screen
							 // 0x02 = high score screen
							 // 0x04 = combat choice screen
							 // 0x05 = defeat screen
							 // 0x06 = victory screen
							 // 0x08 = help screen
							 // 0x10 = combat screen
							 // 0x11 = monster select screen
							 // 0x12 = monster turn blnk



#endif