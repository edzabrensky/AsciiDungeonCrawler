
#include <avr/io.h>
#include <util/delay.h>
#include <timer.h>
//#include "common.h"
#include "nokia5110.h"
#include "selectControl.h"
#include "startScreen.h"
#include "menuScreen.h"
#include "combatChoiceScreen.h"
#include "combat.h"
#include "defeat.h"
#include "victory.h"
#include "outputlcd.h"
#include "helpScreen.h"
#include "highScore.h"

// Struct for Tasks represent a running process in our simple real-time operating system.
typedef struct _task {
	/*Tasks should have members that include: state, period,
		a measurement of elapsed time, and a function pointer.*/
	signed char state; //Task's current state
	unsigned long int period; //Task period
	unsigned long int elapsedTime; //Time elapsed since last task tick
	int (*TickFct)(int); //Task tick function
} task;


//--------Find GCD function --------------------------------------------------
unsigned long int findGCD(unsigned long int a, unsigned long int b)
{
	unsigned long int c;
	while(1){
		c = a%b;
		if(c==0){return b;}
		a = b;
b = c;
	}
	return 0;
}



void ADC_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
	// ADEN: setting this bit enables analog-to-digital conversion.
	// ADSC: setting this bit starts the first conversion.
	// ADATE: setting this bit enables auto-triggering. Since we are
	//        in Free Running Mode, a new conversion will trigger whenever
	//        the previous conversion completes.
}


unsigned char randSeed;

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	DDRD = 0x00; PORTD = 0xFF;
	nokia_lcd_init();
	ADC_init();
	//PWM_on();

	
	unsigned long int SelectTick_calc   = 100;
	unsigned long int StartTick_calc    = 500; 
	unsigned long int MenuTick_calc     = 250;
	unsigned long int CombatChoice_calc = 250;
	unsigned long int Monster_calc		= 100;//combat
	unsigned long int Defeat_calc	    = 750;
	unsigned long int Victory_calc      = 200;
	unsigned long int Lcd_calc			= 50;
	unsigned long int help_calc         = 100;
	unsigned long int highScore_calc    = 500;

	unsigned long int tmpGCD = 1;
	tmpGCD = findGCD(SelectTick_calc, StartTick_calc);
	tmpGCD = findGCD(tmpGCD, MenuTick_calc);
	tmpGCD = findGCD(tmpGCD, CombatChoice_calc);
	tmpGCD = findGCD(tmpGCD, Monster_calc);
	tmpGCD = findGCD(tmpGCD, Defeat_calc);
	tmpGCD = findGCD(tmpGCD, Victory_calc);
	tmpGCD = findGCD(tmpGCD, Lcd_calc);
	tmpGCD = findGCD(tmpGCD, help_calc);
	tmpGCD = findGCD(tmpGCD, highScore_calc);

	//Greatest common divisor for all tasks or smallest time unit for tasks.
	unsigned long int GCD = tmpGCD;

	//Recalculate GCD periods for scheduler
	unsigned long int SelectTick_period   = SelectTick_calc/GCD;
	unsigned long int StartTick_period    = StartTick_calc/GCD;
	unsigned long int MenuTick_period     = MenuTick_calc/GCD;
	unsigned long int CombatChoice_period = CombatChoice_calc/GCD;
	unsigned long int Monster_period	  = Monster_calc/GCD;
	unsigned long int Defeat_period		  = Defeat_calc/GCD;
	unsigned long int Victory_period       = Victory_calc/GCD;
	unsigned long int Lcd_period		  = Lcd_calc/GCD;
	unsigned long int help_period		  = help_calc/GCD;
	unsigned long int highScore_period    = highScore_calc/GCD;

	//Declare an array of tasks
	static task task1, task2, task3, task4, task5, task6, task7, task8, task9, task10;
	task *tasks[] = { &task1, &task2, &task3, &task4, &task5, &task6, &task7, &task8, &task9, &task10};
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

	// Task 1
	task1.state = -1;//Task initial state.
	task1.period = SelectTick_period;//Task Period.
	task1.elapsedTime = SelectTick_period;//Task current elapsed time.
	task1.TickFct = &selectTick;//Function pointer for the tick.
	
	//task2
	task2.state = -1;//Task initial state.
	task2.period = StartTick_period;//Task Period.
	task2.elapsedTime = StartTick_period;//Task current elapsed time.
	task2.TickFct = &startScreenTick;//Function pointer for the tick.

	//task3
	task3.state = -1;//Task initial state.
	task3.period = MenuTick_period;//Task Period.
	task3.elapsedTime = MenuTick_period;//Task current elapsed time.
	task3.TickFct = &menuScreenTick;//Function pointer for the tick.

	//task4
	task4.state = -1;//Task initial state.
	task4.period = CombatChoice_period;//Task Period.
	task4.elapsedTime = CombatChoice_period;//Task current elapsed time.
	task4.TickFct = &ccTick;//Function pointer for the tick.

	//task5
	task5.state = -1;//Task initial state.
	task5.period = Monster_period;//Task Period.
	task5.elapsedTime = Monster_period;//Task current elapsed time.
	task5.TickFct = &combatTick;//Function pointer for the tick.
	
	//task6
	task6.state = -1;//Task initial state.
	task6.period = Defeat_period;//Task Period.
	task6.elapsedTime = Defeat_period;//Task current elapsed time.
	task6.TickFct = &defeatTick;//Function pointer for the tick.
		
	//task7
	task7.state = -1;//Task initial state.
	task7.period = Victory_period;//Task Period.
	task7.elapsedTime = Victory_period;//Task current elapsed time.
	task7.TickFct = &victoryTick;//Function pointer for the tick.

	//task8
	task8.state = -1;//Task initial state.
	task8.period = Lcd_period;//Task Period.
	task8.elapsedTime = Lcd_period;//Task current elapsed time.
	task8.TickFct = &outputTick;//Function pointer for the tick.

	//task9
	task9.state = -1;//Task initial state.
	task9.period = help_period;//Task Period.
	task9.elapsedTime = help_period;//Task current elapsed time.
	task9.TickFct = &helpTick;//Function pointer for the tick.

	//task9
	task10.state = -1;//Task initial state.
	task10.period = highScore_period;//Task Period.
	task10.elapsedTime = highScore_period;//Task current elapsed time.
	task10.TickFct = &highScoreTick;//Function pointer for the tick.
	
	//i counter for schedule function in while loop
	unsigned char i = 0x00;
	
	// Set the timer and turn it on
	TimerSet(GCD);
	TimerOn();
	while(1) {
		// Scheduler code
		for ( i = 0; i < numTasks; i++ ) {
			// Task is ready to tick
			if ( tasks[i]->elapsedTime == tasks[i]->period ) {
				// Setting next state for task
				tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
				// Reset the elapsed time for next tick.
				tasks[i]->elapsedTime = 0;
			}
			tasks[i]->elapsedTime += 1;
		}
		++randSeed;
		while(!TimerFlag);
		TimerFlag = 0;
	}
	/*while(1) {
		_delay_ms(1000);
		nokia_lcd_clear();
		nokia_lcd_set_cursor(0, 0);
		nokia_lcd_render();
	}*/


}