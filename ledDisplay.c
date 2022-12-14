
#include "ledDisplay.h"

tipo_pantalla pantalla_inicial = {
	.matriz = {
	{0,0,0,0,0,0,0,0},
	{0,1,1,0,0,1,1,0},
	{0,1,1,0,0,1,1,0},
	{0,0,0,0,0,0,0,0},
	{0,1,0,0,0,0,1,0},
	{0,0,1,1,1,1,0,0},
	{0,0,0,0,0,0,0,0},
	}
};

tipo_pantalla pantalla_final = {
	.matriz = {
	{0,0,0,0,0,0,0,0},
	{0,1,1,0,0,1,1,0},
	{0,1,1,0,0,1,1,0},
	{0,0,0,0,0,0,0,0},
	{0,0,1,1,1,1,0,0},
	{0,1,0,0,0,0,1,0},
	{0,0,0,0,0,0,0,0},
	}
};

// Maquina de estados: lista de transiciones
// {EstadoOrigen, CondicionDeDisparo, EstadoFinal, AccionesSiTransicion }
fsm_trans_t fsm_trans_excitacion_display[] = {
	{ DISPLAY_ESPERA_COLUMNA, CompruebaTimeoutColumnaDisplay, DISPLAY_ESPERA_COLUMNA, ActualizaExcitacionDisplay },
	{-1, NULL, -1, NULL },
};

//------------------------------------------------------
// PROCEDIMIENTOS DE INICIALIZACION DE LOS OBJETOS ESPECIFICOS
//------------------------------------------------------

void InicializaLedDisplay (TipoLedDisplay *led_display) {
	// A completar por el alumno...
	// ...

    pinMode(GPIO_LED_DISPLAY_COL_1,OUTPUT);
    digitalWrite(GPIO_LED_DISPLAY_COL_1,LOW);
    pinMode(GPIO_LED_DISPLAY_COL_2,OUTPUT);
    digitalWrite(GPIO_LED_DISPLAY_COL_2,LOW);

    pinMode(GPIO_LED_DISPLAY_COL_3,OUTPUT);
    digitalWrite(GPIO_LED_DISPLAY_COL_3,LOW);

    pinMode(GPIO_LED_DISPLAY_ROW_1,OUTPUT);
    digitalWrite(GPIO_LED_DISPLAY_ROW_1,HIGH);
    pinMode(GPIO_LED_DISPLAY_ROW_2,OUTPUT);
    digitalWrite(GPIO_LED_DISPLAY_ROW_2,HIGH);

    pinMode(GPIO_LED_DISPLAY_ROW_3,OUTPUT);
    digitalWrite(GPIO_LED_DISPLAY_ROW_3,HIGH);

    pinMode(GPIO_LED_DISPLAY_ROW_4,OUTPUT);
    digitalWrite(GPIO_LED_DISPLAY_ROW_4,HIGH);

    pinMode(GPIO_LED_DISPLAY_ROW_5,OUTPUT);
    digitalWrite(GPIO_LED_DISPLAY_ROW_5,HIGH);

    pinMode(GPIO_LED_DISPLAY_ROW_6,OUTPUT);
    digitalWrite(GPIO_LED_DISPLAY_ROW_6,HIGH);

    pinMode(GPIO_LED_DISPLAY_ROW_7,OUTPUT);
    digitalWrite(GPIO_LED_DISPLAY_ROW_7,HIGH);

    led_display->p_columna= 0;

	led_display->tmr_refresco_display = tmr_new(timer_refresco_display_isr);

	tmr_startms((led_display->tmr_refresco_display),(TIMEOUT_COLUMNA_DISPLAY));

}

//------------------------------------------------------
// OTROS PROCEDIMIENTOS PROPIOS DE LA LIBRERIA
//------------------------------------------------------

void ApagaFilas (TipoLedDisplay *led_display){
	// A completar por el alumno...
	// ...
	digitalWrite(GPIO_LED_DISPLAY_ROW_1,HIGH);
    digitalWrite(GPIO_LED_DISPLAY_ROW_2,HIGH);
	digitalWrite(GPIO_LED_DISPLAY_ROW_3,HIGH);
	digitalWrite(GPIO_LED_DISPLAY_ROW_4,HIGH);
	digitalWrite(GPIO_LED_DISPLAY_ROW_5,HIGH);
	digitalWrite(GPIO_LED_DISPLAY_ROW_6,HIGH);
	digitalWrite(GPIO_LED_DISPLAY_ROW_7,HIGH);


}

void ExcitaColumnas(int columna) {

	switch(columna){
			// A completar por el alumno...
	        //...
		case 0:
			//piLock(MATRIX_KEY);
			digitalWrite(GPIO_LED_DISPLAY_COL_1,LOW);
			digitalWrite(GPIO_LED_DISPLAY_COL_2,LOW);
			digitalWrite(GPIO_LED_DISPLAY_COL_3,LOW);
			//piUnlock(MATRIX_KEY);
			break;


		case 1:
			//piLock(MATRIX_KEY);
			digitalWrite(GPIO_LED_DISPLAY_COL_1,HIGH);
			digitalWrite(GPIO_LED_DISPLAY_COL_2,LOW);
			digitalWrite(GPIO_LED_DISPLAY_COL_3,LOW);
			//piUnlock(MATRIX_KEY);
			break;


		case 2:
			//piLock(MATRIX_KEY);
			digitalWrite(GPIO_LED_DISPLAY_COL_1,LOW);
			digitalWrite(GPIO_LED_DISPLAY_COL_2,HIGH);
			digitalWrite(GPIO_LED_DISPLAY_COL_3,LOW);
			//piUnlock(MATRIX_KEY);
			break;


		case 3:
			//piLock(MATRIX_KEY);
			digitalWrite(GPIO_LED_DISPLAY_COL_1,HIGH);
			digitalWrite(GPIO_LED_DISPLAY_COL_2,HIGH);
			digitalWrite(GPIO_LED_DISPLAY_COL_3,LOW);
			//piUnlock(MATRIX_KEY);
			break;
		case 4:
			//piLock(MATRIX_KEY);
			digitalWrite(GPIO_LED_DISPLAY_COL_1,LOW);
			digitalWrite(GPIO_LED_DISPLAY_COL_2,LOW);
			digitalWrite(GPIO_LED_DISPLAY_COL_3,HIGH);
			//piUnlock(MATRIX_KEY);
			break;
		case 5:
			//piLock(MATRIX_KEY);
			digitalWrite(GPIO_LED_DISPLAY_COL_1,HIGH);
			digitalWrite(GPIO_LED_DISPLAY_COL_2,LOW);
			digitalWrite(GPIO_LED_DISPLAY_COL_3,HIGH);
			//piUnlock(MATRIX_KEY);
			break;
		case 6:
			//piLock(MATRIX_KEY);
			digitalWrite(GPIO_LED_DISPLAY_COL_1,LOW);
			digitalWrite(GPIO_LED_DISPLAY_COL_2,HIGH);
			digitalWrite(GPIO_LED_DISPLAY_COL_3,HIGH);
			//piUnlock(MATRIX_KEY);
			break;
		case 7:
			//piLock(MATRIX_KEY);
			digitalWrite(GPIO_LED_DISPLAY_COL_1,HIGH);
			digitalWrite(GPIO_LED_DISPLAY_COL_2,HIGH);
			digitalWrite(GPIO_LED_DISPLAY_COL_3,HIGH);
			//piUnlock(MATRIX_KEY);
			break;
		}
   
}

void ActualizaLedDisplay (TipoLedDisplay *led_display) {
	// A completar por el alumno...
	// ...
	//Apago las filas
    ApagaFilas(led_display);
    

     ExcitaColumnas(led_display->p_columna);

	for(int i = 0; i < NUM_FILAS_DISPLAY; i++){
		if(led_display->pantalla.matriz[i][led_display->p_columna]==1){
			digitalWrite(led_display->filas[i],LOW);
		}
	}

}

//------------------------------------------------------
// FUNCIONES DE ENTRADA O DE TRANSICION DE LA MAQUINA DE ESTADOS
//------------------------------------------------------

int CompruebaTimeoutColumnaDisplay (fsm_t* this) {
	int result = 0;
	TipoLedDisplay *p_ledDisplay;
	p_ledDisplay = (TipoLedDisplay*)(this->user_data);

	// A completar por el alumno...
	// ...
	piLock(MATRIX_KEY);
	result = (p_ledDisplay->flags & FLAG_TIMEOUT_COLUMNA_DISPLAY);
	piUnlock(MATRIX_KEY);

	return result;
}

//------------------------------------------------------
// FUNCIONES DE SALIDA O DE ACCION DE LA MAQUINA DE ESTADOS
//------------------------------------------------------

void ActualizaExcitacionDisplay (fsm_t* this) {
	TipoLedDisplay *p_ledDisplay;
	p_ledDisplay = (TipoLedDisplay*)(this->user_data);

	// A completar por el alumno...
	// ...
	piLock(MATRIX_KEY);
	p_ledDisplay->flags &= (~FLAG_TIMEOUT_COLUMNA_DISPLAY);

	p_ledDisplay->p_columna++;
	if( p_ledDisplay->p_columna > NUM_FILAS_DISPLAY){
		p_ledDisplay->p_columna=0;
		}
	//tmr_startms((tmr_t*)(led_display->tmr_refresco_display),(TIMEOUT_COLUMNA_DISPLAY));
	ActualizaLedDisplay(p_ledDisplay);
	piUnlock(MATRIX_KEY);
	tmr_startms((tmr_t*)(p_ledDisplay->tmr_refresco_display),(TIMEOUT_COLUMNA_DISPLAY));

}

//------------------------------------------------------
// SUBRUTINAS DE ATENCION A LAS INTERRUPCIONES
//------------------------------------------------------

void timer_refresco_display_isr (union sigval value) {
	// A completar por el alumno...
	// ...

	piLock(MATRIX_KEY);
	led_display.flags |= FLAG_TIMEOUT_COLUMNA_DISPLAY;
	piUnlock(MATRIX_KEY);
}
