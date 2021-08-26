
#include <adc.h>
#include "graph_signal.h"

PEAK_DETECTION_MODE peaks = MAX;

extern uint16_t FOREGROUND_COLOR;
extern uint16_t BACKGROUND_COLOR;
extern uint16_t COLOR_MARKER;
extern uint16_t COLOR_CURSOR;
uint16_t TIME_POSITION = 1;
uint16_t SIGNAL_POSITION = axis;
uint16_t LEVEL_MIN = axis - 25;
uint16_t LEVEL_MAX = axis + 25;
uint32_t ADVICE_FLAG = 0;


/* Función para el gráfico de la señal 								*
 *  recibe:															*
 *  	- uint16_t, el valor del punto a graficar en pantalla		*
 *  retorna:														*
 *  	- nada                                                      */
void sampling(uint16_t dato)
{

	/* Limpia la muestra anterior */
	TFT_H_Line_Draw(24,TIME_POSITION,220,BACKGROUND_COLOR);

	if(dato>SIGNAL_POSITION)
		TFT_H_Line_Draw(SIGNAL_POSITION,TIME_POSITION,(dato-SIGNAL_POSITION),FOREGROUND_COLOR);
	else
		TFT_H_Line_Draw(dato,TIME_POSITION,(SIGNAL_POSITION-dato),FOREGROUND_COLOR);


	/* Actualiza la ultima muestra */
	SIGNAL_POSITION = dato;

	/* Avanza al proximo pixel correspondiente al tiempo */
	TIME_POSITION++;
	if(TIME_POSITION == 319)
		TIME_POSITION = 1;

}

/* Función para el cálculo del ritmo cardíaco						*
 * 	recibe: 														*
 * 		- uint16_t, el valor de la muestra graficada en pantalla 	*
 * 	retorna:														*
 * 		- nada                                                    	*/
void bpm(uint16_t dato)
{
	static uint32_t bpm = 0;
	static uint32_t flag = 0;
	static uint32_t time_result = 0;
	static uint32_t time[] = {0,0,0,0,0,0,0,0,0,0};
	static uint32_t index = 0;
	static uint32_t sample_time;
	static uint16_t max = 0;
	static uint16_t min = 4095;

	sample_time = get_sample_time();

	/* Para que una pulsación sea válida debe superar el nivel dos veces */
	if(peaks)
	{
		/* Una en forma descendente y otra en forma ascendente para detectar mínimos */
		if(flag == 0)
		{
			if(dato < LEVEL_MIN)
			{
				flag = 1;
			}
		}
		else
		{
			if(dato > LEVEL_MIN)
			{
				flag = 0;
				if(index!=0)
				{
					/* Condición para evitar picos indeseados */
					if((TIME_POSITION - time[index-1]) > 200 )
					{
						cursors();
						time[index] = TIME_POSITION;
						index++;
						if(index == 10)
							index = 0;
					}
				}
				/* El primer pico siempre se guarda */
				else
				{
					cursors();
					time[index] = TIME_POSITION;
					index++;
				}
			}
		}
	}
	else
	{
		/* Una en forma ascendente y otra en forma descendente para detectar máximos */
		if(flag == 0)
		{
			if(dato > LEVEL_MAX)
			{
				flag = 1;
			}
		}
		else
		{
			if(dato < LEVEL_MAX)
			{
				flag = 0;
				if(index!=0)
				{
					/* Condición para evitar picos indeseados */
					if(((TIME_POSITION - time[index-1])*sample_time) > 200 )
					{
						cursors();
						time[index] = TIME_POSITION;
						index++;
						if(index == 10)
							index = 0;
					}
				}
				/* El primer pico siempre se guarda */
				else
				{
					cursors();
					time[index] = TIME_POSITION;
					index++;
				}
			}
		}
	}

	/* Se obtienen los valores máximos y mínimos de la señal */
	/* Sólo se utiliza una porción de la pantalla */
	if((TIME_POSITION > 80)&&(TIME_POSITION < 240))
	{
		if(dato > max)
			max = dato;
		if(dato < min)
			min = dato;
	}

	/* El cálculo lo hace al terminar la pantalla */
	if(TIME_POSITION == 318)
	{
		index = 0;
		time_result = 0;
		/* Suma los intervalos entre picos */
		while((time[index+1])!=0)
		{
			time_result = time_result + (time[index+1]-time[index]);
			index++;
		}
		/* Condición de no pulso */
		if(time_result == 0)
			bpm = 0;
		/* Si hay pulso se calcula un promedio entre los intervalos */
		else
		{
			/* En muestras */
			time_result = time_result / index;
			/* En milisegundos */
			time_result = time_result * sample_time;
			/* El ritmo cardíaco se calcula por proporción directa */
			bpm = 60000 / time_result;
		}

		/* Condiciones de contorno para muestra válida */
		if(((bpm > 55) && (bpm < 90)) || (bpm == 0) )
		{
			/* Se indican las BPM calculadas */
			if(!ADVICE_FLAG)
			{
				TFT_Set_Font(large);
				TFT_Set_Orientation(landscape);
				/*if(bpm<100)
				{
					TFT_String_Print(" ", 19, 142, FOREGROUND_COLOR, BACKGROUND_COLOR);
				}*/
				TFT_Number_Print(bpm, 19, 110, FOREGROUND_COLOR, BACKGROUND_COLOR, true);
				TFT_String_Print("bpm", 19, 160, FOREGROUND_COLOR, BACKGROUND_COLOR);
			}
		}

		/* La señal se ajusta si no se detectó frecuencia válida o si saturó */
		if( !((bpm > 55) && (bpm < 90) && (max<239)) )
			adjust(max,min);

		/* Se resetean los valores máx y min */
		max = 0;
		min = 4095;

		/* Reseteo las muestras */
		for(index=0;index<10;index++)
			time[index] = 0;
		index = 0;

	}
}

/* Acomoda la señal y los niveles						*
 *  recibe:												*
 *  	- uint16_t, el valor máximo graficado			*
 *		- uint16_t, el valor mínimo graficado			*
 *  retorna:											*
 *  	- nada									        */
void adjust(uint16_t max, uint16_t min)
{
	uint32_t coeficiente;
	uint32_t media;

	/* Se calculan la media entre los valores máximo y mínimo */
	media = (max + min) / 2;

	/* Se ajusta el marcador correspondiente al nivel mínimo */
	LEVEL_MIN = min + 25;
	if(LEVEL_MIN > (axis - 25))
		LEVEL_MIN = axis - 25;

	/* Se ajusta el marcador correspondiente al nivel máximo */
	LEVEL_MAX = max - 25;
	if(LEVEL_MAX < (axis + 25))
		LEVEL_MAX = axis + 25;

	/* Se ajusta el coeficiente para llevar la señal a la zona deseada */
	coeficiente = get_coef_2();

	if(media > axis)
		coeficiente = coeficiente + (media - axis);
	if(media < axis)
		coeficiente = coeficiente - (axis - media);

	adjust_coef_2(coeficiente);

}

/* Función para mostrar los niveles característicos		*
 *  recibe:												*
 *  	- nada											*
 *  retorna:											*
 *  	- nada									    	*/
void levels(void)
{
	TFT_V_Line_Draw(axis - 5, TIME_POSITION - 1, 2, COLOR_MARKER);
	TFT_V_Line_Draw(axis + 5, TIME_POSITION - 1, 2, COLOR_MARKER);
	if(peaks)
		TFT_V_Line_Draw(LEVEL_MIN, TIME_POSITION - 1, 2, COLOR_MARKER);
	else
		TFT_V_Line_Draw(LEVEL_MAX, TIME_POSITION - 1, 2, COLOR_MARKER);
}

/* Función para mostrar los cursores horizontales		*
 *  recibe:												*
 *  	- nada											*
 *  retorna:											*
 *  	- nada									    	*/
void cursors(void)
{
	TFT_H_Line_Draw(24, TIME_POSITION-1, 239, COLOR_CURSOR);
}

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
void advice(uint32_t mensaje)
{
	if(mensaje == 0)
	{
		ADVICE_FLAG = 0;
	}
	if(mensaje == 1)
	{
		ADVICE_FLAG = 1;
		TFT_Set_Font(large);
		TFT_Set_Orientation(landscape);
		TFT_String_Print("Creando imagen", 139, 40, COLOR_MARKER, BACKGROUND_COLOR);
	}
	if(mensaje == 2)
	{
		ADVICE_FLAG = 1;
		TFT_Set_Font(large);
		TFT_Set_Orientation(landscape);
		TFT_String_Print("SD ERROR", 139, 110, COLOR_MARKER, BACKGROUND_COLOR);
	}
	if(mensaje == 3)
	{
		ADVICE_FLAG = 1;
		TFT_Set_Font(small);
		TFT_Set_Orientation(landscape);
		TFT_String_Print("SD OK", 19, 120, COLOR_MARKER, BACKGROUND_COLOR);
	}
	if(mensaje == 4)
	{
		ADVICE_FLAG = 1;
		TFT_Set_Font(small);
		TFT_Set_Orientation(landscape);
		TFT_String_Print("SD ERROR", 19, 100, COLOR_MARKER, BACKGROUND_COLOR);
	}

}





