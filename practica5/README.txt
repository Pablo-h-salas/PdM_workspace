--------------------------------------------------
Informacion base para el proyecto
--------------------------------------------------
PLACA: STM32F429ZI
IDE: uvision
PINES EMPLEADOS:
		User button: PC13 
		USART3 TX: PD8
		USART3 RX: PD9

Notas: configurar su terminal serie de acuerdo con:
baud rate : 115200
word length : 8 bits
bit de stop : 1 bit
bit de paridad : none

Observaciones: el punto 1 se puede apreciar en los API_uart.c y API_uart.h
	       el punto 2 en API:debounce.c
--------------------------------------------------

1)
Implementar un módulo de software en un archivos fuente API_uart.c con su correspondiente archivo de cabecera API_uart.h y ubicarlos en el proyecto dentro de  las 
carpetas /drivers/API/src y /drivers/API/inc, respectivamente.
En API_uart.h se deben ubicar los prototipos de las funciones públicas.
bool_t uartInit();
void uartSendString(uint8_t * pstring);
void uartSendStringSize(uint8_t * pstring, uint16_t size);
void uartReceiveStringSize(uint8_t * pstring, uint16_t size);

En API_uart.c se deben ubicar los prototipos de las funciones privadas y la implementación de todas las funciones de módulo, privadas y públicas.

2)
Sobre el ejercicio de la práctica 4, implementar un programa que utilice el módulo de uart implementado por ustedes para informar al usuario cuando se producen los 
flancos ascendentes y descendentes.
En cada flanco se debe enviar un mensaje por la UART que diga “Flanco ascendente/descendente detectado”, según corresponda.  Cada mensaje se debe escribir en una 
línea nueva en la terminal de la PC.