// Directivas de librerías a utilizar
#include "directivas.h"

// Variables globales
const unsigned int tiempo_muestreo = 34953; //Tiempo de muestreo multiplicado por 100 para medirlo en 10^-2 segundos 1/480 codificado en <16,24>
int mic_on = 0;
int cambio_leds = 0;
int FIFO_retardo[32768]; // Vector de enteros donde guardaremos las muestras retardadas, al ser una potencia de dos es mas facil desplazarse a traves de ella
int retardo_eco = 0;
int retardo_rev = 0;
unsigned int wahwah_k = 0;
unsigned int vaiven_k = 1;
long int g = 0;
int puls_sat = 0;
int swnf = 3;
long int acum_1 = 0;
long int acum_2 = 0;

// Tabla de valores para la adaptacion de los leds a la amplitud de la salida
const int valores_led[7][5] = {{102, 165, 287, 410, 512},
							   {204, 410, 615, 820, 1024},
							   {409, 820, 1229, 1639, 2048},
							   {819, 1639, 2458, 3277, 4096},
							   {1639, 3277, 4915, 6554, 8192},
							   {1639, 6554, 8390, 13108, 16384},
							   {3278, 8390, 13108, 16384, 26216}};
// Declaración de MEF
typedef enum { reverb_m, eco_m, coro_m, wahwah_m, vaiven_m, robot_m, telef_m, microfono_m, leds_m } estados_menu;
typedef enum { reverb_e, eco_e, coro_e, wahwah_e, vaiven_e, robot_e, telef_e, menu_e } estados_efectos;

estados_menu estado_menu = microfono_m; // Posición en el menú (para mostrar una cosa u otra por pantalla y el siguiente estado de efectos si se pulsa el botón)
estados_efectos estado_efectos = menu_e; 	/* En función de la posición en el menú, al pulsar un switch cambiaremos el estado de procesamiento
											que determinará lo que pasamos a hacer con la señal a partir de ese momento */

unsigned int twah = ((long)Mwahwah * 44739) >> 16; // Mwahwah es el nº de muestras que recorre para cambiar de canal, 44739 es 1/48k en <16,31>
unsigned int tvai = ((long)(1 << 10) * 44739) >> 16; // (1<<10) es el nº de muestras que recorre para cambiar de canal, 44739 es 1/48k en <16,31>

// Variables para la representacion en la pantalla OLED de los simbolos empleados
char empty_line[19] = "                   ";
unsigned int menu_entrada[96] = { 0x0000, 0x0000, 0x1000, 0x3000, 0x7F00, 0x3000, 0x1400, 0x0600, 0x7F00, 0x0600, 0x0400, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x1C00, 0x1C00, 0x1C00, 0x1C00, 0x1C00, 0x7F00, 0x3E00, 0x1C00, 0x0800, 0x0000, 0x0000 };
unsigned int menu_leds[96] = { 0x0000, 0x0000, 0x1000, 0x3000, 0x7F00, 0x3000, 0x1400, 0x0600, 0x7F00, 0x0600, 0x0400, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0800, 0x7400, 0x0200, 0x6100, 0x0200, 0x7400, 0x0800, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x1C00, 0x1C00, 0x1C00, 0x1C00, 0x1C00, 0x7F00, 0x3E00, 0x1C00, 0x0800, 0x0000, 0x0000 };
unsigned int menu_efectos[96] = { 0x0000, 0x0000, 0x7F00, 0x3E00, 0x1C00, 0x0800, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0800, 0x0800, 0x7400, 0x0200, 0x6100, 0x0200, 0x7400, 0x0800, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x1C00, 0x1C00, 0x1C00, 0x1C00, 0x1C00, 0x7F00, 0x3E00, 0x1C00, 0x0800, 0x0000, 0x0000 };
unsigned int efecto_sin_ajuste[96] = { 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x1000, 0x3900, 0x7D00, 0x1100, 0x1100, 0x1100, 0x1100, 0x1100, 0x1F00, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000 };
unsigned int efecto_con_ajuste[96] = { 0x0000, 0x0000, 0x0800, 0x1800, 0x3F00, 0x7F00, 0x3F00, 0x1800, 0x0800, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x1000, 0x3900, 0x7D00, 0x1100, 0x1100, 0x1100, 0x1100, 0x1100, 0x1F00, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0800, 0x0C00, 0x7E00, 0x7F00, 0x7E00, 0x0C00, 0x0800, 0x0000, 0x0000 };


int main(void)
{
	configuracion();
	while (1)
	{
		/* Evaluamos los cambios de estado en función de la lectura de los switches y
		 mostramos lo que corresponda por la pantalla OLED */
		evaluar_estados(lee_switches(swnf));
	}
}

/**********************************************************************
 *Interrupcion de recepcion de datos del conversor A/D
 * En nuestro caso, dado que las dos interrupciones trabajan a la misma frecuencia,
 * configuraremos en la subrutina de interrupcion de una de ellas lo que queremos que hagan ambas dos
 **********************************************************************/

interrupt void ISR_I2S_rx(void)
{
	int salida;
	int salida_aux;  // Canal 1 cuando lo necesitemos
	if (estado_efectos == menu_e)
	{
		// Por defecto, cuando estemos en el menu, la salida sera igual que la entrada, sin aplicarle ningun efecto ni procesado
		canal0_salida = canal0_entrada;
		canal1_salida = canal1_entrada;
	}
	// En cada uno de los restantes casos, la salida correspondera a la salida de la funcion implementada para cada efecto
	else if (estado_efectos == reverb_e)
	{
		salida = efecto_reverb(canal0_entrada);
		canal0_salida = salida;
		canal1_salida = salida;
	}
	else if (estado_efectos == eco_e)
	{
		salida = efecto_eco(canal0_entrada);
		canal0_salida = salida;
		canal1_salida = salida;
	}
	else if (estado_efectos == coro_e)
	{
		salida = efecto_coro(canal0_entrada);
		canal0_salida = salida;
		canal1_salida = salida;
	}
	else if (estado_efectos == wahwah_e)
	{
		salida = efecto_wahwah1(canal0_entrada);
		canal0_salida = salida;
		salida_aux = efecto_wahwah2(canal0_entrada);
		canal1_salida = salida_aux;
	}
	else if (estado_efectos == vaiven_e)
	{
		salida = efecto_vaiven(canal0_entrada);
		canal0_salida = salida;
		canal1_salida = salida;
	}
	else if (estado_efectos == robot_e)
	{
		salida = efecto_robot(canal0_entrada);
		canal0_salida = salida;
		canal1_salida = salida;
	}
	else if (estado_efectos == telef_e)
	{
		salida = efecto_telef(canal0_entrada);
		canal0_salida = salida;
		canal1_salida = salida;
	}
	ledsminicadena(canal0_salida); // En vez de mostrar canal0_entrada
}

/**********************************************************************
 *Funcion donde agruparemos todas las configuraciones necearias para el funcionamiento del programa
 **********************************************************************/

void configuracion(void)
{
	// Situar los vectores de interrupción
	CPU_IVPD = 0x0480;
	CPU_IVPH = 0x0480;

	// Configuración I2C. Bus de control del codec
	USBSTK5515_I2C_init();

	// Configuracion codec AIC 3204 48 KHz, 16 bits, estereo
	AIC3204_init();
	AIC3204_rset(0, 0); // Select page 0
	AIC3204_rset(65, 0); // DAC gain to 0dB VOL. Controlamos la ganancia de salida para proteger los oídos POSIBLE IMPLEMENTACIÓN DE UN CONTROL DE VOLUMEN

	// Configuracion GPIO para acceder a los leds e inicializarlos
	IODIR1 = 0xC000;
	IODIR2 = 0x03;
	IODATAOUT1 = 0xC000;
	IODATAOUT2 = 0x03;

	// Configuracion del conversor AD
	SARCLKCTRL = 0x0031;
	SARPINCTRL = 0x3100;
	SARGPOCTRL = 0;

	// Configuro el timer_0 a 20ms
	TIM0PRD1 = 0x423F;
	TIM0PRD2 = 0x000F;
	TIM0TCR = 0x8003;

	// Iniciar a 0 la salida para el códec
	I2S2_W0_LSW_W = 0;
	I2S2_W0_MSW_W = 0;
	I2S2_W1_LSW_W = 0;
	I2S2_W1_MSW_W = 0;

	// Activar las interrupciones deseadas
	CPU_IER0 = (1 << 15); // Se habilita solo la interrupción de recepción
	CPU_IER1 = 0;

	_enable_interrupts();
	oled_init();
}

/**********************************************************************
 *Funcion donde cambiaremos las caracteristicas de la entrada para adaptarlas a una entrada de micrófono o de linea
 **********************************************************************/

void activar_microfono(int micro_on)
{
	// Entrada: Un 1 activa la opción micrófono, un 0 activa la entrada de línea
	if (micro_on)
	{
		AIC3204_init();
		// AQUÍ ACTIVAMOS EL MICROFONO
		AIC3204_rset(0, 1); // Select page 1
		AIC3204_rset(51, 0x48); // Power up MICBIAS with AVDD (0x40) or LDOIN (0x48)
		// CONFIGURAR LA GANANCIA DEL MICROFONO
		AIC3204_rset(0, 0); // Select page 0
		AIC3204_rset(83, 0x24);	// Control de volumen del conversor ADC en el canal izquierdo
		AIC3204_rset(84, 0x24); // Control de volumen del conversor ADC en el canal derecho (Máximo 0x28)
	}
	else
	{
		AIC3204_init();	// Reiniciamos los valores para la entrada de línea
	}
}

/**********************************************************************
 *Funcion empleada para el encendido o apagado de los leds
 **********************************************************************/

void estadoleds(int verde, int rojo, int amarillo, int azul)
{
	// Introducir un 0 en cada entrada para apagar el led correspondiente, un 1 para encenderlo y un 2 para dejarlo como está
	if (verde == 0)
	{
		IODATAOUT2 |= 0x02;
	}
	else if (verde == 1)
	{
		IODATAOUT2 &= 0x01;
	}

	if (rojo == 0)
	{
		IODATAOUT2 |= 0x01;
	}
	else if (rojo == 1)
	{
		IODATAOUT2 &= 0x02;
	}

	if (azul == 0)
	{
		IODATAOUT1 |= 0x4000;
	}
	else if (azul == 1)
	{
		IODATAOUT1 &= 0x8000;
	}

	if (amarillo == 0)
	{
		IODATAOUT1 |= 0x8000;
	}
	else if (amarillo == 1)
	{
		IODATAOUT1 &= 0x4000;
	}
}

/* *********************************************************************
 *Funcion para leer la tension correspondiente del circuito R correspondiente a los switches para poder diferenciar
 *cual de ellos se ha pulsado y poder trabajar con el.
 ********************************************************************* */

unsigned int lee_switches(int swn)
{
/* DEVUELVE LA LECTURA DE LOS SWITCHES EN LA FORMA SW1=1, SW2=2, SW12=3
 * el parametro de entrada swn sirve para indicar de cual de los posibles switches (1,2 o 3) se quiere detectar una posible pulsacion prolongada
 * de dicho switch y no de cualquier otro, la variable empleada como booleano (0 o 1) que indicara si se trata de una pulsacion prolongada o no
 * es, en este caso, una variable glogal puls_sat, que sera un 1 si se mantiene un switch pulsado, siendo 1 a partir de un tiempo determinado de pulsacion
 */
	SARCLKCTRL = 0x0031;
	SARPINCTRL = 0x3100;
	SARGPOCTRL = 0;
	SARCTRL = 0xB400;
	unsigned int dato_ADC, puls, dato_leido;
	typedef enum { no_puls, puls_sw1, puls_sw2, puls_sw12 } estados_t;
	static estados_t estado = no_puls;
	static unsigned int contadortiempo = 0;

	while(SARDATA & 0x8000);
	dato_ADC = SARDATA & 0x03FF;

	puls = 4;
	if (dato_ADC > 1023 - 12)
	{
		puls = 0;
	}
	else if ((dato_ADC > 682 - 12) && (dato_ADC < 682 + 12))
	{
		puls = 1;
	}
	else if ((dato_ADC > 512 - 12) && (dato_ADC < 512 + 12))
	{
		puls = 2;
	}
	else if ((dato_ADC > 409 - 12) && (dato_ADC < 409 + 12))
	{
		puls = 3;
	}
	const estados_t trans[4][5] =  {{no_puls, puls_sw1, puls_sw2, puls_sw12, no_puls},
									{no_puls, puls_sw1, no_puls, no_puls, puls_sw1},
									{no_puls, no_puls, puls_sw2, no_puls, puls_sw2},
									{no_puls, puls_sw12, puls_sw12, puls_sw12, puls_sw12}};
	const unsigned int salidas[4][5] = {{0, 1, 2, 3, 0},
										{0, 0, 0, 0, 0},
										{0, 0, 0, 0, 0},
										{0, 0, 0, 0, 0}};

	/* puls indica el switch que ha sido detectado, en caso de que dicha pulsacion coincida con el switch que se quiere controlar se incrementara
	 * un contador hasta cierto tiempo, en caso de que estos no coincidan, quiere decir que el switch a controlar no es el pulsado, por lo que
	 * no se trata de una pulsacion prolongada
	 */
	if(puls == swn)
	{
		contadortiempo += 1;
	}
	else
	{
		contadortiempo = 0;
	}

	if(contadortiempo >= 20) // Tiempo para entender una pulsacion larga
	{
		puls_sat = 1;
	}
	else
	{
		puls_sat = 0;
	}

	dato_leido = salidas[estado][puls];
	estado = trans[estado][puls];
	return dato_leido;
}

/**********************************************************************
 *Funcion donde agruparemos las distintas configuraciones para el "movimiento" de los leds en funcion del uso que se les quiera dar en cada momento
 **********************************************************************/

void ledsminicadena(int entrada)
{
	static int posicion = 3; //en caso de trabajar con adaptacion, situaremos inicialmente la posicion en los valores medios de la tabla
	//Contadores empleados para controlar la adaptacion de los leds a la intensidad
	static unsigned int cnt_asc_rg = 0;
	static unsigned int cnt_dec_rg = 0;

	/*En caso de que la MEF se encuentre en el estado wahwah, los leds "bailaran" de un lado a otro a la misma velocidad y en el mismo sentido
	* que lo hara la salida entre los dos canales
	*/
	if (estado_efectos == wahwah_e)
	{
		if (g <= (Mwahwah >> 2) && g >= 0)	//6554
		{
			estadoleds(0, 0, 0, 1);
		}
		else if (g <= (Mwahwah >> 1) && g > (Mwahwah >> 2))
		{

			estadoleds(0, 0, 1, 0);
		}
		else if (g <= 3 * (Mwahwah >> 2) && g > (Mwahwah >> 1))
		{
			estadoleds(0, 1, 0, 0);
		}
		else if (g > 3 * (Mwahwah >> 2))
		{
			estadoleds(1, 0, 0, 0);
		}
	}
	// En el resto de los casos, los leds iran en funcion de la amplitud de una de las salidas, ya sea con adaptacion o no
	else
	{
		if (cambio_leds)
		{
			/*Para poder realizar la adaptacion de los leds a la amplitud de la salida empleamos una tabla de rangos de valores
			 * previamente definida por la cual nos moveremos, siendo el rango de valores inicial el situado en el punto medio de la tabla
			 * *Para controlar la velocidad de la adaptacion y el cuando puede ser necesaria empleamos dos contadores para ascender o descender
			 * en la tabla.
			 * El contador encargado de hacer descender el rango, es decir, adaptarse a volúmenes menores de los esperados,
			 * se incrementara siempre que todos ellos esten apagados, de forma que si esta situacion se repite reiteradamente se cambiaran los valores
			 * a unos menores.
			 * Por otro lado, el contador encargado de ascender en la tabla se incrementara cuando la amplitud de la señal supere el valor
			 * maximo tenido de los esperados.
			 * Los tiempos para adaptarse son distintos, el tiempo para una adaptacion a rangos menores es mas lento debido a que, en algunas
			 * canciones, o bien a lo largo de una oratoria prolongada hay pausas, las cuales, salvo que sean muy largas, queremos no interpretarlas
			 * como una necesidad de adaptacion.
			 * Ambos contadores se inicializan cada vez que se realiza una adaptacion, por otro lado, si se trata de una amplitud mayor de la
			 * maxima, el contador encargado de descender tambien es inicializado.
			 */
			if ((entrada <= valores_led[posicion][0] && entrada >= 0) || (entrada <= 0 && entrada >= -valores_led[posicion][0]))
			{
				estadoleds(0, 0, 0, 0);
				cnt_dec_rg += 1;
			}
			else if ((entrada <= valores_led[posicion][1] && entrada >= 0) || (entrada <= 0 && entrada >= -valores_led[posicion][1]))
			{
				estadoleds(1, 0, 0, 0);
			}
			else if ((entrada <= valores_led[posicion][2] && entrada >= 0) || (entrada <= 0 && entrada >= -valores_led[posicion][2]))
			{
				estadoleds(1, 1, 0, 0);
			}
			else if ((entrada <= valores_led[posicion][3] && entrada >= 0) || (entrada <= 0 && entrada >= -valores_led[posicion][3]))
			{
				estadoleds(1, 1, 1, 0);
			}
			else if ((entrada <= valores_led[posicion][4] && entrada >= 0) || (entrada <= 0 && entrada >= -valores_led[posicion][4]))
			{
				estadoleds(1, 1, 1, 1);
			}
			else
			{
				cnt_asc_rg += 1;
				cnt_dec_rg = 0;
			}
			if (cnt_dec_rg == 45000)
			{
				cnt_dec_rg = 0;
				cnt_asc_rg = 0;
				if (posicion > 0)
					posicion = posicion - 1;
			}
			else if (cnt_asc_rg == 25000)
			{
				cnt_asc_rg = 0;
				cnt_dec_rg = 0;
				if (posicion < 7)
				{
					posicion = posicion + 1;
				}
			}
		}
		else if (cambio_leds == 0)
		{
			// En caso de no tener adaptacion se fijan unos valores de amplitudes variables en funcion del tipo de entrada con el que se trabaje
			if ((entrada <= 1639 && entrada >= 0) || (entrada <= 0 && entrada >= -1639))
			{
				estadoleds(0, 0, 0, 0);
			}
			else if ((entrada <= 3277 * (2 - mic_on) && entrada >= 0) || (entrada <= 0 && entrada >= -3277 * (2 - mic_on)))
			{
				estadoleds(1, 0, 0, 0);
			}
			else if ((entrada <= 4915 * (2 - mic_on) && entrada >= 0) || (entrada <= 0 && entrada >= -4915 * (2 - mic_on)))
			{
				estadoleds(1, 1, 0, 0);
			}
			else if ((entrada <= 6554 * (2 - mic_on) && entrada >= 0) || (entrada <= 0 && entrada >= -6554 * (2 - mic_on)))
			{
				estadoleds(1, 1, 1, 0);
			}
			else if ((entrada <= 8192 * (2 - mic_on) && entrada >= 0) || (entrada <= 0 && entrada >= -8192 * (2 - mic_on)))
			{
				estadoleds(1, 1, 1, 1);
			}
		}
	}
}

/**********************************************************************
 *Funcion donde declararemos la Maquina de Estados Finitos, esqueleto del programa, moviendonos por ella a traves de los switches.
 *Incluye tambien las representaciones en el display OLED correspondientes a cada uno de los efectos.
 **********************************************************************/

void evaluar_estados(int lect)
{
	// Variables internas
	char pantalla[20] = "                   "; // Variable sobre la que escribiremos los cambios que realice el usuario en los efectos
	int tiempo_pantalla; // Tiempo de duracion del efecto que corresponde a la parte decimal de segundos
	int tiempo_pantalla_s; // Tiempo de duracion del efecto que corresponde a los segundos

	if (estado_efectos == menu_e)
	{
		if (lect == 3) // Para volver más fácilmente al estado inicial desde cualquier otro estado del menu
		{
			estado_menu = microfono_m; // Es el estado mas relevante pues sirve para seleccionar el tipo de entrada
		}
		else if (lect == 2)
		{
			if (estado_menu <= microfono_m) // Recorrer los estados de forma circular hasta que se alcanza el ultimo, aunque es considerado el estado inicial
			{
				estado_menu++;
			}
			else
			{
				estado_menu = 0;
			}
		}
		else if (lect == 1)
		{
			if (estado_menu == microfono_m)
			{
				mic_on ^= 1;
				activar_microfono(mic_on);
			}
			else if (estado_menu == leds_m)
			{
				cambio_leds ^= 1;
			}
			else
			{
				estado_efectos = estado_menu; // La numración de los efectos se corresponde en ambas variables
			}
		}
		// Representaciones en el display OLED de la MEF correspondiente al menu
		if (estado_menu == microfono_m)
		{
			if (mic_on)
			{
				oled_display_message_and_picture("Entrada: MICROFONO ", empty_line, menu_entrada);
			}
			else
			{
				oled_display_message_and_picture("Entrada: LINE IN   ", empty_line, menu_entrada);
			}
		}
		else if (estado_menu == leds_m)
		{
			if (cambio_leds)
			{
				oled_display_message_and_picture("Leds con adaptacion", empty_line, menu_leds);
			}
			else
			{
				oled_display_message_and_picture("Leds sin adaptacion", empty_line, menu_leds);
			}
		}
		else if (estado_menu == reverb_m)
		{
			oled_display_message_and_picture("   REVERBERACION   ", empty_line, menu_efectos);
		}
		else if (estado_menu == eco_m)
		{
			oled_display_message_and_picture("        ECO        ", empty_line, menu_efectos);
		}
		else if (estado_menu == coro_m)
		{
			oled_display_message_and_picture("        CORO       ", empty_line, menu_efectos);
		}
		else if (estado_menu == wahwah_m)
		{
			oled_display_message_and_picture("       WAHWAH      ", empty_line, menu_efectos);
		}
		else if (estado_menu == vaiven_m)
		{
			oled_display_message_and_picture("       VAIVEN      ", empty_line, menu_efectos);
		}
		else if (estado_menu == robot_m)
		{
			oled_display_message_and_picture("       ROBOT       ", empty_line, menu_efectos);
		}
		else if (estado_menu == telef_m)
		{
			oled_display_message_and_picture("     telefonico    ", empty_line, menu_efectos);
		}
	}
	else
	{
		if (lect == 3) // Al detectar el sw3 se vuelve, desde cada efecto, al menu en su posicion correspondiente
		{
			// Al volver al menu hay que reiniciar los parametros modificables por el usuario
			estado_efectos = menu_e;
			vaiven_k = 1; // Reiniciamos el tiempo de vaiven
			wahwah_k = 1; // Reiniciamos el tiempo de wahwah
			retardo_eco = 0; // Reiniciamos el retardo del eco
			retardo_rev = 0; // Reiniciamos el retardo de la reverberación
			acum_1 = 0; // Reiniciamos los acumuladores de los filtros para que no afecten a otros efectos
			acum_2 = 0; // Reiniciamos los acumuladores de los filtros para que no afecten a otros efectos
		}
		// Representacion en el display de los efectos y las modificaciones que realice el usuario
		if (estado_efectos == reverb_e)
		{
			if ((lect == 1 || (puls_sat == 1 && swnf == 1)) && retardo_rev > 0 + 960 * puls_sat)
			{
				retardo_rev -= 960 * (1 + puls_sat);
				swnf = 1;
			}
			else if ((lect == 2 || (puls_sat == 1 && swnf == 2)) && retardo_rev < (32768 - 960 * (1 + puls_sat)))
			{
				retardo_rev += 960 * (1 + puls_sat);
				swnf = 2;
			}
			tiempo_pantalla = ((retardo_rev*(long)tiempo_muestreo + ((long)1 << 22)) >> 24);
			sprintf(pantalla, "REVERBERACION: 0.%.2i", tiempo_pantalla); // Sacamos el valor de retardo del eco con redondeo
			oled_display_message_and_picture(pantalla, empty_line, efecto_con_ajuste);
		}
		else if (estado_efectos == eco_e)
		{
			if ((lect == 1 || (puls_sat == 1 && swnf == 1)) && retardo_eco > 0 + 960 * puls_sat)
			{
				retardo_eco -= 960 * (1 + puls_sat);
				swnf = 1;
			}
			else if ((lect == 2 || (puls_sat == 1 && swnf == 2)) && retardo_eco < (32768 - 960 * (1 + puls_sat)))
			{
				retardo_eco += 960 * (1 + puls_sat);
				swnf = 2;
			}
			tiempo_pantalla = ((retardo_eco*(long)tiempo_muestreo + ((long)1 << 22)) >> 24);
			sprintf(pantalla, "     ECO: 0.%.2i     ", tiempo_pantalla); // Sacamos el valor de retardo del eco con redondeo
			oled_display_message_and_picture(pantalla, empty_line, efecto_con_ajuste);
		}
		else if (estado_efectos == coro_e)
		{
			oled_display_message_and_picture("  CORO DE 4 VOCES  ", empty_line, efecto_sin_ajuste);
		}
		else if (estado_efectos == wahwah_e)
		{
			if ((lect == 1 || (puls_sat == 1 && swnf == 1)) && wahwah_k >= (1 + puls_sat))
			{
				wahwah_k -= (1 + puls_sat);
				swnf = 1;
			}
			else if ((lect == 2 || (puls_sat == 1 && swnf == 2)) && wahwah_k < 502)
			{
				wahwah_k += (1 + puls_sat);
				swnf = 2;
			}
			tiempo_pantalla = (100 * (((long)twah*wahwah_k) & 0x7FFF)) >> 15; // twahwah <16,15> * wahwah_k <16,0> multiplicado por 100 para sacar los 2 decimales
			tiempo_pantalla_s = ((long)twah*wahwah_k) >> 15;
			sprintf(pantalla, "WAHWAH: %.2i.%.2i", tiempo_pantalla_s, tiempo_pantalla);
			oled_display_message_and_picture(pantalla, empty_line, efecto_con_ajuste);
		}
		else if (estado_efectos == vaiven_e)
		{
			if ((lect == 1 || (puls_sat == 1 && swnf == 1)) && (vaiven_k > 1))
			{
				vaiven_k -= (1 + puls_sat);
				swnf = 1;
			}
			else if ((lect == 2 || (puls_sat == 1 && swnf == 2)) && (vaiven_k < 94))
			{
				vaiven_k += (1 + puls_sat);
				swnf = 2;
			}
			tiempo_pantalla = (100 * (((long)tvai * vaiven_k) & 0x7FFF)) >> 15; // tvai <16,15> * vaiven_k <16,0> multiplicado por 100 para sacar los 2 decimales
			tiempo_pantalla_s = ((long)tvai * vaiven_k) >> 15;
			sprintf(pantalla, "vaiven: %.1i.%.2i", tiempo_pantalla_s, tiempo_pantalla);
			oled_display_message_and_picture(pantalla, empty_line, efecto_con_ajuste);
		}
		else if (estado_efectos == robot_e)
		{
			oled_display_message_and_picture("   VOZ DE ROBOT   ", empty_line, efecto_sin_ajuste);
		}
		else if (estado_efectos == telef_e)
		{
			oled_display_message_and_picture("LLAMADA TELEFONICA ", empty_line, efecto_sin_ajuste);
		}
	}
}

/**********************************************************************
 *Funcion donde implementaremos el algoritmo correspondiente al efecto reverberacion
 **********************************************************************/

int efecto_reverb(int entrada_rev)
{
	// El retardo es la diferencia entre el índice de entrada y el índice de salida (de forma circular)
	static unsigned int i_salida_rev = 0;
	static unsigned int i_entrada_rev = 0;
	static const unsigned int atenuacion_rev = 43254; // 0.33 codificado en <16,17>
	int atenuada_rev;
	int salida_rev;

	atenuada_rev = (long)atenuacion_rev*FIFO_retardo[i_salida_rev] >> 17;
	salida_rev = entrada_rev + atenuada_rev;
	i_entrada_rev = (i_entrada_rev + 1) & 32767;
	i_salida_rev = (i_entrada_rev - retardo_rev) & 32767;
	FIFO_retardo[i_entrada_rev] = salida_rev;
	return salida_rev;
}

/**********************************************************************
 *Funcion donde implementaremos el algoritmo correspondiente al efecto eco
 **********************************************************************/

int efecto_eco(int entrada_eco)
{
	// El retardo del eco es la diferencia entre el índice de entrada y el índice de salida (de forma circular)
	static unsigned int i_salida_eco = 0;
	static unsigned int i_entrada_eco = 0;
	static const unsigned int atenuacion_eco = 43254; // 0.33 codificado en <16,17>
	int atenuada_eco;
	int salida_eco;

	i_entrada_eco = (i_entrada_eco + 1) & 32767;
	i_salida_eco = (i_entrada_eco - retardo_eco) & 32767;
	FIFO_retardo[i_entrada_eco] = entrada_eco;

	atenuada_eco = (long)atenuacion_eco*FIFO_retardo[i_salida_eco] >> 17;
	salida_eco = entrada_eco + atenuada_eco;
	return salida_eco;
}

/**********************************************************************
 *Funcion donde implementaremos el algoritmo correspondiente al efecto coro
 **********************************************************************/

int efecto_coro(int entrada_coro)
{
	static unsigned int i_entrada_coro = 0;
	static unsigned int i_salida_coro1 = 0;
	static unsigned int i_salida_coro2 = 0;
	static unsigned int i_salida_coro3 = 0;
	unsigned int atenuacion_coro = 16384; // 0.25 codificado en <16,16>
	int salida_coro;
	int suma_coro;
	static int indice_retardo; // Variaremos el retardo con una frecuencia de 2048Hz
	int retardo_coro = 1920; // 40ms
	int retardo1, retardo2, retardo3;
	indice_retardo = (indice_retardo + 8) & 255;

	i_entrada_coro = (i_entrada_coro + 1) & 32767;

	retardo1 = retardo_coro - (retardo_coro*sin_tbl[indice_retardo] >> 15);
	retardo2 = retardo_coro - (retardo_coro*sin_tbl[(indice_retardo + 85) & 255] >> 15);
	retardo3 = retardo_coro - (retardo_coro*sin_tbl[(indice_retardo + 171) & 255] >> 15);
	i_salida_coro1 = (i_entrada_coro - retardo1) & 32767;
	i_salida_coro2 = (i_entrada_coro - retardo2) & 32767;
	i_salida_coro3 = (i_entrada_coro - retardo3) & 32767;

	FIFO_retardo[i_entrada_coro] = entrada_coro;

	suma_coro = entrada_coro + FIFO_retardo[i_salida_coro1] + FIFO_retardo[i_salida_coro2] + FIFO_retardo[i_salida_coro3];
	salida_coro = ((long)suma_coro*atenuacion_coro) >> 16;
	return salida_coro;
}

/**********************************************************************
 *Funcion donde implementaremos el algoritmo correspondiente al efecto wahwah para el primer canal
 **********************************************************************/

int efecto_wahwah1(entrada_wah)
{
	int salida_wah;
	static int sentido = 0;
	static int comp_t = 0;

	if (comp_t == wahwah_k)
	{
		if (sentido == 0)
		{
			g++;
		}
		else if (sentido == 1)
		{
			g--;
		}
		if (g == Mwahwah || g == 0)
		{
			sentido ^= 1;
		}
		comp_t = 0;
	}
	comp_t++;

	salida_wah = ((long long)(g)* 34953 * entrada_wah) >> 24; // 480-g son valores entre 0 y 480, si multiplicamos por 1/480 ira entre 0 y 1; 34953 = 1/480 <16,24>
	return salida_wah;
}

/**********************************************************************
 *Funcion auxiliar donde implementaremos el algoritmo correspondiente al efecto wahwah para el segundo canal
 **********************************************************************/

int efecto_wahwah2(entrada_wah)
{
	int salida_wah;
	salida_wah = ((long long)(480 - g) * 34953 * entrada_wah) >> 24; // 480-g son valores entre 0 y 480, si multiplicamos por 1/480 ira entre 0 y 1; 34953 = 1/480 <16,24>
	return salida_wah;
}

/**********************************************************************
 *Funcion donde implementaremos el algoritmo correspondiente al efecto vaiven
 **********************************************************************/

int efecto_vaiven(entrada_vaiven)
{
	int salida_vaiven;
	static int cont = 0;
	static int incremento = 0;
	static long int ganancia = 0;

	if (cont == vaiven_k)
	{
		ganancia += incremento;
		if (ganancia >= (1 << 10))
		{
			incremento = -1;
		}
		else if (ganancia <= 0)
		{
			incremento = 1;
		}
		cont = 0;
	}
	cont++;

	salida_vaiven = (ganancia * entrada_vaiven) >> 10;
	return salida_vaiven;
}

/**********************************************************************
 *Funcion donde implementaremos el algoritmo correspondiente al efecto robot
 **********************************************************************/

int efecto_robot(int entrada_robot)
{
	int salida_robot;
	int entrada_filtro, salida_1;
	const int M = 273;
	const int Q = 14;
	const int B1 = 6;
	const int B2 = -47;
	const int b11 = 32093;
	const int b21 = 1;
	const int a11 = -29244;
	const int a21 = 13830;

	const int b12 = -32768;
	const int b22 = 1;
	const int a12 = -31969;
	const int a22 = 15658;

	static int indice = 0;
	static int salida_cos = 0;
	salida_cos = sin_tbl[((indice >> 8) + 64) & 0x00FF];
	entrada_filtro = ((long)(entrada_robot)*salida_cos) >> 15; // El coseno esta en <16,15>

	salida_1 = filtro_orden2(entrada_filtro, b11, b21, a11, a21, Q, B1);
	salida_1 = ((long)salida_1 * 24323) >> 18;
	salida_robot = filtro_orden2(salida_1, b12, b22, a12, a22, Q, B2);
	indice += M;
	return salida_robot;
}

/**********************************************************************
 *Funcion donde implementaremos el algoritmo correspondiente al efecto telefonico
 **********************************************************************/

int efecto_telef(int entrada_telef)
{
	int salida_telef;
	int y1, y2, y3, y4, y5, y0;
	const int Q = 14;
	const int B1 = 4;
	const int B2 = 3;
	const int B3 = -4;
	const int B4 = 2;
	const int B5 = -2;
	const int B6 = -1;
	const long int g0 = 16664; // <16,16>  (2.7025e-04)^(1/6) = 0.2542  separamos la ganancia para tener menos cuantificar
	// Coeficientes primer filtro
	const long int b11 = 6676; // <16,14>
	const long int a11 = -30056; // <16,14>
	const long int a12 = 14297; // <16,14>
	// Coeficientes segundo filtro
	const long int b21 = -20661; // <16,14>
	const long int a21 = -29311; // <16,14>
	const long int a22 = 14742; // <16,14>
	// Coeficientes tercer filtro
	const long int b31 = -32767; // <16,14>
	const long int a31 = -31776; // <16,14>
	const long int a32 = 15499; // <16,14>
	// Coeficientes cuarto filtro
	const long int b41 = -24991; // <16,14>
	const long int a41 = -29439; // <16,14>
	const long int a42 = 15823; // <16,14>
	// Coeficientes quinto filtro
	const long int b51 = -32763; // <16,14>
	const long int a51 = -32465; // <16,14>
	const long int a52 = 16119; // <16,14>
	// Coeficientes sexto filtro
	const long int b61 = -32759; // <16,14>
	const long int a61 = -32705; // <16,14>
	const long int a62 = 16346; // <16,14>

	y0 = entrada_telef;
	y1 = filtro_orden2(y0, b11, 1, a11, a12, Q, B1);
	y1 = (((long)y1*g0) >> 16);
	y2 = filtro_orden2(y1, b21, 1, a21, a22, Q, B2);
	y2 = (((long)y2*g0) >> 16);
	y3 = filtro_orden2(y2, b31, 1, a31, a32, Q, B3);
	y3 = (((long)y3*g0) >> 16);
	y4 = filtro_orden2(y3, b41, 1, a41, a42, Q, B4);
	y4 = (((long)y4*g0) >> 16);
	y5 = filtro_orden2(y4, b51, 1, a51, a52, Q, B5);
	y5 = (((long)y5*g0) >> 16);
	salida_telef = filtro_orden2(y5, b61, 1, a61, a62, Q, B6);
	salida_telef = (((long)salida_telef*g0) >> 16);
	return salida_telef;
}

/**********************************************************************
 *Funcion donde implementaremos el algoritmo correspondiente a un filtro genérico de orden 2 basandonos en la separacion de filtros
 *en diferentes filtros de orden 2 en cascada, empleando matroz SOS para los coeficientes
 **********************************************************************/

int filtro_orden2(int entrada_orden2, int b1i, int b2i, int a1i, int a2i, int Q, int B)
{
	/* Dadas las características de la matriz sos, b2i solo puede ser 1 o -1
	 * Q corresponde a la escala de cuantizacion de los coeficientes, es preciso que todos esten codificados con la misma Q
	 * B corresponde al número de bist extra que hay que dejar para la parte entera correspondiente a la suma de la respuesta impulsional del filtro
	 * dado que en nuestro caso los filtros por separardo no tienen ganancia 1, es necesario realizar este ajuste de escala
	 */
	int salida_filtro;
	long int x_0, x_1, x_2, y_1, y_2;
	x_1 = (long)entrada_orden_2*b1i; // <16,15>*<16,Q> = <32,15+Q>
	x_2 = (long)entrada_orden2*(b2i << Q); // <16,15>*<16,Q> = <32,15+Q>

	x_0 = ((long)entrada_orden2 << (Q - B)); // La entrada codificada como un entero de 32 bits para realizar bien la suma con el acum1
	salida_filtro = (x_0 + acum_1) >> Q; // <32,15+Q-B>+<32,15+Q-B> >>Q=<16,15-B> dejamos B bits para la parte entera, debido a que los filtros no tienen porque tener ganancia 1
	y_1 = (long)salida_filtro*(-a1i); // <16,15-B>*<16,Q> = <32,15+Q-B>
	y_2 = (long)salida_filtro*(-a2i); // <16,15-B>*<16,Q> = <32,15+Q-B>

	acum_1 = (x_1 >> B) + y_1 + acum_2; // <32,15+Q-B>
	acum_2 = (x_2 >> B) + y_2; // <32,15+Q-B>
	return salida_filtro;
}
