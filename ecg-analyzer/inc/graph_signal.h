

#ifndef PROYTD3_INC_GRAPH_SIGNAL_H_
#define PROYTD3_INC_GRAPH_SIGNAL_H_

#include "graph_interface.h"

#define 	axis       125

typedef enum{
	MAX,
	MIN
}PEAK_DETECTION_MODE;


/* Función para el gráfico de la señal 								*
 *  recibe:															*
 *  	- uint16_t, el valor del punto a graficar en pantalla		*
 *  retorna:														*
 *  	- nada                                                      */
void sampling(uint16_t muestra);

/* Función para el cálculo del ritmo cardíaco						*
 * 	recibe: 														*
 * 		- uint16_t, el valor de la muestra graficada en pantalla 	*
 * 	retorna:														*
 * 		- nada                                                    	*/
void bpm(uint16_t muestra);

/* Acomoda la señal y los niveles						*
 *  recibe:												*
 *  	- uint16_t, el valor máximo graficado			*
 *		- uint16_t, el valor mínimo graficado			*
 *  retorna:											*
 *  	- nada									        */
void adjust(uint16_t max, uint16_t min);

/* Función para mostrar los niveles característicos		*
 *  recibe:												*
 *  	- nada											*
 *  retorna:											*
 *  	- nada									    	*/
void levels(void);

/* Función para mostrar los cursores horizontales		*
 *  recibe:												*
 *  	- nada											*
 *  retorna:											*
 *  	- nada									    	*/
void cursors(void);

/* Función para mostrar mensajes en pantalla			*
 *  recibe:												*
 *  	- uint32_t, el mensaje deseado					*
 *  		0) quita el mensaje							*
 *  		1) creando imagen							*
 *  		2) sd error									*
 *  		3) sd ok									*
 *  		4) sd error									*
 *  retorna:											*
 *  	- nada									    	*/
void advice(uint32_t mensaje);



#endif /* PROYTD3_INC_GRAPH_SIGNAL_H_ */
