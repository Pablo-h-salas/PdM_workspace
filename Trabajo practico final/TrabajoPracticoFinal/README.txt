Trabajo final de materia PdM 2024

********************************************** GENERALIDADES ****************************************

Microcontrolador: NUCLEO-STM32F429ZI
Board reference	: MB1137
User manual	: UM1974

perifericos utilizados: UART, I2C
dispositivos asociados: DS1307 (Real time clock), comunicacion i2c.

************************************************** DETALLES ******************************************

USART3 :

- PD8 TX del STM32F429ZI
- PD9 RX del STM32F429ZI
Baud rate: 115200 Bits/s
Word legth: 8Bits
Parity: None
Stop Bits: 1

I2C1 : 
- PB8 I2C1_SCL
- PB9 I2C1_SDA

Se habilitan interrupciones en I2C1

************************************************* RESUMEN **********************************************

El microcontrolador permite configurar fecha y hora de un reloj de tiempo real (DS1307) a través de una 
terminal serie con reconocimiento de comandos simples como “set date dd.mm.aa”, “set time hh:mm”.
Además permite establecer la configuración de una alarma empleando “set alarm dd.mm.aa hh:mm”

Modulos utilizados:

módulo i2c: Se establecen las funciones de inicialización, configuración,  lectura y escritura a través 
	    del periférico i2c. Además de manejar sus interrupciones.

módulo uart: brinda funciones de inicialización, configuración, escritura y lectura a través del periférico UART. 

módulo DS1307: Incluye las funciones necesarias para reconocer los comandos y acceder a la escritura/lectura de 
	       los registros del DS1307. Además contiene la máquina de estados la cual atiende un comando a la vez.
