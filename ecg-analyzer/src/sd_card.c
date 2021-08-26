

#include <sd_card.h>
#include <utft.h>

FATFS FatFs;
FIL File;

volatile UINT Timer;

extern uint16_t FOREGROUND_COLOR;
extern uint16_t BACKGROUND_COLOR;

/*---------------------------------------------------------*/
/* User Provided Timer Function for FatFs module           */
/*---------------------------------------------------------*/
/* This is a real time clock service to be called from     */
/* FatFs module. Any valid time must be returned even if   */
/* the system does not support a real time clock.          */
/* This is not required in read-only configuration.        */
DWORD get_fattime(void)
{
	RTC rtc;

	rtc.year = 2018;
	rtc.month = 12;
	rtc.mday = 1;
	rtc.hour = 12;
	rtc.min = 0;
	rtc.sec = 0;


	return	  ((DWORD)(rtc.year - 1980) << 25)
			| ((DWORD)rtc.month << 21)
			| ((DWORD)rtc.mday << 16)
			| ((DWORD)rtc.hour << 11)
			| ((DWORD)rtc.min << 5)
			| ((DWORD)rtc.sec >> 1);
}


/* Función de inicialización de la memoria				*
 *  recibe:												*
 *  	- nada											*
 *  retorna:											*
 *  	- nada									    	*/
void disk_init(void)
{
	uint8_t sd_status;
	sd_status = disk_initialize(0);
	f_mount(&FatFs, "", 0);
}


/* Función de temporización para la memoria				*
 *  recibe:												*
 *  	- nada											*
 *  retorna:											*
 *  	- nada									    	*/
void memory_clock(void)
{
	Timer++;
	disk_timerproc();
}


/* Crea el archivo del perfil  							*
 *  recibe:												*
 *  	- nada											*
 *  retorna:											*
 *  	- MEMORY_RESULT: OK o FAILED			    	*/
MEMORY_RESULT open_profile_to_write(void)
{
	MEMORY_RESULT result;

	if(f_open(&File,"PROFILE.dat",FA_OPEN_ALWAYS|FA_WRITE)==FR_OK)
		result = OK;
	else
		result = FAILED;
	return result;
}


/* Abre el archivo del perfil en modo lectura			*
 *  recibe:												*
 *  	- nada											*
 *  retorna:											*
 *  	- MEMORY_RESULT: OK o FAILED			    	*/
MEMORY_RESULT open_profile_to_read(void)
{
	MEMORY_RESULT result;
	if(f_open(&File,"PROFILE.dat",FA_READ|FA_OPEN_EXISTING)==FR_OK)
		result = OK;
	else
		result = FAILED;
	return result;
}


/* Cierra el archivo que esté abierto					*
 *  recibe:												*
 *  	- nada											*
 *  retorna:											*
 *  	- nada									    	*/
void close_file(void)
{
	f_close(&File);
}


/* Lee los datos del perfil								*
 *  recibe:												*
 *  	- USER_PROFILE*, puntero a la estructura 		*
 *  		donde se guardaran los datos				*
 *  retorna:											*
 *  	- nada									    	*/
void read_profile(USER_PROFILE* profile)
{
	uint32_t contador;
	f_read(&File,profile,sizeof(USER_PROFILE),&contador);
}


/* Sobrescribe los datos del perfil						*
 *  recibe:												*
 *  	- USER_PROFILE*, puntero a la estructura 		*
 *  		con los datos a escribir					*
 *  retorna:											*
 *  	- nada									    	*/
void write_profile(USER_PROFILE* profile)
{
	uint32_t contador;
	f_write(&File,profile,sizeof(USER_PROFILE),&contador);
}


/* Crea el archivo imagen y el encabezado				*
 *  recibe:												*
 *  	- nada											*
 *  retorna:											*
 *  	- MEMORY_RESULT: OK o FAILED			    	*/
MEMORY_RESULT BMPmake(void)
{
    uint32_t i;
    uint32_t contador;
    uint8_t temp = 0;
    uint8_t dato;
    uint32_t noColor = 256;
    uint32_t end_color = 54+4*noColor;
    MEMORY_RESULT result;


    /* Los datos se sobrescriben */
    if(f_open(&File,"Image.bmp",FA_OPEN_ALWAYS|FA_WRITE)==FR_OK)
    {
    		result = OK;

    		// -- FILE HEADER -- //

    		/* Tipo de fichero */
    		dato = 'B';
    		f_write(&File,&dato,sizeof(uint8_t),&contador);
    		dato = 'M';
    	    f_write(&File,&dato,sizeof(uint8_t),&contador);

    	    /* Tamaño del archivo												*
    	     * 		14 bytes para el encabezado del archivo: 		0x0000E		*
    	     * 		40 bytes para el encabezado del maba de bits:	0x00028		*
    	     * 		256 x 4 bytes para la paleta de colores:		0x00400		*
    	     * 		320 x 240 bytes para los píxeles:				0x12C00		*
    	     *																	*
    	     * 		Tamaño total: 0x13036										*/
    	    dato = 0x36;
    	    f_write(&File,&dato,sizeof(uint8_t),&contador);
    	    dato = 0x30;
    	    f_write(&File,&dato,sizeof(uint8_t),&contador);
    	    dato = 0x01;
    	    f_write(&File,&dato,sizeof(uint8_t),&contador);
    	    dato = 0x00;
    	    f_write(&File,&dato,sizeof(uint8_t),&contador);

    	    /* Campo reservado */
    	    dato = 0x00;
    	    f_write(&File,&dato,sizeof(uint8_t),&contador);
    	    dato = 0x00;
    	    f_write(&File,&dato,sizeof(uint8_t),&contador);
    	    dato = 0x00;
    	    f_write(&File,&dato,sizeof(uint8_t),&contador);
    	    dato = 0x00;
    	    f_write(&File,&dato,sizeof(uint8_t),&contador);

    	    /* Dirección del primer píxel de la imagen: 40 + 14 + 256 x 4 = 1078 = 0x0436 */
    	    dato = 0x36;
    	    f_write(&File,&dato,sizeof(uint8_t),&contador);
    	    dato = 0x04;
    	    f_write(&File,&dato,sizeof(uint8_t),&contador);
    	    dato = 0x00;
    	    f_write(&File,&dato,sizeof(uint8_t),&contador);
    	    dato = 0x00;
    	    f_write(&File,&dato,sizeof(uint8_t),&contador);


    	    // -- BITMAP HEADER -- //

    	    /* Tamaño de la cabecera del bitmap: 40 bytes = 0x28 */
    	    dato = 0x28;
    	    f_write(&File,&dato,sizeof(uint8_t),&contador);
    	    dato = 0x00;
    	    f_write(&File,&dato,sizeof(uint8_t),&contador);
    	    dato = 0x00;
    	    f_write(&File,&dato,sizeof(uint8_t),&contador);
    	    dato = 0x00;
    	    f_write(&File,&dato,sizeof(uint8_t),&contador);

    	    /* Ancho de la imagen en píxels: 240 = 0xF0 */
    	    dato = 0xF0;
    	    f_write(&File,&dato,sizeof(uint8_t),&contador);
    	    dato = 0x00;
    	    f_write(&File,&dato,sizeof(uint8_t),&contador);
    	    dato = 0x00;
    	    f_write(&File,&dato,sizeof(uint8_t),&contador);
    	    dato = 0x00;
    	    f_write(&File,&dato,sizeof(uint8_t),&contador);

    	    /* Alto de la imagen en píxels: 320 = 0x140 */
    	    dato = 0x40;
    	    f_write(&File,&dato,sizeof(uint8_t),&contador);
    	    dato = 0x01;
    	    f_write(&File,&dato,sizeof(uint8_t),&contador);
    	    dato = 0x00;
    	    f_write(&File,&dato,sizeof(uint8_t),&contador);
    	    dato = 0x00;
    	    f_write(&File,&dato,sizeof(uint8_t),&contador);

    	    /* Número de planos */
    	    dato = 0x01;
    	    f_write(&File,&dato,sizeof(uint8_t),&contador);
    	    dato = 0x00;
    	    f_write(&File,&dato,sizeof(uint8_t),&contador);

    	    /* Tamaño de cada píxel: 1 byte = 0x08 */
    	    dato = 0x08;
    	    f_write(&File,&dato,sizeof(uint8_t),&contador);
    	    dato = 0x00;
    	    f_write(&File,&dato,sizeof(uint8_t),&contador);

    	    /* Compresión: no comprimido = 0x00 */
    	    dato = 0x00;
    	    f_write(&File,&dato,sizeof(uint8_t),&contador);
    	    dato = 0x00;
    	    f_write(&File,&dato,sizeof(uint8_t),&contador);
    	    dato = 0x00;
    	    f_write(&File,&dato,sizeof(uint8_t),&contador);
    	    dato = 0x00;
    	    f_write(&File,&dato,sizeof(uint8_t),&contador);

    	    /* Tamaño de la imagen: 320x240 píxels = 76800 = 0x12C00 */
    	    dato = 0x00;
    	    f_write(&File,&dato,sizeof(uint8_t),&contador);
    	    dato = 0x2C;
    	    f_write(&File,&dato,sizeof(uint8_t),&contador);
    	    dato = 0x01;
    	    f_write(&File,&dato,sizeof(uint8_t),&contador);
    	    dato = 0x00;
    	    f_write(&File,&dato,sizeof(uint8_t),&contador);

    	    /* Resolución horizontal */
    	    dato = 0x00;
    	    f_write(&File,&dato,sizeof(uint8_t),&contador);
    	    dato = 0x00;
    	    f_write(&File,&dato,sizeof(uint8_t),&contador);
    	    dato = 0x00;
    	    f_write(&File,&dato,sizeof(uint8_t),&contador);
    	    dato = 0x00;
    	    f_write(&File,&dato,sizeof(uint8_t),&contador);

    	    /* Resolución vertical */
    	    dato = 0x00;
    	    f_write(&File,&dato,sizeof(uint8_t),&contador);
    	    dato = 0x00;
    	    f_write(&File,&dato,sizeof(uint8_t),&contador);
    	    dato = 0x00;
    	    f_write(&File,&dato,sizeof(uint8_t),&contador);
    	    dato = 0x00;
    	    f_write(&File,&dato,sizeof(uint8_t),&contador);

    	    /* Tamaño de la paleta de colores: 256 = 0x100 */
    	    dato = 0x00;
    	    f_write(&File,&dato,sizeof(uint8_t),&contador);
    	    dato = 0x01;
    	    f_write(&File,&dato,sizeof(uint8_t),&contador);
    	    dato = 0x00;
    	    f_write(&File,&dato,sizeof(uint8_t),&contador);
    	    dato = 0x00;
    	    f_write(&File,&dato,sizeof(uint8_t),&contador);

    	    /* Contador de colores importantes */
    	    dato = 0x00;
    	    f_write(&File,&dato,sizeof(uint8_t),&contador);
    	    dato = 0x00;
    	    f_write(&File,&dato,sizeof(uint8_t),&contador);
    	    dato = 0x00;
    	    f_write(&File,&dato,sizeof(uint8_t),&contador);
    	    dato = 0x00;
    	    f_write(&File,&dato,sizeof(uint8_t),&contador);

    	    // -- COLOR PALETTE -- //

    	    /* [AZUL][VERDE][ROJO][RESERVADO] */
    	    dato=0x00;
    	    for (i=54;i<end_color;i+=4)
    	    {
    	        if(( (i>=694)&&(i<822) ) || ( (i>=54)&&(i<182) ) || ( (i>=182)&&(i<310) ) || ( (i>=822)&&(i<950) ))
    	        	f_write(&File,&temp,sizeof(uint8_t),&contador);
    	    	else
    	    		f_write(&File,&dato,sizeof(uint8_t),&contador);

    	        if(( (i>=182)&&(i<310) ) || ( (i>=310)&&(i<438) ) || ( (i>=438)&&(i<566) ) || ( (i>=822)&&(i<950) ))
    	        	f_write(&File,&temp,sizeof(uint8_t),&contador);
    	    	else
    	    		f_write(&File,&dato,sizeof(uint8_t),&contador);

    	    	if(( (i>=438)&&(i<566) ) || ( (i>=566)&&(i<694) ) || ( (i>=694)&&(i<822) ) || ( (i>=822)&&(i<950) ))
    	    		f_write(&File,&temp,sizeof(uint8_t),&contador);
    	    	else
    	    		f_write(&File,&dato,sizeof(uint8_t),&contador);

    	    	f_write(&File,&dato,sizeof(uint8_t),&contador);

    	    	temp+=8;
    	    }

    }

    else
    	result = FAILED;

	return result;
}


/* Dibuja una muestra a lo ancho de la imagen			*
 *  recibe:												*
 *  	- uint16_t, la muestra a dibujar				*
 *  retorna:											*
 *  	- nada									    	*/
void BMPdraw(uint16_t muestra)
{
	static uint32_t muestra_previa = 120;
	uint32_t i;
	uint8_t dato;
	uint32_t contador;

	/* Hago un barrido a lo ancho de la imagen */
	/* La esquina inferior izquierda de la imagen es la superior izquierda de la pantalla */

	for(i=240;i>0;i--)
	{
		/* El intervalo entre la muestra actual y la anterior con el color de fuente */
		if( ((i<=muestra)&&(i>=muestra_previa))  ||  ((i>=muestra)&&(i<=muestra_previa)) )
		{
			switch(FOREGROUND_COLOR)
			{
				case BLACK:
					dato = 0;
					break;
				case BLUE:
					dato = 31;
					break;
				case GREEN:
					dato = 78;
					break;
				case RED:
					dato = 159;
					break;
				case WHITE:
					dato = 223;
					break;
				case AQUA:
					dato = 63;
					break;
				case LIME:
					dato = 95;
					break;
				case YELLOW:
					dato = 127;
					break;
				case FUCHSIA:
					dato = 191;
					break;
				default:
					break;
			}

			f_write(&File,&dato,sizeof(uint8_t),&contador);
		}

		/* El resto con el color de fondo */
		else
		{
			switch(BACKGROUND_COLOR)
			{
				case BLACK:
					dato = 0;
					break;
				case BLUE:
					dato = 31;
					break;
				case GREEN:
					dato = 78;
					break;
				case RED:
					dato = 159;
					break;
				case WHITE:
					dato = 223;
					break;
				case AQUA:
					dato = 63;
					break;
				case LIME:
					dato = 95;
					break;
				case YELLOW:
					dato = 127;
					break;
				case FUCHSIA:
					dato = 191;
					break;
				default:
					break;
			}

			f_write(&File,&dato,sizeof(uint8_t),&contador);
		}
	}

	/* Actualiza el valor de la muestra anterior */
	muestra_previa = muestra;
}
