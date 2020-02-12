// DOM-IGNORE-BEGIN
/*
    (c) 2019 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
 */
// DOM-IGNORE-END

#include <xc.h>
#include "sercom.h"

/** 
 * init the SERCOM3 module to 115200 baud, 8N1
 */
void SERCOM3_init(void) {

	// connect GLCK0 with SERCOM3 module (core clock)
	GCLK_REGS->GCLK_CLKCTRL =
        GCLK_CLKCTRL_GEN(GCLK_CLKCTRL_GEN_GCLK0_Val) |
        GCLK_CLKCTRL_ID(GCLK_CLKCTRL_ID_SERCOM3_CORE_Val) |
        GCLK_CLKCTRL_CLKEN(1);
    	
    // connect APBC clock for SERCOM3 module
    PM_REGS->PM_APBCMASK |= PM_APBCMASK_SERCOM3(1);
    
    
	// do a software reset of the module (write-synchronized)
	SERCOM3_REGS->USART_INT.SERCOM_CTRLA = SERCOM_USART_INT_CTRLA_SWRST(1);
	while (SERCOM3_REGS->USART_INT.SERCOM_SYNCBUSY & SERCOM_USART_INT_SYNCBUSY_SWRST(1));

	// configure UART mode and bit order
	SERCOM3_REGS->USART_INT.SERCOM_CTRLA =
		  SERCOM_USART_INT_CTRLA_MODE(1)
		| SERCOM_USART_INT_CTRLA_RXPO(1)
		| SERCOM_USART_INT_CTRLA_TXPO(0)
		| SERCOM_USART_INT_CTRLA_DORD(1);

	// enable transmitter and receiver
	SERCOM3_REGS->USART_INT.SERCOM_CTRLB =
		  SERCOM_USART_INT_CTRLB_TXEN(1)
		| SERCOM_USART_INT_CTRLB_RXEN(1);
	while(SERCOM3_REGS->USART_INT.SERCOM_SYNCBUSY & SERCOM_USART_INT_SYNCBUSY_CTRLB(1));

	// setup baud rate
	SERCOM3_REGS->USART_INT.SERCOM_BAUD = CONF_SERCOM_3_USART_BAUD_RATE_REGISTER_VAL;

	// enable UART
	SERCOM3_REGS->USART_INT.SERCOM_CTRLA |= SERCOM_USART_INT_CTRLA_ENABLE(1);
	while(SERCOM3_REGS->USART_INT.SERCOM_SYNCBUSY & SERCOM_USART_INT_SYNCBUSY_ENABLE(1));
}

int32_t SERCOM3_write(const char *const buf, const uint32_t length) {
	uint32_t offset = 0;
	
	while(!(SERCOM3_REGS->USART_INT.SERCOM_INTFLAG & SERCOM_USART_INT_INTFLAG_DRE(1)));
	
	do {
		SERCOM3_REGS->USART_INT.SERCOM_DATA = buf[offset];
		while(!(SERCOM3_REGS->USART_INT.SERCOM_INTFLAG & SERCOM_USART_INT_INTFLAG_DRE(1)));
	} while (++offset < length);
	
	while(!(SERCOM3_REGS->USART_INT.SERCOM_INTFLAG & SERCOM_USART_INT_INTFLAG_TXC(1)));
	
	return (int32_t)offset;
}

int32_t SERCOM3_read(char *const buf, const uint32_t length) {
	uint32_t offset = 0;
	
	do {
		while(!(SERCOM3_REGS->USART_INT.SERCOM_INTFLAG & SERCOM_USART_INT_INTFLAG_RXC(1)));
		buf[offset] = SERCOM3_REGS->USART_INT.SERCOM_DATA;
	} while (++offset < length);

	return (int32_t)offset;
}

int32_t SERCOM3_IsDataAvailable(void)
{
	return (SERCOM3_REGS->USART_INT.SERCOM_INTFLAG & SERCOM_USART_INT_INTFLAG_RXC(1)) ? 1 : 0;
}