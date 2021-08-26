

#include <adc.h>
#include "graph_signal.h"

static ADC_CLOCK_SETUP_T ADCSetup;
uint32_t ADC_SAMPLE_TIME_MS;
uint32_t coef_1;
uint32_t coef_2;
float coef_3;


/* Inicialización del ADC								*
 *  recibe:												*
 *  	- nada											*
 *  retorna:											*
 *  	- nada									        */
void init_adc(void)
{
	/* Las entradas de ADC no utilizadas se declaran como salidas digitales */
	Chip_IOCON_PinMux(LPC_IOCON,ADC_CH0_PORT,ADC_CH0_PIN,IOCON_MODE_INACT,IOCON_FUNC1);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, ADC_CH1_PORT, ADC_CH1_PIN);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, ADC_CH2_PORT, ADC_CH2_PIN);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, ADC_CH3_PORT, ADC_CH3_PIN);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, ADC_CH4_PORT, ADC_CH4_PIN);
	//Chip_GPIO_SetPinDIROutput(LPC_GPIO, ADC_CH5_PORT, ADC_CH5_PIN);
	//Chip_GPIO_SetPinDIROutput(LPC_GPIO, ADC_CH6_PORT, ADC_CH6_PIN);
	//Chip_GPIO_SetPinDIROutput(LPC_GPIO, ADC_CH7_PORT, ADC_CH7_PIN);


	/* Inicializa el ADC */
	Chip_ADC_Init(LPC_ADC, &ADCSetup);
	/* Habilita el canal */
	Chip_ADC_EnableChannel(LPC_ADC, ADC_CH0, ENABLE);
	/* Muestras por segundo */
	Chip_ADC_SetSampleRate(LPC_ADC, &ADCSetup, 100000);
	/* Modo ráfaga deshabilitado */
	Chip_ADC_SetBurstCmd(LPC_ADC, DISABLE);
	/* Habilita la interrupción de ADC */
	Chip_ADC_Int_SetChannelCmd(LPC_ADC, ADC_CH0, ENABLE);
	NVIC_EnableIRQ(ADC_IRQn);

}


/* Filtro FIR pasa-bajos de primer orden  							*
 *  recibe:															*
 *  	- uint16_t, el valor de la muestra actual					*
 *  retorna:														*
 *  	- uint16_t, el valor de la muestra filtrada                 */
uint16_t filter(uint16_t data_new)
{
#ifdef FILTER_PLUS

	static uint16_t data_old = 0;
	static uint16_t result_old = 0;
	static uint16_t result_aux = 0;
	static uint32_t flag = 0;
	uint16_t result;

	/* Se calcula la media de la muestra actual y de la anterior */
	result = data_new / 2 + data_old / 2 ;

	if(((result-result_old) > 100)&&(flag==0))
	{
		flag = 3;
	}

	/* Se actualiza la muestra anterior */
	data_old = data_new;
	result_old = result;

	if(flag)
	{
		flag--;
		return result_aux;
	}

	result_aux = result;
	return result;

#else

	static uint16_t data_old = 0;
	uint16_t result;

	/* Se calcula la media de la muestra actual y de la anterior */
	result = data_new / 2 + data_old / 2;

	/* Se actualiza la muestra anterior */
	data_old = data_new;

	return result;

#endif
}


/* Acondiciona la señal a los límites de la pantalla				*
 *  recibe:															*
 *  	- uint16_t, el valor de la muestra filtrada					*
 *  retorna:														*
 *  	- uint32_t, el valor de la muestra acondicionada            */
uint32_t acond(uint16_t dato)
{
	uint32_t result;

	/* Aplica los coeficientes de acondicionamiento */

	/* El coeficiente 1 es un atenuador */
	result = dato / coef_1;

	if(result < coef_2)
		result = 0;
	else
		/* El coeficiente 2 modifica el valor medio */
		result = result - coef_2;

	/* El coeficiente 3 modifica la amplitud */
	if(coef_3 >= 1)
		result = (float)result * coef_3 - (float)axis * (coef_3-1);
	else
		result = (float)result * coef_3 + (float)axis * (1-coef_3);

	/* Recorta la señal si se va de la pantalla */
	if(result<25)
		result = 25;
	else if(result>238)
		result = 238;

	return result;
}


/* Función de ajuste del tiempo de muestreo				*
 *  recibe:												*
 *  	- uint32_t, el tiempo en milisegundos			*
 *  retorna:											*
 *  	- nada									        */
void adjust_sample_time(uint32_t dato)
{
	ADC_SAMPLE_TIME_MS = dato;
}


/* Función de ajuste del coeficiente 1 					*
 * Este coeficiente es un atenuador						*
 * Se deshabilita valiendo 1							*
 *  recibe:												*
 *  	- uint32_t, el valor del coeficiente			*
 *  retorna:											*
 *  	- nada									        */
void adjust_coef_1(uint32_t dato)
{
	coef_1 = dato;
}


/* Función de ajuste del coeficiente 2 					*
 * Este coeficiente modifica el valor medio sin 		*
 * modificar la amplitud								*
 * Se deshabilita valiendo 0							*
 *  recibe:												*
 *  	- uint32_t, el valor del coeficiente			*
 *  retorna:											*
 *  	- nada									        */
void adjust_coef_2(uint32_t dato)
{
	coef_2 = dato;
}


/* Función de ajuste del coeficiente 3 					*
 * Este coeficiente modifica la amplitud sin 			*
 * modificar el valor medio								*
 * Se deshabilita valiendo 1							*
 *  recibe:												*
 *  	- float, el valor del coeficiente				*
 *  retorna:											*
 *  	- nada									        */
void adjust_coef_3(float dato)
{
	coef_3 = dato;
}


/* Devuelve el tiempo de muestreo actual				*
 *  recibe:												*
 *  	- nada											*
 *  retorna:											*
 *  	- uint32_t, el tiempo en milisegundos	        */
uint32_t get_sample_time(void)
{
	return ADC_SAMPLE_TIME_MS;
}


/* Devuelve el coeficiente 1 actual						*
 *  recibe:												*
 *  	- nada											*
 *  retorna:											*
 *  	- uint32_t, el coeficiente				        */
uint32_t get_coef_1(void)
{
	return coef_1;
}


/* Devuelve el coeficiente 2 actual						*
 *  recibe:												*
 *  	- nada											*
 *  retorna:											*
 *  	- uint32_t, el coeficiente				        */
uint32_t get_coef_2(void)
{
	return coef_2;
}


/* Devuelve el coeficiente 3 actual						*
 *  recibe:												*
 *  	- nada											*
 *  retorna:											*
 *  	- float, el coeficiente				      	    */
float get_coef_3(void)
{
	return coef_3;
}
