/* 
 **************************************************************************************************
 *
 * @file    : timerInterrupt.cpp
 * @author  : Oussama Darouez 
 * @version : 1.0
 * @date    : March 2025
 * @brief   : Source file responsible for initiallizing the timer interrupt 
 * 
 **************************************************************************************************
 * 
 * @project  : {Inverted Pendulum}
 * @board    : {Arduino Uno}
 * @compiler : {gcc-arm-none-eabi}
 * 
 **************************************************************************************************
 *
 */

/*-----------------------------------------------------------------------------------------------*/
/* Includes                                                                                      */
/*-----------------------------------------------------------------------------------------------*/
#include"timerInterrupt.h"

/*-----------------------------------------------------------------------------------------------*/
/* Private Constants                                                                             */
/*-----------------------------------------------------------------------------------------------*/
static const uint32_t clockFrequency = 16000000;
static const uint16_t prescalerValues[5] = {1, 8, 64, 256, 1024};

/*-----------------------------------------------------------------------------------------------*/
/* Public methods                                                                                */
/*-----------------------------------------------------------------------------------------------*/
/**************************************************************************************************
  * @brief      Configure the Timer Interrput 
  * @param      prescaler clock prescaler value 
  * @param      clockCompare compare value  
  * @return     void 
  ********************************************************************************************** */
void timerInterruptSetup(float frequency){
  uint16_t prescaler = 0;
  uint32_t compareValue = 0;
  // choose the right prescaler and compare value 
  for(int i = 0; i < 5; i++)
  { 
    compareValue = (uint32_t)((clockFrequency / (frequency * prescalerValues[i])) - 1);
    prescaler = prescalerValues[i];
    if (compareValue < 65536) 
    {
      break;
    }   
  }  
  // stop all interrupt 
  cli();
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1=0;
  // configure the capture compare value 
  OCR1A = (uint16_t)compareValue; 
  // configure CTC mode 
  TCCR1B|=(1<<WGM12);
  // configure prescaler value   
  switch(prescaler){
    case 1 : 
      TCCR1B|=(1<<CS10);
      break;
    case 8 : 
      TCCR1B|=(1<<CS11); 
      break;
    case 64 : 
      TCCR1B|=(1<<CS10) | (1<<CS11); 
      break;
    case 256 : 
      TCCR1B|=(1<<CS12); 
      break;
    case 1024 : 
      TCCR1B|=(1<<CS10) | (1<<CS12); 
      break; 
  }
  // enable output compare interrupt 
  TIMSK1|=(1<<OCIE1A);
  // allow all interrupts 
  sei();
}
