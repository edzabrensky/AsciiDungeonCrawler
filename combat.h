#ifndef __MONSTER_H__
#define __MONSTER_H__
#include "common.h"
#include "nokia5110.h"



unsigned char msOffset;
unsigned char msCount;
unsigned char enemyTurnBlinkCounter;
unsigned char enemyDefendingBool;
unsigned char enemyAttackingBool;
unsigned char enemyDefendingBool1;
unsigned char enemyAttackingBool1;

enum monsterStates {
					msWaitMode, //monster state wait mode
					msIdle,		//monster state idle
					msSelect,	//monster select
					msAction,    //monster turn 
					blinkEnemy1,
					blinkEnemy1a,
					blinkEnemy1d,
					blinkEnemy2,
					blinkEnemy2a,
					blinkEnemy2d,
					endMonsterTurn
					};


//WriteHand function
//Writes the player's hand on the monsterLcdBuffer
void writeHand() {
	nokia_lcd_set_cursorM(0, 30, 0x10);
	unsigned char i = 0; 
	for(i = 0; i < 14; ++i) {
		nokia_lcd_write_charM(' ', 1, 0x10);
	}
	nokia_lcd_set_cursorM(0, 30, 0x10);
	for(i = 0; i < numCardsInHand; ++i) {
		if(i < 3) {
			if(choice == i) {
				nokia_lcd_write_stringM(">", 1, 0x10);
			}
			else {
				nokia_lcd_write_stringM(" ", 1, 0x10);
			}
			nokia_lcd_write_charM(hand[i].symbol, 1, 0x10);
			//nokia_lcd_write_string("|", 1);
			char buff[5];
			itoa(hand[i].magnitude, buff, 10);
			nokia_lcd_write_stringM(buff, 1, 0x10);
			//nokia_lcd_write_string("", 1);
		}
		else if(i == 4) {
			nokia_lcd_set_cursorM(0, 40, 0x10);
			if(choice == i) {
				nokia_lcd_write_stringM(">", 1, 0x10);
			}
			else {
				nokia_lcd_write_stringM(" ", 1, 0x10);
			}
			nokia_lcd_write_charM(hand[i].symbol, 1, 0x10);
			//nokia_lcd_write_string("|", 1);
			char buff[5];
			itoa(hand[i].magnitude, buff, 10);
			nokia_lcd_write_stringM(buff, 1, 0x10);
			//nokia_lcd_write_string("|", 1);
		}
		else {
			if(choice == i) {
				nokia_lcd_write_stringM(">", 1, 0x10);
			}
			else {
				nokia_lcd_write_stringM(" ", 1, 0x10);
			}
			nokia_lcd_write_charM(hand[i].symbol, 1, 0x10);
			//nokia_lcd_write_string("|", 1);
			char buff[5];
			itoa(hand[i].magnitude, buff, 10);
			nokia_lcd_write_stringM(buff, 1, 0x10);
			//nokia_lcd_write_string("|  ", 1);

		}
	}
	if(numCardsInHand < 5) {
			nokia_lcd_set_cursorM(0, 40, 0x10);
			for(i = 0; i < 14; ++i) {
				nokia_lcd_write_charM(' ', 1, 0x10);
			}
			nokia_lcd_set_cursorM(0, 40, 0x10);
	}
	if(choice == 5) {

		nokia_lcd_write_stringM(" >EndTurn  ", 1, 0x10);
	}
	else {
		nokia_lcd_write_stringM("  EndTurn  ", 1, 0x10);
	}
}


//displayTop
//Displays HP of player and cost
//used in idle state and when the player HP blinks
void displayTop() {
	unsigned char j = 0;
	nokia_lcd_set_cursorM(0 , 0, 0x10);
	for( j = 0; j < 15; ++j) {
		nokia_lcd_write_stringM(" ", 1, 0x10);
	} 
	nokia_lcd_set_cursorM(0, 0, 0x10);
	char buff[5];
	itoa(HP + shield, buff, 10);
	nokia_lcd_write_stringM("HP", 1, 0x10);
	if(blinkPlayerHP == 0x00) {
		nokia_lcd_write_stringM(buff, 1, 0x10);
	}
	else {
		if(blinkPlayerHPcounter % 2 == 0) {
			nokia_lcd_write_stringM("  ", 1, 0x10);
		}
		else {
			nokia_lcd_write_stringM(buff, 1, 0x10);
		}
		++blinkPlayerHPcounter;
	}
	nokia_lcd_write_stringM("/30Cost",1, 0x10);
	itoa(cost, buff, 10);
	if(blinkPlayerCost == 0x00) {
		nokia_lcd_write_stringM(buff, 1, 0x10);
	}
	else {
		if(blinkPlayerCostcounter % 2 == 0) {
			nokia_lcd_write_stringM(" ", 1, 0x10);
		}
		else {
			nokia_lcd_write_stringM(buff, 1, 0x10);
		}
		++blinkPlayerCostcounter;
	}
	nokia_lcd_write_stringM("/3",1, 0x10);
	nokia_lcd_set_cursorM(0, 10, 0x10);
	unsigned i = 0x00;
	for(i = 0; i < 15; ++i) {
		nokia_lcd_write_charM(' ', 1, 0x10);
	}
	nokia_lcd_set_cursorM(0, 10, 0x10);
	nokia_lcd_write_stringM("Pwr:", 1, 0x10);
	itoa(numPower, buff, 10);
	if(buff[1] == '\0') {
		nokia_lcd_write_stringM(" ", 1, 0x10);
	}
	if(blinkPlayerPower == 0x00) {
		nokia_lcd_write_stringM(buff, 1, 0x10);
	}
	else {
		if(blinkPlayerPowercounter % 2 == 0) {
			nokia_lcd_write_stringM(" ", 1, 0x10);
		}
		else {
			nokia_lcd_write_stringM(buff, 1, 0x10);
		}
		++blinkPlayerPowercounter;
	}
	nokia_lcd_write_stringM("  ", 1, 0x10);
	itoa(enemies[0].magnitude, buff, 10);
	if(enemies[0].intent == 0) {
		nokia_lcd_write_stringM("!", 1, 0x10);
	}
	else {
		nokia_lcd_write_stringM("d", 1, 0x10);
	}
	nokia_lcd_write_stringM(buff, 1, 0x10);
	if(numEnemies == 2) {
		itoa(enemies[1].magnitude, buff, 10);
		nokia_lcd_write_stringM("  ", 1, 0x10);
		if(enemies[1].intent == 0) {
			nokia_lcd_write_stringM("!", 1, 0x10);
		}
		else {
			nokia_lcd_write_stringM("d", 1, 0x10);
		}
		nokia_lcd_write_stringM(buff, 1, 0x10);
	}
}

//blinkMonsterHP Function
//used when attacking enemies
void blinkMHP() {
	nokia_lcd_set_cursorM(0, 20, 0x10);
	unsigned i = 0x00;
	for(i = 0; i < 15; ++i) {
		nokia_lcd_write_charM(' ', 1, 0x10);
	}
	nokia_lcd_set_cursorM(0, 20, 0x10);
	for(i = 0; i < 9; ++i) {
		nokia_lcd_write_stringM(" ", 1, 0x10);
	}
	nokia_lcd_write_charM(enemies[0].symbol, 1, 0x10);
	nokia_lcd_write_stringM(" ",1, 0x10);
	if(numEnemies == 2) {
    	nokia_lcd_write_stringM("  ", 1, 0x10);
		nokia_lcd_write_charM(enemies[1].symbol, 1, 0x10);
	}
	nokia_lcd_set_cursorM(0, 30, 0x10);
	nokia_lcd_write_stringM("Enemy HP", 1, 0x10);
	char buff[5];
	itoa(enemies[0].HP + enemies[0].shield, buff, 10);
	if(buff[1] == '\0') { //only 1 digit long
		nokia_lcd_write_stringM(" ", 1, 0x10);
		if(choice != 1) {
			nokia_lcd_write_stringM(buff, 1, 0x10);
		}
		else if(blinkMonsterHPcounter % 2 == 0) {
			nokia_lcd_write_stringM(buff, 1, 0x10);
		}
		else {
			nokia_lcd_write_stringM(" ", 1, 0x10);
		}
	}
	else {
		if(choice != 1) {
			nokia_lcd_write_stringM(buff, 1, 0x10);
		}
		else if(blinkMonsterHPcounter % 2 == 0) {
			nokia_lcd_write_stringM(buff, 1, 0x10);
		}
		else {
			nokia_lcd_write_stringM("  ", 1, 0x10);
		}
	}
	nokia_lcd_write_stringM("  ", 1, 0x10);
	if(numEnemies == 2) {
		itoa(enemies[1].HP + enemies[1].shield, buff, 10);
		if(buff[1] == '\0') { //only 1 digit long
			nokia_lcd_write_stringM(" ", 1, 0x10);
			if(choice != 2) {
				nokia_lcd_write_stringM(buff, 1, 0x10);
			}
			else if(blinkMonsterHPcounter % 2 == 0) {
				nokia_lcd_write_stringM(buff, 1, 0x10);
			}
			else {
				nokia_lcd_write_stringM(" ", 1, 0x10);
			}
		}
		else {
			if(choice != 2) {
				nokia_lcd_write_stringM(buff, 1, 0x10);
			}
			else if(blinkMonsterHPcounter % 2 == 0) {
				nokia_lcd_write_stringM(buff, 1, 0x10);
			}
			else {
				nokia_lcd_write_stringM("  ", 1, 0x10);
			}
		}
	}
	nokia_lcd_set_cursorM(0, 40, 0x10);
	for(i = 0; i < 14; ++i) {
		nokia_lcd_write_charM(' ', 1, 0x10);
	}
	nokia_lcd_set_cursorM(0, 40, 0x10);
	nokia_lcd_write_stringM(" Attack!!!",1, 0x10);
	if(blinkMonsterHPcounter < MAX_ENEMYHPBLINK) {
		displayTop();
		++blinkMonsterHPcounter;
	}
}


//DisplaySelectFunction
//Displays which enemy to attack
//used when attack card is played
void displaySelect() {
	nokia_lcd_set_cursorM(0, 20, 0x10);
    	unsigned i = 0x00;
    	for(i = 0; i < 15; ++i) {
    		nokia_lcd_write_charM(' ', 1, 0x10);
    	}
    	nokia_lcd_set_cursorM(0, 20, 0x10);
    	if(choice == 0) {
    		nokia_lcd_write_stringM(">Cancel ", 1, 0x10);
    		
    	}
    	else {
    		nokia_lcd_write_stringM(" Cancel ", 1, 0x10);
    	}
    	if(choice == 1) {
    		nokia_lcd_write_stringM(">",1, 0x10);
    	}
    	else {
    		nokia_lcd_write_stringM(" ",1, 0x10);
    	}
    	nokia_lcd_write_charM(enemies[0].symbol, 1, 0x10);
    	nokia_lcd_write_stringM(" ",1, 0x10);
    	if(numEnemies == 2) {
	    	if(choice == 2) {
	    		nokia_lcd_write_stringM(" >", 1, 0x10);
	    	}
	    	else {
	    		nokia_lcd_write_stringM("  ", 1, 0x10);
	    	}
			nokia_lcd_write_charM(enemies[1].symbol, 1, 0x10);
		}
		nokia_lcd_set_cursorM(0, 30, 0x10);
		nokia_lcd_write_stringM("Enemy HP", 1, 0x10);
		char buff[5];
		itoa(enemies[0].HP + enemies[0].shield, buff, 10);
		if(buff[1] == '\0') { //only 1 digit long
			nokia_lcd_write_stringM(" ", 1,0x10);
			nokia_lcd_write_stringM(buff, 1, 0x10);
		}
		else {
			nokia_lcd_write_stringM(buff, 1, 0x10);
		}
		nokia_lcd_write_stringM("  ", 1, 0x10);
		if(numEnemies == 2) {
			itoa(enemies[1].HP + enemies[1].shield, buff, 10);
			if(buff[1] == '\0') { //only 1 digit long
				nokia_lcd_write_stringM(" ", 1, 0x10);
				nokia_lcd_write_stringM(buff, 1, 0x10);
			}
			else {
				nokia_lcd_write_stringM(buff, 1, 0x10);
			}
		}
		nokia_lcd_set_cursorM(0, 40, 0x10);
		nokia_lcd_write_stringM(" Select Target",1, 0x10);
}


//enemyTurnDisplayHP Function
//Displays enemy's HP during the enemy's turn
//Blinks depending on defending and the counter variable
void enemyTurnDisplayHP() {
	nokia_lcd_set_cursorM(0, 30, 0x10);
	unsigned i = 0;
	for(i =0 ; i < 14; ++i) {
		nokia_lcd_write_charM(' ', 1, 0x10);
	}
	nokia_lcd_set_cursorM(0, 30, 0x10);
	nokia_lcd_write_stringM("Enemy HP", 1, 0x10);
	char buff[5];
	itoa(enemies[0].HP + enemies[0].shield, buff, 10);
	if(buff[1] == '\0') { //only 1 digit long
		nokia_lcd_write_stringM(" ", 1, 0x10);
		nokia_lcd_write_stringM(buff, 1, 0x10);
	}
	else {
		nokia_lcd_write_stringM(buff, 1, 0x10);
	}
	nokia_lcd_write_stringM("  ", 1, 0x10);
	if(numEnemies == 2) {
		itoa(enemies[1].HP + enemies[1].shield, buff, 10);
		if(buff[1] == '\0') { //only 1 digit long
			nokia_lcd_write_stringM(" ", 1, 0x10);
			nokia_lcd_write_stringM(buff, 1, 0x10);
		}
		else {
			nokia_lcd_write_stringM(buff, 1, 0x10);
		}
	}
	nokia_lcd_set_cursorM(0, 40, 0x10);
	for(i =0 ; i < 14; ++i) {
		nokia_lcd_write_charM(' ', 1, 0x10);
	}
	nokia_lcd_set_cursorM(0, 40, 0x10);
	nokia_lcd_write_stringM(" EnemyTurn",1, 0x10);
}

//used to see which enemy to blink
unsigned char enemyPointer; 

//EnemyTurnDisplayEnemy FUnction
//Used when its the mosnter's turn, it blinks the enemies to indicate who is attacking or defending
void enemyTurnDisplayEnemy() {
	nokia_lcd_set_cursorM(0, 20, 0x10);
	unsigned i = 0x00;
	for(i = 0; i < 14; ++i) {
		nokia_lcd_write_charM(' ', 1, 0x10);
	}
	nokia_lcd_set_cursorM(0, 20, 0x10);
	for(i = 0; i < 9; ++i) {
		nokia_lcd_write_stringM(" ", 1, 0x10);
	}
	if(enemyPointer == 0 && enemyTurnBlinkCounter % 2 == 0) {
		nokia_lcd_write_charM(' ', 1, 0x10);
	}
	else {
		nokia_lcd_write_charM(enemies[0].symbol, 1,0x10);
	}
	nokia_lcd_write_stringM(" ",1, 0x10);
	if(numEnemies == 2) {
    	nokia_lcd_write_stringM("  ", 1, 0x10);
    	if(enemyPointer == 1 && enemyTurnBlinkCounter % 2 ==  0) {
    		nokia_lcd_write_charM(' ', 1, 0x10);
    	}	
    	else {
			nokia_lcd_write_charM(enemies[1].symbol, 1, 0x10);
		}
	}
}

int combatTick(int state) {
	switch(state) {
		case msWaitMode:
			if(!(mode & 0x10)) {
				state = msWaitMode;
			}
			else if(mode == 0x10) {
				nokia_lcd_clearM(0x10);
				displayTop();
				state = msIdle;
				blinkPlayerPowercounter = 0;
				blinkPlayerCostcounter = 0;
				blinkPlayerHPcounter = 0;
				blinkMonsterHPcounter = 0;
			}
			else {
				state = msWaitMode;
			}
		break;
		case msIdle:
			if(mode == 0x10) {
				state = msIdle;
				turnDone = 0x00;
			}
			else if(mode == 0x11) {
				state = msSelect;
			}
			else if(mode == 0x12) {
				enemyTurnBlinkCounter = 0;
				enemyPointer = 0;
				enemyTurnDisplayHP();
				state = blinkEnemy1;
			}
			else {
				state = msWaitMode;
			}
		break;
		case msSelect:
			if(mode == 0x11) {
				state = msSelect;
			}
			else if(mode == 0x10) {
				state = msIdle;
			}
			else {
				state = msWaitMode;
			}
		break;
		case blinkEnemy1:
			if(mode == 0x12 && enemyTurnBlinkCounter >= MAX_ENEMY_TURN_BLINK_TIME) {
				enemyTurnBlinkCounter = 0x00;
				enemyDefendingBool = 0x00;
				enemyAttackingBool = 0x00;
				enemyDefendingBool1 = 0x00;
				enemyAttackingBool1 = 0x00;
				if(enemies[0].intent == 0x00) {
					state = blinkEnemy1a;
				}
				else {
					state = blinkEnemy1d;
				}
			} 
			else if(mode == 0x12 && enemyTurnBlinkCounter < MAX_ENEMY_TURN_BLINK_TIME) {
				state = blinkEnemy1;
			}
			else {
				state = msWaitMode;
			}
		break;
		case blinkEnemy1a:
			if(mode == 0x12 && enemyTurnBlinkCounter >= MAX_ENEMY_TURN_BLINK_TIME) {
				enemyTurnBlinkCounter = 0x00;
				enemyPointer = 0x01;
				enemyAttackingBool = 0x00;
				enemyDefendingBool = 0x00;
				enemyDefendingBool1 = 0x00;
				enemyAttackingBool1 = 0x00;
				state = blinkEnemy2;
			} 
			else if(mode == 0x12 && enemyTurnBlinkCounter < MAX_ENEMY_TURN_BLINK_TIME) {
				enemyAttackingBool = 0x01;
				state = blinkEnemy1a;
			}
			else {
				state = msWaitMode;
			}
		break;
		case blinkEnemy1d:
			if(mode == 0x12 && enemyTurnBlinkCounter >= MAX_ENEMY_TURN_BLINK_TIME) {
				enemyTurnBlinkCounter = 0x00;
				enemyAttackingBool = 0x00;
				enemyDefendingBool = 0x00;
				enemyDefendingBool1 = 0x00;
				enemyAttackingBool1 = 0x00;
				enemyPointer = 0x01;
				state = blinkEnemy2;
			} 
			else if(mode == 0x12 && enemyTurnBlinkCounter < MAX_ENEMY_TURN_BLINK_TIME) {
				state = blinkEnemy1d;
				enemyDefendingBool = 0x01;
			}
			else {
				state = msWaitMode;
			}
		break;
		case blinkEnemy2:
			if(mode == 0x12 && enemyTurnBlinkCounter >= MAX_ENEMY_TURN_BLINK_TIME && numEnemies > 1) {
				enemyTurnBlinkCounter = 0x00;
				enemyAttackingBool = 0x00;
				enemyDefendingBool = 0x00;
				enemyDefendingBool1 = 0x00;
				enemyAttackingBool1 = 0x00;
				if(enemies[1].intent == 0x00) {
					state = blinkEnemy2a;
				}
				else {
					state = blinkEnemy2d;
				}
			} 
			else if(mode == 0x12 && enemyTurnBlinkCounter < MAX_ENEMY_TURN_BLINK_TIME) {
				state = blinkEnemy2;
			}
			else if(mode == 0x12 && enemyTurnBlinkCounter >= MAX_ENEMY_TURN_BLINK_TIME 
						&& numEnemies == 1) {
				turnDone = 0x01;
				state = blinkEnemy2;
			}
			else if(mode == 0x10) {
				state = msIdle;
			}
			else {
				state = msWaitMode;
			}
		break;
		case blinkEnemy2a:
			if(mode == 0x12 && enemyTurnBlinkCounter >= MAX_ENEMY_TURN_BLINK_TIME) {
				enemyTurnBlinkCounter = 0x00;
				enemyAttackingBool = 0x00;
				enemyDefendingBool = 0x00;
				enemyDefendingBool1 = 0x00;
				enemyAttackingBool1 = 0x00;
				state = endMonsterTurn;
			} 
			else if(mode == 0x12 && enemyTurnBlinkCounter < MAX_ENEMY_TURN_BLINK_TIME) {
				enemyAttackingBool1 = 0x01;
				state = blinkEnemy2a;
			}
			else {
				state = msWaitMode;
			}
		break;
		case blinkEnemy2d:
			if(mode == 0x12 && enemyTurnBlinkCounter >= MAX_ENEMY_TURN_BLINK_TIME) {
				enemyTurnBlinkCounter = 0x00;
				enemyAttackingBool = 0x00;
				enemyDefendingBool = 0x00;
				enemyDefendingBool1 = 0x00;
				enemyAttackingBool1 = 0x00;
				state = endMonsterTurn;
			} 
			else if(mode == 0x12 && enemyTurnBlinkCounter < MAX_ENEMY_TURN_BLINK_TIME) {
				enemyDefendingBool1 = 0x01;
				state = blinkEnemy2d;
			}
			else {
				state = msWaitMode;
			}
		break;
		case endMonsterTurn:
			if(mode == 0x12) {
				turnDone = 0x01;
				state = endMonsterTurn;
			}
			else if(mode == 0x10) {
				state = msIdle;
			}
			else {
				state = msWaitMode;
			}
		break;
		default:
			state = msWaitMode;
		break;
	}
	switch(state) {
		case msWaitMode:
		break;
		case blinkEnemy1:
			enemyTurnDisplayEnemy();
			enemyTurnDisplayHP();
			if(enemyTurnBlinkCounter <= MAX_ENEMY_TURN_BLINK_TIME) {
				++enemyTurnBlinkCounter;
			}
		break;
		case blinkEnemy1a:
			displayTop();
			if(enemyTurnBlinkCounter <= MAX_ENEMY_TURN_BLINK_TIME) {
				++enemyTurnBlinkCounter;
			}
		break;
		case blinkEnemy1d:
			enemyTurnDisplayHP();
			if(enemyTurnBlinkCounter <= MAX_ENEMY_TURN_BLINK_TIME) {
				++enemyTurnBlinkCounter;
			}
		break;
		case blinkEnemy2:
			if(numEnemies == 2) {
				enemyTurnDisplayEnemy();
			}
			if(enemyTurnBlinkCounter <= MAX_ENEMY_TURN_BLINK_TIME) {
				++enemyTurnBlinkCounter;
			}
		break;
		case blinkEnemy2a:
			displayTop();
			if(enemyTurnBlinkCounter <= MAX_ENEMY_TURN_BLINK_TIME) {
				++enemyTurnBlinkCounter;
			}
		break;
		case blinkEnemy2d:
			enemyTurnDisplayHP();
			if(enemyTurnBlinkCounter <= MAX_ENEMY_TURN_BLINK_TIME) {
				++enemyTurnBlinkCounter;
			}
		break;
		case endMonsterTurn:

		break;
		case msIdle:
			displayTop();
			if(msCount >= MAX_MS_COUNT) {
				if(msOffset == 0x00) {
					msOffset = 0x01;
				}
				else {
					msOffset = 0x00;
				}
				nokia_lcd_set_cursorM(0, 20, 0x10);
				unsigned i = 0x00;
		    	for(i = 0; i < 15; ++i) {
		    		nokia_lcd_write_charM(' ', 1, 0x10);
		    	}
				nokia_lcd_set_cursorM(0, 20, 0x10);
	    		nokia_lcd_write_stringM("               ", 1, 0x10);
	    		nokia_lcd_set_cursorM(0, 20,0x10);
	    		nokia_lcd_write_stringM("        ", 1, 0x10);
	    		if(msOffset == 1) {
	    			nokia_lcd_write_stringM(" ", 1, 0x10);
	    		}
	    		nokia_lcd_write_charM(enemies[0].symbol, 1, 0x10);
	    		nokia_lcd_write_stringM("  ", 1, 0x10);
	    		if(numEnemies == 2) {
	    			if(msOffset == 0) {
	    				nokia_lcd_write_stringM("  ", 1, 0x10);
	    			}

	    			
	    			nokia_lcd_write_charM(enemies[1].symbol, 1, 0x10);
	    		}
	    		if(blinkPlayerHP == 0x00 && blinkPlayerPower == 0x00 && blinkPlayerCost == 0x00) {
	    			writeHand();
	    		}
	    		else {

	    			nokia_lcd_set_cursorM(0, 30, 0x10);
	 				for(i = 0; i < 14; ++i) {
	 					nokia_lcd_write_charM(' ', 1, 0x10);
	 				}  

	 				nokia_lcd_set_cursorM(0, 40, 0x10);
	 				for(i = 0; i < 14; ++i) {
	 					nokia_lcd_write_charM(' ', 1, 0x10);
	 				}
	 							
	    		}
	    		msCount = 0;
    		}
    		else {
    			if(blinkPlayerHP == 0x00 && blinkPlayerPower == 0x00 && blinkPlayerCost == 0x00) {
	    			writeHand();
	    		}
	    		else {
	    			unsigned char i = 0;
	    			nokia_lcd_set_cursorM(0, 30, 0x10);
	 				for(i = 0; i < 14; ++i) {
	 					nokia_lcd_write_charM(' ', 1, 0x10);
	 				}  

	 				nokia_lcd_set_cursorM(0, 40, 0x10);
	 				for(i = 0; i < 14; ++i) {
	 					nokia_lcd_write_charM(' ', 1, 0x10);
	 				}			
	    		}
    		}
    		++msCount;
		break;
		case msSelect:
			if(!blinkMonsterHP) {
				displaySelect();
			}
			else {
				blinkMHP();
			}
		break;
		default:
		break;
	}
	return state;
}






#endif