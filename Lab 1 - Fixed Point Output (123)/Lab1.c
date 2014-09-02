// Lab1.c
// Runs on TM4C123
// Test the functions in fixed.c by printing fixed point numbers on the ST7735 LCD screen.
// Brandon Boesch
// Curtis Martin
// September 1st, 2014

/* This example accompanies the book
   "Embedded Systems: Real Time Interfacing to Arm Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2014

 Copyright 2014 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */

// The ST7735 LCD screen is connected as follows:
//   Backlight (pin 10) connected to +3.3 V
//   MISO (pin 9) unconnected 
//   SCK (pin 8) connected to PA2 (SSI0Clk)
//   MOSI (pin 7) connected to PA5 (SSI0Tx)
//   TFT_CS (pin 6) connected to PA3 (SSI0Fss)
//   CARD_CS (pin 5) unconnected
//   Data/Command (pin 4) connected to PA6 (GPIO)
//   RESET (pin 3) connected to PA7 (GPIO)
//   VCC (pin 2) connected to +3.3 V
//   Gnd (pin 1) connected to ground

#include <stdio.h>
#include <stdint.h>
#include "ST7735.h"
#include "PLL.h"
#include "SysTick.h"
#include "inc/tm4c123gh6pm.h"
#include "fixed.h"
#include "bmp.h"

// PortF definitions
#define SYSCTL_RCGC2_GPIOF      0x00000020  // port F Clock Gating Control
#define SW1       0x10                      // on the left side of the Launchpad board
#define SW2       0x01                      // on the right side of the Launchpad board
#define PF0       (*((volatile uint32_t *)0x40025004))
#define PF4       (*((volatile uint32_t *)0x40025040))
// SysTick definitions
#define NVIC_ST_CTRL_COUNT      0x00010000  // Count flag
#define NVIC_ST_CTRL_CLK_SRC    0x00000004  // Clock Source
#define NVIC_ST_CTRL_INTEN      0x00000002  // Interrupt enable
#define NVIC_ST_CTRL_ENABLE     0x00000001  // Counter mode
#define NVIC_ST_RELOAD_M        0x00FFFFFF  // Counter load value

// declare functtions
void Fixed_uDecOut2_test(void);
void Fixed_sDecOut3_test(void);
void Fixed_uBinOut8_test(void);
void Switch_Init(void);

// initialize ROM space
const uint32_t ARRAY_SIZE = 14;
const uint32_t uDecCases[ARRAY_SIZE] = {12345, 22100, 102, 31, 0, 1, 99, 100, 999,
	1000, 9999, 10000, 99999, 100000};
const int32_t DecCases[ARRAY_SIZE] = {2345, -8100, -102, 31, -100000, -10000, -9999, 
	-999, -1, 0, 123, 1234, 9999, 10000};
const uint32_t BinCases[] = {0, 2, 64, 100, 500, 512, 5000, 30000, 
	255997, 256000, 1, 1024, 20560};

//------------main----------------------------
int main(void){
  PLL_Init();
	SysTick_Init();
	Switch_Init();
  Output_Init();
	
	// print intro screen to lab
	printf("   Fixed-point Lab\n");
  printf("\n\n\n\n\n\n\n\n\n\n");
	printf("          By:\n");
	printf("    Brandon Boesch\n");
	printf("    Curtis Martin\n");
	printf("  Press SW2 to begin\n");
	ST7735_DrawBitmap(30, 108, bmp_Logo, 70, 100);

	// begin tests
	while(PF0 == 0x01){};
  Fixed_uDecOut2_test();
	while(PF0 == 0x01){};
	Fixed_sDecOut3_test();
	while(PF0 == 0x01){};
	Fixed_uBinOut8_test();
  while(PF0 == 0x01){};
	// outro screen
	Output_Clear();
	ST7735_SetCursor(0,0);
  printf("Tests complete!");
	ST7735_DrawBitmap(6, 159, bmp_thumbs, 114, 146);
	
  while(1){};
}

//------------Fixed_uDecOut2_test------------
// Test case for Fixed_uDecOut2 function. Unsigned 32-bit decimal fixed-point with a resolution of 0.01.
void Fixed_uDecOut2_test(void){
	Output_Clear();
	ST7735_SetCursor(0,0);
	printf("Fixed_uDecOut2 test\n");
	for(int i = 0; i < ARRAY_SIZE; i++){
		Fixed_uDecOut2(uDecCases[i]); 
		SysTick_Wait10ms(10);
	}
	printf("Press SW2 to cont.");
}

//------------Fixed_DecOut3s_test------------
// Test case for Fixed_sDecOut3 function. Signed 32-bit decimal fixed-point with a resolution of 0.001.
void Fixed_sDecOut3_test(void){
	Output_Clear();
	ST7735_SetCursor(0,0);
	printf("Fixed_DecOut3s test\n");
	for(int i = 0; i < ARRAY_SIZE; i++){
		Fixed_sDecOut3(DecCases[i]);
		SysTick_Wait10ms(10);
	}
	printf("Press SW2 to cont.");
}

//------------Fixed_uBinOut8_test------------
// Test case for Fixed_sDecOut3 function. Unigned 32-bit binary fixed-point with a resolution of 1/256.
void Fixed_uBinOut8_test(void){
	Output_Clear();
	ST7735_SetCursor(0,0);
	printf("Fixed_uBinOut8_test\n");
	for(int i = 0; i < ARRAY_SIZE; i++){
		Fixed_uBinOut8(BinCases[i]);
		SysTick_Wait10ms(10);
	}
	printf("Press SW2 to cont.");
}

//------------Switch_Init------------
// Initialize GPIO Port F for negative logic switches on PF0 and
// PF4 as the Launchpad is wired.  Weak internal pull-up
// resistors are enabled, and the NMI functionality on PF0 is
// disabled.
// Input: none
// Output: none
void Switch_Init(void){            
  SYSCTL_RCGCGPIO_R |= 0x20;     // 1) activate Port F
  while((SYSCTL_PRGPIO_R&0x20) == 0){};// ready?
                                 // 2a) unlock GPIO Port F Commit Register
  GPIO_PORTF_LOCK_R = GPIO_LOCK_KEY;
  GPIO_PORTF_CR_R |= (SW1|SW2);  // 2b) enable commit for PF4 and PF0
                                 // 3) disable analog functionality on PF4 and PF0
  GPIO_PORTF_AMSEL_R &= ~(SW1|SW2);
                                 // 4) configure PF0 and PF4 as GPIO
  GPIO_PORTF_PCTL_R = (GPIO_PORTF_PCTL_R&0xFFF0FFF0)+0x00000000;
  GPIO_PORTF_DIR_R &= ~(SW1|SW2);// 5) make PF0 and PF4 in (built-in buttons)
                                 // 6) disable alt funct on PF0 and PF4
  GPIO_PORTF_AFSEL_R &= ~(SW1|SW2);
//  delay = SYSCTL_RCGC2_R;        // put a delay here if you are seeing erroneous NMI
  GPIO_PORTF_PUR_R |= (SW1|SW2); // enable weak pull-up on PF0 and PF4
  GPIO_PORTF_DEN_R |= (SW1|SW2); // 7) enable digital I/O on PF0 and PF4
}


