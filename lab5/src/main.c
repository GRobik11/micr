#include <stdint.h>
#include "stm32f10x.h"

#define USER_LED_PIN    13
#define USER_LED_PORT   GPIOC
#define USER_BTN_PIN    0
#define USER_BTN_PORT   GPIOB

#define PRESCALER_INIT  40000
#define AUTORELOAD_INIT 500
#define DEBOUNCE_LIMIT  3000

void Timer2_InterruptHandler(void) {
    if (TIM2->SR & TIM_SR_UIF) {
        TIM2->SR &= ~TIM_SR_UIF;
        USER_LED_PORT->ODR ^= (1U << USER_LED_PIN);
    }
}

void InitializeLED(void) {
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
    USER_LED_PORT->CRH &= ~(0xF << (4 * (USER_LED_PIN - 8)));
    USER_LED_PORT->CRH |=  (0x1 << (4 * (USER_LED_PIN - 8)));
}

void InitializeButton(void) {
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
    USER_BTN_PORT->CRL &= ~(0xF << (4 * USER_BTN_PIN));
    USER_BTN_PORT->CRL |=  (0x2 << (4 * USER_BTN_PIN + 2));
    USER_BTN_PORT->ODR |= (1U << USER_BTN_PIN);
}

void ConfigureTimer(void) {
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    RCC->APB1RSTR |= RCC_APB1RSTR_TIM2RST;
    RCC->APB1RSTR &= ~RCC_APB1RSTR_TIM2RST;

    TIM2->PSC = PRESCALER_INIT - 1;
    TIM2->ARR = AUTORELOAD_INIT - 1;

    TIM2->DIER |= TIM_DIER_UIE;
    NVIC_EnableIRQ(TIM2_IRQn);
    TIM2->CR1 |= TIM_CR1_CEN;
}

int main(void) {
    uint8_t previousButtonState = 1;
    uint32_t buttonCounter = 0;
    
    InitializeLED();
    InitializeButton();
    ConfigureTimer();

    while (1) {
        uint8_t buttonCurrent = (USER_BTN_PORT->IDR & (1U << USER_BTN_PIN)) ? 0 : 1;
        
        if (buttonCurrent != previousButtonState) {
            buttonCounter = 0;
        } else {
            if (buttonCounter++ > DEBOUNCE_LIMIT) {
                if (buttonCurrent == 1) {
                    TIM2->PSC <<= 1;
                    if (TIM2->PSC > 500000) {
                        TIM2->PSC = 1000;
                    }
                }
            }
        }
        
        previousButtonState = buttonCurrent;
    }
}