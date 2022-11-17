#include <inttypes.h>

#define C_BLACK 0x0000
#define C_WHITE 0xFFFF
#define C_RED 0xF800
#define C_GREEN 0x07E0
#define C_YELLOW 0xFFE0
#define C_BLUE 0x001F
#define C_HOTPINK 0xF819



void LCD_Output16BitWord(uint16_t data);
void LCD_WriteData(uint16_t data);
void LCD_WriteCommand(uint16_t data);
void LCD_WriteReg(uint16_t cmd, uint16_t data);
void LCD_Init(void);
void LCD_SetCursor(uint32_t x, uint32_t y);
void LCD_DrawPixel(uint16_t color);
void LCD_ClearDisplay(uint16_t color);
void LCD_DrawLetter(char c, uint32_t x, uint32_t y, uint16_t fcolor, uint16_t bcolor);
void LCD_WriteString(char str[], uint32_t x, uint32_t y, uint16_t fcolor, uint16_t bcolor);
void delay(int ms);
void DisplayMessung(void);
void LCD_drawPic(uint8_t select);
void LCD_drawGIF(void);
