#ifndef SSD1306_H
#define SSD1306_H

#include <stdint.h>

void OLED_Initialize(void);
void OLED_WriteCommand(uint8_t command);
void OLED_WriteData(uint8_t data);
void OLED_UpdateDisplay(uint8_t *buffer);
void OLED_DrawChessboard(void);

#endif