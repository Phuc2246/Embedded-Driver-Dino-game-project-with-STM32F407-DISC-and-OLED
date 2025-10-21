#ifndef STM32F407XX_GPIO_DRIVER
#define STM32F407XX_GPIO_DRIVER
#include "stm32f407xx.h"
typedef struct 
{
    uint8_t GPIO_PinNumber; /*GPIO pin numbers*/
    uint8_t GPIO_Pinmode;   /*GPIO output mode*/
    uint8_t GPIO_OutType;   /*GPIO output type*/
    uint8_t GPIO_PUPD;      /*pull-up pull down register selection*/
} GPIO_PinConf_t;

#define GPIO_PIN_NUM_0      0
#define GPIO_PIN_NUM_1      1
#define GPIO_PIN_NUM_2      2
#define GPIO_PIN_NUM_3      3
#define GPIO_PIN_NUM_4      4
#define GPIO_PIN_NUM_5      5
#define GPIO_PIN_NUM_6      6
#define GPIO_PIN_NUM_7      7
#define GPIO_PIN_NUM_8      8
#define GPIO_PIN_NUM_9      9
#define GPIO_PIN_NUM_10     10
#define GPIO_PIN_NUM_11     11
#define GPIO_PIN_NUM_12     12
#define GPIO_PIN_NUM_13     13
#define GPIO_PIN_NUM_14     14
#define GPIO_PIN_NUM_15     15
 
/*gpio output mode*/
#define GPIO_MODE_INPUT     0
#define GPIO_MODE_OUTPUT    1
#define GPIO_MODE_ALT       2
#define GPIO_MODE_ANALOG    3
/*gpio output type*/
#define GPIO_OUT_PP         0
#define GPIO_OUT_OD         1

/*gpio pupd*/
#define GPIO_NO_PUPD        0
#define GPIO_PU             1
#define GPIO_PD             2


void GPIO_Init(GPIO_RegDef_t * GPIOx, GPIO_PinConf_t PinConf);
void GPIO_PinWrite(GPIO_RegDef_t * GPIOx, uint8_t PinNumber, uint8_t Value);
void GPIO_PinToggle(GPIO_RegDef_t * GPIOx, uint8_t PinNumber);

uint8_t GPIO_PinRead(GPIO_RegDef_t * GPIOx, uint8_t PinNumber);

#endif