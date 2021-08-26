

#include "graph_interface.h"

uint16_t FOREGROUND_COLOR;
uint16_t BACKGROUND_COLOR;
uint16_t COLOR_MARKER;
uint16_t COLOR_CURSOR;


/* Seteo el color de fuente */
void set_forecolor(uint16_t color)
{
	FOREGROUND_COLOR = color;
	set_markercolor();
}

/* Setea el color de fondo */
void set_backcolor(uint16_t color)
{
	BACKGROUND_COLOR = color;
	set_markercolor();
}

/* Devuelve el color de fuente */
uint16_t get_forecolor(void)
{
	return FOREGROUND_COLOR;
}

/* Devuelve el color de fondo */
uint16_t get_backcolor(void)
{
	return BACKGROUND_COLOR;
}

/* Ajusta el color de los marcadores de acuerdo a los colores de fondo y fuente */
void set_markercolor(void)
{
	/* Fondo oscuro */
	if( (BACKGROUND_COLOR == BLACK) || (BACKGROUND_COLOR == BLUE) || (BACKGROUND_COLOR == GREEN) || (BACKGROUND_COLOR == RED) )
	{
			if((FOREGROUND_COLOR == WHITE)||(FOREGROUND_COLOR == BLACK))
			{
				COLOR_MARKER = LIME;
				COLOR_CURSOR = AQUA;
			}
			if((FOREGROUND_COLOR == AQUA)||(FOREGROUND_COLOR == BLUE))
			{
				COLOR_MARKER = YELLOW;
				COLOR_CURSOR = WHITE;
			}
			if((FOREGROUND_COLOR == LIME)||(FOREGROUND_COLOR == GREEN))
			{
				COLOR_MARKER = WHITE;
				COLOR_CURSOR = YELLOW;
			}
			if((FOREGROUND_COLOR == YELLOW)||(FOREGROUND_COLOR == RED))
			{
				COLOR_MARKER = AQUA;
				COLOR_CURSOR = LIME;
			}
	}

	/* Fondo claro */
	if( (BACKGROUND_COLOR == WHITE) || (BACKGROUND_COLOR == AQUA) || (BACKGROUND_COLOR == LIME) || (BACKGROUND_COLOR == YELLOW) )
	{
			if((FOREGROUND_COLOR == WHITE)||(FOREGROUND_COLOR == BLACK))
			{
				COLOR_MARKER = RED;
				COLOR_CURSOR = BLUE;
			}
			if((FOREGROUND_COLOR == AQUA)||(FOREGROUND_COLOR == BLUE))
			{
				COLOR_MARKER = GREEN;
				COLOR_CURSOR = RED;
			}
			if((FOREGROUND_COLOR == LIME)||(FOREGROUND_COLOR == GREEN))
			{
				COLOR_MARKER = BLACK;
				COLOR_CURSOR = BLUE;
			}
			if((FOREGROUND_COLOR == YELLOW)||(FOREGROUND_COLOR == RED))
			{
				COLOR_MARKER = BLUE;
				COLOR_CURSOR = BLACK;
			}
	}
}

/* Limpia la pantalla */
void clean_display(void)
{
	TFT_Filled_Rectangle_Draw(0, 0, 240, 320, BACKGROUND_COLOR);
}

/* Presentación */
void intro(void)
{
	clean_display();
	TFT_Set_Font(large);
	TFT_Set_Orientation(landscape);

	TFT_String_Print("Tecnicas Digitales 3", 185, 0, FOREGROUND_COLOR, BACKGROUND_COLOR);
	TFT_String_Print("Analizador ECG", 140, 45, FOREGROUND_COLOR, BACKGROUND_COLOR);
	TFT_String_Print("Fabian Massotto", 95, 40, FOREGROUND_COLOR, BACKGROUND_COLOR);
	TFT_String_Print("5to 12 - 2018", 50, 55, FOREGROUND_COLOR, BACKGROUND_COLOR);
}

/* Plantilla básica de menú */
void menu_template(void)
{
	clean_display();
	TFT_Set_Font(large);
	TFT_Set_Orientation(landscape);

	TFT_String_Print("Color de fuente", 217, 30, FOREGROUND_COLOR, BACKGROUND_COLOR);
	TFT_Rectangle_Draw(157,29,189,286,COLOR_MARKER);
	TFT_Rectangle_Draw(156,28,190,287,COLOR_MARKER);
	option_forecolor();
	TFT_String_Print("Color de fondo", 140, 30, FOREGROUND_COLOR, BACKGROUND_COLOR);
	TFT_Rectangle_Draw(80,29,112,286,COLOR_MARKER);
	TFT_Rectangle_Draw(79,28,113,287,COLOR_MARKER);
	option_backcolor();
	TFT_String_Print("OK", 20, 7, FOREGROUND_COLOR, BACKGROUND_COLOR);
}

/* Plantilla básica de pantalla que muestra la señal */
void pulse_template(void)
{
	clean_display();
	TFT_Set_Font(small);
	TFT_Set_Orientation(landscape);

	TFT_V_Line_Draw(22,0,320,COLOR_MARKER);
	TFT_V_Line_Draw(23,0,320,COLOR_MARKER);
	TFT_H_Line_Draw(0,70,23,COLOR_MARKER);
	TFT_H_Line_Draw(0,71,23,COLOR_MARKER);
	TFT_String_Print("MENU", 16, 7, COLOR_MARKER, BACKGROUND_COLOR);
}

/* Imprime el color de fuente elegido */
void option_forecolor(void)
{
	TFT_Set_Font(large);
	TFT_Set_Orientation(landscape);
	TFT_String_Print("                ", 188, 30, FOREGROUND_COLOR, FOREGROUND_COLOR);
	TFT_String_Print("                ", 173, 30, FOREGROUND_COLOR, FOREGROUND_COLOR);
}

/* Imprime el color de fondo elegido */
void option_backcolor(void)
{
	TFT_Set_Font(large);
	TFT_Set_Orientation(landscape);
	TFT_String_Print("                ", 111, 30, BACKGROUND_COLOR, BACKGROUND_COLOR);
	TFT_String_Print("                ", 96, 30, BACKGROUND_COLOR, BACKGROUND_COLOR);
}

/* Cambia al siguiente color de fuente */
void change_forecolor(void)
{
	switch(FOREGROUND_COLOR)
	{
	case BLACK:
			if(BACKGROUND_COLOR!=BLUE)
				set_forecolor(BLUE);
			else
				set_forecolor(GREEN);
			break;
	case BLUE:
			if(BACKGROUND_COLOR!=GREEN)
				set_forecolor(GREEN);
			else
				set_forecolor(RED);
			break;
	case GREEN:
			if(BACKGROUND_COLOR!=RED)
				set_forecolor(RED);
			else
				set_forecolor(WHITE);
			break;
	case RED:
			if(BACKGROUND_COLOR!=WHITE)
				set_forecolor(WHITE);
			else
				set_forecolor(AQUA);
			break;
	case WHITE:
			if(BACKGROUND_COLOR!=AQUA)
				set_forecolor(AQUA);
			else
				set_forecolor(LIME);
			break;
	case AQUA:
			if(BACKGROUND_COLOR!=LIME)
				set_forecolor(LIME);
			else
				set_forecolor(YELLOW);
			break;
	case LIME:
			if(BACKGROUND_COLOR!=YELLOW)
				set_forecolor(YELLOW);
			else
				set_forecolor(BLACK);
			break;
	case YELLOW:
			if(BACKGROUND_COLOR!=BLACK)
				set_forecolor(BLACK);
			else
				set_forecolor(BLUE);
			break;
	default:
			break;
	}

	option_forecolor();
}

/* Cambia al siguiente color de fondo */
void change_backcolor(void)
{
	switch(BACKGROUND_COLOR)
		{
		case BLACK:
				if(FOREGROUND_COLOR!=BLUE)
					set_backcolor(BLUE);
				else
					set_backcolor(GREEN);
				break;
		case BLUE:
				if(FOREGROUND_COLOR!=GREEN)
					set_backcolor(GREEN);
				else
					set_backcolor(RED);
				break;
		case GREEN:
				if(FOREGROUND_COLOR!=RED)
					set_backcolor(RED);
				else
					set_backcolor(WHITE);
				break;
		case RED:
				if(FOREGROUND_COLOR!=WHITE)
					set_backcolor(WHITE);
				else
					set_backcolor(AQUA);
				break;
		case WHITE:
				if(FOREGROUND_COLOR!=AQUA)
					set_backcolor(AQUA);
				else
					set_backcolor(LIME);
				break;
		case AQUA:
				if(FOREGROUND_COLOR!=LIME)
					set_backcolor(LIME);
				else
					set_backcolor(YELLOW);
				break;
		case LIME:
				if(FOREGROUND_COLOR!=YELLOW)
					set_backcolor(YELLOW);
				else
					set_backcolor(BLACK);
				break;
		case YELLOW:
				if(FOREGROUND_COLOR!=BLACK)
					set_backcolor(BLACK);
				else
					set_backcolor(BLUE);
				break;
		default:
				break;
		}

		option_backcolor();
}

