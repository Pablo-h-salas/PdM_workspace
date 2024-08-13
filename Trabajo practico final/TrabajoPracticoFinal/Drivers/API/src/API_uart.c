#include "stm32f4xx_hal.h"
#include "API_uart.h"
#include "API_DS1307.h"

#define UART_POINTER 				&huart3
#define UART_INSTANCE 			USART3
#define BAUDRATE 						115200
#define TIMEOUT 						200
#define MAX_BUFFER_SIZE 		256
#define ENTER_CODE					('\r')
// tambien llamado retorno de carro CR
/*----------------------------------------------------------------------------*/
/*												DECLARACION DE VARIABLES Y TIPOS 						  			*/
/*----------------------------------------------------------------------------*/
UART_HandleTypeDef huart3;
char receptorUart[MAX_BUFFER_SIZE];
uint8_t rxByte;
uint8_t rxIndex =0;
bool uartStatus =0;
timeDate_t timeParameters;
/*----------------------------------------------------------------------------*/
/*											PROTOTIPOS DE FUNCIONES PRIVADAS 						  				*/
/*----------------------------------------------------------------------------*/
static void uartErrorHandler();
/*----------------------------------------------------------------------------*/
/*														DEFINICION DE FUNCIONES 						  					*/
/*----------------------------------------------------------------------------*/
/**
	*@brief 	Inicializa la UART e imprime por teminal serie un mensaje con sus 
						parametros de configuración
	*@param 	none
	*@return 	devuelve true si la inicializacion es exitosa, caso contrario false
	*/
bool_t uartInit(){
	static char msg [] = "BaudRate = 115200\r\nWordLength = 8 bits\r\nStopBits = 1 bit\r\nParity = none\r\n";
	static size_t dataSize = sizeof(msg)-1;
	uint32_t timeMsg = (dataSize*10*1000)/BAUDRATE;
	
	/*Establecer parametros de configuración*/
	huart3.Instance = UART_INSTANCE;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
	
	/*consultar resultado de la inicializion*/
  if (HAL_UART_Init(UART_POINTER) != HAL_OK)
  {
    return false;
  }else{
		/*imprimir mensaje por terminal serie*/
		HAL_UART_Transmit(UART_POINTER, (uint8_t *)msg,dataSize, 3*timeMsg);
		uartStatus = 1;
		// comienza con un 1 byte para detectar el inicio
		if(HAL_UART_Receive_IT(UART_POINTER,&rxByte, 1)!= HAL_OK){
			uartSendString((uint8_t *) "Error al activar RX UART IT");
		}else{
			uartSendString((uint8_t *) "RX UART IT activado");
		}
		return true;
	}
	
	/*Habilitar interrupciones UART*/
	// comienza con un 1 byte para detectar el inicio
	if(HAL_UART_Receive_IT(UART_POINTER,&rxByte, 1)!= HAL_OK){
		uartSendString((uint8_t *) "error en la función HAL_UART_Receive_IT");
	}else{
		uartSendString((uint8_t *) "no hay error en la función HAL_UART_Receive_IT");
	}
}

/**
	*@brief 	Se llama automaticamente cuando se completa una recepcion por UART en modo
						interrupción. 
	*@param 	puntero a estructura UART_HandkeTypeDef
	*@return 	-
	*/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	// IMPORTANTE: rxIndex nos da la cantidad de caracteres + 1 (retorno de carro)
	if (huart->Instance == UART_INSTANCE){
		receptorUart [rxIndex]= rxByte;
		/*consultar si es el final del mensaje*/
		if (rxByte == ENTER_CODE){
			receptorUart [rxIndex+1]= '\0';
			validateCommand((uint8_t *)receptorUart, &timeParameters);
			uartSendString((uint8_t *)receptorUart);
			uartSendString((uint8_t *)"\r\n");
			rxIndex = 0;
		}
		else{
			rxIndex ++;
			// poner codigo para evitar el desbordamiento del buffer
			
		}
		
	/*Inicio nuevamente la recepcion con interrupcion*/
	HAL_UART_Receive_IT(UART_POINTER,&rxByte, 1);
	}
}

/**
	*@brief 	recibe un puntero a un string que se desea enviar por la UART
	*@param 	puntero a string
	*/
void uartSendString(uint8_t * pstring){
	size_t size = 0;
	/*verificar si el puntero no es nulo*/
	if (pstring == NULL){
		return;
	}
	/*Determinar el tamaño del string, recorriendolo hasta llegar al vector nulo*/
	while(pstring[size]!= '\0'){
		size++;
	}
	/*Asegurar que no exceda el tamaño maximo del buffer de transmision*/
	if (size > MAX_BUFFER_SIZE) {
       size = MAX_BUFFER_SIZE;
    }
	
	/*Enviar el string*/
	if(HAL_UART_Transmit(UART_POINTER, pstring, size, TIMEOUT)!= HAL_OK){
		return;
	}
}

/**
	*@brief 	recibe un puntero a un string que se desea enviar por la UART y la 
						cantidad de bytes que se desea enviar.
	*@param 	puntero a string
	*@param		cantidad de caracteres a enviar
	*/
void uartSendStringSize(uint8_t * pstring, uint16_t size){
	size_t realSize = 0;
	/*verificar si el puntero no es nulo*/
	if (pstring == NULL){
		return;
	}
	/*Verificar que no exceda el tamaño del buffer*/
		while(pstring[realSize]!= '\0'){
		realSize++;
	}
	if (size > realSize){
		size = realSize;
	}
	/*Asegurar que no exceda el tamaño maximo del buffer de transmision*/
	if (size > MAX_BUFFER_SIZE) {
       size = MAX_BUFFER_SIZE;
    }
	
	/*Enviar el string*/
	if (HAL_UART_Transmit(UART_POINTER, pstring, size, TIMEOUT)!=HAL_OK){
		return;
	}
	
}

/**
	*@brief 	recibe un puntero a un string donde se guardan los datos recibidos 
						por UART y la cantidad de bytes de dicho string.
	*@param 	puntero a string
	*@param		cantidad de caracteres a recibir
	*/
void uartReceiveStringSize(uint8_t * pstring, uint16_t size){
	/*verificar si el puntero no es nulo*/
	if (pstring == NULL){
		return;
	}
		/*Asegurar que no exceda el tamaño maximo del buffer de transmision*/
	if (size > MAX_BUFFER_SIZE) {
       size = MAX_BUFFER_SIZE;
    }
	/*Recibir el string, si hay error -> encender led*/
	if(HAL_UART_Receive(UART_POINTER, pstring, size,TIMEOUT)!= HAL_OK){
		uartErrorHandler();
	}
}

/**
	*@brief 	Encender un LED
	*@param 	none
	*@param		void
	*/
static void uartErrorHandler() {
	while (1) {
		//BSP_LED_On(LED2);
	}
}

/**
	*@brief 	Consultar estado de inicializacion de UART
	*@param 	none
	*@return		void
	*/
bool readuartStatus(){
	if(uartStatus){
		return true;
	}else{
		return false;
	}
}

/**
	*@brief 	Consulta el comando actual.
	*@param 	none
	*@return	devuelve tipo de comando
	*/
commandType readCommand(){
	return timeParameters.currentCommand;
}

/**
	*@brief 	Copia la informacion contenidos en una estructura timeDate
	*@param 	none
	*@return	
	*/
void bringData(timeDate_t * rtcSettings){
	
}
