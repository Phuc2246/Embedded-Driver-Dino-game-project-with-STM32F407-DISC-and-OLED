#ifndef STM32F407XX
#define STM32F407XX

#include <stdint.h>
/*GPIO register definiiton struct*/
typedef struct
{
    volatile uint32_t MODER;    /* GPIO port mode register */
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

typedef struct
{
    volatile uint32_t CR;            /* 0x00 */
    volatile uint32_t PLLCFGR;       /* 0x04 */
    volatile uint32_t CFGR;          /* 0x08 */
    volatile uint32_t CIR;           /* 0x0C */
    volatile uint32_t AHB1RSTR;      /* 0x10 */
    volatile uint32_t AHB2RSTR;      /* 0x14 */
    volatile uint32_t AHB3RSTR;      /* 0x18 */
    uint32_t          RESERVED0;     /* 0x1C */
    volatile uint32_t APB1RSTR;      /* 0x20 */
    volatile uint32_t APB2RSTR;      /* 0x24 */
    uint32_t          RESERVED1[2];  /* 0x28, 0x2C */
    volatile uint32_t AHB1ENR;       /* 0x30 */
    volatile uint32_t AHB2ENR;       /* 0x34 */
    volatile uint32_t AHB3ENR;       /* 0x38 */
    uint32_t          RESERVED2;     /* 0x3C */ // phan ma nha sx chua dung toi... dung cho con chip sau nay...layout tuong tu f411..
    volatile uint32_t APB1ENR;       /* 0x40 */
    volatile uint32_t APB2ENR;       /* 0x44 */
    uint32_t          RESERVED3[2];  /* 0x48, 0x4C */
    volatile uint32_t AHB1LPENR;     /* 0x50 */
    volatile uint32_t AHB2LPENR;     /* 0x54 */
    volatile uint32_t AHB3LPENR;     /* 0x58 */
    uint32_t          RESERVED4;     /* 0x5C */
    volatile uint32_t APB1LPENR;     /* 0x60 */
    volatile uint32_t APB2LPENR;     /* 0x64 */
    uint32_t          RESERVED5[2];  /* 0x68, 0x6C */
    volatile uint32_t BDCR;          /* 0x70 */
    volatile uint32_t CSR;           /* 0x74 */
    uint32_t          RESERVED6;     /* 0x78 */
    volatile uint32_t SSCGR;         /* 0x7C */
    volatile uint32_t PLLI2SCFGR;
} RCC_RegDef_t;

#define AHB1_BASSADDR             0x40020000U

#define GPIOA ((GPIO_RegDef_t *) (AHB1_BASSADDR + 0x0000U))
#define GPIOB ((GPIO_RegDef_t *) (AHB1_BASSADDR + 0x0400U))
#define GPIOC ((GPIO_RegDef_t *) (AHB1_BASSADDR + 0x0800U))
#define GPIOD ((GPIO_RegDef_t *) (AHB1_BASSADDR + 0x0C00U))
#define GPIOE ((GPIO_RegDef_t *) (AHB1_BASSADDR + 0x1000U))
#define GPIOF ((GPIO_RegDef_t *) (AHB1_BASSADDR + 0x1400U))
#define GPIOG ((GPIO_RegDef_t *) (AHB1_BASSADDR + 0x1800U))
#define GPIOH ((GPIO_RegDef_t *) (AHB1_BASSADDR + 0x1C00U))
#define GPIOI ((GPIO_RegDef_t *) (AHB1_BASSADDR + 0x2000U))
#define GPIOJ ((GPIO_RegDef_t *) (AHB1_BASSADDR + 0x2400U))
#define GPIOK ((GPIO_RegDef_t *) (AHB1_BASSADDR + 0x2800U))


#define RCC   ((RCC_RegDef_t *) (AHB1_BASSADDR + 0x3800U))




#define GPIOA_CLK_ENB()          (RCC->AHB1ENR |= (0x01U << 0U))
#define GPIOB_CLK_ENB()          (RCC->AHB1ENR |= (0x01U << 1U))
#define GPIOC_CLK_ENB()          (RCC->AHB1ENR |= (0x01U << 2U))
#define GPIOD_CLK_ENB()          (RCC->AHB1ENR |= (0x01U << 3U))
#define GPIOE_CLK_ENB()          (RCC->AHB1ENR |= (0x01U << 4U))
#define GPIOF_CLK_ENB()          (RCC->AHB1ENR |= (0x01U << 5U))
#define GPIOG_CLK_ENB()          (RCC->AHB1ENR |= (0x01U << 6U))
#define GPIOH_CLK_ENB()          (RCC->AHB1ENR |= (0x01U << 7U))
#define GPIOI_CLK_ENB()          (RCC->AHB1ENR |= (0x01U << 8U))

#define BIT_RESET 0
#define BIT_SET   1
#endif