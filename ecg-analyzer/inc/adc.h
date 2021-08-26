

#ifndef ADC_DRIVERS_H_
#define ADC_DRIVERS_H_

#include "chip.h"
#include <cr_section_macros.h>


#define ADC_CH0_PORT	0
#define ADC_CH0_PIN		23
#define ADC_CH1_PORT	0
#define ADC_CH1_PIN		24
#define ADC_CH2_PORT	0
#define ADC_CH2_PIN		25
#define ADC_CH3_PORT	0
#define ADC_CH3_PIN		26
#define ADC_CH4_PORT	1
#define ADC_CH4_PIN		30
#define ADC_CH5_PORT	1
#define ADC_CH5_PIN		31
#define ADC_CH6_PORT	0
#define ADC_CH6_PIN		3
#define ADC_CH7_PORT	0
#define ADC_CH7_PIN		2



/* Inicialización del ADC								*
 *  recibe:												*
 *  	- nada											*
 *  retorna:											*
 *  	- nada									        */
void init_adc(void);

/* Filtro FIR pasa-bajos de primer orden  							*
 *  recibe:															*
 *  	- uint16_t, el valor de la muestra actual					*
 *  retorna:														*
 *  	- uint16_t, el valor de la muestra filtrada                 */
uint16_t filter(uint16_t);

/* Acondiciona la señal a los límites de la pantalla				*
 *  recibe:															*
 *  	- uint16_t, el valor de la muestra filtrada					*
 *  retorna:														*
 *  	- uint32_t, el valor de la muestra acondicionada            */
uint32_t acond(uint16_t dato);

/* Función de ajuste del tiempo de muestreo				*
 *  recibe:												*
 *  	- uint32_t, el tiempo en milisegundos			*
 *  retorna:											*
 *  	- nada									        */
void adjust_sample_time(uint32_t tiempo_ms);

/* Función de ajuste del coeficiente 1 					*
 * Este coeficiente es un atenuador						*
 * Se deshabilita valiendo 1							*
 *  recibe:												*
 *  	- uint32_t, el valor del coeficiente			*
 *  retorna:											*
 *  	- nada									        */
void adjust_coef_1(uint32_t coef_1);

/* Función de ajuste del coeficiente 2 					*
 * Este coeficiente modifica el valor medio sin 		*
 * modificar la amplitud								*
 * Se deshabilita valiendo 0							*
 *  recibe:												*
 *  	- uint32_t, el valor del coeficiente			*
 *  retorna:											*
 *  	- nada									        */
void adjust_coef_2(uint32_t coef_2);

/* Función de ajuste del coeficiente 3 					*
 * Este coeficiente modifica la amplitud sin 			*
 * modificar el valor medio								*
 * Se deshabilita valiendo 1							*
 *  recibe:												*
 *  	- float, el valor del coeficiente				*
 *  retorna:											*
 *  	- nada									        */
void adjust_coef_3(float coef_3);

/* Devuelve el tiempo de muestreo actual				*
 *  recibe:												*
 *  	- nada											*
 *  retorna:											*
 *  	- uint32_t, el tiempo en milisegundos	        */
uint32_t get_sample_time(void);

/* Devuelve el coeficiente 1 actual						*
 *  recibe:												*
 *  	- nada											*
 *  retorna:											*
 *  	- uint32_t, el coeficiente				        */
uint32_t get_coef_1(void);

/* Devuelve el coeficiente 2 actual						*
 *  recibe:												*
 *  	- nada											*
 *  retorna:											*
 *  	- uint32_t, el coeficiente				        */
uint32_t get_coef_2(void);

/* Devuelve el coeficiente 3 actual						*
 *  recibe:												*
 *  	- nada											*
 *  retorna:											*
 *  	- float, el coeficiente				      	    */
float get_coef_3(void);

#endif /* ADC_DRIVERS_H_ */
