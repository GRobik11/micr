// [file name]: spi.c
#include "spi.h"

void SPI1_Init(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
    
    GPIOA->CRL &= ~(GPIO_CRL_MODE5 | GPIO_CRL_CNF5);
    GPIOA->CRL |= (GPIO_CRL_MODE5_1 | GPIO_CRL_MODE5_0);
    GPIOA->CRL |= GPIO_CRL_CNF5_1;
    
    GPIOA->CRL &= ~(GPIO_CRL_MODE7 | GPIO_CRL_CNF7);
    GPIOA->CRL |= (GPIO_CRL_MODE7_1 | GPIO_CRL_MODE7_0);
    GPIOA->CRL |= GPIO_CRL_CNF7_1;
    
    SPI1->CR1 = 0;
    SPI1->CR1 |= SPI_CR1_MSTR;
    SPI1->CR1 |= SPI_CR1_BR_2 | SPI_CR1_BR_1 | SPI_CR1_BR_0;
    SPI1->CR1 |= SPI_CR1_CPOL;
    SPI1->CR1 |= SPI_CR1_CPHA;
    SPI1->CR1 |= SPI_CR1_SSM | SPI_CR1_SSI;
    
    SPI1->CR1 |= SPI_CR1_SPE;
}

void SPI1_Write(uint8_t data)
{
    while(!(SPI1->SR & SPI_SR_TXE)){}
    SPI1->DR = data;
    while(SPI1->SR & SPI_SR_BSY){}
}

uint8_t SPI1_Read(void)
{
    SPI1->DR = 0x00;
    while(!(SPI1->SR & SPI_SR_RXNE)){}
    return SPI1->DR;
}