#ifndef STM32F407XX
#define STM32F407XX

#include <stdint.h>
#include "cortexM4.h"

/*GPIO register definition struct*/
typedef struct
{
    volatile uint32_t MODER;        /*GPIO mode register*/
    volatile uint32_t OTYPER;
    volatile uint32_t OSPEEDR;
    volatile uint32_t PUPDR;
    volatile uint32_t IDR;
    volatile uint32_t ODR;
    volatile uint32_t BSRR;
    volatile uint32_t LCKR;
    volatile uint32_t AFRL;
    volatile uint32_t AFRH;
} GPIO_RegDef_t;

/*RCC register definition struct*/
typedef struct 
{
    volatile uint32_t CR;
    volatile uint32_t PLLCFGR;
    volatile uint32_t CFGR;
    volatile uint32_t CIR;
    volatile uint32_t AHB1RSTR;
    volatile uint32_t AHB2RSTR;
    volatile uint32_t AHB3RSTR;
             uint32_t RESERVED0;
    volatile uint32_t APB1RSTR;
    volatile uint32_t APB2RSTR;
             uint32_t RESERVED1[2];
    volatile uint32_t AHB1ENR;
    volatile uint32_t AHB2ENR;
    volatile uint32_t AHB3ENR;
             uint32_t RESERVED2;
    volatile uint32_t APB1ENR;
    volatile uint32_t APB2ENR;
             uint32_t RESERVED3[2];
    volatile uint32_t AHB1LPENR;
    volatile uint32_t AHB2LPENR;
    volatile uint32_t AHB3LPENR;
             uint32_t RESERVED4;
    volatile uint32_t APB1LPENR;
    volatile uint32_t APB2LPENR;
             uint32_t RESERVED5[2];
    volatile uint32_t BDCR;
    volatile uint32_t CSR;
             uint32_t RESERVED6[2];
    volatile uint32_t SSCGR;
    volatile uint32_t PLLI2SCFGR;
} RCC_RegDef_t;

/*SYSCFG register definition struct*/
typedef struct
{
    volatile uint32_t MEMRMP;
    volatile uint32_t PMC;
    volatile uint32_t EXTICR[4];
    volatile uint32_t CMPCR;
} SYSCFG_RegDef_t;

/*EXTI register definition struct*/
typedef struct 
{
    volatile uint32_t IMR;
    volatile uint32_t EMR;
    volatile uint32_t RTSR;
    volatile uint32_t FTSR;
    volatile uint32_t SWIER;
    volatile uint32_t PR;
} EXTI_RegDef_t;

/*USART register definition struct*/
/*TODO-----------------------------------------------*/
/*USART_RegDef_t*/

/*USART register definition struct*/
typedef struct 
{
  volatile uint32_t SR;
  volatile uint32_t DR;
  volatile uint32_t BRR;
  volatile uint32_t CR1;
  volatile uint32_t CR2;
  volatile uint32_t CR3;
  volatile uint32_t GTPR;
} USART_RegDef_t;


// /*Timer register definition struct*/
// typedef struct 
// {
//   volatile uint32_t CR1;
//   volatile uint32_t CR2;
//   uint32_t RESERVERD0;
//   volatile uint32_t DIER;
//   volatile uint32_t SR;
//   volatile uint32_t EGR;
//   uint32_t RESERVERD1[3];
//   volatile uint32_t CNT;
//   volatile uint32_t PSC;
//   volatile uint32_t ARR;
// } TIM_RegDef_t;

/*Timer register definition struct*/
typedef struct 
{
  volatile uint32_t CR1;
  volatile uint32_t CR2;
  volatile uint32_t SMCR;
  volatile uint32_t DIER;
  volatile uint32_t SR;
  volatile uint32_t EGR;
  volatile uint32_t CCMR[2];
  volatile uint32_t CCER;
  volatile uint32_t CNT;
  volatile uint32_t PSC;
  volatile uint32_t ARR;
  uint32_t RESERVERD0;
  volatile uint32_t CCR[4];
  uint32_t RESERVERD1;
  volatile uint32_t DCR;
  volatile uint32_t DMAR;
  volatile uint32_t OR;
} TIM_RegDef_t;


#define AHB1_BASSADDR               (0x40020000U) /*AHB1 bass address*/
#define APB1_BASEADDR               (0x40000000U) /*APB1 base address*/
#define APB2_BASEADDR               (0x40010000U) /*APB2 base address*/

#define GPIOA   ((GPIO_RegDef_t *) (AHB1_BASSADDR + 0x0000U))   /*GPIOA bass address*/
#define GPIOB   ((GPIO_RegDef_t *) (AHB1_BASSADDR + 0x0400U))
#define GPIOC   ((GPIO_RegDef_t *) (AHB1_BASSADDR + 0x0800U))
#define GPIOD   ((GPIO_RegDef_t *) (AHB1_BASSADDR + 0x0C00U))
#define GPIOE   ((GPIO_RegDef_t *) (AHB1_BASSADDR + 0x1000U))
#define GPIOF   ((GPIO_RegDef_t *) (AHB1_BASSADDR + 0x1400U))
#define GPIOG   ((GPIO_RegDef_t *) (AHB1_BASSADDR + 0x1800U))
#define GPIOH   ((GPIO_RegDef_t *) (AHB1_BASSADDR + 0x1C00U))
#define GPIOI   ((GPIO_RegDef_t *) (AHB1_BASSADDR + 0x2000U))
#define GPIOJ   ((GPIO_RegDef_t *) (AHB1_BASSADDR + 0x2400U))
#define GPIOK   ((GPIO_RegDef_t *) (AHB1_BASSADDR + 0x2800U))

/*RCC base address*/
#define RCC     ((RCC_RegDef_t *)  (AHB1_BASSADDR + 0x3800UL))

/*SYSCFG base address*/
#define SYSCFG ((SYSCFG_RegDef_t *) (APB2_BASEADDR + 0x3800UL))

/*EXTI base address*/
#define EXTI ((EXTI_RegDef_t*) (APB2_BASEADDR + 0x3C00UL))

/* USART peripheral base address */
#define USART1  ((USART_RegDef_t *) (APB2_BASEADDR + 0x1000UL))   /* USART 1 peripheral base address */
#define USART2  ((USART_RegDef_t *) (APB1_BASEADDR + 0x4400UL))   /* USART 2 peripheral base address */
#define USART3  ((USART_RegDef_t *) (APB1_BASEADDR + 0x4800UL))   /* USART 3 peripheral base address */
#define UART4   ((USART_RegDef_t *) (APB1_BASEADDR + 0x4C00UL))   /* UART 4 peripheral base address */
#define UART5   ((USART_RegDef_t *) (APB1_BASEADDR + 0x5000UL))   /* UART 5 peripheral base address */
#define USART6  ((USART_RegDef_t *) (APB2_BASEADDR + 0x1400UL))   /* USART 6 peripheral base address */


// /*Timer peripheral base address*/
// #define TIM6    ((TIM_RegDef_t *) (APB1_BASEADDR + 0x1000UL))     /*Timer 6 peripheral base address */
// #define TIM7    ((TIM_RegDef_t *) (APB1_BASEADDR + 0x1400UL))     /*Timer 7 peripheral base address */

/*Timer peripheral base address*/
#define TIM2    ((TIM_RegDef_t *) (APB1_BASEADDR + 0x0000UL))     /*Timer 2 peripheral base address */
#define TIM3    ((TIM_RegDef_t *) (APB1_BASEADDR + 0x0400UL))     /*Timer 3 peripheral base address */
#define TIM4    ((TIM_RegDef_t *) (APB1_BASEADDR + 0x0800UL))     /*Timer 4 peripheral base address */
#define TIM5    ((TIM_RegDef_t *) (APB1_BASEADDR + 0x0C00UL))     /*Timer 5 peripheral base address */
#define TIM6    ((TIM_RegDef_t *) (APB1_BASEADDR + 0x1000UL))     /*Timer 6 peripheral base address */
#define TIM7    ((TIM_RegDef_t *) (APB1_BASEADDR + 0x1400UL))     /*Timer 7 peripheral base address */

/*GPIO clock enable*/
#define GPIOA_CLK_ENB()     (RCC->AHB1ENR |= (0x01U << 0U)) /*GPIOA peripheral clock enable*/
#define GPIOB_CLK_ENB()     (RCC->AHB1ENR |= (0x01U << 1U)) /*GPIOB peripheral clock enable*/ 
#define GPIOC_CLK_ENB()     (RCC->AHB1ENR |= (0x01U << 2U)) /*GPIOC peripheral clock enable*/
#define GPIOD_CLK_ENB()     (RCC->AHB1ENR |= (0x01U << 3U)) /*GPIOD peripheral clock enable*/
#define GPIOE_CLK_ENB()     (RCC->AHB1ENR |= (0x01U << 4U)) /*GPIOE peripheral clock enable*/
#define GPIOF_CLK_ENB()     (RCC->AHB1ENR |= (0x01U << 5U)) /*GPIOF peripheral clock enable*/
#define GPIOG_CLK_ENB()     (RCC->AHB1ENR |= (0x01U << 6U)) /*GPIOG peripheral clock enable*/
#define GPIOH_CLK_ENB()     (RCC->AHB1ENR |= (0x01U << 7U)) /*GPIOH peripheral clock enable*/
#define GPIOI_CLK_ENB()     (RCC->AHB1ENR |= (0x01U << 8U)) /*GPIOI peripheral clock enable*/

#define SYSCFG_CLK_ENB()    (RCC->APB2ENR |= (0x01U << 14U)) /*SYSCFG peripheral clock enable*/

/* USART peripheral clock enable */
#define USART1_CLK_ENB()    (RCC->APB2ENR |= (0x01U << 4))   /* USART1 peripheral clock enable */       
#define USART2_CLK_ENB()    (RCC->APB1ENR |= (0x01U << 17))  /* USART2 peripheral clock enable */
#define USART3_CLK_ENB()    (RCC->APB1ENR |= (0x01U << 18))  /* USART3 peripheral clock enable */
#define UART4_CLK_ENB()     (RCC->APB1ENR |= (0x01U << 19))  /* UART4 peripheral clock enable */
#define UART5_CLK_ENB()     (RCC->APB1ENR |= (0x01U << 20))  /* UART5 peripheral clock enable */
#define USART6_CLK_ENB()    (RCC->APB2ENR |= (0x01U << 5))   /* USART6 peripheral clock enable */   

// /*Timer peripheral clock enable*/
// #define TIM6_CLK_ENB()      (RCC->APB1ENR |= (0x01 << 4U))  /*Timer 6 peripheral clock enable*/
// #define TIM7_CLK_ENB()      (RCC->APB1ENR |= (0x01 << 5U))  /*Timer 7 peripheral clock enable*/

/*Timer peripheral clock enable*/
#define TIM2_CLK_ENB()      (RCC->APB1ENR |= (0x01 << 0U))  /*Timer 2 peripheral clock enable */
#define TIM3_CLK_ENB()      (RCC->APB1ENR |= (0x01 << 1U))  /*Timer 3 peripheral clock enable */
#define TIM4_CLK_ENB()      (RCC->APB1ENR |= (0x01 << 2U))  /*Timer 4 peripheral clock enable */
#define TIM5_CLK_ENB()      (RCC->APB1ENR |= (0x01 << 3U))  /*Timer 5 peripheral clock enable */
#define TIM6_CLK_ENB()      (RCC->APB1ENR |= (0x01 << 4U))  /*Timer 6 peripheral clock enable*/
#define TIM7_CLK_ENB()      (RCC->APB1ENR |= (0x01 << 5U))  /*Timer 7 peripheral clock enable*/


#define BIT_RESET   0
#define BIT_SET     1
#define FALSE       0
#define TRUE        1
#define DISABLE     0
#define ENABLE      1

#endif
