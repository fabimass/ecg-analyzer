

#include "chip.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"
#include "utft.h"
#include "adc.h"
#include "graph_signal.h"
#include "sd_card.h"
#include <cr_section_macros.h>


xSemaphoreHandle sPulseTFT;

xQueueHandle qADC;
xQueueHandle qSignal2TFT;
xQueueHandle qSignal2SD;
xQueueHandle qTouchScreen;
xQueueHandle qUserProfile;
xQueueHandle qControlSD;


/****************************************************************************************************/
/*************************************** TACTIL *****************************************************/
/****************************************************************************************************/
#ifdef TOUCH

#define LPC_SSP           LPC_SSP0
#define BUFFER_SIZE       6
#define MUESTRAS          11

typedef struct{
	uint32_t X;
	uint32_t Y;
}TOUCH_COORDS;

/* Tarea dedicada al panel táctil */
static void vTouchTask(void *pvParameters)
{
	static Chip_SSP_DATA_SETUP_T xf_setup;
	static uint8_t Tx_Buf[BUFFER_SIZE];
	static uint8_t Rx_Buf[BUFFER_SIZE];

	/* Asigno el tamaño de los buffers */
	xf_setup.length = BUFFER_SIZE;
	/* Asigno el buffer de transmisión */
	xf_setup.tx_data = Tx_Buf;
	/* Asigno el buffer de recepción */
	xf_setup.rx_data = Rx_Buf;

	Chip_SSP_Init(LPC_SSP);
	/* Le cambio la fase al clock. Esto es necesario para la correcta comunicacion con el XPT2046 */
	Chip_SSP_SetFormat(LPC_SSP, SSP_BITS_8, SSP_FRAMEFORMAT_SPI, SSP_CLOCK_CPHA1_CPOL1);
	Chip_SSP_Enable(LPC_SSP);

	/* Cargo los datos a transmitir en el buffer */
	Tx_Buf[0] = 0xD0; // Comando para obtener las coordenadas de x
	Tx_Buf[1] = 0x00;
	Tx_Buf[2] = 0x00;
	Tx_Buf[3] = 0x90; // Comando para obtener las coordenadas de y
	Tx_Buf[4] = 0x00;
	Tx_Buf[5] = 0x00;

	uint32_t index = 0;
	uint32_t x[MUESTRAS];
	uint32_t y[MUESTRAS];
	uint32_t i,j,aux;
	TOUCH_COORDS coords;

	while(1)
	{
		vTaskDelay(10/portTICK_RATE_MS);

		xf_setup.rx_cnt = 0;
		xf_setup.tx_cnt = 0;

		/* Lectura-escritura por SSP (en modo SPI) */
		Chip_SSP_RWFrames_Blocking(LPC_SSP, &xf_setup);

		/* Primero toma una determinada cantidad de muestras */
		if(index < MUESTRAS)
		{
			x[index] = ((Rx_Buf[1]<<4) | (Rx_Buf[2]>>4)) / 8;
			y[index] = ((Rx_Buf[4]<<4) | (Rx_Buf[5]>>4)) / 6;
			index++;
		}

		/* Una vez que tomó todas las muestras ordena los vectores de menor a mayor */
		if(index == MUESTRAS)
		{
			index = 0;
			for(i=0;i<MUESTRAS-1;i++)
			{
				for(j=i+1;j<MUESTRAS;j++)
				{
					if(x[j]<x[i])
					{
						aux = x[j];
						x[j] = x[i];
						x[i] = aux;
					}
					if(y[j]<y[i])
					{
						aux = y[j];
						y[j] = y[i];
						y[i] = aux;
					}
				}
			}

			/* Se queda con la muestra del medio del vector */
			coords.X = x[MUESTRAS/2];
			coords.Y = y[MUESTRAS/2];
			xQueueSendToBack(qTouchScreen,&coords,portMAX_DELAY);

			/* Lo que sigue es para mostrar las coordenadas en pantalla */
			/*if(coords.X < 100)
				TFT_String_Print(" ", 100, 130, LIME, BLACK);
			TFT_Number_Print(coords.X, 100, 100, LIME, BLACK, true);
			if(coords.Y < 100)
				TFT_String_Print(" ", 100, 240, LIME, BLACK);
			TFT_Number_Print(coords.Y, 100, 210, LIME, BLACK, true);*/
		}

	}
}

#endif



/****************************************************************************************************/
/************************************** PANTALLA ****************************************************/
/****************************************************************************************************/
#ifdef TFT_DISPLAY

/* Muestra la señal en pantalla */
static void vPulseTFTTask(void *pvParameters)
{
	/* Configuración de los pines */
	TFT_GPIO_Init();

	/* Hardware reset */
	TFT_Control_Pin_State(TFT_RST, true);
	vTaskDelay(100/portTICK_RATE_MS);
	TFT_Control_Pin_State(TFT_RST, false);
	vTaskDelay(100/portTICK_RATE_MS);
	TFT_Control_Pin_State(TFT_RST, true);
	vTaskDelay(500/portTICK_RATE_MS);
	TFT_Control_Pin_State(TFT_CS, false);

	/* Software reset */
	TFT_Command_Send(0x0001);
	vTaskDelay(1000/portTICK_RATE_MS);

	/* Inicialización */
	TFT_Init();
	/* Sleep Out */
	TFT_Command_Send(0x0011);
	vTaskDelay(200/portTICK_RATE_MS);
	/* Display On */
	TFT_Command_Send(0x29);
	/* Memory write */
	TFT_Command_Send(0x2C);

	TFT_Control_Pin_State(TFT_CS, true);

#ifdef SD_CARD

	USER_PROFILE profile;

	/* Recibe los datos del perfil */
	xQueuePeek(qUserProfile,&profile,portMAX_DELAY);
	/* Si los datos son válidos */
	if(profile.validate==1)
	{
		set_backcolor(profile.backcolor);
		set_forecolor(profile.forecolor);
	}
	/* Si no está la memoria */
	if(profile.validate==0)
	{
		/* Impone colores por defecto */
		set_backcolor(BLACK);
		set_forecolor(LIME);
	}

	/* Presentación */
	intro();
	if(profile.validate==1)
		/* Mensaje de SD OK */
		advice(3);
	if(profile.validate==0)
		/* Mensaje de NO SD */
		advice(4);

	vTaskDelay(4000/portTICK_RATE_MS);
	advice(0);

#else
	set_backcolor(BLACK);
	set_forecolor(YELLOW);

	/* Presentación */
	intro();
	vTaskDelay(4000/portTICK_RATE_MS);
#endif

	pulse_template();
	//xSemaphoreGive(sPulseTFT);

	uint16_t data;

	while(1)
	{
		xSemaphoreTake(sPulseTFT,portMAX_DELAY);
		xSemaphoreGive(sPulseTFT);
		/* Recibe la muestra tomada por el adc, ya acondicionada */
		xQueueReceive(qSignal2TFT,&data,portMAX_DELAY);
		/* Grafica la muestra tomada */
		sampling(data);
		/* Procesa la muestra para el cálculo de las BPM */
		bpm(data);
		/* Muestra los niveles */
		levels();
	}
}

/* Interfaz gráfica */
static void vInterfaceTFTTask(void *pvParameters)
{
	TOUCH_COORDS coords;
	USER_PROFILE profile;
	/* Para saber en qué pantalla está */
	uint32_t menu = 0;
	/* Para dar órdenes a la memoria SD */
	uint32_t order;

	while(1)
	{
		/* Recibe las coordenadas del táctil */
		xQueueReceive(qTouchScreen,&coords,portMAX_DELAY);

		/* Verifica si se presionó la pantalla */
		if((coords.X!=0) || (coords.Y!=341))
		{

				switch(menu)
				{
				/* Está en la pantalla del pulso */
				case 0:

					/* MENU */
					if((coords.Y >= 282) && (coords.Y <= 320) && (coords.X >= 15) && (coords.X <= 34))
					{
						/* Bloquea la tarea que grafica la señal */
						xSemaphoreTake(sPulseTFT,portMAX_DELAY);
						/* Pasa a la pantalla de menú */
						menu_template();
						menu = 1;
					}

					/* SEÑAL */
					if((coords.Y >= 10) && (coords.Y <= 310) && (coords.X >= 50) && (coords.X <= 240))
					{
						order = CREATE_IMAGE;
						xQueueSendToBack(qControlSD,&order,portMAX_DELAY);
					}
					break;

				/* Está en la pantalla de selección de colores */
				case 1:

					/* COLOR DE FUENTE */
					if((coords.Y >= 65) && (coords.Y <= 296) && (coords.X >= 163) && (coords.X <= 190))
					{
						/* Cambia el color de fuente */
						change_forecolor();
					}

					/* COLOR DE FONDO */
					if((coords.Y >= 65) && (coords.Y <= 296) && (coords.X >= 89) && (coords.X <= 117))
					{
						/* Cambia el color de fondo */
						change_backcolor();
					}

					/* OK */
					if((coords.Y >= 282) && (coords.Y <= 320) && (coords.X >= 15) && (coords.X <= 34))
					{
						/* Aplica los cambios */
#ifdef SD_CARD
						xQueueReceive(qUserProfile,&profile,portMAX_DELAY);
						profile.backcolor = get_backcolor();
						profile.forecolor = get_forecolor();
						xQueueSendToBack(qUserProfile,&profile,portMAX_DELAY);
						order = SAVE_PROFILE;
						xQueueSendToBack(qControlSD,&order,portMAX_DELAY);
#endif
						/* Vuelve a la pantalla del pulso */
						pulse_template();
						xSemaphoreGive(sPulseTFT);
						menu = 0;
					}
					break;
				}

				/* Verificación de que se dejó de presionar la pantalla */
				while((coords.X!=0) || (coords.Y!=341))
					xQueueReceive(qTouchScreen,&coords,portMAX_DELAY);


		}
	}
}

#endif



/****************************************************************************************************/
/***************************************** ADC ******************************************************/
/****************************************************************************************************/
#ifdef ADC_SENSOR

/* Temporización para las lecturas del ADC */
static void vTimeADCTask(void *pvParameters)
{
	uint32_t sample_time;

	init_adc();

	/* Setea el tiempo de muestreo */
	adjust_sample_time(12);

	/* Setea los coeficientes de acondicionamiento */
	adjust_coef_1(6);
	adjust_coef_2(180);
	adjust_coef_3(1);

	while(1)
	{
		sample_time = get_sample_time();

		vTaskDelay(sample_time/portTICK_RATE_MS);

		/* Comienza una nueva conversión */
		Chip_ADC_SetStartMode(LPC_ADC, ADC_START_NOW, ADC_TRIGGERMODE_RISING);
	}
}

/* Lectura del ADC */
static void vReadADCTask(void *pvParameters)
{
	uint16_t dataADC;
	while(1)
	{
		/* Recibe el dato del ADC */
		xQueueReceive(qADC,&dataADC,portMAX_DELAY);

		/* Filtra la señal */
		dataADC = filter(dataADC);

		/* Acondiciona el dato */
		dataADC = acond(dataADC);

		/* Envía el dato procesado */
		xQueueOverwrite(qSignal2TFT,&dataADC);
		xQueueOverwrite(qSignal2SD,&dataADC);
	}
}

/* Interrupción de ADC */
void ADC_IRQHandler(void)
{
	static portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
	uint16_t dataADC;

	/* Se lee el ADC */
	Chip_ADC_ReadValue(LPC_ADC, ADC_CH0, &dataADC);

	/* Pone el dato en una cola para su procesamiento */
	xQueueSendToBackFromISR(qADC,&dataADC,&xHigherPriorityTaskWoken);

	portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
}

#endif



/****************************************************************************************************/
/************************************** MEMORIA SD **************************************************/
/****************************************************************************************************/
#ifdef SD_CARD

/* Control de tiempo para la SD */
static void vTimeSDTask(void *pvParameters)
{
	while (1)
	{
		/* Esta función es necesaria para la comunicación con la SD */
		/* Se llama a la función periódicamente cada 50mseg         */
		memory_clock();
		vTaskDelay(50/portTICK_RATE_MS);
	}
}

uint16_t imagen[320];

/* Control de la SD */
static void vControlSDTask(void *pvParameters)
{
	USER_PROFILE profile;
	uint32_t order;
	uint16_t data;
	uint32_t i;

	/* Se inicializa la SD */
	disk_init();

	/* Lo primero que se hace es leer el perfil de usuario */
	if(open_profile_to_read()==OK)
	{
		/* El archivo se abrió correctamente  */
		/* Se lee el archivo de configuración */
		read_profile(&profile);
		/* Cierra el archivo */
		close_file();
		/* Indica que los datos son válidos */
		profile.validate = 1;
		/* Se envían los datos a una cola */
		xQueueSendToBack(qUserProfile,&profile,portMAX_DELAY);
	}
	else
	{
		/* El archivo no existe. Crea el archivo */
		if(open_profile_to_write()==OK)
		{
			profile.backcolor = YELLOW;
			profile.forecolor = BLACK;
			profile.validate = 1;
			write_profile(&profile);
			close_file();
			xQueueSendToBack(qUserProfile,&profile,portMAX_DELAY);
		}
		/* Hay un error con la memoria */
		else
		{
			/* Los datos no son válidos */
			profile.validate = 0;
			xQueueSendToBack(qUserProfile,&profile,portMAX_DELAY);
		}
	}

	while(1)
	{
		/* Espera una orden */
		xQueueReceive(qControlSD,&order,portMAX_DELAY);

		switch(order)
		{
			case SAVE_PROFILE: /* Sobrescribe la información del perfil */

				/* Lee los datos del perfil */
				xQueuePeek(qUserProfile,&profile,portMAX_DELAY);

				/* Se abre el archivo en modo de sobrescritura */
				if(open_profile_to_write()==OK)
				{
					/* El archivo se abrió correctamente */
					write_profile(&profile);
					/* Se cierra el archivo */
					close_file();
				}

				break;

			case CREATE_IMAGE: /* Guarda en la memoria una foto de la señal en pantalla */

				/* Bloquea la tarea que grafica el pulso en la pantalla */
				xSemaphoreTake(sPulseTFT,portMAX_DELAY);

				/* Crea el archivo de imagen */
				if(BMPmake() == OK)
				{
					/* Indica en la pantalla que se está creando la imagen */
					advice(1);

					/* La imagen tiene 320 pixels de alto */
					for(i=0;i<320;i++)
					{
						xQueueReceive(qSignal2SD,&data,portMAX_DELAY);
						imagen[i] = data;
					}

					for(i=0;i<320;i++)
					{
						data = imagen[i];
						/* Dibuja una muestra a lo ancho de la imagen */
						BMPdraw(data);
					}

					/* Cierra el archivo */
					close_file();

					/* Quita el aviso */
					advice(0);
				}

				else
				{
					/* Indica en la pantalla que se produjo un error con la SD */
					advice(2);
					vTaskDelay(1800/portTICK_RATE_MS);
					/* Quita el aviso */
					advice(0);
				}

				/* Libera la gráfica del pulso */
				xSemaphoreGive(sPulseTFT);
		}
	}
}

#endif




/****************************************************************************************************/
/**************************************** MAIN ******************************************************/
/****************************************************************************************************/
int main(void)
{
	SystemCoreClockUpdate();

	vSemaphoreCreateBinary(sPulseTFT);
	//xSemaphoreTake(sPulseTFT,0);

	qADC = xQueueCreate(1,sizeof(uint16_t));
	qSignal2TFT = xQueueCreate(1,sizeof(uint16_t));
	qSignal2SD = xQueueCreate(1,sizeof(uint16_t));
	qTouchScreen = xQueueCreate(1,sizeof(TOUCH_COORDS));
	qUserProfile = xQueueCreate(1,sizeof(USER_PROFILE));
	qControlSD = xQueueCreate(1,sizeof(uint32_t));


#ifdef TFT_DISPLAY
    xTaskCreate(vPulseTFTTask, (char *) "vTaskPulseTFT",
            	configMINIMAL_STACK_SIZE, NULL, (tskIDLE_PRIORITY + 1UL),
            	(xTaskHandle *) NULL);
    xTaskCreate(vInterfaceTFTTask, (char *) "vTaskInterfaceTFT",
                configMINIMAL_STACK_SIZE, NULL, (tskIDLE_PRIORITY + 1UL),
                (xTaskHandle *) NULL);
#endif

#ifdef ADC_SENSOR
    xTaskCreate(vTimeADCTask, (char *) "vTaskTimeADC",
    			configMINIMAL_STACK_SIZE, NULL, (tskIDLE_PRIORITY + 1UL),
    			(xTaskHandle *) NULL);
    xTaskCreate(vReadADCTask, (char *) "vTaskReadADC",
        		configMINIMAL_STACK_SIZE, NULL, (tskIDLE_PRIORITY + 1UL),
        		(xTaskHandle *) NULL);
#endif

#ifdef TOUCH
    xTaskCreate(vTouchTask, (char *) "vTaskTouch",
                configMINIMAL_STACK_SIZE, NULL, (tskIDLE_PRIORITY + 1UL),
                (xTaskHandle *) NULL);
#endif

#ifdef SD_CARD
    xTaskCreate(vTimeSDTask, (char *) "vTaskTimeSD",
            	configMINIMAL_STACK_SIZE, NULL, (tskIDLE_PRIORITY + 2UL),
            	(xTaskHandle *) NULL);
    xTaskCreate(vControlSDTask, (char *) "vTaskControlSD",
                configMINIMAL_STACK_SIZE, NULL, (tskIDLE_PRIORITY + 1UL),
                (xTaskHandle *) NULL);
#endif

    /* Start the scheduler */
	vTaskStartScheduler();

	/* Nunca debería arribar aquí */

    return 0;
}

