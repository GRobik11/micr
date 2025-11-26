// [file name]: main.c
#include <stdint.h>
#include <stm32f10x.h>
#include "spi.h"
#include "ssd1306.h"

int main(void)
{
    SPI1_Init();
    SSD1306_Init();

    SSD1306_DrawChessboard();

    while(1){}
}