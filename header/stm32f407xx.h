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

#define AHB1_BASSADDR               (0x40020000U) /*AHB1 bass address*/

#define APB2_BASEADDR               (0x40010000UL) /*APB2 base address*/

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

#define BIT_RESET   0
#define BIT_SET     1

#endif
