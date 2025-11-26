// [file name]: ssd1306.c
#include "ssd1306.h"
#include "spi.h"
#include <stm32f10x.h>

#define CS_LOW  (GPIOA->ODR &= ~GPIO_ODR_ODR4)
#define CS_HIGH (GPIOA->ODR |= GPIO_ODR_ODR4)
#define DC_CMD  (GPIOA->ODR &= ~GPIO_ODR_ODR1)
#define DC_DATA (GPIOA->ODR |= GPIO_ODR_ODR1)
#define RST_LOW (GPIOA->ODR &= ~GPIO_ODR_ODR0)
#define RST_HIGH (GPIOA->ODR |= GPIO_ODR_ODR0)

void delay_small(uint32_t t)
{
    while(t--){__NOP();}
}

void SSD1306_SendCommand(uint8_t cmd)
{
    CS_LOW;
    DC_CMD;
    delay_small(50);
    SPI1_Write(cmd);
    CS_HIGH;
}

void SSD1306_SendData(uint8_t data)
{
    CS_LOW;
    DC_DATA;
    delay_small(50);
    SPI1_Write(data);
    CS_HIGH;
}

void SSD1306_Init(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    
    GPIOA->CRL &= ~(GPIO_CRL_MODE0 | GPIO_CRL_CNF0 |
                   GPIO_CRL_MODE1 | GPIO_CRL_CNF1 |
                   GPIO_CRL_MODE4 | GPIO_CRL_CNF4);
    
    GPIOA->CRL |= (GPIO_CRL_MODE0_1 | GPIO_CRL_MODE0_0);
    GPIOA->CRL |= (GPIO_CRL_MODE1_1 | GPIO_CRL_MODE1_0);
    GPIOA->CRL |= (GPIO_CRL_MODE4_1 | GPIO_CRL_MODE4_0);
    
    RST_LOW;
    delay_small(50000);
    RST_HIGH;
    
    SSD1306_SendCommand(0xAE);
    SSD1306_SendCommand(0x20);
    SSD1306_SendCommand(0x00);
    SSD1306_SendCommand(0xB0);
    SSD1306_SendCommand(0xC8);
    SSD1306_SendCommand(0x00);
    SSD1306_SendCommand(0x10);
    SSD1306_SendCommand(0x40);
    SSD1306_SendCommand(0x81);
    SSD1306_SendCommand(0x7F);
    SSD1306_SendCommand(0xA1);
    SSD1306_SendCommand(0xA6);
    SSD1306_SendCommand(0xA8);
    SSD1306_SendCommand(0x3F);
    SSD1306_SendCommand(0xA4);
    SSD1306_SendCommand(0xD3);
    SSD1306_SendCommand(0x00);
    SSD1306_SendCommand(0xD5);
    SSD1306_SendCommand(0xF0);
    SSD1306_SendCommand(0xD9);
    SSD1306_SendCommand(0x22);
    SSD1306_SendCommand(0xDA);
    SSD1306_SendCommand(0x12);
    SSD1306_SendCommand(0xDB);
    SSD1306_SendCommand(0x20);
    SSD1306_SendCommand(0x8D);
    SSD1306_SendCommand(0x14);
    SSD1306_SendCommand(0xAF);
}

void SSD1306_DrawBuffer(uint8_t *buf)
{
    for(int p=0; p<8; p++){
        SSD1306_SendCommand(0xB0+p);
        SSD1306_SendCommand(0x00);
        SSD1306_SendCommand(0x10);
        for(int i=0; i<128; i++){
            SSD1306_SendData(buf[p*128+i]);
        }
    }
}

void SSD1306_DrawChessboard(void)
{
    static uint8_t buf[1024];
    
    for(int p=0; p<8; p++){
        for(int x=0; x<128; x++){
            buf[p*128+x] = ((p+(x/16))&1)?0xFF:0x00;
        }
    }
    
    SSD1306_DrawBuffer(buf);
}