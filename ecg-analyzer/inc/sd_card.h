

#ifndef MEMORY_DRIVERS_H_
#define MEMORY_DRIVERS_H_

#include "chip.h"
#include <string.h>
#include <cr_section_macros.h>

#include "diskio.h"
#include "ff.h"


typedef enum{
	OK,
	FAILED
}MEMORY_RESULT;

typedef enum{
	READ_PROFILE,
	SAVE_PROFILE,
	CREATE_IMAGE
}MEMORY_CONTROL;

typedef struct{
	uint16_t backcolor;
	uint16_t forecolor;
	uint32_t validate;
}USER_PROFILE;

typedef struct {
	WORD	year;	/* 1..4095 */
	BYTE	month;	/* 1..12 */
	BYTE	mday;	/* 1.. 31 */
	BYTE	wday;	/* 1..7 */
	BYTE	hour;	/* 0..23 */
	BYTE	min;	/* 0..59 */
	BYTE	sec;	/* 0..59 */
} RTC;

/*---------------------------------------------------------*/
/* User Provided Timer Function for FatFs module           */
/*---------------------------------------------------------*/
/* This is a real time clock service to be called from     */
/* FatFs module. Any valid time must be returned even if   */
/* the system does not support a real time clock.          */
/* This is not required in read-only configuration.        */
DWORD get_fattime(void);

/* Función de inicialización de la memoria				*
 *  recibe:												*
 *  	- nada											*
 *  retorna:											*
 *  	- nada									    	*/
void disk_init(void);

/* Función de temporización para la memoria				*
 *  recibe:												*
 *  	- nada											*
 *  retorna:											*
 *  	- nada									    	*/
void memory_clock(void);

/* Crea el archivo del perfil  							*
 *  recibe:												*
 *  	- nada											*
 *  retorna:											*
 *  	- MEMORY_RESULT: OK o FAILED			    	*/
MEMORY_RESULT open_profile_to_write(void);

/* Abre el archivo del perfil en modo lectura			*
 *  recibe:												*
 *  	- nada											*
 *  retorna:											*
 *  	- MEMORY_RESULT: OK o FAILED			    	*/
MEMORY_RESULT open_profile_to_read(void);

/* Cierra el archivo que esté abierto					*
 *  recibe:												*
 *  	- nada											*
 *  retorna:											*
 *  	- nada									    	*/
void close_file(void);

/* Lee los datos del perfil								*
 *  recibe:												*
 *  	- USER_PROFILE*, puntero a la estructura 		*
 *  		donde se guardaran los datos				*
 *  retorna:											*
 *  	- nada									    	*/
void read_profile(USER_PROFILE* profile);

/* Sobrescribe los datos del perfil						*
 *  recibe:												*
 *  	- USER_PROFILE*, puntero a la estructura 		*
 *  		con los datos a escribir					*
 *  retorna:											*
 *  	- nada									    	*/
void write_profile(USER_PROFILE* profile);

/* Crea el archivo imagen y el encabezado				*
 *  recibe:												*
 *  	- nada											*
 *  retorna:											*
 *  	- MEMORY_RESULT: OK o FAILED			    	*/
MEMORY_RESULT BMPmake(void);

/* Dibuja una muestra a lo ancho de la imagen			*
 *  recibe:												*
 *  	- uint16_t, la muestra a dibujar				*
 *  retorna:											*
 *  	- nada									    	*/
void BMPdraw(uint16_t muestra);


#endif /* MEMORY_DRIVERS_H_ */
