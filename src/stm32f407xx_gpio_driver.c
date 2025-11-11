#include "stm32f407xx_gpio_driver.h"

/**
 * @brief This function initilize GPIOx peripheral
 * 
 * @param GPIOx GPIO peripheral to be initialized
 * @param PinConf Pin configuration struct
 */
void GPIO_Init(GPIO_RegDef_t * GPIOx, GPIO_PinConf_t PinConf)
{
    /*Initialize pin mode*/
    GPIOx->MODER &= ~(0x03 << PinConf.GPIO_PinNumber * 2);
    GPIOx->MODER |= (PinConf.GPIO_PinMode << PinConf.GPIO_PinNumber * 2);

    /*Initialize output type*/
    if (PinConf.GPIO_PinMode == GPIO_MODE_OUTPUT)
    {
        GPIOx->OTYPER |= (PinConf.GPIO_OutType << PinConf.GPIO_PinNumber);
    }

    if (PinConf.GPIO_PinMode == GPIO_MODE_ALT)
    {
        if (PinConf.GPIO_PinNumber < 8)
        {
            /*Configure the alternate function low register*/
            GPIOx->AFRL &= ~(0x0F << PinConf.GPIO_PinNumber * 4);
            GPIOx->AFRL |= (PinConf.GPIO_AltFunc << PinConf.GPIO_PinNumber * 4);
        }
        else
        {
            /*Configure the alternate function high register*/
            GPIOx->AFRH &= ~(0x0F << (PinConf.GPIO_PinNumber - 8) * 4);
            GPIOx->AFRH |= (PinConf.GPIO_AltFunc << (PinConf.GPIO_PinNumber - 8) * 4);
        }
    }
}

void GPIO_PinWrite(GPIO_RegDef_t * GPIOx, uint8_t PinNumber, uint8_t Value)
{
    if (Value == BIT_SET)
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

uint8_t GPIO_PinRead(GPIO_RegDef_t * GPIOx, uint8_t PinNumber)
{
    uint8_t ret;

    ret = (GPIOx->IDR >> PinNumber) & 0x01;

    return ret;
}

void GPIO_IT_Init(GPIO_RegDef_t * GPIOx, GPIO_PinConf_t GPIOPinConf, uint8_t Priority)
{
    uint8_t index, bitpos, portcode;
    /*1. Configure the SYSCFG*/
        /*1.1 Enable the clock for SYSCFG*/
    SYSCFG_CLK_ENB();
        /*1.2 Select the source (GPIO pin) for the respective EXTI line*/
            /*1.2.1 Specify SYSCFG_EXTICR register index*/
    index = GPIOPinConf.GPIO_PinNumber / 4;
            /*1.2.2 Specify SYSCFG_EXTICR bit position*/
    bitpos = (GPIOPinConf.GPIO_PinNumber % 4) * 4;
    /*Ex. PA15 to be input interrput pin
    bit field index = Pin number % 4 = 15 % 4 = 3.
    bit pos = bit field index * 4 = 3 * 4 = 12
    */
            /*1.2.3 Specify GPIO port to be mapped in SYSCFG_EXTICR*/
    portcode = SYSCFG_EXTICR_PORTCODE(GPIOx);
    SYSCFG->EXTICR[index] &= ~(0x0FU << bitpos);
    SYSCFG->EXTICR[index] |= (portcode << bitpos);

    /*2. Configure the EXTI*/
    /*2.1 Select the edge trigger for the interrupt*/
    switch (GPIOPinConf.GPIO_EdgeTrigger)
    {
        case GPIO_IT_EDGE_FT:
        {
            /*Disbale the raising edge trigger selection*/
            EXTI->RTSR &= ~(0x01U << GPIOPinConf.GPIO_PinNumber);
            /*Enable the falling edge trigger selection*/
            EXTI->FTSR |= (0x01U << GPIOPinConf.GPIO_PinNumber);
            break;
        }
        case GPIO_IT_EDGE_RT:
        {
            /*Disbale the falling edge trigger selection*/
            EXTI->FTSR &= ~(0x01U << GPIOPinConf.GPIO_PinNumber);
            /*Enable the raising edge trigger selection*/
            EXTI->RTSR |= (0x01U << GPIOPinConf.GPIO_PinNumber);
            break;
        }
        default:
        {
            /*Enable the falling edge trigger selection*/
            EXTI->FTSR |= (0x01U << GPIOPinConf.GPIO_PinNumber);
            /*Enable the raising edge trigger selection*/
            EXTI->RTSR |= (0x01U << GPIOPinConf.GPIO_PinNumber);
            break;
        }
    }
    /*2.2 Enable the interrupt mask for the respective EXTI line*/
    EXTI->IMR |= (0x01U << GPIOPinConf.GPIO_PinNumber);

    /*3. Configure the NVIC*/
    /*3.1 Set the interrupt priority*/
    NVIC_SetPriority(GPIO_PIN_TO_IRQ(GPIOPinConf.GPIO_PinNumber), Priority);
    /*3.2 Enable interrupt request*/
    NVIC_EnableIRQ(GPIO_PIN_TO_IRQ(GPIOPinConf.GPIO_PinNumber));
}