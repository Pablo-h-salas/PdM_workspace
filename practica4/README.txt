PRACTICA 4
autor: Pablo Horacio Salas
mail de contacto: pablo.salas.94@gmail.com

Descripción:
                          Se implementa un modulo de software para trabajar con retardos no bloqueantes y otro modulo de software para leer pulsadores (con antirebote).
                          Las funciones principales se encuentran dentro de un bucle infinito, estas son:

			  a) funcion 'LED_parpadeo' permite establecer el LED, el tiempo de encendido y el duty. 
                          La funcion devuelve 'true' si se han cumplido con el periodo (tiempo de encendido + tiempo de apagado) 
                          Cada vez que devuelve 'true' se recorre el vector TIEMPOS, este vector indica los tiempode ENCENDIDO!. Para calcular el tiempo de apagado de 
			  acuerdo al DUTY:
				-------------------------------------
				TIME OFF = (TIME ON)*(100-DUTY)/DUTY
				-------------------------------------
				
			   Ejemplos:
				- si el tiempo de encendido es 30 ms y el DUTY es 30, claramente el tiempo de apagado es de 70 ms, como esta en la fomula:
					TIME OFF = 30 *(70)/30 =70ms

				- en el caso de DUTY = 50, TIME OFF = TIME ON.
			
			 Las declaraciones de variables y prototipos de funciones se encuentran en API_delay.h y sus definiciones en API_delay.c. Ambos archivos se encuentran 
			 en la carpeta DRIVERS/API.
			
			b) debounceFSM_update actualiza la maquina de estados, la cual contiene la logica para determinar si el pulsador esta presionado o no. 
			c) readKey devuelve 'true' si un pulsador ha sido presionado. para detectar esto utiliza una varibale interna dentro de la logica de la maquina de 
			   estados.

Estado del proyecto:  
                          completo
Herramientas de Software:
                          CubeMx : software generador de codigo fuente
                          uvison : Entorno de desarrollo version 5.25.2.0
