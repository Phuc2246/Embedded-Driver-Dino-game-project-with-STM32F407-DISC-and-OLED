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
    uint8_t index, bitpos;//Vì sao có index và bitpos?
    /*Mỗi thanh ghi NVIC->IPR[x] là 32-bit, chứa 4 ô priority, mỗi ô 8 bit
    index là chỉ số đại diện như i trong mảng 
    vd lấy IRQ_NO_EXTI0        6U/4U=1 sẽ lấy IPR[1]
    bitpos = (IRQNumber % 4) * 8  -> byte offset (tính theo bit) của IRQ đó trong IPR[index].
    [7:0] Priority, byte offset = 0
    [15:8] Priority, byte offset = 1
    [23:16] Priority, byte offset = 2
    [31:24] Priority, byte offset = 3
==>6 % 4 = 2 → 2 * 8 = 16 ô priority nằm ở bit [23:16] của IPR[1].
Trên Cortex-M4, chỉ 4 bit cao của byte priority có tác dụng: [7:4] trong mỗi byte.
Bốn bit có tác dụng chính là [23:20], tức là (bitpos + 4) … (bitpos + 7).
    */

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
