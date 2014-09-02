// fixed.c
// Runs on TM4C123G
// A set of fixed point output functions that print to the ST7735 LCD display
// Brandon Boesch
// Curtis Martin
// Aug 31, 2014

#include <stdio.h>
#include <stdint.h>
#include "fixed.h"
#include "ST7735.h"
#include "math.h"

 /****************Fixed_uDecOut2***************
 outputs the fixed-point value on the display
 format unsigned 32-bit with resolution 0.01
 range 0 to 999.99
 Input: unsigned 32-bit integer part of fixed point number
         greater than 99999 means invalid fixed-point number
 Output: none
 Examples
 12345 to "123.45"  
 22100 to "221.00"
   102 to "  1.02" 
    31 to "  0.31" 
100000 to "***.**"    */ 
void Fixed_uDecOut2(uint32_t n){

  Fixed_uDecPrep(n);
	if(n>99999){                   // input value out of range 
		printf("***.**\n");
	}
	else{                          // input value in range
		uint32_t I = n/100;          // interger portion of fixed point number
		if(I<100){
			printf(" ");
		}
		if(I<10){
			printf(" ");
		}
		printf("%d.",I);
		
		uint32_t delta = n%100;      // decimal portion of fixed point number
		if(delta<10){
			printf("0");
		}
		printf("%d\n",delta);
  }
}

/****************Fixed_sDecOut3***************
 converts fixed point number to the display
 format signed 32-bit with resolution 0.001
 range -9.999 to +9.999
 Input: signed 32-bit integer part of fixed point number
 Output: none
 Output to display has exactly 6 characters
 Examples
  2345 to " 2.345"  
 -8100 to "-8.100"
  -102 to "-0.102" 
    31 to " 0.031" 
 */ 
void Fixed_sDecOut3(int32_t n){
	Fixed_DecPrep(n);
	if((n>9999) || (n<-9999)){     // input value out of range 
		printf(" *.***\n");
	}
	else{                          // input value in range
		int32_t negFlag = 0;         // a flag which determines is the input is negative or positive.  Initialized to false.
		if(n<0){
			negFlag = 1;
			printf("-");
			n = n*-1;
		}
	  int32_t I = n/1000;          // interger portion of fixed point number
		if(negFlag == 0){            // add a space for non-negative integer values to keep screen alligned      
			printf(" ");         
		}
		printf("%d.",I);
		
		int32_t delta = n%1000;       // decimal portion of fixed point number
		if(delta<100){
			printf("0");
		}
		if(delta<10){
			printf("0");
		}
		printf("%d\n",delta);
  }
}

/**************Fixed_uBinOut8***************
 unsigned 32-bit binary fixed-point with a resolution of 1/256. 
 The full-scale range is from 0 to 999.99. 
 If the integer part is larger than 256000, it signifies an error. 
 The Fixed_uBinOut8 function takes an unsigned 32-bit integer part 
 of the binary fixed-point number and outputs the fixed-point value on the OLED. 
 Input: unsigned 32-bit integer part of fixed point number
 Output: none
Parameter LCD display
     0	  0.00
     2	  0.01
    64	  0.25
   100	  0.39
   500	  1.95
   512	  2.00
  5000	 19.53
 30000	117.19
255997	999.99
256000	***.**
*/
void Fixed_uBinOut8(uint32_t n){
	if(n>255999){                   // input value out of range 
		printf("***.**\n");
	}
	else{                           // input value in range
	  uint32_t I = n/256;           // integer portion of fixed point number
		if(I<100){
			printf(" ");
		}
		if(I<10){
			printf(" ");
		}
		printf("%d.",I);
	
		uint32_t delta = n%256;      // decimal portion of fixed point number
		if(delta<10){
			printf("0");
		}
		printf("%d\n",delta);
	}	
}

/**************Fixed_uDecPrep***************
 Creates the framework for the output screen on unsigned 32bit inputs
*/
void Fixed_uDecPrep(uint32_t n){
	if(n>99999){
		printf("%d,  ",n);
	}
	else if(n>9999){
		printf(" %d,  ",n);
	}
	else if(n>999){
		printf("  %d,  ",n);
	}
	else if(n>99){
		printf("   %d,  ",n);
	}
	else if(n>9){
		printf("    %d,  ",n);
	}
	else{
		printf("     %d,  ",n);
	}
}

/**************Fixed_DecPrep***************
 Creates the framework for the output screen on signed 32bit inputs
*/
void Fixed_DecPrep(int32_t n){
		int32_t negFlag = 0;         // a flag which determines is the input is negative or positive.  Initialized to false.
		if(n<0){
			negFlag = 1;
			printf("-");
			n = n*-1;
		}
		if(negFlag == 0){            // add a space for non-negative integer values to keep screen alligned      
			printf(" ");         
		}
		printf("%d,  ",n);
		if(n<100000){
		  printf(" ");
		}
		if(n<10000){
			printf(" ");
		}
		if(n<1000){
			printf(" ");
		}
		if(n<100){
			printf(" ");
		}
		if(n<10){
			printf(" ");
		}
}
