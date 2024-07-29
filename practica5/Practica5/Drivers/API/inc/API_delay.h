#ifndef __API_DELAY_H
#define __API_DELAY_H

#include<stdint.h>
#include<stdbool.h>

/*----------------------------------------------------------------------------*/
/*												DECLARACION DE VARIABLES Y TIPOS 						  			*/
/*----------------------------------------------------------------------------*/
typedef uint32_t tick_t;
typedef bool bool_t;
typedef struct{
   tick_t startTime;
   tick_t duration;
   bool_t running;
} delay_t;

/*----------------------------------------------------------------------------*/
/*														PROTOTIPOS DE FUNCIONES 						  					*/
/*----------------------------------------------------------------------------*/

void delayInit( delay_t * delay, tick_t duration );
bool_t delayRead( delay_t * delay );
void delayWrite( delay_t * delay, tick_t duration );
bool_t delayIsRunning(delay_t * delay);

#endif /*API_DELAY_H*/