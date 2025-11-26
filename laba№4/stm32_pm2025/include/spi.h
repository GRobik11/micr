#ifndef SPI_H
#define SPI_H

#include <stdint.h>
#include "stm32f10x.h"

void SPI_Initialize(void);
void SPI_Transmit(uint8_t tx_data);
uint8_t SPI_Receive(void);

#endif