#ifndef STM32F407XX_GPIO_DRIVER
#define STM32F407XX_GPIO_DRIVER
#include "stm32f407xx.h"

typedef struct
{
    uint8_t GPIO_PinNumber;             /*GPIO pin number*/
    uint8_t GPIO_PinMode;               /*GPIO pin mode*/
    uint8_t GPIO_OutType;               /*GPIO output type*/
    uint8_t GPIO_PUPD;                  /*Pull-up/pull-down resistor selection*/
    uint8_t GPIO_EdgeTrigger;           /*Edge trigger selection*/
    uint8_t GPIO_AltFunc;               /*Alternate function*/
} GPIO_PinConf_t;

/*GPIO pin number*/
#define GPIO_PIN_NUM_0          0       /*GPIO pin number 0*/
#define GPIO_PIN_NUM_1          1
#define GPIO_PIN_NUM_2          2
#define GPIO_PIN_NUM_3          3
#define GPIO_PIN_NUM_4          4
#define GPIO_PIN_NUM_5          5
#define GPIO_PIN_NUM_6          6
#define GPIO_PIN_NUM_7          7
#define GPIO_PIN_NUM_8          8
#define GPIO_PIN_NUM_9          9
#define GPIO_PIN_NUM_10         10
#define GPIO_PIN_NUM_11         11
#define GPIO_PIN_NUM_12         12
#define GPIO_PIN_NUM_13         13
#define GPIO_PIN_NUM_14         14
#define GPIO_PIN_NUM_15         15

/*GPIO pin mode*/
#define GPIO_MODE_INPUT         0
#define GPIO_MODE_OUTPUT        1
#define GPIO_MODE_ALT           2
#define GPIO_MODE_ANALOG        3

/*GPIO output type*/
#define GPIO_OUT_PP             0
#define GPIO_OUT_OD             1    

/*GPIO pull-up/pull-down*/
#define GPIO_NO_PUPD            0
#define GPIO_PU                 1
#define GPIO_PD                 2

/*Edge trigger selection*/
#define GPIO_IT_EDGE_FT         0
#define GPIO_IT_EDGE_RT         1

/*Alternate function*/
#define GPIO_ALT_AF0    0U
#define GPIO_ALT_AF1    1U
#define GPIO_ALT_AF2    2U
#define GPIO_ALT_AF3    3U
#define GPIO_ALT_AF4    4U
#define GPIO_ALT_AF5    5U
#define GPIO_ALT_AF6    6U
#define GPIO_ALT_AF7    7U
#define GPIO_ALT_AF8    8U
#define GPIO_ALT_AF9    9U
#define GPIO_ALT_AF10   10U
#define GPIO_ALT_AF11   11U
#define GPIO_ALT_AF12   12U
#define GPIO_ALT_AF13   13U
#define GPIO_ALT_AF14   14U
#define GPIO_ALT_AF15   15U


/*Macro to get GPIO port code for SYSCFG_EXTICR register configuration*/
#define SYSCFG_EXTICR_PORTCODE(GPIOx)\
        ((GPIOx == GPIOA) ? 0U : \
         (GPIOx == GPIOB) ? 1U : \
         (GPIOx == GPIOC) ? 2U : \
         (GPIOx == GPIOD) ? 3U : \
         (GPIOx == GPIOE) ? 4U : \
         (GPIOx == GPIOF) ? 5U : \
         (GPIOx == GPIOG) ? 6U : \
         (GPIOx == GPIOH) ? 7U : \
         (GPIOx == GPIOI) ? 8U : 0U)

/*Macro to get IRQ number from GPIO pin number*/
#define GPIO_PIN_TO_IRQ(PinNum) \
        ((PinNum == 0U) ? IRQ_NO_EXTI0 : \
         (PinNum == 1U) ? IRQ_NO_EXTI1 : \
         (PinNum == 2U) ? IRQ_NO_EXTI2 : \
         (PinNum == 3U) ? IRQ_NO_EXTI3 : \
         (PinNum == 4U) ? IRQ_NO_EXTI4 : \
         ((PinNum >= 5U) && (PinNum <= 9U)) ? IRQ_NO_EXTI9_5 : \
         ((PinNum >= 10U) && (PinNum <= 15U)) ? IRQ_NO_EXTI10_15 : IRQ_NO_EXTI0)

void GPIO_Init(GPIO_RegDef_t * GPIOx, GPIO_PinConf_t PinConf);
void GPIO_PinWrite(GPIO_RegDef_t * GPIOx, uint8_t PinNumber, uint8_t Value);
void GPIO_PinToggle(GPIO_RegDef_t * GPIOx, uint8_t PinNumber);
uint8_t GPIO_PinRead(GPIO_RegDef_t * GPIOx, uint8_t PinNumber);
void GPIO_IT_Init(GPIO_RegDef_t * GPIOx, GPIO_PinConf_t GPIOPinConf, uint8_t Priority);
#endif
