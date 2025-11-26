#include <stdint.h>
#include "stm32f10x.h"

#define WAIT_CYCLES 1000000

void pause(uint32_t count) {
    for (volatile uint32_t i = 0; i < count; i++) {
        __asm__("nop");
    }
}

int main(void) {
    // Turn on AFIO clock
    RCC->APB2ENR |= (1U << 0);
    // Activate GPIOC clock
    RCC->APB2ENR |= (1U << 4);
    
    // Configure PC13 as output
    GPIOC->CRH &= ~(0xFU << 20);
    GPIOC->CRH |= (1U << 20);
    
    for (;;) {
        // Set pin high
        GPIOC->BSRR = (1U << 13);
        pause(WAIT_CYCLES);
        
        // Set pin low
        GPIOC->BRR = (1U << 13);
        pause(WAIT_CYCLES);
    }
}