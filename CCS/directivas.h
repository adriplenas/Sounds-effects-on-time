/*
 * directivas.h
 *
 *  Created on: 30/11/2014
 *      Author: Selarep
 */

#ifndef DIRECTIVAS_H_
#define DIRECTIVAS_H_
// Directivas de librerías a utilizar
#include "SEmP_5515.h"
#include "tabla_seno.h"
#include "usbstk5515_i2c.h"
#include "aic3204.h"
#include "oled.h"
#include <stdio.h>
#include "stdint.h"

// Directivas para hacer más legibles los puertos del codec
#define canal0_salida I2S2_W0_MSW_W
#define canal1_salida I2S2_W1_MSW_W
#define canal0_entrada I2S2_W0_MSW_R
#define canal1_entrada I2S2_W1_MSW_R
#define Mwahwah 480
// Declaración de funciones
void estadoleds(int verde, int rojo, int amarillo, int azul);
void ledsminicadena(int entrada);
void evaluar_estados(int lect);
int efecto_reverb(int entrada);
int efecto_eco(int entrada_eco);
int efecto_coro(int entrada_coro);
int efecto_wahwah1(int entrada_wah);
int efecto_wahwah2(int entrada_wah);
int efecto_vaiven(int entrada_vaiven);
int efecto_robot(int entrada_robot);
int efecto_telef(int entrada_telef);
int filtro_orden2(int entrada_orden2, int b1i, int b2i, int a1i, int a2i, int Q, int B);
unsigned int lee_switches(int swn);
void configuracion(void);
void activar_microfono(int micro_on);
#endif /* DIRECTIVAS_H_ */
