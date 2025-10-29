#include "stm32f407xx_gpio_driver.h"
/**
 * @brief 
 * 
 * @param miliseconds 
 */

void delayms(uint32_t miliseconds)
{
    uint32_t i;

    for (i = 0; i < miliseconds*100000; i++)
    {
        /*Do nothing*/
    }
}
    /*Configure GPIOD pin number 12 as an output pin*/
    GPIO_PinConf_t GPIOD_PinConf;
    /*Configure GPIOA pin number 0 as an output pin*/
    GPIO_PinConf_t GPIOA_PinConf;
int main(void)
{

    GPIOD_PinConf.GPIO_PinNumber    = GPIO_PIN_NUM_12;
    GPIOD_PinConf.GPIO_PinMode      = GPIO_MODE_OUTPUT;
    GPIOD_PinConf.GPIO_OutType      = GPIO_OUT_PP;

    /*enable clock for GPIOD*/
    GPIOD_CLK_ENB();
    GPIO_Init(GPIOD, GPIOD_PinConf);
    
    GPIOA_PinConf.GPIO_PinMode      = GPIO_MODE_INPUT;
    GPIOA_PinConf.GPIO_PUPD         = GPIO_NO_PUPD;
    GPIOA_CLK_ENB();
    GPIO_Init(GPIOA, GPIOA_PinConf);

    /*TODO*/
    /*Configure GPIOA as input interrupt*/
    GPIOA_PinConf.GPIO_PinNumber   = GPIO_PIN_NUM_0;     
    GPIOA_PinConf.GPIO_PUPD        = GPIO_PU;            
    GPIOA_PinConf.GPIO_EdgeTrigger = GPIO_IT_EDGE_FT; 
    GPIO_Init(GPIOA, GPIOA_PinConf);
    GPIO_IT_Init(GPIOA, GPIOA_PinConf, 5);   

    while (1)
    {
        // if (BIT_SET == GPIO_PinRead(GPIOA, GPIO_PIN_NUM_0))
        // {
            //     delayms(2);

        //     if (BIT_SET == GPIO_PinRead(GPIOA, GPIO_PIN_NUM_0))
        //     {
        //         GPIO_PinToggle(GPIOD, GPIO_PIN_NUM_12);
        //     }
        // }
    }   

    return 0;
}


void EXTI0_IRQHandler(void)
{
    GPIO_PinToggle(GPIOD, GPIOD_PinConf.GPIO_PinNumber);
    delayms(1);
    /*if (EXTI->PR & (1U << 0))              
    {
        EXTI->PR = (1U << 0);                                  
    }
        */
       EXTI->PR |= (1U << 0);
}
