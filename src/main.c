#include "stm32f407xx_gpio_driver.h"

void delayms(uint32_t miliseconds)
{
    for(uint32_t i=0; i<miliseconds*100000; i++)
    {
        /*do nothing*/
    }
}
int main(void)
{
    /*Configure GPIOD pin number 15 as an output pin*/
    GPIO_PinConf_t GPIOD_PinConf;

    GPIOD_PinConf.GPIO_PinNumber    = GPIO_PIN_NUM_15;
    GPIOD_PinConf.GPIO_Pinmode      = GPIO_MODE_OUTPUT;
    GPIOD_PinConf.GPIO_OutType      = GPIO_OUT_PP;
    GPIOD_PinConf.GPIO_PUPD         = GPIO_NO_PUPD;

    /*enable clock for GPIOD*/
    GPIOD_CLK_ENB();
    
    GPIO_Init(GPIOD, GPIOD_PinConf);
    GPIO_PinWrite(GPIOD, GPIO_PIN_NUM_15, BIT_SET);
    /*todo2*/
    /*Tạo GPIOx_PinConf GPIOx(tìm nút nhấn kết nối với chân nào,port nào)*/
    /*Cai dat giá trị cho GPIOx để đọc nút nhất */
    
    /*Configure nút nhấn PA0 as an input with PD*/
    GPIO_PinConf_t GPIOA_PinConf;
    GPIOA_PinConf.GPIO_PinNumber     = GPIO_PIN_NUM_0;
    GPIOA_PinConf.GPIO_Pinmode       = GPIO_MODE_INPUT;
    GPIOA_PinConf.GPIO_OutType       = GPIO_OUT_PP;
    GPIOA_PinConf.GPIO_PUPD          = GPIO_PD;

    GPIOA_CLK_ENB();
    GPIO_Init(GPIOA, GPIOA_PinConf);






    while (1)
    {
        //GPIO_PinWrite(GPIOD, GPIO_PIN_NUM_15, BIT_SET);
        delayms(1);
       // GPIO_PinWrite(GPIOD, GPIO_PIN_NUM_15, BIT_RESET);
       // delayms(1);
       uint8_t btn = GPIO_PinRead(GPIOA, GPIO_PIN_NUM_0);
       GPIO_PinWrite(GPIOD, GPIO_PIN_NUM_15, btn ? BIT_SET : BIT_RESET);
       /*đọc nút nhất và điều khiển LED(on/off)*/
       

    }

    return 0;
}