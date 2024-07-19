#ifndef __API_DEBOUNCE_H
#define __API_DEBOUNCE_H
#include "API_delay.h"
/*----------------------------------------------------------------------------*/
/*												DECLARACION DE VARIABLES Y TIPOS 						  			*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*														PROTOTIPOS DE FUNCIONES 						  					*/
/*----------------------------------------------------------------------------*/
void debounceFSM_init();		
void debounceFSM_update();
bool_t readKey();

void buttonPressed();				
void buttonReleased();			


#endif //__API_DEBOUNCE_H