#include "stm32f407xx_timer_driver.h"

/**
 * @brief This function is used to initialize the timer base unit
 * 
 * @param TIMx Pointer to the TIMx (e.g, TIM4, TIM6).
 * @param TIM_BaseConf Structer that contains the configuration information of a specified TIMx.
 */
void TIM_Base_Init(TIM_RegDef_t * TIMx, TIM_Base_Conf_t TIM_BaseConf)
{
    /*Set the auto-reload preload*/
    TIMx->CR1 |= (TIM_BaseConf.AutoReloadPreload << TIM_CR1_ARPE);
    /*Set the auto-reload value*/
    TIMx->ARR = TIM_BaseConf.Period;
    /*Set the prescaler value*/
    TIMx->PSC = TIM_BaseConf.Prescaler;
    /*Set timer counting mode*/
    TIMx->CR1 |= (TIM_BaseConf.CounterMode << TIM_CR1_DIR);
}

void TIM_Base_ForceUpdate(TIM_RegDef_t * TIMx)
{
    /*Generate an update event to reload the period and prescaler value*/
    TIMx->EGR |= (0x01U << TIM_EGR_UG);
    /*Clear the update flag*/
    TIMx->SR &= ~(0x01 << TIM_SR_UIF);    
}

/**
 * @brief This function starts the timer.
 * 
 * @param TIMx Pointer to the TIMx (e.g, TIM4, TIM6).
 */
void TIM_Base_Start(TIM_RegDef_t * TIMx)
{
    /* Start the timer */
    TIMx->CR1 |= (0x01 << TIM_CR1_CEN);
}

/**
 * @brief This function stops the timer.
 * 
 * @param TIMx Pointer to the TIMx (e.g, TIM4, TIM6).
 */
void TIM_Base_Stop(TIM_RegDef_t * TIMx)
{
    /* Stop the timer */
    TIMx->CR1 &= ~(1 << TIM_CR1_CEN);
}

/**
 * @brief This function initializes timer base interrupt
 * 
 * @param TIMx Pointer to the TIMx (e.g, TIM4, TIM6).
 * @param Priority Interrupt priority to be set
 */
void TIM_Base_IT_Init(TIM_RegDef_t * TIMx, uint8_t Priority)
{
    /* Set the interrupt priority for TIMx */
    NVIC_SetPriority(TIMx_TO_IRQ(TIMx), Priority);
    /* Enable the IRQ of TIMx */
    NVIC_EnableIRQ(TIMx_TO_IRQ(TIMx));
    /* Enable the TIMx update interrupt */
    TIMx->DIER |= (0x01 << TIM_DIER_UIE);
}


/**
 * @brief This function is used to initialize the output compare
 * 
 * @param TIMx Pointer to the TIMx (e.g, TIM4, TIM6).
 * @param TIM_OCConf Structer that contains the output compare configuration.
 * @param Channel Output compare channel to be configured.
 */
void TIM_OC_Init(TIM_RegDef_t * TIMx, TIM_OC_Conf_t TIM_OCConf, uint8_t Channel)
{
    uint8_t CCMR_Reg_Index, CCMR_Bit_Section;
    CCMR_Reg_Index = Channel / 2;
    CCMR_Bit_Section = Channel % 2;

    /*Set the output compare mode*/
    TIMx->CCMR[CCMR_Reg_Index] &= ~(TIM_OCConf.OCMode << (CCMR_Bit_Section*8 + 4));
    TIMx->CCMR[CCMR_Reg_Index] |= (TIM_OCConf.OCMode << (CCMR_Bit_Section*8 + 4));
    /*Set the output compare polarity*/
    TIMx->CCER |= (TIM_OCConf.OCPolarity << (Channel*4 + 1));
    /*Set the pulse value*/
    TIMx->CCR[Channel] = TIM_OCConf.Pulse;
    /*Enable the compare*/
    TIMx->CCER |= (ENABLE << Channel*4);
}