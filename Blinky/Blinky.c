/*----------------------------------------------------------------------------
 * Name:    Blinky.c
 * Purpose: LED Flasher for FRDM-K64F board
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2004-2014 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include "MK64F12.h"

uint32_t LEDOn, LEDOff; 

#define NUM_LEDS  3                     /* Number of LEDs                     */

const uint32_t led_mask[] = { 1UL << 22,
                              1UL << 21,
                              1UL << 26 };

GPIO_Type * led_addr[]    = { PTB,
                              PTB,
                              PTE };

/*----------------------------------------------------------------------------
  Function that initializes LEDs
 *----------------------------------------------------------------------------*/
void LED_Initialize(void) {

  SIM->SCGC5     |= ((1UL <<  13) | (1UL <<  10));   /* Enable Port B,E Clock */
  PORTB->PCR[22]  = (1UL <<  8);                     /* PTB22 is GPIO pin     */
  PORTB->PCR[21]  = (1UL <<  8);                     /* PTB21 is GPIO pin     */
  PORTE->PCR[26]  = (1UL <<  8);                     /* PTE26 is GPIO pin     */

  /* Switch LEDs off and enable output                                        */
  PTB->PDOR = (led_mask[1] | led_mask[0]);
  PTE->PDOR = led_mask[2];
  PTB->PDDR = (led_mask[1] | led_mask[0]);
  PTE->PDDR = led_mask[2];
}

/*----------------------------------------------------------------------------
  Function that turns on requested LED
 *----------------------------------------------------------------------------*/
void LED_On (unsigned int idx) {
  if (idx < NUM_LEDS) {
    led_addr[idx]->PCOR = led_mask[idx];
  }
}

/*----------------------------------------------------------------------------
  Function that turns off requested LED
 *----------------------------------------------------------------------------*/
void LED_Off (unsigned int idx) {
  if (idx < NUM_LEDS) {
    led_addr[idx]->PSOR = led_mask[idx];
  }
}

/*----------------------------------------------------------------------------
  SysTick IRQ Handler
 *----------------------------------------------------------------------------*/
void SysTick_Handler (void) {
  static uint32_t ticks;
  
  switch (ticks++) {
    case   0: LEDOn  = 1; break;
    case  25: LEDOff = 1; break;
    case  45: ticks  = 0; break;
    
    default:
      if (ticks > 50) {
        ticks = 0;
      }
  }
}

/*----------------------------------------------------------------------------
  Main function
 *----------------------------------------------------------------------------*/
int main (void) {
  int32_t max_num = NUM_LEDS - 1;
  int32_t num = 0;
  int32_t dir = 1;

  SystemCoreClockUpdate ();                 /* Update system core clock       */
  SysTick_Config(SystemCoreClock/100);      /* Generate interrupt each 10 ms  */
  LED_Initialize();                         /* LED Initialization             */

  while (1) {
    if (LEDOn) {
      LEDOn = 0;
      LED_On (num);                         /* Turn specified LED on          */
    }

    if (LEDOff) {
      LEDOff = 0;
      LED_Off (num);                        /* Turn specified LED off         */

      num += dir;                           /* Change LED number              */
      
      if (dir == 1 && num == max_num) {
        dir = -1;                           /* Change direction to down       */
      }
      else if (num == 0) {
        dir =  1;                           /* Change direction to up         */
      }
    }
  }
}
