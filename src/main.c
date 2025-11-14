#include "stm32f407xx_gpio_driver.h"
#include "stm32f407xx_usart_driver.h"
#include "stm32f407xx_timer_driver.h"
#include <string.h>
#include <stdlib.h>

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
USART_Conf_t USART3_Conf;
/*Configure TIM6 for time base*/
TIM_Base_Conf_t TIM6_Conf;
/*Configure TIM4 for time base*/
TIM_Base_Conf_t TIM4_Conf;
TIM_OC_Conf_t TIM4_OC_Conf;

#define RX_BUFFER_SIZE  8U
#define TX_BUFFER_SIZE  8U
#define BUTTON_DEBOUNCE_TIME    100U
volatile uint8_t ReceivedMess[RX_BUFFER_SIZE];
volatile uint8_t TransmitMess[TX_BUFFER_SIZE]   = "J\n";
volatile uint8_t TransmitMessSize               = 2U;
volatile uint8_t RxData                         = 0U;
volatile uint8_t RxIndex                        = 0U;
volatile uint8_t IsRxAvailable                  = FALSE;
volatile uint16_t Timer6DelayCounter = 0U;


/**
 * @brief   This function initializes timer 6 as specific configuration
 *          The timer 6 is used to generate delay in this project
 *          The timer period is 1ms
 * 
 */
void TIM6_Init(void)
{
    TIM6_Conf.AutoReloadPreload = ENABLE;   
    TIM6_Conf.Period = 999;                 /*1ms period*/
    TIM6_Conf.Prescaler = 15;               /*Counter clock is 1Mhz (with 16 Mhz timer clock)*/
    TIM6_CLK_ENB();
    TIM_Base_Init(TIM6, TIM6_Conf);
    TIM_Base_ForceUpdate(TIM6);

}

/**
 * @brief   This function is used to start timer 6
 * 
 */
void TIM6_Start(void)
{
    TIM_Base_Start(TIM6);
}

/**
 * @brief   This function is used to stop timer 6
 * 
 */
void TIM6_Stop(void)
{
    TIM_Base_Stop(TIM6);
}

/**
 * @brief   This function is used to initialize timer 6 interrupt
 *          Priority is set to 1
 * 
 */
void TIM6_IT_Init(void)
{
    uint8_t Priority = 1U;
    TIM_Base_IT_Init(TIM6, Priority);
}

/**
 * @brief   This function initializes timer 4 channel 4 to used in output compare mode
 *          This configuration is applied for peripheral clock at 16Mhz 
 * 
 */
void TIM4_OC_Init(void)
{
    GPIO_PinConf_t OC_Pin;

    /*GPIO - TIM4 output compare pin configuration
      Configure GPIO (GPIOD pin15/channel 4) pin to use in alternate function mode (TIM4 - OC Channel 4)*/
    OC_Pin.GPIO_PinMode   = GPIO_MODE_ALT;
    OC_Pin.GPIO_PUPD      = GPIO_NO_PUPD;
    OC_Pin.GPIO_OutType   = GPIO_OUT_PP;
    OC_Pin.GPIO_PinNumber = GPIO_PIN_NUM_15;
    OC_Pin.GPIO_AltFunc   = GPIO_ALT_AF2;
    GPIOD_CLK_ENB();
    GPIO_Init(GPIOD, OC_Pin);

    /*Timer 4 configuration*/
    /*Timer base init*/
    TIM4_Conf.AutoReloadPreload = ENABLE;
    TIM4_Conf.Period            = 999;      /*1ms period*/
    TIM4_Conf.Prescaler         = 15;       /*Counter clock is 1Mhz (with 16 Mhz timer clock)*/
    TIM4_Conf.CounterMode       = TIM_UPCOUNTING;
    TIM4_CLK_ENB();
    TIM_Base_Init(TIM4, TIM4_Conf);

    /*Output compare init*/
    TIM4_OC_Conf.OCMode         = TIM_OCMODE_PWM1;
    TIM4_OC_Conf.OCPolarity     = TIM_OCPOLARITY_HIGH;
    TIM4_OC_Conf.Pulse          = 0;
    TIM_OC_Init(TIM4, TIM4_OC_Conf, TIM_OC_CHANNEL_4);
}

/**
 * @brief This function is used to start timer 4
 * 
 */
void TIM4_Start(void)
{
    TIM_Base_Start(TIM4);
}


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
    USART3_Conf.Mode            = USART_MODE_TX_RX;         /*Transmit and receive mode*/
    USART3_Conf.Parity          = USART_PARITY_NONE;        /*None parity control*/
    USART3_Conf.StopBits        = USART_STOPBITS_1;         /*1 stop bit*/
    USART3_Conf.WordLength      = USART_WORDLENGTH_8B;      /*8 bit word length*/
    USART3_Conf.OverSampling    = USART_OVERSAMPLING_16;    /*Oversampling by 16*/
    USART3_Conf.BaudRate        = USART_BAUDRATE_9600;                     
    USART3_CLK_ENB();
    /*TODO-----------------------------------------------------*/
    USART3_RXNEIE_ENB();                                    /*Enable receive not empty interrupt*/
    NVIC_SetPriority(IRQ_NO_USART3, 0U);                    /*Set USART3 interrupt priority*/
    NVIC_EnableIRQ(IRQ_NO_USART3);                          /*Enable USART3 IRQ*/
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
    uint8_t DutyCycle = 0;

    GPIOD_Init();
    GPIOA_Init();
    /*Configure GPIOA as input interrupt*/
    GPIO_IT_Init(GPIOA, GPIOA_PinConf, 1);

    USART3_Init();

    /*Init timer 6*/
    TIM6_Init();
    TIM6_IT_Init();
    /*Start timer 6*/
    // TIM6_Start();
    TIM4_OC_Init();
    TIM4_Start();
    // uint16_t Timer6DelayCounter = 0U;
    while (1)
    {
        /*TODO-------------------------------------------------*/
        /*Compare buffer*/
        /*Control LED*/
        /*Is new data available?*/
        if (IsRxAvailable == TRUE)
        {
            USART3_RXNEIE_DIS();
            /*Reset the Rx data available flag to FALSE*/
            IsRxAvailable = FALSE;
            
            /*Null-terminate the string/message*/
            ReceivedMess[RxIndex - 1] = '\0';
            // /*Check if the received message is "ON"*/
            // if (strcmp((const char *)ReceivedMess, "ON") == 0)
            // {
            //     /*Turn blue LED ON*/
            //     GPIO_PinWrite(GPIOD, GPIOD_PinConf.GPIO_PinNumber, BIT_SET);
            // }
            // /*Check if the received message is "OFF"*/
            // if (strcmp((const char *)ReceivedMess, "OFF") == 0)
            // {
            //     /*Turn blue LED OFF*/
            //     GPIO_PinWrite(GPIOD, GPIOD_PinConf.GPIO_PinNumber, BIT_RESET);
            // }

            DutyCycle = atoi((const char *) ReceivedMess);
            /*Set duty cycle for timer 4 pwm channel 4*/
            TIM4_OC_PWM_SET_DUTY(TIM_OC_CHANNEL_4, (TIM4_Conf.Period * DutyCycle) / 100);
            // USART_Transmit(USART3,(uint8_t *)&ReceivedMess, RxIndex);
            /*Reset the index*/
            RxIndex = 0U;
            USART3_RXNEIE_ENB();
        }

                
        // /*Check if update event generated*/
        // if (TIM6_UEV_STS() == BIT_SET)
        // {
        //     /*Clear the update event status*/
        //     TIM6_UEV_STS_CLR();
        //     /*Increase the timer delay counter by 1*/
        //     Timer6DelayCounter++;
        //     /*Check if 1 second has elapsed*/
        //     if (Timer6DelayCounter == 1000)
        //     {
        //         /*Toggle the Blue LED*/
        //         GPIO_PinToggle(GPIOD, GPIOD_PinConf.GPIO_PinNumber);
        //         /*Reset timer 6 delay counter*/
        //         Timer6DelayCounter = 0U; 
        //     }
        // }
        
    }

    return 0;
}

void EXTI0_IRQHandler(void)
{

    
    /*TODO----------------------------------------------------------*/
    /*Thay the delayms bang timer interrupt de chong doi nut nhan*/
    // delayms(1);
    /*Clear pending interrupt bit in EXTI_PR*/
    // EXTI->PR |= (0x01 << GPIOA_PinConf.GPIO_PinNumber);
        /*Start timer 6*/
    TIM6_Start();
    /*Is the corresponding bit in the EXTI_PR register set?*/
    if((EXTI->PR >> GPIOA_PinConf.GPIO_PinNumber) & 0x01U)
    {
        /*Clear the pending bit by writing 1*/
        EXTI->PR |= (0x01U << GPIOA_PinConf.GPIO_PinNumber);
    }
}

/**
 * @brief This is interrupt service routine for USART3
 * 
 */
void USART3_IRQHandler(void)
{
    /*Check if the receive register is not empty*/
    if ((USART3->SR >> USART_SR_RXNE) & 0x01U)
    {
        /*TODO-------------------------------------------------*/
        /*USART_Reseive()*/
        /*Push character to buffer*/

        /*Read received data*/
        RxData = USART3->DR;
        /*Check overflow status and store data*/
        if (RxIndex < RX_BUFFER_SIZE)
        {
            /*Store new data to the received message*/
            ReceivedMess[RxIndex] = RxData;
            /*Increase the index*/
            RxIndex++;
        }
        else
        {
            RxIndex = 0U; /*Overflow recovery*/
        }

        if (RxData == '\n')
        {
            /*Set the Rx data available flag to TRUE*/
            IsRxAvailable = TRUE;
        }
    }
}

/*TODO-----------------------------------------------------------*/
/*Viet timer base interrupt*/
/**
 * @brief This is interrupt service routine for Timer 6 and DAC
 * 
 */
void TIM6_DAC_IRQHandler(void)
{
    /*Check if update event generated*/
    if (TIM6_UEV_STS() == BIT_SET)
    {
        /*Clear the update event status*/
        TIM6_UEV_STS_CLR();
        /*Increase the timer delay counter by 1*/
        Timer6DelayCounter++;
        /*Check if BUTTON_DEBOUNCE_TIME ms has elapsed*/
        if (Timer6DelayCounter == BUTTON_DEBOUNCE_TIME)
        {
            /*Transmit data*/
            USART_Transmit(USART3,(uint8_t *)TransmitMess, TransmitMessSize);
            /*Reset timer 6 delay counter*/
            Timer6DelayCounter = 0U;
            /*Stop timer 6*/
            TIM6_Stop(); 
        }
    }
}