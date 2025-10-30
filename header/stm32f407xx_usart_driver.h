/*Define USART configuration struct*/
/*Define value for the items in the struct*/
/*Define function prototypes*/
#ifndef STM32F407XX_USART_DRIVER_H
#define STM32F407XX_USART_DRIVER_H
#include "stm32f407xx.h"
#include <stdint.h>

typedef struct
{
    uint32_t BaudRate;          
    uint8_t  WordLength;        /* USART_WORDLEN_x */
    uint8_t  StopBits;          /* USART_STOPBITS_x */
    uint8_t  Parity;            /* USART_PARITY_x */
    uint8_t  Mode;              /* USART_MODE_x */
    uint8_t  HwFlowCrtl;         /* USART_HW_FLOW_CTRL_x */
    uint8_t  Oversampling;      /* USART_OVERSAMPLING_x */
} USART_Config_t;


/* USART operating modes */
#define USART_MODE_RX                   0x01U /*RE*/
#define USART_MODE_TX                   0x02U /*TE*/
#define USART_MODE_TXRX                 0x03U /*Enable all tx-rx*/

/* Word length (CR1.M) */
#define USART_WORDLEN_8BITS             0x00U
#define USART_WORDLEN_9BITS             0x01U

/* Parity (CR1.PCE/PS) */
#define USART_PARITY_DISABLE            0U
#define USART_PARITY_EVEN               2U
#define USART_PARITY_ODD                3U

/* Stop bits (CR2.STOP[13:12]) */
#define USART_STOPBITS_1                0x00U  
#define USART_STOPBITS_0_5              0x01U  
#define USART_STOPBITS_2                0x02U  
#define USART_STOPBITS_1_5              0x03U

/* Hardware Flow Control (CR3.RTSE/CTSE) */
#define USART_HW_FLOW_CTRL_NONE         0x00U
#define USART_HW_FLOW_CTRL_RTS          0x01U
#define USART_HW_FLOW_CTRL_CTS          0x02U
#define USART_HW_FLOW_CTRL_RTS_CTS      0x03U

/* Oversampling (CR1.OVER8) */
#define USART_OVERSAMPLING_16           0
#define USART_OVERSAMPLING_8            1

/* Baud presets */
#define USART_BAUD_9600                 9600U
#define USART_BAUD_19200                19200U
#define USART_BAUD_57600                57600U
#define USART_BAUD_115200               115200U
#define USART_BAUD_230400               230400U
#define USART_BAUD_460800               460800U
#define USART_BAUD_921600               921600U

/* USART register bits ---------------------------------------------------------------*/
/*Register CR1*/
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
//Dùng để kiểm tra tình trạng truyền/nhận dữ liệu của USART
//TXE = 1 Nghĩa là TDR đã trống-> có thể ghi byte tiếp theo vào DR để gửi


void USART_PeriphClockCtrl(USART_RegDef_t *USARTx, uint8_t EnOrDi);
/* Function ptorotypes */
void USART_Init(USART_RegDef_t * USARTx, USART_Config_t USART_Conf);
void USART_Transmit(USART_RegDef_t * USARTx, uint8_t * Mess, uint8_t MessSize);//truyen 
void USART_Receive(USART_RegDef_t * USARTx, uint8_t * Mess, uint8_t MessSize);//nhan 



#endif