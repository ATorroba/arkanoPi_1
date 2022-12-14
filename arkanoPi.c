
#include "arkanoPi.h"

int flags = 0;

TipoSistema sistema;

// Declaracion del objeto teclado
TipoTeclado teclado = {
	.columnas = {
		// A completar por el alumno...
		// ...
			GPIO_KEYBOARD_COL_1,
			GPIO_KEYBOARD_COL_2,
			GPIO_KEYBOARD_COL_3,
			GPIO_KEYBOARD_COL_4,

	},
	.filas = {
		// A completar por el alumno...
		// ...
			GPIO_KEYBOARD_ROW_1,
			GPIO_KEYBOARD_ROW_2,
			GPIO_KEYBOARD_ROW_3,
			GPIO_KEYBOARD_ROW_4,
	},
	.rutinas_ISR = {
		// A completar por el alumno...
		// ...
			teclado_fila_1_isr,
			teclado_fila_2_isr,
			teclado_fila_3_isr,
			teclado_fila_4_isr,
	},

	// A completar por el alumno...
	// ...

	.columna_actual = 0,
	.teclaPulsada = {-1,-1},
	.flags = 0,

};

// Declaracion del objeto display
TipoLedDisplay led_display = {
	.pines_control_columnas = {
		// A completar por el alumno...
		// ...
			GPIO_LED_DISPLAY_COL_1,
			GPIO_LED_DISPLAY_COL_2,
			GPIO_LED_DISPLAY_COL_3,

	},
	.filas = {
		// A completar por el alumno...
		// ...
			GPIO_LED_DISPLAY_ROW_1,
			GPIO_LED_DISPLAY_ROW_2,
			GPIO_LED_DISPLAY_ROW_3,
			GPIO_LED_DISPLAY_ROW_4,
			GPIO_LED_DISPLAY_ROW_5,
			GPIO_LED_DISPLAY_ROW_6,
			GPIO_LED_DISPLAY_ROW_7,
	},
	// A completar por el alumno...
	// ...
    .p_columna = 0,
	.flags=0,
	.pantalla = 	{.matriz = {
			{0,0,0,0,0,0,0,0},
			{0,1,1,0,0,1,1,0},
			{0,1,1,0,0,1,1,0},
			{0,0,0,0,0,0,0,0},
			{0,1,0,0,0,0,1,0},
			{0,0,1,1,1,1,0,0},
			{0,0,0,0,0,0,0,0},
			}
	}

};

//------------------------------------------------------
// FUNCIONES DE CONFIGURACION/INICIALIZACION
//------------------------------------------------------

// int ConfiguracionSistema (TipoSistema *p_sistema): procedimiento de configuracion
// e inicializacion del sistema.
// Realizará, entra otras, todas las operaciones necesarias para:
// configurar el uso de posibles librerías (e.g. Wiring Pi),
// configurar las interrupciones externas asociadas a los pines GPIO,
// configurar las interrupciones periódicas y sus correspondientes temporizadores,
// la inicializacion de los diferentes elementos de los que consta nuestro sistema,
// crear, si fuese necesario, los threads adicionales que pueda requerir el sistema
// como el thread de exploración del teclado del PC
int ConfiguraInicializaSistema (TipoSistema *p_sistema) {
	int result = 0;
	// A completar por el alumno...
	// ...


	if(wiringPiSetupGpio()<0){
		printf("Unable to setup wiringPi\n");
		return -1;
	}
	InicializaTeclado(&(teclado));
	InicializaLedDisplay(&(led_display));
	p_sistema->arkanoPi.tmr_actualizacion_juego = tmr_new(tmr_actualizacion_juego_isr);
	p_sistema->arkanoPi.tmr_actualizacion_juegodif = tmr_new(tmr_actualizacion_juego_isrdif);
	//(*p_sistema).arkanoPi.p_pantalla = &(led_display.pantalla);


	// Lanzamos thread para exploracion del teclado convencional del PC
	//result = piThreadCreate (thread_explora_teclado_PC);
	//if (result != 0) {
		//printf ("Thread didn't start!!!\n");
		//return -1;
	//}


	return result;

}

//------------------------------------------------------
// FUNCIONES LIGADAS A THREADS ADICIONALES
//------------------------------------------------------

//PI_THREAD (thread_explora_teclado_PC) {
//	int teclaPulsada;
//
//	while(1) {
//		delay(10); // Wiring Pi function: pauses program execution for at least 10 ms
//
//		piLock (STD_IO_BUFFER_KEY);
//
//		if(kbhit()) {
//			teclaPulsada = kbread();
//
//			switch(teclaPulsada) {
//				// A completar por el alumno...
//				// ...
//	     	case 'a':
//			    piLock(SYSTEM_FLAGS_KEY);
//			    flags |= FLAG_MOV_IZQUIERDA;
//			    piUnlock(SYSTEM_FLAGS_KEY);
//			    break;
//	     	case 'd':
//			    piLock(SYSTEM_FLAGS_KEY);
//			    flags |= FLAG_MOV_DERECHA;
//			    piUnlock(SYSTEM_FLAGS_KEY);
//			    break;
//	     	case 'c':
//			    piLock(SYSTEM_FLAGS_KEY);
//			    flags |= FLAG_TIMER_JUEGO;
//			    piUnlock(SYSTEM_FLAGS_KEY);
//			    break;
//			case 's':
//				// A completar por el alumno...
//				// ...
//			    piLock(SYSTEM_FLAGS_KEY);
//			    flags |= FLAG_BOTON;
//			    piUnlock(SYSTEM_FLAGS_KEY);
//				printf("Tecla S pulsada!\n");
//				fflush(stdout);
//				break;
//
//				case 'q':
//					exit(0);
//					break;
//
//				default:
//					printf("INVALID KEY!!!\n");
//					break;
//			}
//		}
//
//		piUnlock (STD_IO_BUFFER_KEY);
//	}
//}

// wait until next_activation (absolute time)
void delay_until (unsigned int next) {
	unsigned int now = millis();
	if (next > now) {
		delay (next - now);
	}
}

int main () {

	unsigned int next;

	// Maquina de estados: lista de transiciones
	// {EstadoOrigen, CondicionDeDisparo, EstadoFinal, AccionesSiTransicion }
	fsm_trans_t arkanoPi[] = {
		{ WAIT_START, CompruebaBotonPulsado, WAIT_PUSH, InicializaJuego },
		{ WAIT_START, CompruebaVida, WAIT_PUSH, 0 },

		{ WAIT_PUSH, CompruebaTimeoutActualizacionJuego, WAIT_PUSH, ActualizarJuego },
		{ WAIT_PUSH, CompruebaMovimientoIzquierda, WAIT_PUSH, MuevePalaIzquierda },
		{ WAIT_PUSH, CompruebaMovimientoDerecha, WAIT_PUSH, MuevePalaDerecha },
		{ WAIT_PUSH, CompruebaFinalJuego, WAIT_END, FinalJuego },

		{ WAIT_END, CompruebaNivel, WAIT_DIF, InicializaJuegoDif },
		{ WAIT_END, CompruebaVida, WAIT_PUSH, InicializaJuegoVida },

		{ WAIT_END,  CompruebaBotonPulsado, WAIT_START, ReseteaJuego },

		{ WAIT_START, CompruebaBotonPulsadoDif, WAIT_DIF, InicializaJuegoDif },
		{ WAIT_DIF, CompruebaTimeoutActualizacionJuegoDif, WAIT_DIF, ActualizarJuegoDif },
		{ WAIT_DIF, CompruebaMovimientoIzquierda, WAIT_DIF, MuevePalaIzquierda },
		{ WAIT_DIF, CompruebaMovimientoDerecha, WAIT_DIF, MuevePalaDerecha },
		{ WAIT_DIF, CompruebaFinalJuego, WAIT_END, FinalJuego },
		{ WAIT_END, CompruebaVida, WAIT_DIF, InicializaJuegoVidaDif },
        { WAIT_END,  CompruebaBotonPulsadoDif, WAIT_START, ReseteaJuegoDif },

		{-1, NULL, -1, NULL },
	};

	// Configuracion e incializacion del sistema
	ConfiguraInicializaSistema (&sistema);

	printf("\nCOMIENZA EL JUEGO!\n");
	printf("\nPuedes comenzar en el nivel dificil directamente\n"
	"\n o comenzar por el nivel más fácil y pasar al siguiente si ganas\n");

	printf("\nDispones  de 2 vidas\n");
	printf("\n\t 2 -> Inicia el Juego\n");
	printf("\n\t 5 -> Inicia el Juego nivel medio\n");
	printf("\n\t 4 -> Mueve pala izquierda\n");
	printf("\n\t 6 -> Mueve pala derecha\n");
	printf("\n\t 1 -> Termina Juego\n");
	printf("\n \n");
	//fsm_t* arkanoPi_fsm = fsm_new (WAIT_START, arkanoPi, &sistema.arkanoPi);

	// A completar por el alumno...
	// ...


	sistema.arkanoPi.p_pantalla=&led_display.pantalla;
	fsm_t* arkanoPi_fsm = fsm_new (WAIT_START, arkanoPi, &sistema.arkanoPi);
	fsm_t* teclado_fsm = fsm_new (TECLADO_ESPERA_COLUMNA, fsm_trans_excitacion_columnas, &(teclado));
	fsm_t* tecla_fsm = fsm_new (TECLADO_ESPERA_TECLA, fsm_trans_deteccion_pulsaciones, &(teclado));

    fsm_t* led_display_fsm = fsm_new (DISPLAY_ESPERA_COLUMNA, fsm_trans_excitacion_display, &(led_display));

	next = millis();
	while (1) {
		fsm_fire (arkanoPi_fsm);

		// A completar por el alumno...
		// ...
		fsm_fire (teclado_fsm);
		fsm_fire (tecla_fsm);
        fsm_fire (led_display_fsm);
		next += CLK_MS;
		delay_until (next);
	}

	fsm_destroy (arkanoPi_fsm);
	fsm_destroy (arkanoPi_fsm->user_data);

}
