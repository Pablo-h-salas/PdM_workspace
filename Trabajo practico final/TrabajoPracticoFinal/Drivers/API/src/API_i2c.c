#include "stm32f4xx_hal.h"
#include "main.h"
#include "API_i2c.h"
#include "API_uart.h"
#include "stdio.h"
/*rango de 0x00 a 0x7F + caracter nulo */
#define MAX_SIZE_ADDRESS 3
#define TIMEOUT 100

/*----------------------------------------------------------------------------*/
/*												DECLARACION DE VARIABLES Y TIPOS 						  			*/
/*----------------------------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;
static bool i2cState = 0;

/*----------------------------------------------------------------------------*/
/*											PROTOTIPOS DE FUNCIONES PRIVADAS 						  				*/
/*----------------------------------------------------------------------------*/
static void i2cScanner();
/*----------------------------------------------------------------------------*/
/*												DEFINICION DE FUNCIONES  						  							*/
/*----------------------------------------------------------------------------*/

/**
	*@brief 	Inicializa el i2c 
	*@param 	none
	*@return 	devuelve true si la inicializacion es exitosa, caso contrario false
	*/
bool i2cInit(){
	/*Establecer parametros de configuración*/
	hi2c1.Instance		 				 	 = I2C1;
  hi2c1.Init.ClockSpeed				 = 100000;
  hi2c1.Init.DutyCycle 				 = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1			 = 0;
  hi2c1.Init.AddressingMode		 = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode	 = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 			 = 0;
  hi2c1.Init.GeneralCallMode   = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode		 = I2C_NOSTRETCH_DISABLE;
	
	/*consultar resultado de la inicializion*/
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    return false;
  }else{
		uartSendString((uint8_t *)"\r\n RESULTADOS DE SCAN I2C \r\n");
		i2cScanner();
		return true;
	}
	
	  /** Configure Analogue filter */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
	*@brief 	Encuentra las direcciones de 7 bits de los dispositivos 
						asociados a I2C e imprime la informacion por UART.
	*@param 	none
	*@return 	none
	*/
void i2cScanner(){
	/*string que contiene la direccion I2C*/
	char hexAddress [MAX_SIZE_ADDRESS];
	/*preguntar a todos los dispositivos conectados a hi2c1 si estan listos*/
	for(uint8_t i=1; i<128; i++){
	/*si estan listos, informar su direccion por UART*/
	/*i<<1 convierte la direccion de 7 bits a 8 bits, el bit menos 
		significativo se usa para indicar lectura o escritura*/
		if(HAL_I2C_IsDeviceReady(&hi2c1, (uint16_t)(i<<1), 1, 100) == HAL_OK){
			sprintf(hexAddress,"%02X",i);
			uartSendString((uint8_t *)"Dispositivo detectado: 0x");
			uartSendString((uint8_t *)hexAddress);
			uartSendString((uint8_t *)"\r\n");
			i2cState = 1;
		}
	}
}

/**
	*@brief 	Envia informacion por I2C
	*@param 	direccion del dispositivo I2C
	*@param 	puntero al buffer de datos
	*@param 	tamaño del buffer de datos
	*@return 	true si el envio es exitoso, caso contrario false
	*/
bool i2cSendData(uint16_t devAddress, uint8_t *pData, uint16_t size){	
	/*verificar si el puntero no es nulo*/
	if (pData == NULL){
		return false;
	}
	/*Transmitir los bytes de informacion*/
	if(HAL_I2C_Master_Transmit(&hi2c1,devAddress<<1,pData, size, TIMEOUT)== HAL_OK){
		return true;
	}else{
		i2cState = 0;
		uartSendString((uint8_t *)"Error al comunicarse con el dispositivo I2C");
		
	}
}

/**
	*@brief 	Recive informacion por I2C
	*@param 	direccion del dispositivo I2C
	*@param 	puntero al buffer de datos donde se guardaran los datos
	*@param 	tamaño del buffer de datos
	*@return 	true si el envio es exitoso, caso contrario false
	*/
bool i2cReceiveData(uint16_t devAddress, uint8_t *pData, uint16_t size){
	/*verificar si el puntero no es nulo*/
	if (pData == NULL){
		return false;
	}
	/*Recibir los bytes de informacion*/
	if (HAL_I2C_Master_Receive(&hi2c1, devAddress<<1, pData, size, TIMEOUT) == HAL_OK){
		return true;
	}
}

/**
	*@brief 	consulta el estado de dispositivos i2c encontrados al inicio
	*@param 	none
	*@return 	true si se ha encontrado dispositivo i2c, caso contrario false
	*/
bool readi2cStatus(){
	if (i2cState){
		return true;
	}else{
		return false;
	}
}

