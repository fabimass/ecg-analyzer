

#ifndef GRAPH_INTERFACE_H_
#define GRAPH_INTERFACE_H_

#include <utft.h>

#define small      0
#define large      1
#define portrait   0
#define landscape  1


/* Fija el color de fuente */
void set_forecolor(uint16_t color);
/* Fija el color de fondo */
void set_backcolor(uint16_t color);
/* Devuelve el color de fuente */
uint16_t get_forecolor(void);
/* Devuelve el color de fondo */
uint16_t get_backcolor(void);
/* Ajusta el color de los marcadores */
void set_markercolor(void);
/* Pinta la pantalla con el color de fondo */
void clean_display(void);
/* Presentación */
void intro(void);
/* Plantilla básica del menú */
void menu_template(void);
/* Plantilla básica de pantalla que muestra la señal */
void pulse_template(void);
/* Imprime el color de fuente elegido */
void option_forecolor(void);
/* Imprime el color de fondo elegido */
void option_backcolor(void);
/* Cambia al siguiente color de fuente */
void change_forecolor(void);
/* Cambia al siguiente color de fondo */
void change_backcolor(void);


#endif /* GRAPH_INTERFACE_H_ */
