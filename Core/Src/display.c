#include "STM32F4xx.h"
#include "cmsis_os.h"
#include "usb_host.h"
#include <inttypes.h>
#include <string.h>
#include "display.h"
#include "fonts.h"
#include "pics.h"



void LCD_drawGIF(void)
{
    int str_len = 192*158;
	int frame = 0;
    int pos = 0;
    int act_bit = 0;
    int pos_i = 0 ;
	uint16_t x = 0 ;
	uint16_t y = 80;
    LCD_SetCursor(0, 80);
	LCD_WriteCommand(0x0022);		// Command: Pixel "anmalen"
 
	for(frame = 0; frame < 13; frame++)
	{
		pos_i = 0;
		x= 0;
		y = 80;
	for (pos = 0; pos < str_len; pos ++)
		{
			act_bit = walk_gif[pos + (frame * 192*158)];
			LCD_DrawPixel(act_bit);
			pos_i++;
     if(pos_i == 192){
       pos_i = 0;
       y = y+1;
       x = 0;
       LCD_SetCursor(x, y);
			}
			}
	osDelay(100);
			LCD_SetCursor(0, 0);
	}
}


void LCD_drawPic(uint8_t select)
{
    int str_len = 320*240;
    int pos = 0;
    int act_bit = 0;
    int pos_i = 0 ;
    LCD_SetCursor(0, 0);
	LCD_WriteCommand(0x0022);		// Command: Pixel "anmalen"
    switch(select)
	{
		case 0:
			for (pos = 0; pos < str_len; pos ++)
			{
				act_bit = rosi1_320x240[pos];
				LCD_WriteData(act_bit);
				pos_i++;
			}
			break;
			
		case 1:
			for (pos = 0; pos < str_len; pos ++)
			{
				act_bit = rosi2_320x240[pos];
				LCD_WriteData(act_bit);
				pos_i++;
			}
			break;
		
		case 2:
			for (pos = 0; pos < str_len; pos ++)
			{
				act_bit = thi_logo_320x240[pos];
				LCD_WriteData(act_bit);
				pos_i++;
			}
			break;
		
		default:
			break;
	}
}


void DisplayMessung(void)
{
uint16_t colorbg = 0xF800; // Rot
uint16_t colorfg;
while(1) {
GPIOD->ODR ^= 0x1000; // toggle Pin PD12 (gr�ne LED)
LCD_ClearDisplay(colorbg); // l�sche das Display
// gib einen Text aus
LCD_WriteString( "Hallo Welt!", 20, 220, colorfg, colorbg);
// Wechsle Farbe
colorfg = colorbg;
colorbg = ~colorbg;
}
}


// Warte ms Millisekunden und mache nichts...
void delay(int ms){
	int i = 0;
	for(i = 0; i < ms; i++){
	i = i;
 }
}

// Den Cursor auf (X,Y) auf dem Display setzen
void LCD_SetCursor(uint32_t x, uint32_t y){
	LCD_WriteReg(0x004E, x);
	LCD_WriteReg(0x004F, y);
}

// Einzelnen Pixel auf dem Display setzen (-> SetCursor nicht vergessen)
void LCD_DrawPixel(uint16_t color){
	LCD_WriteReg(0x0022, color);
}


// Display mit einer Farbe fuellen
void LCD_ClearDisplay(uint16_t color){
	uint32_t pos = 0;
	LCD_SetCursor(0x0000, 0x0000);
	LCD_WriteCommand(0x0022);
	
	LCD_WriteData(color);
	
	for(pos = 76801; pos > 0; pos--){
		//LCD_WriteData(color);
		GPIOD->ODR &= ~(0x00A0);
	  GPIOD->ODR |= 0x00A0;
		//delay(400);
	}
}

// Gebe einen Character aus. fcolor = Font Farbe, bcolor = Hintergrundfarbe
void LCD_DrawLetter(char c, uint32_t x, uint32_t y, uint16_t fcolor, uint16_t bcolor){
	
	int start_i = c * 32;
	int pos = 0;
	int spalten = 0;
	int zeilen = 0;

	
	LCD_SetCursor(x, y);
	LCD_WriteCommand(0x0022);
    for(spalten = 0; spalten < 16; spalten++){
           
    for(zeilen = 7; zeilen >= 0; zeilen--){
           pos = console_font_12x16[start_i];
        if(pos &= (1 << zeilen)){

            LCD_WriteData(fcolor);
        } else {

            LCD_WriteData(bcolor);
            }
        }
    start_i = start_i +1;
    for(zeilen = 7; zeilen >= 0; zeilen--){
            pos = console_font_12x16[start_i];
        if(pos &= (1 << zeilen)){
            LCD_WriteData(fcolor);
        } else {
            LCD_WriteData(bcolor);
            }
        }
				y++;
        LCD_SetCursor(x, y);					// Neue Zeile
				LCD_WriteCommand(0x0022);		
        start_i = start_i +1;
 }
	 
}


// Gebe einen String aus
void LCD_WriteString(char str[], uint32_t x, uint32_t y, uint16_t fcolor, uint16_t bcolor){
	
	int i = 0;
	int str_len = strlen(str);
	for(i = 0; i < str_len; i++){	
		LCD_DrawLetter( str[i], x, y, fcolor, bcolor);
		x = x + 12;
		LCD_SetCursor(x, y);								// Cursorposition fuer neuen char
		
	}
}	


// Verteilen der einzelnen Bits
void LCD_Output16BitWord(uint16_t data){

    GPIOD->ODR = ((data & 0xE000) >> 5) | ((data & 0x000C) >> 2) | ((data & 0x0003) << 14) | (GPIOD->ODR & 0x38FC);
    GPIOE->ODR = (data & 0x1FF0) << 3 | (GPIOE->ODR & 0x007F);
}

// Implementierung der Sendesequenz fuer das Display (Daten)
void LCD_WriteData(uint16_t data){
	
	GPIOE->ODR |= 1 << 3;		// DC HIGH (Data)
	GPIOD->ODR &= ~(0x00A0);

	LCD_Output16BitWord(data);

	GPIOD->ODR |= 0x00A0;		
}

// Implementierung der Sendesequenz fuer das Display (Befehle)
void LCD_WriteCommand(uint16_t cmd){
	
	GPIOE->ODR &= ~(1 << 3);	// DC HIGH (Command)		<--- !!!
	GPIOD->ODR &= ~(0x00A0);
	LCD_Output16BitWord(cmd);
	GPIOD->ODR |= 0x00A0;		
}

// Schreibe in ein Register des Displays
void LCD_WriteReg(uint16_t cmd, uint16_t data){
	
	LCD_WriteCommand(cmd);
	LCD_WriteData(data);
}

// Initialisierung des Displays

void LCD_Init(void){
		
    // GPIOs fuer das Display schalten
	GPIOD->MODER |= 0x50154545;		//bin 

  GPIOE->MODER |= 0x55554040;		//bin 0101 0101 0101 0101 0100 0000 0100 0000

	GPIOD->ODR |= 1 << 13;			// Backlight aktivieren
  GPIOD->ODR |= 1 << 4;			// Read auf HIGH (ungenutzt)

  GPIOD->ODR &= ~(1 << 3);		// Reset LOW Signal
  delay(600);
  GPIOD->ODR |= 1 << 3;
	
	LCD_WriteReg(0x0010, 0x0001); /* Enter sleep mode */
	LCD_WriteReg(0x001E, 0x00B2); /* Set initial power parameters. */
	LCD_WriteReg(0x0028, 0x0006); /* Set initial power parameters. */
	LCD_WriteReg(0x0000, 0x0001); /* Start the oscillator.*/
	LCD_WriteReg(0x0001, 0x72EF); /* Set pixel format and basic display orientation */
	LCD_WriteReg(0x0002, 0x0600);
	LCD_WriteReg(0x0010, 0x0000); /* Exit sleep mode.*/
	delay(30);					  //30ms warten 
	LCD_WriteReg(0x0011, 0x6870); /* Configure pixel color format and MCU interface parameters.*/
	LCD_WriteReg(0x0012, 0x0999); /* Set analog parameters */
	LCD_WriteReg(0x0026, 0x3800);
	LCD_WriteReg(0x0007, 0x0033); /* Enable the display */
	LCD_WriteReg(0x000C, 0x0005); /* Set VCIX2 voltage to 6.1V.*/
	LCD_WriteReg(0x000D, 0x000A); /* Configure Vlcd63 and VCOMl */
	LCD_WriteReg(0x000E, 0x2E00);
	LCD_WriteReg(0x0044, (240-1) << 8); /* Set the display size and ensure that the GRAM window
											is set to allow access to the full display buffer.*/
	LCD_WriteReg(0x0045, 0x0000);
	LCD_WriteReg(0x0046, 320-1);
	LCD_WriteReg(0x004E, 0x0000); /*Set cursor to 0,0 */
	LCD_WriteReg(0x004F, 0x0000);
	
	GPIOD->ODR |= (1 << 7);		// CS LOW (invertiert)
	GPIOD->ODR |= (1 << 5);		// WR LOW (invertiert)
	

	LCD_ClearDisplay(0xFFFF);
}
