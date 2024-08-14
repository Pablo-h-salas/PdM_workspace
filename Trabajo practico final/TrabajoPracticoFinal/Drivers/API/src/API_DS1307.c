#include "API_DS1307.h"
#include "API_i2c.h"
#include "API_uart.h"
#include "stdio.h"

#define DS1307ADDRESS 				0x68
#define DATE_REG_ADDRESS			0x04	
#define TOTAL_REG							7		// seg, min, horas, dia de semana, fecha de mes,
																	// mes, año																	
#define COMMAND_SET_TIME_FORMAT 		"set time hh:mm"
#define COMMAND_SET_TIME_BASE				"set time"
#define COMMAND_SET_DATE_FORMAT			"set date dd.mm.aa"
#define COMMAND_SET_DATE_BASE				"set date"
#define COMMAND_SET_ALARM_FORMAT 		"set alarm dd.mm.aa hh:mm"
#define COMMAND_SET_ALARM_BASE			"set alarm"
/*----------------------------------------------------------------------------*/
/*												DECLARACION DE VARIABLES Y TIPOS 						  			*/
/*----------------------------------------------------------------------------*/
static const uint8_t initialRegisterAddress = 0x00;
timeDate_t 	myrtcData;
alarm_t 		myAlarm;

/*----------------------------------------------------------------------------*/
/*											PROTOTIPOS DE FUNCIONES PRIVADAS 						  				*/
/*----------------------------------------------------------------------------*/
typedef enum{
	INIT,
	WAIT,
	SET_TIME,
	SET_DATE,
	SET_ALARM,
	ALARM_TRIGGERED
}rtcState_t;

static 			rtcState_t estadoActual;
static bool compare (uint8_t *string1, uint8_t *string2, size_t size);
/*----------------------------------------------------------------------------*/
/*														DEFINICION DE FUNCIONES 						  					*/
/*----------------------------------------------------------------------------*/
/**
	*@brief carga el estado inicial de FSM 
	*@param no hay parametros a recibir
	*/
void rtcFSM_init(){
	estadoActual = INIT;
}

/**
	*@brief Lee las entradas, realiza la logica de transicion de datos y 
					actualiza las salidas
	*@param no hay parametros a recibir
	*/
void rtcFSM_update(){
	commandType comando;
	switch(estadoActual){
		
		case INIT:
				if (readi2cStatus()&& readuartStatus()){
					estadoActual = WAIT;
				}else{
						uartInit();
						i2cInit();
				}
			break;
				
		case WAIT:
			// chequear el estado de i2c o uart
			if (!(readi2cStatus()&& readuartStatus())){
				estadoActual = INIT;
			}
			comando = readCommand();
			// pasar al SET_TIME
			if(comando == COMMAND_SET_TIME) estadoActual = SET_TIME;
			// pasar al SET_DATE
			if(comando == COMMAND_SET_DATE) estadoActual = SET_DATE;
			// pasar al SET_ALARM	
			if (comando == COMMAND_SET_ALARM) estadoActual = SET_ALARM;
			// revisar parametros de alarma y pasar al ALARM_TRIGGERED de ser necesario
			readData(&myrtcData);
			//showInfo(&myrtcData);
			if (checkAlarm(&myAlarm, &myrtcData)){
				estadoActual = ALARM_TRIGGERED;
			}
			break;
		
		case SET_TIME:
			if(setTime()){
				estadoActual = WAIT;
			}else{
				settingsErrorHandler();
			}
			break;
			
		case SET_DATE:			
			if(setDate()){
				estadoActual = WAIT;
			}else{
				settingsErrorHandler();
			}
			break;
			
		case SET_ALARM:
			if(setAlarm(&myAlarm)){
				estadoActual = WAIT;
			}else{
				settingsErrorHandler();
			}
			break;
		case ALARM_TRIGGERED:
			triggerAlarm();
			estadoActual = WAIT;
			break;
		default:
			rtcFSM_init();
		break;
	}
}

/**
	*@brief 	lee los datos del dispositivo I2C y lo almacena en la estructura.
	*@param 	puntero a estructura timeDate_t
	*@return 	none
	*/
void readData(timeDate_t * rtc){
	uint8_t lectura_RTC[TOTAL_REG] = {0};// para seg, min, hora, dia de semana, fecha del mes, mes, año.
	char 		miString[3];

/*Indicar la direccion a partir de la cual se hará la LECTURA*/	
i2cSendData(DS1307ADDRESS, (uint8_t *)&initialRegisterAddress, 1);
/*Leer los bytes de informacion*/
i2cReceiveData(DS1307ADDRESS, lectura_RTC, sizeof(lectura_RTC));
	
/*Almacenarlos en la estructura tipo timeDate_t*/
rtc-> second 	= lectura_RTC[0];
rtc-> minute 	= lectura_RTC[1];
rtc-> hour 		= lectura_RTC[2];
rtc-> weekDay = lectura_RTC[3];
rtc-> day 		= lectura_RTC[4];
rtc-> month 	= lectura_RTC[5];
rtc-> year 		= lectura_RTC[6];

}

/**
	*@brief 	Verifica si los comandos son validos,si lo son escribe sobre una estructura del tipo 
						timeDate_t
	*@param 	tamaño del comando
	*@param   puntero al buffer que contiene el comando 
	*@return 	none
	*/
void validateCommand(uint8_t * commandBuffer, timeDate_t * timeSettings){
	uint8_t weekDay, day, month, year, hours, minutes;
	/*verificar si el puntero no es nulo*/
	if (commandBuffer == NULL){
		return;
	}
	/*Establecer comando nulo si no coincide con los establecidos*/
	timeSettings->currentCommand =COMMAND_NONE;
	
	/*************consultar si el comando es del tipo "set time"************/
	 if (compare(commandBuffer,(uint8_t *)COMMAND_SET_TIME_BASE,sizeof(COMMAND_SET_TIME_BASE)-1 )){
        // extraer horas y minutos del string
		 if (sscanf((const char *)commandBuffer, "set time %2hhu:%2hhu", &hours, &minutes)== 2){
			 if(hours<=23 && minutes<=59){
			 timeSettings ->hour 		= toBCD(hours);
			 timeSettings ->minute	= toBCD(minutes);
			 timeSettings ->currentCommand = COMMAND_SET_TIME;
			 }else{
				 commandErrorHandler();
			 }
		 }
    }
	 
	/****************consultar si el comando es del tipo "set date"********/
	if (compare(commandBuffer,(uint8_t *)COMMAND_SET_DATE_BASE,sizeof(COMMAND_SET_DATE_BASE)-1 )){
		// extraer dia, mes y año del string
		if (sscanf((const char *)commandBuffer, "set date %2hhu.%2hhu.%2hhu", &day, &month,&year)== 3){
				if(day<=31 && month<=12){
					timeSettings->day			= toBCD(day);
					timeSettings->month 	= toBCD(month);
					timeSettings->year 		= toBCD(year);
					timeSettings->currentCommand = COMMAND_SET_DATE;
				}
		}
	}
	/**************consultar si el comando es del tipo "set alarm"*********/
	if (compare(commandBuffer,(uint8_t *)COMMAND_SET_ALARM_BASE,sizeof(COMMAND_SET_ALARM_BASE)-1 )){
		// extraer dia, mes, año, hora, minutos
		if (sscanf((const char *)commandBuffer, "set alarm %2hhu.%2hhu.%2hhu %2hhu:%2hhu", &day, &month, &year, &hours, &minutes)== 5){
			if(day<=31 && month<=12 && hours<=23 && minutes<=59){
			timeSettings->day 		= toBCD(day);
			timeSettings->month 	= toBCD(month);
			timeSettings->year 		= toBCD(year);
			timeSettings->hour		= toBCD(hours);
			timeSettings->minute	= toBCD(minutes);
			timeSettings->currentCommand = COMMAND_SET_ALARM;
			}else{
				commandErrorHandler();
			}
		}
	}
}

/**
	*@brief 	Ejecuta la escritura de los registros para configurar hora, min, sec
	*@param 	none
	*@return 	true si se ha escrito con exito, caso contrario false.
	*/
bool setTime(){
	//establecer segundos a cero y construir vector a enviar
	timeParameters.second=0x00;
	uint8_t escritura_rtc[]= {initialRegisterAddress,timeParameters.second, timeParameters.minute,timeParameters.hour};
	//enviar información
	if(i2cSendData(DS1307ADDRESS, escritura_rtc, sizeof(escritura_rtc))){
		timeParameters.currentCommand = COMMAND_NONE;
		uartSendString((uint8_t *)"Nuevo horario establecido!\r\n");
		readData(&myrtcData);
		showInfo(&myrtcData);
		return true;
	}
	return false;
}

/**
	*@brief 	Ejecuta la escritura de los registros para configurar dia, mes y año
	*@param 	none
	*@return 	true si se ha escrito con exito, caso contrario false.
	*/
bool setDate(){
	//construir vector a enviar
	uint8_t escritura_rtc[]= {DATE_REG_ADDRESS, timeParameters.day, timeParameters.month, timeParameters.year};
	//enviar información
	if(i2cSendData(DS1307ADDRESS, escritura_rtc, sizeof(escritura_rtc))){
		timeParameters.currentCommand = COMMAND_NONE;
		uartSendString((uint8_t *)"Nuevo fecha establecida!\r\n");
		readData(&myrtcData);
		showInfo(&myrtcData);
		return true;
	}
	return false;
}

/**
	*@brief 	Escribe en una estructura del tipo
	*@param 	none
	*@return 	true si se ha escrito con exito, caso contrario false.
	*/
bool setAlarm(alarm_t * alarm){
	if(alarm ==NULL){
		return false;
	}
	
	alarm->minute 			= timeParameters.minute;
	alarm->hour 				= timeParameters.hour;
	alarm->day 					= timeParameters.day;
	alarm->month 				= timeParameters.month;
	alarm->year 				= timeParameters.year;
	alarm->alarmEnabled = true;
	/*una vez leido, actualizar comando*/
	timeParameters.currentCommand = COMMAND_NONE;
	uartSendString((uint8_t *)"alarma configurada correctamente\r\n");
	return true;
}

/**
	*@brief 	compara 2 estructuras para detectar si hay alarma o no 
	*@param 	none
	*@return 	true si se ha escrito con exito, caso contrario false.
	*/
bool checkAlarm(alarm_t* alarm, timeDate_t *rtcTime){
	if(alarm->alarmEnabled &&
		rtcTime->hour == alarm->hour &&
		rtcTime->minute == alarm->minute &&
		rtcTime->day == alarm->day &&
		rtcTime->month == alarm->month &&
		rtcTime->year == alarm->year){
			return true;
		}
		return false;
}

/**
	*@brief 	dispara alarma
	*@param 	none
	*@return 	true indicando que se ha disparado alarma con exito
	*/
bool triggerAlarm(){
	uartSendString((uint8_t *)"************TIME TO WAKE UP******************");
}

/**
	*@brief 	compara 2 string, solo compara la cantidad los primeros caracteres de forma 
						consecutiva. La cantidad de caracteres a comparar esta determinada por size 					
	*@param 	cadena de caracteres 1
	*@param   cadena de caracteres 2
	*@param		cantidad de caracteres a comparar
	*@return 	devuelve true si coinciden en la cantidad de caracteres indicadas por size
	*/
static bool compare (uint8_t *string1, uint8_t *string2, size_t size){
	/*verificar que los punteros no son nulos*/
	if (string1 == NULL || string2 == NULL) {
        return false;
  }
	/*comparar caracter a caracter hasta alcanzar size o encontrar diferencia*/
	for(size_t i=0; i<size; i++){
		if(string1[i]!= string2[i]){
			return false; //diferencia encontrada
		}
	// en este punto todo ha coincidido
	return true;
	}
}


/**
	*@brief 	obtiene la representacion BCD a partir de un entero				
	*@param 	valor entero tipo uint8_t
	*@return 	devuelve un uint8_t, pero como representacion BCD
	*/
uint8_t toBCD (uint8_t value){
	 return ((value / 10) << 4) | (value % 10);
}

/**
	*@brief 	Mostrar la informacion de la estructura timeDate_t		
	*@param 	puntero a estructura timeDate_t
	*@return 	none
	*/
void showInfo(timeDate_t *rtc){
	char myString[20];
	snprintf(myString, sizeof(myString), "%02X/%02X/%02X %02X:%02X\r\n",
		rtc->day, rtc->month, rtc->year, rtc->hour, rtc->minute);
	uartSendString((uint8_t *)myString);
}

void commandErrorHandler(){
	uartSendString((uint8_t *)" los datos ingresados no son correctos, por favor intente nuevamente\r\n ");
}

void settingsErrorHandler(){
	uartSendString((uint8_t *)" Configuracion no exitosa, revise la conexion de sus perifericos\r\n ");
}


