En este repositorio se encuentran los practicos de la materia "Programacion de microcontroladores", a continuacion se desarrollan cada uno:

******************************************************************************************************************************************************************
									PRACTICA 1
******************************************************************************************************************************************************************
1)

Implementar un programa que genere una secuencia periódica con los leds de la placa NUCLEO-F429ZI.

La secuencia debe seguir el orden: LED1, LED2, LED3, LED1,... etc.
Cada led debe permanecer encendido 200 ms.  No debe encenderse más de un led simultáneamente. Debe haber 200 ms de separación temporal entre encendidas sucesivas.

Ejemplo: Led1(ON) 200ms, Leds(OFF) 200ms, Led2(ON) 200ms, Leds(OFF) 200ms, Led3(ON) 200ms, Leds(OFF) 200ms, Led1(ON) 200ms, Leds(OFF) 200ms, etc...

2)

Utilizar el pulsador azul (USER_BUTTON) para controlar cómo se recorre la secuencia de leds.  Cada vez que se presiona el pulsador se debe alternar el orden de 
la secuencia entre:
LED1, LED2, LED3, LED1,... etc.
LED1, LED3, LED2, LED1,... etc.

******************************************************************************************************************************************************************
									PRACTICA 2
******************************************************************************************************************************************************************
1) 

Implementar las funciones auxiliares necesarias para usar retardos no bloqueantes en un archivo fuente main.c con su correspondiente archivo de cabecera main.h.
En main.h se deben ubicar los prototipos de las siguientes funciones y las declaraciones
typedef uint32_t tick_t; // Qué biblioteca se debe incluir para que esto compile?
typedef bool bool_t;	  // Qué biblioteca se debe incluir para que esto compile?
typedef struct{
   tick_t startTime;
   tick_t duration;
   bool_t running;
} delay_t;
void delayInit( delay_t * delay, tick_t duration );
bool_t delayRead( delay_t * delay );
void delayWrite( delay_t * delay, tick_t duration );

2) 

Sobre el código desarrollado para el punto 1 y sobre el mismo proyecto, implementar un programa que utilice retardos no bloqueantes y  haga parpadear el leds de 
la placa de desarrollo: 100 ms encendido, 100 ms apagado, en forma periódica.

3) 

Sobre el código desarrollado para el punto 2 y sobre el mismo proyecto, implementar un programa que haga parpadear el led de la placa de desarrollo en forma 
periódica con el siguiente patrón:
5 veces con período 1 segundo y ciclo de trabajo 50%.
5 veces con período 200 ms y ciclo de trabajo 50%.
5 veces con período 100 ms y ciclo de trabajo 50%. 
Utilizar un vector o arreglo para definir el patrón y cambiar los tiempos de parpadeo.

******************************************************************************************************************************************************************
									PRACTICA 3
******************************************************************************************************************************************************************
1)

Crear un nuevo proyecto como copia del proyecto realizado para la práctica 2.
Crear una carpeta API dentro de la carpeta Drivers en la estructura de directorios del nuevo proyecto. Crear dentro de la carpeta API, subcarpetas /Src y /Inc.
Encapsular las funciones necesarias para usar retardos no bloqueantes en un archivo fuente API_delay.c con su correspondiente archivo de cabecera API_delay.h, y
 ubicar estos archivos en la carpeta API creada.

2)

Implementar un programa que utilice retardos no bloqueantes y haga titilar en forma periódica un led de la placa NUCLEO-F4xx de acuerdo a una secuencia 
predeterminada:

const uint32_t TIEMPOS[] = {500, 100, 100, 1000};
Utilizar la función delayWrite y una única variable tipo delay_t para cambiar el tiempo de encendido del led.
NOTA: los tiempos indicados son de encendido y el led debe trabajar con duty = 50%

3)
Implementar la siguiente función auxiliar pública en API_delay.c

bool_t delayIsRunning(delay_t * delay);

Esta función debe devolver una copia del valor del campo running de la estructura delay_t

Utilizar esta función en el código implementado para el punto dos para verificar que el delay no esté corriendo antes de cambiar su valor con delayWrite


******************************************************************************************************************************************************************
									PRACTICA 4
******************************************************************************************************************************************************************
1)

Crear un nuevo proyecto como copia del proyecto realizado para la práctica 3.

Implementar una MEF anti-rebote que permita leer el estado del pulsador de la placa NUCLEO-F4 y generar acciones o eventos ante un flanco descendente o ascendente.

2)

Implementar un módulo de software en un archivo fuente API_debounce.c con su correspondiente archivo de cabecera API_debounce.h y ubicarlos en el proyecto dentro de  
las carpetas /drivers/API/src y /drivers/API/inc, respectivamente.
En API_debounce.h se deben ubicar los prototipos de las funciones públicas y las declaraciones:
void debounceFSM_init();
void debounceFSM_update();

/* La función readKey debe leer una variable interna del módulo y devolver true o false si la tecla fue presionada.  Si devuelve true, debe resetear (poner en false) 
el estado de la variable.*/
bool_t readKey();

En API_debounce.c se deben ubicar las declaraciones privadas, los prototipos de las funciones privadas y la implementación de todas las funciones del módulo, 
privadas y públicas:

La declaración de debounceState_t debe ser privada en el archivo .c y la variable de estado de tipo debounceState_t debe ser global privada (con static).

Declarar en API_debounce.c una variable tipo bool_t global privada que se ponga en true cuando ocurre un flanco descendente y se ponga en false cuando se llame a la
 función readKey();

Implementar un programa que cambie la frecuencia de parpadeo del LED entre 100 ms y 500 ms cada vez que se presione la tecla.  El programa debe usar las funciones 
anti-rebote del módulo API_debounce y los retardos no bloqueantes del módulo API_delay y la función readKey.


