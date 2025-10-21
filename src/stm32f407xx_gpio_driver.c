#include "stm32f407xx_gpio_driver.h"
void GPIO_Init(GPIO_RegDef_t * GPIOx, GPIO_PinConf_t PinConf){
    /*initilaize pin mode*/
    GPIOx->MODER &= ~(0x3U << (PinConf.GPIO_PinNumber * 2));
    GPIOx->MODER |=  (PinConf.GPIO_Pinmode << (PinConf.GPIO_PinNumber * 2));
    /*init output mode*/
    if (PinConf.GPIO_Pinmode == GPIO_MODE_OUTPUT)
    {
        GPIOx->OTYPER |= (PinConf.GPIO_OutType << PinConf.GPIO_PinNumber);
    }
    GPIOx->PUPDR &= ~(0x3U << (PinConf.GPIO_PinNumber * 2));  
    GPIOx->PUPDR |=  (PinConf.GPIO_PUPD << (PinConf.GPIO_PinNumber * 2));  

}
void GPIO_PinWrite(GPIO_RegDef_t * GPIOx, uint8_t PinNumber, uint8_t Value)
{
    if(Value == BIT_SET)
    {
        GPIOx->ODR |= (0x01 << PinNumber);
    }
    else
    {
        GPIOx->ODR &= ~(0x01 << PinNumber);
    }
}
void GPIO_PinToggle(GPIO_RegDef_t * GPIOx, uint8_t PinNumber)
    {
        GPIOx->ODR ^= (0x01 << PinNumber);
    }

uint8_t GPIO_PinRead(GPIO_RegDef_t * GPIOx, uint8_t PinNumber){
    uint8_t ret;
    ret = (GPIOx->IDR >> PinNumber) & 0x01;//dich 1 bit ra ngoai thanh ghi, lay 1 bit nen moi AND 0x01
    return ret;
    
}

