#ifndef __API_DEBOUNCE_H
#define __API_DEBOUNCE_H

/*----------------------------------------------------------------------------*/
/*												DECLARACION DE VARIABLES Y TIPOS 						  			*/
/*----------------------------------------------------------------------------*/
typedef enum{
BUTTON_UP,
BUTTON_FALLING,
BUTTON_DOWN,
BUTTON_RAISING,
} debounceState_t;
/*----------------------------------------------------------------------------*/
/*														PROTOTIPOS DE FUNCIONES 						  					*/
/*----------------------------------------------------------------------------*/
void debounceFSM_init();		
void debounceFSM_update();
void buttonPressed();				
void buttonReleased();			


#endif //__API_DEBOUNCE_H