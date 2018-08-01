#ifndef __SELECT_CONTROL_H__
#define __SELECT_CONTROL_H__
#include "common.h"
#include <avr/eeprom.h>
#include <string.h>
#include <stdlib.h>

//redeclare common.h variables to complete linking
unsigned char mode;
unsigned char choice;
unsigned char HP;
unsigned char score;
unsigned char highScore;
unsigned char newHighScoreBool;
unsigned char deckSize;	
card deck[99];    //card contains symbol and magnitude (both a byte long)
unsigned char numPower;
unsigned char reward;
enemy enemies[2];
unsigned char numEnemies;
unsigned char cost;
card hand[5];
unsigned char numCardsInHand;
unsigned char blinkMonsterHP;
unsigned char blinkMonsterHPcounter;
unsigned char attack;
unsigned char shield;
unsigned char blinkPlayerHP;
unsigned char blinkPlayerPower;
unsigned char blinkPlayerPowercounter;
unsigned char blinkPlayerHPcounter;
unsigned char tmpShield;
unsigned char tmpPower;
unsigned char blinkPlayerCost;
unsigned char blinkPlayerCostcounter;
unsigned char turnDone;

//used when we start new game
//resets the progress of the game
//does not save
unsigned char deckPtr = 0;
void resetProgress() { //dont need to erase eeprom - only save when save
	HP = 30;
	score = 0;
	deckSize = 12;
	deckPtr = 0;
	unsigned char i = 0;
	srand(randSeed);
	for(i = 0; i < deckSize; ++i) {
		if(i < 5) {
			deck[i].symbol    = 'a';
			deck[i].magnitude = rand() % 2 + 3; 
		}
		else if(i < 10) {
			deck[i].symbol    = 'd';
			deck[i].magnitude = rand() % 2 + 2; 
		}
		else {
			deck[i].symbol    = 'p';
			deck[i].magnitude = rand() % 1 + 1; 
		}
	}
}
//populates global variables with what is stored in the EEPROM
//used after we start the game
void populateGlobals() {
	HP        = eeprom_read_byte(EEPROM_BASE_ADDR);
	score     = eeprom_read_byte(EEPROM_BASE_ADDR + 8);
	highScore = eeprom_read_byte(EEPROM_BASE_ADDR + 16);
	if(highScore == 255) {
		highScore = 0;
	}
	deckSize  = eeprom_read_byte(EEPROM_BASE_ADDR + 24);
	unsigned char i = 0; 
	for(i = 0; i < deckSize; ++i) {
		deck[i].symbol = eeprom_read_byte(EEPROM_BASE_ADDR + 32 + 8*i);
	}
	for(i = 0; i < deckSize; ++i) {
		deck[i].magnitude = eeprom_read_byte(EEPROM_BASE_ADDR + 32+ 8*(deckSize) + 8*i);
	}
}


//save
//saves global variables in eeprom
//used after victory and defeat
void save() {
	eeprom_write_byte(EEPROM_BASE_ADDR, HP);
	eeprom_write_byte(EEPROM_BASE_ADDR + 8, score);
	if(score > highScore || highScore == 255) {
		eeprom_write_byte(EEPROM_BASE_ADDR + 16, score);
		newHighScoreBool = 0x01;
	}
	eeprom_write_byte(EEPROM_BASE_ADDR + 24, deckSize);
	unsigned char i = 0;
	for(i = 0; i < deckSize; ++i) {
		eeprom_write_byte(EEPROM_BASE_ADDR + 32 + 8*i, deck[i].symbol);
	}
	for(i = 0; i < deckSize; ++i) {
		eeprom_write_byte(EEPROM_BASE_ADDR + 32+ 8*(deckSize) + 8*i, deck[i].magnitude);
	}
}


//From lab
//Reads adc value from certain pin
unsigned short readADC(uint8_t ch) {
	ADMUX = (1<<REFS0);
	ADCSRA = (1<<ADEN) | (1<<ADPS2);
	ch = ch&0b00000111;
	ADMUX |= ch;

	ADCSRA |= (1<<ADSC);
	while(!(ADCSRA & (1<<ADIF)));
	ADCSRA |= (1<< ADIF);
	return ADC;
}

//TOGGLE FUNCTION: gets and returns joystick toggle
//returns 0x00 for left toggle,
//        0x01 for resting toggle,
//		  0x02 for right toggle
//        0x03 for up
//        0x04 for down
unsigned char toggleRead() {
	unsigned short x = 0x00;
	unsigned short y = 0x00;
	x = readADC(0);
	y = readADC(1);
	unsigned char tmp = 0x00;
	if(x < 480) {
		tmp = 0x02;
	}
	else if(x > 570) {
		tmp = 0x00;
	}
	else if(x >= 480 && x <= 570 && y >= 480 && y <= 570){
		tmp = 0x01;
	}
	else if(y > 570) {
		tmp = 0x03;
	}
	else if(y < 480) {
		tmp = 0x04;
	}
	else {
		tmp = 0x01;
	}
	return tmp;
}

//shuffledeck Function
//shuffles deck so that the order is random
//used when we draw cards
void shuffleDeck() {
	unsigned char i = 0;
	for(i = 0; i < deckSize; ++i) {
		unsigned char r = rand() % deckSize;
		card tmp = deck[r];
		tmp.symbol = deck[r].symbol;
		tmp.magnitude = deck[r].magnitude;
		deck[r].symbol = deck[i].symbol;
		deck[r].magnitude = deck[i].magnitude;
		deck[i].symbol = tmp.symbol;
		deck[i].magnitude = tmp.magnitude; 
	}
}

//draw function
//used after the enemy turn and at the beginning of combat
//fills the hand array with cards
void draw() {
	if(deckPtr >= deckSize - 1 || deckPtr + 5 >= deckSize) {
		deckPtr = 0;
		shuffleDeck();
	}
	unsigned char i = 0; 
	for(i = 0; i < 5; ++i) {
		hand[i].symbol = deck[i + deckPtr].symbol;
		hand[i].magnitude = deck[i + deckPtr].magnitude;
	}
	numCardsInHand = 5;
	deckPtr += 5;
}

//update combat function
//used after enemy turn
//reinitializes enemy's intents and shield of the player
//also draws cards
void updateCombat() {
	draw();
	shield = 0;
	cost = 3;
	unsigned char i = 0;
	for(i = 0; i < numEnemies; ++i) {
		enemies[i].intent    = rand() % 2;
		enemies[i].magnitude = rand() % 4 + 5;
	}
}


//initialize combat function
//used after the player chooses a combat
//also initializes reward
void initializeCombat(unsigned char _numEnemies, unsigned char _reward) {
	numEnemies = _numEnemies;
	reward = _reward;
	unsigned char i = 0x00;
	for(i = 0; i < numEnemies; ++i) {
		enemies[i].symbol    = rand() % 3 + 128;
		enemies[i].intent    = rand() % 2;
		enemies[i].magnitude = rand() % 5 + 4;
		enemies[i].HP		 = rand() % 5 + 3;//10 + 10;
		enemies[i].shield    = 0x00;
	}
	cost = 3;
	numPower = 0;
	shield = 0;
	tmpShield = 0;
	shuffleDeck();
	draw();
	numCardsInHand = 5;
	blinkPlayerHP = 0;
	blinkPlayerCost = 0;
	blinkPlayerPower = 0;
	newHighScoreBool = 0x00;
}


enum selectStates { start, 
					menu, 
					menuWait,
					help,
					helpWait,
					highScoreWait,
					highScoreState,
					ccWait,		       	   //combat choice wait
					cc,			       	   //combat choice
					combatWait,            //combat wait
					combat,                //combat
					combatSelectWait,      //combat select wait
					combatSelect,          //select enemy
					combatSelectAttackWait,//attack enemy
					combatSelectAttack,    //attack enemy
					combatSelectCancelWait,
					defendWait,	
					defend,
					powerWait,
					power,
					costWait,
					costState,	
					monsterTurnWait,
					monsterTurnState,
					defeat,
					victory
					};		

//prevJoyStick
//used to eliminate successive toggles in the same direction
unsigned char  prevJoystick    = 0x00;

//cardpos
//used to avoid the card that was chosen in shiftHand
unsigned char cardPos = 0x00;

//shiftHand Func
//used to shift the cards in the hand after a player
//played a card so that players dont see the cards they played
void shiftHand() {
	if(cardPos == numCardsInHand -1) {
		--numCardsInHand;
	}
	else {
		unsigned char i = 0;
		for(i = cardPos; i < numCardsInHand - 1; ++i) {
			hand[i].symbol = hand[i+1].symbol;
			hand[i].magnitude = hand[i+1].magnitude;
		}
		--numCardsInHand;
	}
}

//shift boss function
//shifts bosses if there are multiplebosses
//or if player eliminated a boss
void shiftBoss() { // check if any boss died
	if(enemies[0].HP == 0 && numEnemies == 2) {
		enemies[0].HP = enemies[1].HP;
		enemies[0].magnitude = enemies[1].magnitude;
		enemies[0].symbol = enemies[1].symbol;
		enemies[0].intent = enemies[1].intent;
		--numEnemies;
	}
	else if(enemies[1].HP == 0 && numEnemies == 2) {
		--numEnemies;
	}
	else if(enemies[0].HP == 0 && numEnemies == 1) {
		--numEnemies;
	}
}
//grandReward function
//gives the player the reward for defeating the boss
//used after victory
void grantReward() {
	if(reward == 8 && HP + 8 >= 30) {
		HP = 30;
	} 
	else if(reward == 8 && HP + 8 < 30) {
		HP += 8;
	}
	else {
		score += reward;
	}
}
#define selectButton (~PIND & 0x20)
#define resetButton  (~PIND & 0x40)

int selectTick(int state) {
	unsigned char joystick = toggleRead();
	switch(state) {
		case start:
			if(!selectButton && !resetButton) {
				state = start;
			}
			else if(selectButton && !resetButton) {
				populateGlobals();
				state = menuWait;
			}
			else {
				state = start;
			}
		break;
		case menuWait:
			if(selectButton && !resetButton) {
				state = menuWait;
			}
			else if(!selectButton && !resetButton) {
				choice = 0;
				state = menu;
			}
			else {
				state = start;
			}
		break;
		case menu:
			if(!selectButton && !resetButton) {
				state = menu;
			}
			else if(selectButton && !resetButton && choice == 0x00) {
				resetProgress();
				state = ccWait;
			}
			else if(selectButton && !resetButton && choice == 0x01) {
				state = highScoreWait;
			}
			else if(selectButton && !resetButton && choice == 0x02) {
				choice = 0;
				state = helpWait;
			}
			else if(selectButton && !resetButton && choice == 3) {
				state = ccWait;
			}
			else {
				state = start;
			}
		break;
		case highScoreWait:
			if(selectButton && !resetButton) {
				state = highScoreWait;
			}
			else if(!selectButton && !resetButton) {
				state = highScoreState;
			}
			else {
				state = start;
			}
		break;
		case highScoreState:
			if(selectButton && !resetButton) {
				state = menuWait;
			}
			else if(!selectButton && !resetButton) {
				state = highScoreState;
			}
			else {
				state = start;
			}
		break;
		case helpWait:
			if(selectButton && !resetButton) {
				state = helpWait;
			}
			else if(!selectButton && !resetButton) {
				state = help;
			}
			else {
				state = start;
			}
		break;
		case help:
			if(selectButton && !resetButton) {
				state = menuWait;
			}
			else if(!selectButton && !resetButton) {
				state = help;
			}
			else {
				state = start;
			}
		break;
		case ccWait:
			if(selectButton && !resetButton) {
				state = ccWait;
			}
			else if(!selectButton && !resetButton) {
				state = cc;
			}
			else {
				state = start;
			}
		break;
		case cc:
			if(!selectButton && !resetButton) {
				state = cc;
			}
			else if(selectButton && !resetButton && choice == 0x00) {
				initializeCombat(1, 1);
				state = combatWait;
			}
			else if(selectButton && !resetButton && choice == 0x01) {
				initializeCombat(2, 3);
				state = combatWait;
			}
			else if(selectButton && !resetButton && choice == 0x02) {
				initializeCombat(1, 8);
				state = combatWait;
			}
			else {
				state = start;
			}
		break;
		case combatWait:
			if(selectButton && !resetButton) {
				state = combatWait;
			}
			else if(!selectButton && !resetButton) {
				state = combat;
			}
			else {
				state = start;
			}
		break;
		case combat:
			if(numEnemies > 0 && selectButton && !resetButton && cost > 0 && choice != 5 && hand[choice].symbol == 'a') {
				attack = hand[choice].magnitude;
				cardPos = choice;
				state = combatSelectWait;
			}
			else if(numEnemies > 0 && selectButton && !resetButton && cost > 0 && choice != 5 && hand[choice].symbol == 'd') {
				tmpShield = hand[choice].magnitude;
				cardPos = choice;
				state = defendWait;
			}
			else if(numEnemies > 0 && selectButton && !resetButton && cost > 0 && choice != 5 && hand[choice].symbol == 'p') {
				tmpPower = hand[choice].magnitude;
				cardPos = choice;
				state = powerWait;
			}
			else if(numEnemies > 0 && selectButton && !resetButton && cost == 0 && choice != 5) {
				state = costWait;
			}
			else if(!selectButton && !resetButton && numEnemies > 0) {
				state = combat;
			}
			else if(numEnemies > 0 && selectButton && !resetButton && choice == 5) {
				enemies[0].shield = 0;
				enemies[1].shield = 0;
				state = monsterTurnWait;
			}
			else if(numEnemies == 0 && !selectButton && !resetButton) {
				state = victory;
			}
			else {
				state = start;
			}
		break;
		case victory:
			if(!selectButton && !resetButton) {
				state = victory;
			}
			else if(selectButton && !resetButton) {
				if(choice != 3) {
					deck[deckSize].symbol = rewardCard[choice].symbol;
					deck[deckSize].magnitude = rewardCard[choice].magnitude;
					++deckSize;
				}
				grantReward();
				save();
				state = ccWait;
			}
			else {
				state = start;
			}
		break;
		case monsterTurnWait:
			if(selectButton && !resetButton) {
				state = monsterTurnWait;
			}
			else if(!selectButton && !resetButton) {
				//monsterTurn = 0x01;
				//turnDone = 0x00;
				state = monsterTurnState;
			}
			else {
				state = start;
			}
		break;
		case monsterTurnState:
			if(!selectButton && !resetButton && turnDone == 1 && HP > 0) {
				//turnDone = 0x00;
				updateCombat();
				state = combat;
			}
			else if(selectButton && !resetButton) {
				state = monsterTurnState;
			}
			else if(!selectButton && !resetButton && turnDone == 0) {
				state = monsterTurnState;
			}
			else if(turnDone == 1 && HP == 0) { // go to defeat screen and save
				save();
				state = defeat;
			}
			else {
				state = start;
			}
		break;
		case defeat:
			if(!selectButton && !resetButton) {
				state = defeat;
			}
			else if(selectButton && !resetButton) {
				state = start;
			}
			else {
				state = start;
			}
		break;
		case costWait:
			if(selectButton && !resetButton) {
				state = costWait;
			}
			else if(!selectButton && !resetButton) {
				blinkPlayerCostcounter = 0x00;
				blinkPlayerCost = 0x01;
				state = costState;
			}
			else {
				state = start;
			}
		break;
		case costState:
			if(!resetButton && !selectButton && blinkPlayerCostcounter >= MAX_PLAYERCOSTBLINK) {
				blinkPlayerCost = 0x00;
				state = combat;
			}
			else if(!resetButton && blinkPlayerCostcounter < MAX_PLAYERCOSTBLINK) {
				state = costState;
			}
			else if(selectButton && !resetButton) {
				state = costState;
			}
			else {
				state = start;
			}
		break;
		case powerWait:
			if(selectButton && !resetButton) {
				state = powerWait;
			}
			else if(!selectButton && !resetButton) {
				blinkPlayerPowercounter = 0x00;
				blinkPlayerPower = 0x01;
				state = power;
			}
			else {
				state = start;
			}
		break;
		case power:
			if(!resetButton && !selectButton && blinkPlayerPowercounter >= MAX_PLAYERPWRBLINK) {
				blinkPlayerPower = 0x00;
				shiftHand();
				choice = 0;
				state = combat;
			}
			else if(!resetButton && blinkPlayerPowercounter < MAX_PLAYERPWRBLINK) {
				if(blinkPlayerPowercounter == MAX_PLAYERPWRBLINK/2) {
					numPower += tmpPower;
					--cost;
				}
				state = power;
			}
			else if(selectButton && !resetButton) {
				state = power;
			}
			else {
				state = start;
			}
		break;
		case defendWait:
			if(selectButton && !resetButton) {
				state = defendWait;
			}
			else if(!selectButton && !resetButton) {
				blinkPlayerHPcounter = 0x00;
				blinkPlayerHP = 0x01;
				state = defend;
			}
			else {
				state = start;
			}
		break;
		case defend:
			if(!resetButton && !selectButton && blinkPlayerHPcounter >= MAX_PLAYERHPBLINK) {
				blinkPlayerHP = 0x00;
				shiftHand();
				choice = 0;
				state = combat;
			}
			else if(!resetButton && blinkPlayerHPcounter < MAX_PLAYERHPBLINK) {
				if(blinkPlayerHPcounter == MAX_PLAYERHPBLINK/2) {
					shield += tmpShield;
					--cost;
				}
				state = defend;
			}
			else if(selectButton && !resetButton) {
				state = defend;
			}
			else {
				state = start;
			}

		break;
		case combatSelectWait:
			if(selectButton && !resetButton) {
				state = combatSelectWait;
			}
			else if(!selectButton && !resetButton) {
				state = combatSelect;
			}
			else {
				state = start;
			}

		break;
		case combatSelect:
			if(!selectButton && !resetButton) {
				state = combatSelect;
			}
			else if(selectButton && !resetButton && choice == 0) { //cancel
				state = combatSelectCancelWait;
			}
			else if(selectButton && !resetButton && (choice == 1 || choice == 2)) {
				shiftHand();
				state = combatSelectAttackWait;
			}
			else {
				state = start;
			}
		break;
		case combatSelectCancelWait:
			if(selectButton && !resetButton) {
				state = combatSelectCancelWait;
			}
			else if(!selectButton && !resetButton) {
				state = combat;
			}
			else {
				state = start;
			}
		break;
		case combatSelectAttackWait:
			if(selectButton && !resetButton) {
				state = combatSelectAttackWait;
			}
			else if(!selectButton && !resetButton) {
				blinkMonsterHPcounter = 0x00;
				blinkMonsterHP = 0x01;
				state = combatSelectAttack;
			}
			else {
				state = start;
			}
		break;
		case combatSelectAttack:
			if(resetButton) {
				state = start;
			}
			else if(blinkMonsterHPcounter >= MAX_ENEMYHPBLINK && !selectButton) {
				blinkMonsterHP = 0x00;
				attack = 0;
				choice = 0x00;
				shiftBoss();
				state = combat;
			}
			else if(blinkMonsterHPcounter < MAX_ENEMYHPBLINK || selectButton) {
				if(MAX_ENEMYHPBLINK/2 == blinkMonsterHPcounter) {
					if(enemies[choice - 1].HP + enemies[choice - 1].shield <= attack + numPower) {
						enemies[choice - 1].HP = 0;
						enemies[choice - 1].shield = 0;
					}
					else {
						if(enemies[choice-1].shield >= attack + numPower) {
							enemies[choice-1].shield = enemies[choice-1].shield - attack - numPower;
						}
						else {
							unsigned char diff = attack + numPower - enemies[choice-1].shield;
							enemies[choice - 1].HP = enemies[choice - 1].HP - diff;
							enemies[choice-1].shield = 0;
						}
					}
					--cost;
				}
				state = combatSelectAttack;
			}
			else {
				state = start;
			}

		break;
		default:
			state = start;
		break;
	}
	switch(state) {
		case start:
			mode = 0x00;
			choice = 0x00;
		break;
		case menuWait:
			mode = 0x01;
		break;
		case menu:
			mode = 0x01;
			if(prevJoystick != joystick) {
				if(joystick == 0x03 && choice != 0) {
					--choice;
				}
				else if(joystick == 0x04 && choice != NUM_MENU_CHOICE - 1 && HP > 0 && HP <= 30) {
					++choice;
				}
				else if(joystick == 0x04 && choice != NUM_MENU_CHOICE - 2 && (HP == 0 || HP > 30)) {
					++choice;
				}
			}
			prevJoystick = joystick;
		break;
		case highScoreState:
			mode = 0x02;
		break;
		case highScoreWait:
			mode = 0x02;
		break;
		case helpWait:
			mode = 0x08;
		break;
		case help:
			mode = 0x08;
			if(prevJoystick != joystick) {
				if(joystick == 0x00 && choice != 0) {
					--choice;
				}
				else if(joystick == 0x02 && choice != NUM_HELP_PAGES - 1) {
					++choice;
				}
			}
			prevJoystick = joystick;
		break;
		case ccWait:
			choice = 0x00;
		break;
		case cc:
			mode = 0x04;
			if(prevJoystick != joystick) {
				if(joystick == 0x03 && choice != 0) {
					--choice;
				}
				else if(joystick == 0x04 && choice != NUM_COMBAT_CHOICE - 1) {
					++choice;
				}
			}
			prevJoystick = joystick;
		break;
		case monsterTurnWait:
		break;
		case monsterTurnState:
			mode = 0x12;
			if((enemyTurnBlinkCounter == MAX_ENEMY_TURN_BLINK_TIME/2) && enemyAttackingBool == 0x01) {
				if(HP + shield <= enemies[0].magnitude) {
					HP = 0;
					shield = 0;
				}
				else if(shield >= enemies[0].magnitude) {
					shield -= enemies[0].magnitude;
				}
				else {
					unsigned char diff = enemies[0].magnitude - shield;
					shield = 0;
					HP -= diff;
				}
			}
			else if((enemyTurnBlinkCounter == MAX_ENEMY_TURN_BLINK_TIME / 2) && enemyDefendingBool == 0x01) {
				enemies[0].shield += enemies[0].magnitude;
			}
			else if((enemyTurnBlinkCounter == MAX_ENEMY_TURN_BLINK_TIME/2) && enemyAttackingBool1 == 0x01) {
				if(HP + shield <= enemies[1].magnitude) {
					HP = 0;
					shield = 0;
				}
				else if(shield >= enemies[1].magnitude) {
					shield -= enemies[1].magnitude;
				}
				else {
					unsigned char diff = enemies[1].magnitude - shield;
					shield = 0;
					HP -= diff;
				}
			}
			else if((enemyTurnBlinkCounter == MAX_ENEMY_TURN_BLINK_TIME / 2) && enemyDefendingBool1 == 0x01) {
				enemies[1].shield += enemies[1].magnitude;
			}
		break;
		case combatWait:
			choice = 0;
		break;
		case combat:
			mode = 0x10;
			if(prevJoystick != joystick) {
				if(numCardsInHand <= 4) {
					if(joystick == 0x00 && choice != 0 && choice != 5) {
						--choice;
					}
					else if(joystick == 0x00 && choice != 0 && choice == 5) {
						choice = numCardsInHand - 1;
					}
					else if(joystick == 0x02 && choice == numCardsInHand - 1) {
						choice = 5;
					}
					else if(joystick == 0x02 && choice != 5) {
						++choice;
					}
					else if(joystick == 0x03 && choice == 5) {
						choice = 0;
					}
					else if(joystick == 0x04 && choice != 5) {
						choice = 5;
					} 
				}
				else {
					if(joystick == 0x00 && choice != 0 && choice != 5) {
						--choice;
					}
					else if(joystick == 0x00 && choice != 0 && choice == 5) {
						choice = numCardsInHand - 1;
					}
					else if(joystick == 0x02 && choice == numCardsInHand - 1) {
						choice = 5;
					}
					else if(joystick == 0x02 && choice != 5) {
						++choice;
					}
					else if(joystick == 0x03 && choice == 5) {
						choice = 2;
					}
					else if(joystick == 0x03 && choice == 4) {
						choice = 0;
					}
					else if(joystick == 0x04 && choice < 2) {
						choice = 4;
					}
					else if(joystick == 0x04 && choice < 4) {
						choice = 5;
					}
				}
			}
			prevJoystick = joystick;
		break;
		case victory:
			mode = 0x06;
			if(prevJoystick != joystick) {
				if(joystick == 0x00 && choice != 0) {
					--choice;
				}
				else if(joystick == 0x02 && choice != NUM_REWARD_CARDS_CHOICE - 1) {
					++choice;
				}
			}
			prevJoystick = joystick;
		break;
		case defeat:
			mode = 0x05;
		break;
		case combatSelectWait:
			choice = 0;
		break;
		case combatSelect:
			mode = 0x11;
			if(prevJoystick != joystick) {
				if(joystick == 0x00 && choice != 0) {
					--choice;
				}
				else if(joystick == 0x02 && choice != numEnemies) {
					++choice;
				}
			}
			prevJoystick = joystick;
		break;
		case combatSelectAttackWait:
			mode = 0x11;
		break;
		case combatSelectAttack:
		break;
		case combatSelectCancelWait:
		break;
		case defendWait:
		break;
		case defend:
		break;
		default:
		break;
	}

	return state;
}



#endif