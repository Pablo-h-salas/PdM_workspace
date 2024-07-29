#include "stm32f4xx_hal.h"
#include "API_uart.h"
#define BAUDRATE 115200
#define TIMEOUT 100
#define MAX_BUFFER_SIZE 256

UART_HandleTypeDef huart3;

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
	huart3.Instance = USART1;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
	
	/*consultar resultado de la inicializion*/
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    return false;
  }else{
		/*imprimir mensaje por terminal serie*/
		HAL_UART_Transmit(&huart3, (uint8_t *)msg,dataSize, 3*timeMsg);
		return true;
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
	if(HAL_UART_Transmit(&huart3, pstring, size, TIMEOUT)!= HAL_OK){
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
	if (HAL_UART_Transmit(&huart3, pstring, size, TIMEOUT)!=HAL_OK){
		return;
	}
	
}