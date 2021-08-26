

#include <utft.h>

TFT_ORIENTATION_t DisplayOrientation;
TFT_FONT_SIZE_t FontSize;
extern unsigned char TFT_Large_Font[32 * 95];
extern unsigned char TFT_Small_Font[12 * 95];



/* Rutina de envío de comando										*
 *  recibe:															*
 *  	- uint16_t, el comando a enviar								*
 *  retorna:														*
 *  	- nada                                                      */
void TFT_Command_Send(uint16_t Command)
{
	TFT_Control_Pin_State(TFT_RS, false);
	TFT_Control_Pin_State(TFT_WR, false);
	TFT_Bus_Write(Command);
    TFT_Control_Pin_State(TFT_WR, true);
}


/* Rutina de envío de dato											*
 *  recibe:															*
 *  	- uint16_t, el dato a enviar								*
 *  retorna:														*
 *  	- nada                                                      */
void TFT_Data_Send(uint16_t Data)
{
	TFT_Control_Pin_State(TFT_RS, true);
	TFT_Control_Pin_State(TFT_WR, false);
	TFT_Bus_Write(Data);
    TFT_Control_Pin_State(TFT_WR, true);
}


/* Pone el dato/comando en el bus paralelo							*
 *  recibe:															*
 *  	- uint16_t, el valor a poner en el bus						*
 *  retorna:														*
 *  	- nada                                                      */
void TFT_Bus_Write(uint16_t Value)
{
    bool pinState;
    TFT_Control_Pin_State(TFT_CS, false);

    pinState = (Value & (1<<0)) ? true : false;
    Chip_GPIO_SetPinState(LPC_GPIO, TFT_DB0_PORT, TFT_DB0_PIN, pinState);
    pinState = (Value & (1<<1)) ? true : false;
    Chip_GPIO_SetPinState(LPC_GPIO, TFT_DB1_PORT, TFT_DB1_PIN, pinState);
    pinState = (Value & (1<<2)) ? true : false;
    Chip_GPIO_SetPinState(LPC_GPIO, TFT_DB2_PORT, TFT_DB2_PIN, pinState);
    pinState = (Value & (1<<3)) ? true : false;
    Chip_GPIO_SetPinState(LPC_GPIO, TFT_DB3_PORT, TFT_DB3_PIN, pinState);
    pinState = (Value & (1<<4)) ? true : false;
    Chip_GPIO_SetPinState(LPC_GPIO, TFT_DB4_PORT, TFT_DB4_PIN, pinState);
    pinState = (Value & (1<<5)) ? true : false;
    Chip_GPIO_SetPinState(LPC_GPIO, TFT_DB5_PORT, TFT_DB5_PIN, pinState);
    pinState = (Value & (1<<6)) ? true : false;
    Chip_GPIO_SetPinState(LPC_GPIO, TFT_DB6_PORT, TFT_DB6_PIN, pinState);
    pinState = (Value & (1<<7)) ? true : false;
    Chip_GPIO_SetPinState(LPC_GPIO, TFT_DB7_PORT, TFT_DB7_PIN, pinState);
    pinState = (Value & (1<<8)) ? true : false;
    Chip_GPIO_SetPinState(LPC_GPIO, TFT_DB8_PORT, TFT_DB8_PIN, pinState);
    pinState = (Value & (1<<9)) ? true : false;
    Chip_GPIO_SetPinState(LPC_GPIO, TFT_DB9_PORT, TFT_DB9_PIN, pinState);
    pinState = (Value & (1<<10)) ? true : false;
    Chip_GPIO_SetPinState(LPC_GPIO, TFT_DB10_PORT, TFT_DB10_PIN, pinState);
    pinState = (Value & (1<<11)) ? true : false;
    Chip_GPIO_SetPinState(LPC_GPIO, TFT_DB11_PORT, TFT_DB11_PIN, pinState);
    pinState = (Value & (1<<12)) ? true : false;
    Chip_GPIO_SetPinState(LPC_GPIO, TFT_DB12_PORT, TFT_DB12_PIN, pinState);
    pinState = (Value & (1<<13)) ? true : false;
    Chip_GPIO_SetPinState(LPC_GPIO, TFT_DB13_PORT, TFT_DB13_PIN, pinState);
    pinState = (Value & (1<<14)) ? true : false;
    Chip_GPIO_SetPinState(LPC_GPIO, TFT_DB14_PORT, TFT_DB14_PIN, pinState);
    pinState = (Value & (1<<15)) ? true : false;
    Chip_GPIO_SetPinState(LPC_GPIO, TFT_DB15_PORT, TFT_DB15_PIN, pinState);
}


/* Rutina de inicialización de la pantalla							*
 *  recibe:															*
 *  	- nada														*
 *  retorna:														*
 *  	- nada                                                      */
void TFT_Init()
{
    /* Power control A */
    TFT_Command_Send(0x00CB);
    TFT_Data_Send(0x0039);
    TFT_Data_Send(0x002C);
    TFT_Data_Send(0x0000);
    TFT_Data_Send(0x0034);
    TFT_Data_Send(0x0002);

    /* Power control B */
    TFT_Command_Send(0x00CF);
    TFT_Data_Send(0x0000);
    TFT_Data_Send(0X00C1);
    TFT_Data_Send(0X0030);

    /* Driver timing control A */
    TFT_Command_Send(0x00E8);
    TFT_Data_Send(0x0085);
    TFT_Data_Send(0x0000);
    TFT_Data_Send(0x0078);

    /* Driver timing control B */
    TFT_Command_Send(0x00EA);
    TFT_Data_Send(0x0000);
    TFT_Data_Send(0x0000);

    /* Power on sequence control */
    TFT_Command_Send(0x00ED);
    TFT_Data_Send(0x0064);
    TFT_Data_Send(0x0003);
    TFT_Data_Send(0X0012);
    TFT_Data_Send(0X0081);

    /* Pump ratio control */
    TFT_Command_Send(0x00F7);
    TFT_Data_Send(0x0020);

    /* Power Control 1 */
    TFT_Command_Send(0x00C0);
    TFT_Data_Send(0x0023);

    /* Power Control 2 */
    TFT_Command_Send(0x00C1);
    TFT_Data_Send(0x0010);

    /* VCOM Control 1 */
    TFT_Command_Send(0x00C5);
    TFT_Data_Send(0x003E);
    TFT_Data_Send(0x0028);

    /* VCOM Control 2 */
    TFT_Command_Send(0x00C7);
    TFT_Data_Send(0x0086);

    /* Memory Access Control */
    TFT_Command_Send(0x0036);
    TFT_Data_Send(0x0048);

    /* Pixel Format 16 bits */
    TFT_Command_Send(0x003A);
    TFT_Data_Send(0x0055);

    /* Frame Rate 79Hz */
    TFT_Command_Send(0x00B1);
    TFT_Data_Send(0x0000);
    TFT_Data_Send(0x001B);

    /* Display Function Control */
    TFT_Command_Send(0x00B6);
    TFT_Data_Send(0x0008);
    TFT_Data_Send(0x0082); // Normally white
    TFT_Data_Send(0x0027); // 320 lines
}

/* Inicialización de los pines correspondientes						*
 *  recibe:															*
 *  	- nada														*
 *  retorna:														*
 *  	- nada                                                      */
void TFT_GPIO_Init(void)
{
    /* Inicializa los pines de datos */
    Chip_GPIO_SetPinDIROutput(LPC_GPIO, TFT_DB0_PORT, TFT_DB0_PIN);
    Chip_GPIO_SetPinDIROutput(LPC_GPIO, TFT_DB1_PORT, TFT_DB1_PIN);
    Chip_GPIO_SetPinDIROutput(LPC_GPIO, TFT_DB2_PORT, TFT_DB2_PIN);
    Chip_GPIO_SetPinDIROutput(LPC_GPIO, TFT_DB3_PORT, TFT_DB3_PIN);
    Chip_GPIO_SetPinDIROutput(LPC_GPIO, TFT_DB4_PORT, TFT_DB4_PIN);
    Chip_GPIO_SetPinDIROutput(LPC_GPIO, TFT_DB5_PORT, TFT_DB5_PIN);
    Chip_GPIO_SetPinDIROutput(LPC_GPIO, TFT_DB6_PORT, TFT_DB6_PIN);
    Chip_GPIO_SetPinDIROutput(LPC_GPIO, TFT_DB7_PORT, TFT_DB7_PIN);
    Chip_GPIO_SetPinDIROutput(LPC_GPIO, TFT_DB8_PORT, TFT_DB8_PIN);
    Chip_GPIO_SetPinDIROutput(LPC_GPIO, TFT_DB9_PORT, TFT_DB9_PIN);
    Chip_GPIO_SetPinDIROutput(LPC_GPIO, TFT_DB10_PORT, TFT_DB10_PIN);
    Chip_GPIO_SetPinDIROutput(LPC_GPIO, TFT_DB11_PORT, TFT_DB11_PIN);
    Chip_GPIO_SetPinDIROutput(LPC_GPIO, TFT_DB12_PORT, TFT_DB12_PIN);
    Chip_GPIO_SetPinDIROutput(LPC_GPIO, TFT_DB13_PORT, TFT_DB13_PIN);
    Chip_GPIO_SetPinDIROutput(LPC_GPIO, TFT_DB14_PORT, TFT_DB14_PIN);
    Chip_GPIO_SetPinDIROutput(LPC_GPIO, TFT_DB15_PORT, TFT_DB15_PIN);

    /* Inicializa los pines de control */
    Chip_GPIO_SetPinDIROutput(LPC_GPIO, TFT_RST_PORT, TFT_RST_PIN);
    Chip_GPIO_SetPinDIROutput(LPC_GPIO, TFT_WR_PORT, TFT_WR_PIN);
    Chip_GPIO_SetPinDIROutput(LPC_GPIO, TFT_RS_PORT, TFT_RS_PIN);
    Chip_GPIO_SetPinDIROutput(LPC_GPIO, TFT_CS_PORT, TFT_CS_PIN);
    Chip_GPIO_SetPinDIROutput(LPC_GPIO, TFT_RD_PORT, TFT_RD_PIN);

    /* Estado inicial de los pines de control */
    TFT_Control_Pin_State(TFT_RD, true);
    TFT_Control_Pin_State(TFT_RS, true);
    TFT_Control_Pin_State(TFT_CS, true);
    TFT_Control_Pin_State(TFT_WR, true);

    /* Inicializa el touch pad */
    Chip_IOCON_PinMux(LPC_IOCON, TFT_TOUCH_CLK_PORT, TFT_TOUCH_CLK_PIN, IOCON_MODE_INACT, IOCON_FUNC2);
    Chip_IOCON_PinMux(LPC_IOCON, TFT_TOUCH_DIN_PORT, TFT_TOUCH_DIN_PIN, IOCON_MODE_INACT, IOCON_FUNC2);
    Chip_IOCON_PinMux(LPC_IOCON, TFT_TOUCH_CS_PORT, TFT_TOUCH_CS_PIN, IOCON_MODE_INACT, IOCON_FUNC2);
    Chip_IOCON_PinMux(LPC_IOCON, TFT_TOUCH_DO_PORT, TFT_TOUCH_DO_PIN, IOCON_MODE_INACT, IOCON_FUNC2);
}


/* Tamaño de fuente													*
 *  recibe:															*
 *  	- uint32_t Mode,											*
 *  			TFT_FONT_SIZE_LARGE									*
 *  			TFT_FONT_SIZE_SMALL									*
 *  retorna:														*
 *  	- nada                                                      */
void TFT_Set_Font(uint32_t Mode)
{
	if(Mode)
		FontSize = TFT_FONT_SIZE_LARGE;
	else
		FontSize = TFT_FONT_SIZE_SMALL;
}


/* Orientación de la pantalla										*
 *  recibe:															*
 *  	- uint32_t Mode,											*
 *  			TFT_ORIENTATION_LANDSCAPE							*
 *  			TFT_ORIENTATION_PORTRAIT							*
 *  retorna:														*
 *  	- nada                                                      */
void TFT_Set_Orientation(uint32_t Mode)
{
	if(Mode)
		DisplayOrientation = TFT_ORIENTATION_LANDSCAPE;
	else
		DisplayOrientation = TFT_ORIENTATION_PORTRAIT;
}


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
void TFT_Control_Pin_State(TFT_CTRL_PIN_t CTRLPin, bool State)
{
    switch(CTRLPin)
    {
        case TFT_RST:
            Chip_GPIO_SetPinState(LPC_GPIO, TFT_RST_PORT, TFT_RST_PIN, State);
            break;
        case TFT_WR:
            Chip_GPIO_SetPinState(LPC_GPIO, TFT_WR_PORT, TFT_WR_PIN, State);
            break;
        case TFT_RS:
            Chip_GPIO_SetPinState(LPC_GPIO, TFT_RS_PORT, TFT_RS_PIN, State);
            break;
        case TFT_CS:
            Chip_GPIO_SetPinState(LPC_GPIO, TFT_CS_PORT, TFT_CS_PIN, State);
            break;
        case TFT_RD:
        	Chip_GPIO_SetPinState(LPC_GPIO, TFT_RD_PORT, TFT_RD_PIN, State);
        	break;
        default:
        	break;
    }
}


/* Establece el área de trabajo										*
 *  recibe:															*
 *  	- uint16_t x0, coordenada inicial en eje X					*
 *  	- uint16_t y0, coordenada inicial en eje Y					*
 *  	- uint16_t x1, coordenada final en eje X					*
 *  	- uint16_t y1, coordenada final en eje Y					*
 *  retorna:														*
 *  	- nada                                                      */
void TFT_Set_Area(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
	/* Column address set */
	TFT_Command_Send(0x2A);
	TFT_Data_Send(x0>>8);
	TFT_Data_Send(x0);
	TFT_Data_Send(x1>>8);
	TFT_Data_Send(x1&0xff);

	/* Page address set */
	TFT_Command_Send(0x2B);
	TFT_Data_Send(y0>>8);
	TFT_Data_Send(y0);
	TFT_Data_Send(y1>>8);
	TFT_Data_Send(y1);

	/* Memory write */
	TFT_Command_Send(0x2C);
}


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
void TFT_Pixel_Set(uint16_t color)
{
	TFT_Data_Send(color);
}


/* Dibuja un caracter												*
 *  recibe:															*
 *  	- uint8_t Char, el caracter a dibujar						*
 *  	- uint16_t X, coordenada en eje X							*
 *  	- uint16_t Y, coordenada en eje Y							*
 *  	- uint16_t ForeColor, color del caracter					*
 *  	- uint16_t BackColor, color del fondo						*
 *  retorna:														*
 *  	- nada                                                      */
void TFT_Char_Print(uint8_t Char, uint16_t X, uint16_t Y, uint16_t ForeColor, uint16_t BackColor)
{
    uint8_t i, j, pixels;
    uint16_t index;

    if(FontSize == TFT_FONT_SIZE_LARGE)
    {
    	if(DisplayOrientation == TFT_ORIENTATION_PORTRAIT)
    	{
    		TFT_Control_Pin_State(TFT_CS, false);

    		/* Es un contador para recorrer la matriz de caracteres */
    		index = ((Char - 32) * 32);
    		/* Establece el área del caracter */
    		TFT_Set_Area(X, Y, X + 15, Y + 15);

    		/* Recorre cada fila de pixeles del caracter */
    		for (j = 0; j < 32; j++)
    		{
    			/* Variable que almacena el valor de los pixeles correspondientes al caracter */
    			pixels = TFT_Large_Font[index];
    			/* Recorre cada pixel */
    			for (i = 0; i < 8; i++)
    			{
    				/* Si el pixel está lleno le corresponde el color de fuente */
    				if ((pixels & (1 << (7 - i))) != 0)
    				{
    					TFT_Pixel_Set(ForeColor);
    				}
    				/* Si el pixel está vacío le corresponde el color de fondo */
    				else
    				{
    					TFT_Pixel_Set(BackColor);
    				}
    			}
    			/* Incrementa el contador de la matriz de caracteres */
    			/* (busca los pixeles de la sig. fila)               */
    			index++;
    		}

    		TFT_Control_Pin_State(TFT_CS, true);
    	}

    	if(DisplayOrientation == TFT_ORIENTATION_LANDSCAPE)
    	{
    	    TFT_Char_Rotate(Char, X, Y, ForeColor, BackColor);
    	}
    }

    /* Misma rutina para fuente pequeña */
    if(FontSize == TFT_FONT_SIZE_SMALL)
    {
        if(DisplayOrientation == TFT_ORIENTATION_PORTRAIT)
        {
        	TFT_Control_Pin_State(TFT_CS, false);

        	index = ((Char - 32) * 12);
        	TFT_Set_Area(X, Y, X + 7, Y + 11);
        	for (j = 0; j < 12; j++)
        	{
        		pixels = TFT_Small_Font[index];
        		for (i = 0; i < 8; i++)
        		{
        			if ((pixels & (1 << (7 - i))) != 0)
        			{
        				TFT_Pixel_Set(ForeColor);
        			}
        			else
        			{
        				TFT_Pixel_Set(BackColor);
        			}
        		}
        		index++;
        	}

        	TFT_Control_Pin_State(TFT_CS, true);
        }

        if(DisplayOrientation == TFT_ORIENTATION_LANDSCAPE)
        {
        	TFT_Char_Rotate(Char, X, Y, ForeColor, BackColor);
        }
    }
}


/* Dibuja un caracter de forma apaisada								*
 *  recibe:															*
 *  	- uint8_t Char, el caracter a dibujar						*
 *  	- uint16_t X, coordenada en eje X							*
 *  	- uint16_t Y, coordenada en eje Y							*
 *  	- uint16_t ForeColor, color del caracter					*
 *  	- uint16_t BackColor, color del fondo						*
 *  retorna:														*
 *  	- nada                                                      */
void TFT_Char_Rotate(uint8_t Char, uint16_t X, uint16_t Y, uint16_t ForeColor, uint16_t BackColor)
{
    uint8_t i, j, pixels;
    uint16_t index;
    uint16_t newx, newy;

    TFT_Control_Pin_State(TFT_CS, false);

    if(FontSize == TFT_FONT_SIZE_LARGE)
    {
    	/* Es un contador para recorrer la matriz de caracteres */
    	index = ((Char - 32) * 32);
    	/* Recorre cada columna de pixeles del caracter */
    	for (j = 0; j < 16; j++)
    	{
    		/* Almacena los primero ocho pixeles correspondientes a la columna */
    		pixels = TFT_Large_Font[index];
    		/* Recorre cada pixel */
    		for (i = 0; i < 8; i++)
        	{
            	/* Cambia la ubicación del pixel para rotar el caracter */
    			newx = X - j;
            	newy = Y + i;
            	/* Establece la nueva ubicación del pixel */
            	TFT_Set_Area(newx, newy, newx + 1, newy + 1);

            	/* Si el pixel está lleno le corresponde el color de fuente */
            	if ((pixels & (1 << (7 - i))) != 0)
            	{
                	TFT_Pixel_Set(ForeColor);
            	}
            	/* Si el pixel está lleno le corresponde el color de fondo */
            	else
            	{
                	TFT_Pixel_Set(BackColor);
            	}
        	}
        	/* Incrementa el contador de la matriz de caracteres */
    		index++;
        	/* Almacena los segundos ocho pixeles correspondientes a la misma columna */
    		pixels = TFT_Large_Font[index];
        	/* Recorre cada pixel */
    		for (i = 8; i < 16; i++)
    		{
            	newx = X - j;
            	newy = Y + i;

            	TFT_Set_Area(newx, newy, newx + 1, newy + 1);

            	if ((pixels & (1 << (15 - i))) != 0)
            	{
                	TFT_Pixel_Set(ForeColor);
            	}
            	else
            	{
                	TFT_Pixel_Set(BackColor);
            	}
        	}

    		index++;
    	}
    }

    /* Misma rutina para fuente pequeña */
    if(FontSize == TFT_FONT_SIZE_SMALL)
    {
    	index = ((Char - 32) * 12);
    	for (j = 0; j < 12; j++)
    	{
    	    pixels = TFT_Small_Font[index];
    	    for (i = 0; i < 8; i++)
    	    {
    	    	newx = X - j;
    	        newy = Y + i;
    	       	TFT_Set_Area(newx, newy, newx + 1, newy + 1);

    	       	if ((pixels & (1 << (7 - i))) != 0)
    	        {
    	            TFT_Pixel_Set(ForeColor);
    	        }
    	        else
    	        {
    	            TFT_Pixel_Set(BackColor);
    	        }
    	     }
    	     index++;
    	}
    }

    TFT_Control_Pin_State(TFT_CS, true);
}


/* Dibuja una cadena de caracteres									*
 *  recibe:															*
 *  	- uint8_t *String, puntero a la cadena						*
 *  	- uint16_t X, coordenada en eje X							*
 *  	- uint16_t Y, coordenada en eje Y							*
 *  	- uint16_t ForeColor, color del caracter					*
 *  	- uint16_t BackColor, color del fondo						*
 *  retorna:														*
 *  	- nada                                                      */
void TFT_String_Print(uint8_t *String, uint16_t X, uint16_t Y, uint16_t ForeColor, uint16_t BackColor)
{
    uint16_t StringLength, i;

    /* Calcula el largo de la frase */
    StringLength = strlen(String);

    /* Recorre cada caracter */
    for (i = 0; i < StringLength; i++)
    {
    	if(DisplayOrientation == TFT_ORIENTATION_PORTRAIT)
            TFT_Char_Print(*String++, X+(i*16), Y, ForeColor, BackColor);
    	if(DisplayOrientation == TFT_ORIENTATION_LANDSCAPE)
            TFT_Char_Rotate(*String++, X, Y+(i*16), ForeColor, BackColor);
    }
}


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
void TFT_Number_Print(uint32_t Number, uint16_t X, uint16_t Y, uint16_t ForeColor, uint16_t BackColor, bool flag_zero)
{
    uint32_t i = 0;
    uint32_t c = 0;
    uint8_t buffer[20]; // Es un buffer donde se va guardando la factorización del número
    uint8_t string[21]; // El número se transforma en una cadena de caracteres
    bool negative = false;
    bool single = 0;

    if (0 == Number)
    {
        string[0] = 48;
        if(flag_zero)
        	string[1] = 48;
        string[2] = 0;
    }
    else
    {
        /* Si es negativo le aplica el módulo */
    	if (Number < 0)
        {
            negative = true;
            Number = -Number;
            /* Es el caracter correspondiente al signo '-' */
            string[0] = 45;
        }

        if (flag_zero && (Number<10))
        {
        	string[0] = 48;
        	single = 1;
        }

        /* Se factoriza el número */
    	while (Number > 0)
        {
            buffer[c] = 48 + (Number % 10);
            c++;
            Number = (Number - (Number % 10)) / 10;
        }

        /* Arma la cadena de caracteres */
    	for (i = 0; i < c; i++)
        {
            string[i + negative + single] = buffer[c - i - 1];
        }

    	/* Es el caracter NULL */
    	string[c + negative + single] = 0;
    }

    TFT_String_Print(string, X, Y, ForeColor, BackColor);
}


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
void TFT_Screen_Fill(uint16_t color)
{
    uint32_t i;
    uint32_t j;

    TFT_Control_Pin_State(TFT_CS, false);
    TFT_Set_Area(0, 0, 239, 319);
    for(i=0;i<320;i++)
    {
    	for(j=0;j<240;j++)
    	{
    		TFT_Pixel_Set(color);
    	}
    }
    TFT_Control_Pin_State(TFT_CS, true);
}


/* Dibuja una linea horizontal										*
 *  recibe:															*
 *  	- uint16_t X, coordenada en eje X							*
 *  	- uint16_t Y, coordenada en eje Y							*
 *  	- uint16_t Length, largo de la línea en píxels				*
 *  	- uint16_t Color, color de la línea 						*
 *  retorna:														*
 *  	- nada                                                      */
void TFT_H_Line_Draw(uint16_t X, uint16_t Y, uint16_t Length, uint16_t Color)
{
    uint16_t i = 0;
    TFT_Control_Pin_State(TFT_CS, false);
    TFT_Set_Area(X, Y, X + Length, Y);
    for (i = 0; i < Length + 1; i++)
    {
	    TFT_Pixel_Set(Color);
    }
    TFT_Control_Pin_State(TFT_CS, true);
}


/* Dibuja una linea vertical										*
 *  recibe:															*
 *  	- uint16_t X, coordenada en eje X							*
 *  	- uint16_t Y, coordenada en eje Y							*
 *  	- uint16_t Length, largo de la línea en píxels				*
 *  	- uint16_t Color, color de la línea 						*
 *  retorna:														*
 *  	- nada                                                      */
void TFT_V_Line_Draw(uint16_t X, uint16_t Y, uint16_t Length, uint16_t Color)
{
    uint16_t i = 0;
    TFT_Control_Pin_State(TFT_CS, false);
    TFT_Set_Area(X, Y, X, Y + Length);
    for (i = 0; i < Length; i++)
    {
	    TFT_Pixel_Set(Color);
    }
    TFT_Control_Pin_State(TFT_CS, true);
}


/* Dibuja un rectángulo vacío										*
 *  recibe:															*
 *  	- uint16_t X1, coordenada inicial en eje X					*
 *  	- uint16_t Y1, coordenada inicial en eje Y					*
 *  	- uint16_t X2, coordenada final en eje X					*
 *  	- uint16_t Y2, coordenada final en eje Y					*
 *  	- uint16_t Color, color del rectángulo 						*
 *  retorna:														*
 *  	- nada                                                      */
void TFT_Rectangle_Draw(uint16_t X1, uint16_t Y1, uint16_t X2, uint16_t Y2, uint16_t Color)
{
    uint16_t swappingTemporary;

    if (X1 > X2)
    {
        swappingTemporary = X1;
        X1 = X2;
        X2 = swappingTemporary;
    }
    if (Y1 > Y2)
    {
        swappingTemporary = Y1;
        Y1 = Y2;
        Y2 = swappingTemporary;
    }

    TFT_H_Line_Draw(X1, Y1, X2 - X1, Color);
    TFT_H_Line_Draw(X1, Y2, X2 - X1, Color);
    TFT_V_Line_Draw(X1, Y1, Y2 - Y1, Color);
    TFT_V_Line_Draw(X2, Y1, Y2 - Y1, Color);
}


/* Dibuja un rectángulo relleno										*
 *  recibe:															*
 *  	- uint16_t X1, coordenada inicial en eje X					*
 *  	- uint16_t Y1, coordenada inicial en eje Y					*
 *  	- uint16_t X2, coordenada final en eje X					*
 *  	- uint16_t Y2, coordenada final en eje Y					*
 *  	- uint16_t Color, color del rectángulo 						*
 *  retorna:														*
 *  	- nada                                                      */
void TFT_Filled_Rectangle_Draw(uint16_t X1, uint16_t Y1, uint16_t X2, uint16_t Y2, uint16_t Color)
{
    uint16_t swappingTemporary, i = 0;

    if (X1 > X2)
    {
        swappingTemporary = X1;
        X1 = X2;
        X2 = swappingTemporary;
    }
    if (Y1 > Y2)
    {
        swappingTemporary = Y1;
        Y1 = Y2;
        Y2 = swappingTemporary;
    }

    for (i = 0; i < ((X2 - X1) / 2) + 1; i++)
    {
        TFT_V_Line_Draw(X1 + i, Y1, Y2 - Y1, Color);
        TFT_V_Line_Draw(X2 - i, Y1, Y2 - Y1, Color);
    }
}
