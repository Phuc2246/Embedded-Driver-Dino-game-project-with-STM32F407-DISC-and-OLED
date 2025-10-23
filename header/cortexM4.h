#ifndef CORTEX_M4
#define CORTEX_M4
#include <stdint.h>

typedef struct
{
    volatile uint32_t ISER[8];          /*Interrupt set-enable register*/
             uint32_t RESERVED0[24];
    volatile uint32_t ICER[8];          /*Interrupt clear enable register*/
             uint32_t RESERVED1[24];
    volatile uint32_t ISPR[8];          /*Interrupt set pending register*/
             uint32_t RESERVED2[24];
    volatile uint32_t ICPR[8];          /*Interrupt clear pending register*/
             uint32_t RESERVED3[24];
    volatile uint32_t IABR[8];          /*Interrupt active bit register*/
             uint32_t RESERVED4[56];
    volatile uint32_t IPR[60];          /*Interrupt prority register*/
             uint32_t RESERVED5[644];
    volatile uint32_t STIR;             /*Software trigger interrupt register*/
} NVIC_RegDef_t;

/*NVIC base address*/
#define NVIC    ((NVIC_RegDef_t *) (0xE000E100UL))

/*Interrupt request number (IRQn)*/
#define IRQ_NO_EXTI0        6U
#define IRQ_NO_EXTI1        7U
#define IRQ_NO_EXTI2        8U
#define IRQ_NO_EXTI3        9U
#define IRQ_NO_EXTI4        10U
#define IRQ_NO_EXTI9_5      23U
#define IRQ_NO_EXTI10_15    40U


#define NULL ((void *)0)

void NVIC_SetPriority(uint8_t IRQNumber, uint8_t Priority);
void NVIC_EnableIRQ(uint8_t IRQNumber);
#endif
