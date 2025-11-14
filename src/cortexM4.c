#include "cortexM4.h"

/**
 * @brief This function sets the priority of the given IRQ number in the NVIC
 * 
 * @param IRQNumber Interrupt request number to be set the priority
 * @param Priority  The priority to be set, this should be the value in [0..16]
 *                  The NVIC_IPR register uses only bit [7:4] of each bit field to configure the priority
 */
void NVIC_SetPriority(uint8_t IRQNumber, uint8_t Priority)
{
    uint8_t index, bitpos;

    /*Specify the NVIC_IPRx register index*/
    index = IRQNumber / 4U;
    /*Specify the bit position to start writing the priority*/
    bitpos = (IRQNumber % 4U) * 8U;
    /*Set the priority*/
    NVIC->IPR[index] &= ~(0x0F << (bitpos + 4U));
    NVIC->IPR[index] |= (Priority << (bitpos + 4U));
}

/**
 * @brief This function enables interrupt request for the given IRQ
 * 
 * @param IRQNumber Interrupt request number to be set the priority
 */
void NVIC_EnableIRQ(uint8_t IRQNumber)
{
    uint8_t index, bitpos;

    /*Specify the NVIC_ISERx register index*/
    index = IRQNumber / 32U;
    /*Specify the bit position*/
    bitpos = IRQNumber % 32U;
    /*Enable the IRQ*/
    NVIC->ISER[index] |= (0x01U << bitpos);
}
