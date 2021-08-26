

#ifndef TFT_H_
#define TFT_H_

#include "chip.h"

typedef enum {
    TFT_RST,
    TFT_WR,
    TFT_RS,
    TFT_CS,
	TFT_RD
} TFT_CTRL_PIN_t;

typedef enum {
    TFT_ORIENTATION_LANDSCAPE,
    TFT_ORIENTATION_PORTRAIT
} TFT_ORIENTATION_t;

typedef enum {
    TFT_FONT_SIZE_SMALL,
    TFT_FONT_SIZE_LARGE
} TFT_FONT_SIZE_t;


// Pines utilizados
#define TFT_DB0_PORT    0
#define TFT_DB0_PIN     4

#define TFT_DB1_PORT    0
#define TFT_DB1_PIN     5

#define TFT_DB2_PORT    0
#define TFT_DB2_PIN     10

#define TFT_DB3_PORT    0
#define TFT_DB3_PIN     11

#define TFT_DB4_PORT    2
#define TFT_DB4_PIN     0

#define TFT_DB5_PORT    2
#define TFT_DB5_PIN     1

#define TFT_DB6_PORT    2
#define TFT_DB6_PIN     2

#define TFT_DB7_PORT    2
#define TFT_DB7_PIN     3

#define TFT_DB8_PORT    2
#define TFT_DB8_PIN     4

#define TFT_DB9_PORT    2
#define TFT_DB9_PIN     5

#define TFT_DB10_PORT   2
#define TFT_DB10_PIN    6

#define TFT_DB11_PORT   2
#define TFT_DB11_PIN    7

#define TFT_DB12_PORT   2
#define TFT_DB12_PIN    8

#define TFT_DB13_PORT   2
#define TFT_DB13_PIN    10

#define TFT_DB14_PORT   2
#define TFT_DB14_PIN    11

#define TFT_DB15_PORT   2
#define TFT_DB15_PIN    12

#define TFT_RST_PORT    2
#define TFT_RST_PIN     13

#define TFT_WR_PORT     0
#define TFT_WR_PIN      21

#define TFT_RS_PORT     0
#define TFT_RS_PIN      3

#define TFT_CS_PORT     1
#define TFT_CS_PIN      31

#define TFT_RD_PORT		0
#define TFT_RD_PIN      22

#define TFT_TOUCH_CLK_PORT		0
#define TFT_TOUCH_CLK_PIN       15

#define TFT_TOUCH_CS_PORT		0
#define TFT_TOUCH_CS_PIN        16

#define TFT_TOUCH_DIN_PORT		0
#define TFT_TOUCH_DIN_PIN       18

#define TFT_TOUCH_DO_PORT		0
#define TFT_TOUCH_DO_PIN        17

#define TFT_TOUCH_IRQ_PORT		0
#define TFT_TOUCH_IRQ_PIN		2


// Colores
#define BLACK		0x0000
#define WHITE		0xFFFF
#define RED			0xF800
#define GREEN		0x0400
#define BLUE		0x001F
#define SILVER		0xC618
#define GRAY		0x8410
#define MAROON		0x8000
#define YELLOW		0xFFE0
#define OLIVE		0x8400
#define LIME		0x07E0
#define AQUA		0x07FF
#define TEAL		0x0410
#define NAVY		0x0010
#define FUCHSIA		0xF81F
#define PURPLE		0x8010



/* Rutina de envío de comando										*
 *  recibe:															*
 *  	- uint16_t, el comando a enviar								*
 *  retorna:														*
 *  	- nada                                                      */
void TFT_Command_Send(uint16_t);

/* Rutina de envío de dato											*
 *  recibe:															*
 *  	- uint16_t, el dato a enviar								*
 *  retorna:														*
 *  	- nada                                                      */
void TFT_Data_Send(uint16_t);

/* Pone el dato/comando en el bus paralelo							*
 *  recibe:															*
 *  	- uint16_t, el valor a poner en el bus						*
 *  retorna:														*
 *  	- nada                                                      */
void TFT_Bus_Write(uint16_t);

/* Rutina de inicialización de la pantalla							*
 *  recibe:															*
 *  	- nada														*
 *  retorna:														*
 *  	- nada                                                      */
void TFT_Init(void);

/* Inicialización de los pines correspondientes						*
 *  recibe:															*
 *  	- nada														*
 *  retorna:														*
 *  	- nada                                                      */
void TFT_GPIO_Init(void);

/* Tamaño de fuente													*
 *  recibe:															*
 *  	- uint32_t Mode,											*
 *  			TFT_FONT_SIZE_LARGE									*
 *  			TFT_FONT_SIZE_SMALL									*
 *  retorna:														*
 *  	- nada                                                      */
void TFT_Set_Font(uint32_t Mode);

/* Orientación de la pantalla										*
 *  recibe:															*
 *  	- uint32_t Mode,											*
 *  			TFT_ORIENTATION_LANDSCAPE							*
 *  			TFT_ORIENTATION_PORTRAIT							*
 *  retorna:														*
 *  	- nada                                                      */
void TFT_Set_Orientation(uint32_t Mode);

/* Setea el estado de los pines de control							*
 *  recibe:															*
 *  	- TFT_CTRL_PIN_t CTRLPin,									*
 *  			TFT_RST (Reset)										*
 *  			TFT_WR	(Write Signal)								*
 *  			TFT_RS	(Data or Command)							*
 *  			TFT_CS	(Chip Select)								*
 *  			TFT_RD	(Read Signal)								*
 *  	- bool State, 												*
 *  			TRUE												*
 *  			FALSE												*
 *  retorna:														*
 *  	- nada                                                      */
void TFT_Control_Pin_State(TFT_CTRL_PIN_t, bool);

/* Establece el área de trabajo										*
 *  recibe:															*
 *  	- uint16_t x0, coordenada inicial en eje X					*
 *  	- uint16_t y0, coordenada inicial en eje Y					*
 *  	- uint16_t x1, coordenada final en eje X					*
 *  	- uint16_t y1, coordenada final en eje Y					*
 *  retorna:														*
 *  	- nada                                                      */
void TFT_Set_Area(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);

/* Establece el color de un píxel									*
 *  recibe:															*
 *  	- uint16_t color,					 						*
 *  			BLACK												*
 *  			WHITE												*
 *  			RED													*
 *  			GREEN												*
 *  			BLUE												*
 *  			SILVER												*
 *  			GRAY												*
 *  			MAROON												*
 *  			YELLOW												*
 *  			OLIVE												*
 *  			LIME												*
 *  			AQUA												*
 *  			TEAL												*
 *  			NAVY												*
 *  			FUCHSIA												*
 *  			PURPLE												*
 *  retorna:														*
 *  	- nada                                                      */
void TFT_Pixel_Set(uint16_t Color);

/* Dibuja un caracter												*
 *  recibe:															*
 *  	- uint8_t Char, el caracter a dibujar						*
 *  	- uint16_t X, coordenada en eje X							*
 *  	- uint16_t Y, coordenada en eje Y							*
 *  	- uint16_t ForeColor, color del caracter					*
 *  	- uint16_t BackColor, color del fondo						*
 *  retorna:														*
 *  	- nada                                                      */
void TFT_Char_Print(uint8_t Char, uint16_t X, uint16_t Y, uint16_t ForeColor, uint16_t BackColor);

/* Dibuja un caracter de forma apaisada								*
 *  recibe:															*
 *  	- uint8_t Char, el caracter a dibujar						*
 *  	- uint16_t X, coordenada en eje X							*
 *  	- uint16_t Y, coordenada en eje Y							*
 *  	- uint16_t ForeColor, color del caracter					*
 *  	- uint16_t BackColor, color del fondo						*
 *  retorna:														*
 *  	- nada                                                      */
void TFT_Char_Rotate(uint8_t Char, uint16_t X, uint16_t Y, uint16_t ForeColor, uint16_t BackColor);

/* Dibuja una cadena de caracteres									*
 *  recibe:															*
 *  	- uint8_t *String, puntero a la cadena						*
 *  	- uint16_t X, coordenada en eje X							*
 *  	- uint16_t Y, coordenada en eje Y							*
 *  	- uint16_t ForeColor, color del caracter					*
 *  	- uint16_t BackColor, color del fondo						*
 *  retorna:														*
 *  	- nada                                                      */
void TFT_String_Print(uint8_t *String, uint16_t X, uint16_t Y, uint16_t ForeColor, uint16_t BackColor);

/* Dibuja un número													*
 *  recibe:															*
 *  	- uint32_t Number, el número a dibujar						*
 *  	- uint16_t X, coordenada en eje X							*
 *  	- uint16_t Y, coordenada en eje Y							*
 *  	- uint16_t ForeColor, color del caracter					*
 *  	- uint16_t BackColor, color del fondo						*
 *  	- bool	flag_zero,											*
 *  				TRUE, pone un 0 delante si nro < 10				*
 *  				FALSE, no lo hace								*
 *  retorna:														*
 *  	- nada														*/
void TFT_Number_Print(uint32_t Number, uint16_t X, uint16_t Y, uint16_t ForeColor, uint16_t BackColor, bool flag_zero);

/* Llena la pantalla de un color									*
 *  recibe:															*
 *  	- uint16_t, el color elegido								*
 *  			BLACK												*
 *  			WHITE												*
 *  			RED													*
 *  			GREEN												*
 *  			BLUE												*
 *  			SILVER												*
 *  			GRAY												*
 *  			MAROON												*
 *  			YELLOW												*
 *  			OLIVE												*
 *  			LIME												*
 *  			AQUA												*
 *  			TEAL												*
 *  			NAVY												*
 *  			FUCHSIA												*
 *  			PURPLE												*
 *  retorna:														*
 *  	- nada														*/
void TFT_Screen_Fill(uint16_t);

/* Dibuja una linea horizontal										*
 *  recibe:															*
 *  	- uint16_t X, coordenada en eje X							*
 *  	- uint16_t Y, coordenada en eje Y							*
 *  	- uint16_t Length, largo de la línea en píxels				*
 *  	- uint16_t Color, color de la línea 						*
 *  retorna:														*
 *  	- nada                                                      */
void TFT_H_Line_Draw(uint16_t X, uint16_t Y, uint16_t Length, uint16_t Color);

/* Dibuja una linea vertical										*
 *  recibe:															*
 *  	- uint16_t X, coordenada en eje X							*
 *  	- uint16_t Y, coordenada en eje Y							*
 *  	- uint16_t Length, largo de la línea en píxels				*
 *  	- uint16_t Color, color de la línea 						*
 *  retorna:														*
 *  	- nada                                                      */
void TFT_V_Line_Draw(uint16_t X, uint16_t Y, uint16_t Length, uint16_t Color);

/* Dibuja un rectángulo vacío										*
 *  recibe:															*
 *  	- uint16_t X1, coordenada inicial en eje X					*
 *  	- uint16_t Y1, coordenada inicial en eje Y					*
 *  	- uint16_t X2, coordenada final en eje X					*
 *  	- uint16_t Y2, coordenada final en eje Y					*
 *  	- uint16_t Color, color del rectángulo 						*
 *  retorna:														*
 *  	- nada                                                      */
void TFT_Rectangle_Draw(uint16_t X1, uint16_t Y1, uint16_t X2, uint16_t Y2, uint16_t Color);

/* Dibuja un rectángulo relleno										*
 *  recibe:															*
 *  	- uint16_t X1, coordenada inicial en eje X					*
 *  	- uint16_t Y1, coordenada inicial en eje Y					*
 *  	- uint16_t X2, coordenada final en eje X					*
 *  	- uint16_t Y2, coordenada final en eje Y					*
 *  	- uint16_t Color, color del rectángulo 						*
 *  retorna:														*
 *  	- nada                                                      */
void TFT_Filled_Rectangle_Draw(uint16_t X1, uint16_t Y1, uint16_t X2, uint16_t Y2, uint16_t Color);



#endif /* TFT_H_ */
