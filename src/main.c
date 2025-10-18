#include "stm32f407xx_gpio_driver.h"

int main(void)
{
    /*Configure GPIOD pin number 12 as an output pin*/
    GPIO_PinConf_t GPIOD_PinConf;

    GPIOD_PinConf.GPIO_PinNumber    = GPIO_PIN_NUM_12;
    GPIOD_PinConf.GPIO_Pinmode      = GPIO_MODE_OUTPUT;
    GPIOD_PinConf.GPIO_OutType      = GPIO_OUT_PP;

    /*enable clock for GPIOD*/
    /*TODO*/
    GPIOD_CLK_ENB();
    
    GPIO_Init(GPIOD, GPIOD_PinConf);
    GPIO_PinWrite(GPIOD, GPIO_PIN_NUM_12, BIT_SET);

    while (1)
    {

    }

    return 0;
}