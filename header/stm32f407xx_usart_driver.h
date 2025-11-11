/*Define USART configuration struct*/
/*Define value for the items in the struct*/
/*Define function prototypes*/

#ifndef STM32F407XX_USART_DRIVER_H
#define STM32F407XX_USART_DRIVER_H
#include "stm32f407xx.h"

/*USART configuration struct*/
typedef struct 
{
    uint8_t Mode;           /*Specifies whether the Receive or Transmit mode is enabled or disabled.
                            This parameter can be a value of @ref USART_Mode*/

    uint32_t BaudRate;      /*This member configures the USART/UART communication baud rate.
                            This parameter can be a value of @ref USART_Common_Baudrate*/

    uint8_t WordLength;     /*Specifies the number of data bits transmitted or received in a frame.
                            This parameter can be a value of @ref USART_Word_Length*/

    uint8_t StopBits;       /*Specifies the number of stop bits transmitted.
                            This parameter can be a value of @ref USART_Stop_Bits*/

    uint8_t Parity;         /*Specifies the parity mode.
                            This parameter can be a value of @ref USART_Parity
                            @note When parity is enabled, the computed parity is inserted
                            at the MSB position of the transmitted data (9th bit when
                            the word length is set to 9 data bits; 8th bit when the
                            word length is set to 8 data bits).*/

    uint8_t OverSampling;  /*Specifies whether the Oversampling 8 is enabled or disabled, to achieve higher speed (up to fPCLK/8).
                            This parameter can be a value of @ref USART_Over_Sampling*/
}USART_Conf_t;

/*USART_Mode*/
#define USART_MODE_RX            1U      /*Only CR1_RE bit is enabled, receive mode only*/
#define USART_MODE_TX            2U      /*Only CR1_TE bit is enabled, transmit mode only*/
#define USART_MODE_TX_RX         3U      /*CR1_RE and CR1_TE bit are enabled, both receive and transmit mode*/

/*USART_Common_Baudrate*/
#define USART_BAUDRATE_115200   115200U  /*115200 bit per second*/
#define USART_BAUDRATE_57600    57600U   /*57600 bit per second*/
#define USART_BAUDRATE_38400    38400U   /*38400 bit per second*/
#define USART_BAUDRATE_19200    19200U   /*19200 bit per second*/
#define USART_BAUDRATE_9600     9600U    /*9600 bit per second*/
#define USART_BAUDRATE_4800     4800U    /*4800 bit per second*/

/*USART_Word_Length*/
#define USART_WORDLENGTH_8B      0U      /*8 bit word length; 1 Start bit, 8 Data bits, n Stop bit*/
#define USART_WORDLENGTH_9B      1U      /*9 bit word length; 1 Start bit, 9 Data bits, n Stop bit*/

/*USART_Stop_Bits*/
#define USART_STOPBITS_1         0U      /*1 stop bit*/
#define USART_STOPBITS_0_5       1U      /*0.5 stop bit*/
#define USART_STOPBITS_2         2U      /*2 stop bits*/
#define USART_STOPBITS_1_5       3U      /*1.5 stop bits*/

/*USART_Parity*/
#define USART_PARITY_NONE        0U      /*Parity control is disabled*/
#define USART_PARITY_EVEN        2U      /*Parity control is enabled; Even parity is selected*/
#define USART_PARITY_ODD         3U      /*Parity control is enabled; Odd parity is selected*/


/*USART_Over_Sampling*/
#define USART_OVERSAMPLING_16    0U      /*Oversampling by 16*/
#define USART_OVERSAMPLING_8     1U      /*Oversampling by 8*/

/* USART register bits ---------------------------------------------------------------*/
/* USART_CR1 */
#define USART_CR1_RE            2U      /* RE bit */
#define USART_CR1_TE            3U      /* TE bit */
#define USART_CR1_RXNEIE        5U      /* RXNEIE bit */
#define USART_CR1_PS            9U      /* PS bit */
#define USART_CR1_PCE           10U     /* PCE bit*/
#define USART_CR1_M             12U     /* M bit */
#define USART_CR1_UE            13U     /* UE bit */
#define USART_CR1_OVER8         15U     /* OVER8 bit */

/* USART_CR2*/
#define USART_CR2_STOP          12U     /* STOP bit */

/* USART_BRR */
#define USART_DIV_MANTISSA      4U      /* Div_Mantissa */
#define USART_DIV_FRACTION      0U      /* Div_Fraction */

/* USART SR */
#define USART_SR_TXE            7U      /* TXE bit */
#define USART_SR_RXNE           5U      /* RXNE bit: Read data register not empty */

/*Interrupt configuration*/
#define USART3_RXNEIE_ENB()     (USART3->CR1 |= (1U << USART_CR1_RXNEIE))      /*Enable receive not empty interrupt*/
#define USART3_RXNEIE_DIS()     (USART3->CR1 &= ~(1U << USART_CR1_RXNEIE))     /*Disable receive not empty interrupt*/


/* Function ptorotypes */
void USART_Init(USART_RegDef_t * USARTx, USART_Conf_t USART_Conf);
void USART_Transmit(USART_RegDef_t * USARTx, uint8_t * Mess, uint8_t MessSize);
void USART_Receive(USART_RegDef_t * USARTx, uint8_t * Mess, uint8_t MessSize);

#endif