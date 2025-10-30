#include "stm32f407xx_gpio_driver.h"
#include "stm32f407xx_usart_driver.h"
#include <string.h>
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
/*Configure USART3 for comunication*/
USART_Config_t USART3_Conf;

/**
 * @brief USART3 init function
 *        This function initializes the USART3 which includes:
 *        GPIOB configuration - Configure GPIOB pin10 and pin11 to use in alternate function mode
 *        USART3 configuration 
 */
void USART3_Init(void)
{
    GPIO_PinConf_t USART_Pin;
    /*GPIO - USART pin configuration
      Configure GPIO (GPIOB pin10/Tx and pin11/Rx) pin to use in alternate function mode (USART3)*/
    USART_Pin.GPIO_PinMode   = GPIO_MODE_ALT;
    USART_Pin.GPIO_PUPD      = GPIO_NO_PUPD;
    USART_Pin.GPIO_OutType   = GPIO_OUT_PP;
    USART_Pin.GPIO_AltFunc   = GPIO_ALT_AF7;
    GPIOB_CLK_ENB();
    USART_Pin.GPIO_PinNumber = GPIO_PIN_NUM_10; /* Tx pin */
    GPIO_Init(GPIOB, USART_Pin);
    USART_Pin.GPIO_PinNumber = GPIO_PIN_NUM_11; /* Rx pin */
    GPIO_Init(GPIOB, USART_Pin);

    /*USART3 configuration*/
    USART3_Conf.Mode            = USART_MODE_TXRX;         /*Transmit and receive mode*/
    USART3_Conf.Parity          = USART_PARITY_DISABLE;        /*None parity control*/
    USART3_Conf.StopBits        = USART_STOPBITS_1;         /*1 stop bit*/
    USART3_Conf.WordLength      = USART_WORDLEN_8BITS;      /*8 bit word length*/
    USART3_Conf.Oversampling    = USART_OVERSAMPLING_16;    /*Oversampling by 16*/
    USART3_Conf.BaudRate        = USART_BAUD_9600;                     
    USART3_CLK_ENB();
    USART_Init(USART3, USART3_Conf);
}

void GPIOD_Init(void)
{
    GPIOD_PinConf.GPIO_PinNumber    = GPIO_PIN_NUM_12;
    GPIOD_PinConf.GPIO_PinMode      = GPIO_MODE_OUTPUT;
    GPIOD_PinConf.GPIO_OutType      = GPIO_OUT_PP;

    /*enable clock for GPIOD*/
    GPIOD_CLK_ENB();
    GPIO_Init(GPIOD, GPIOD_PinConf);
}

void GPIOA_Init(void)
{
    GPIOA_PinConf.GPIO_PinMode      = GPIO_MODE_INPUT;
    GPIOA_PinConf.GPIO_PUPD         = GPIO_NO_PUPD;
    GPIOA_PinConf.GPIO_EdgeTrigger  = GPIO_IT_EDGE_RT;
    GPIOA_CLK_ENB();
    GPIO_Init(GPIOA, GPIOA_PinConf);
}

int main(void)
{
    uint8_t RecievedMess[10] = {0};
    uint8_t ReceivedMessSize = 3U;
    GPIOD_Init();
    GPIOA_Init();
    /*Configure GPIOA as input interrupt*/
    GPIO_IT_Init(GPIOA, GPIOA_PinConf, 1);

    USART3_Init();

    while (1)
    {
        /*Receice message*/
        USART_Receive(USART3, RecievedMess, ReceivedMessSize);
        RecievedMess[ReceivedMessSize] = '\0';
        USART_Transmit(USART3, RecievedMess, ReceivedMessSize);
        /*Check if the received message is on*/
        if(strcmp((const char *)RecievedMess, "on_") == 0)
        {
            GPIO_PinWrite(GPIOD, GPIO_PIN_NUM_12, BIT_SET);
        }
        else if(strcmp((const char *)RecievedMess,"off") == 0)
        {
            GPIO_PinWrite(GPIOD, GPIO_PIN_NUM_12, BIT_RESET);
        }
        else{
            /*do nothing*/
        }
    }

    return 0;
}

void EXTI0_IRQHandler(void)
{
    /*Toggle LED*/
    GPIO_PinToggle(GPIOD, GPIOD_PinConf.GPIO_PinNumber);
    delayms(1);
    /*Clear pending interrupt bit in EXTI_PR*/
    EXTI->PR |= (0x01 << GPIOA_PinConf.GPIO_PinNumber);
}