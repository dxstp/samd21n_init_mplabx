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
#include "port.h"

enum gpio_port { GPIO_PORTA, GPIO_PORTB, GPIO_PORTC };

/**
 * init the GPIO module
 */
void PORT_init(void) {
	
    // turn on output for LED
    // SAMD21 Xplained Pro:
    // PB30
    PORT_REGS->GROUP[GPIO_PORTB].PORT_DIRSET = (1 << 30);
    
    // turn on SERCOM functionality
	// SAMD21 Xplained Pro:
	// PA22 = SERCOM3, Pad[0], SAMD21 UART TX line
	// PA23 = SERCOM3, Pad[1], SAMD21 UART RX line
	PORT_REGS->GROUP[GPIO_PORTA].PORT_WRCONFIG =
		  PORT_WRCONFIG_PMUX(MUX_PA22C_SERCOM3_PAD0)
		| PORT_WRCONFIG_WRPMUX(1)
	    | PORT_WRCONFIG_PMUXEN(1)
		| PORT_WRCONFIG_WRPINCFG(1)
        | PORT_WRCONFIG_HWSEL(1)
	    | ((1 << 6) & 0xffff);
	PORT_REGS->GROUP[GPIO_PORTA].PORT_WRCONFIG =
		  PORT_WRCONFIG_PMUX(MUX_PA23C_SERCOM3_PAD1)
        | PORT_WRCONFIG_WRPMUX(1)
		| PORT_WRCONFIG_PMUXEN(1)
		| PORT_WRCONFIG_WRPINCFG(1)
		| ((1 << 7) & 0xffff);
    
	// turn on GCLK1 output
	// SAMD21 Xplained Pro:
	// PB15 = EXT2, Pin 10
	PORT_REGS->GROUP[GPIO_PORTB].PORT_WRCONFIG =
		  PORT_WRCONFIG_PMUX(MUX_PB15H_GCLK_IO1)
		| PORT_WRCONFIG_WRPMUX(1)
		| PORT_WRCONFIG_PMUXEN(1)
		| PORT_WRCONFIG_WRPINCFG(1)
		| PORT_WRCONFIG_HWSEL(0)
		| ((1 << 15) & 0xffff);
    
    // turn on GCLK3 output
	// SAMD21 Xplained Pro:
	// PA17 = EXT2, Pin 15
	PORT_REGS->GROUP[GPIO_PORTA].PORT_WRCONFIG =
		  PORT_WRCONFIG_PMUX(MUX_PA17H_GCLK_IO3)
		| PORT_WRCONFIG_WRPMUX(1)
		| PORT_WRCONFIG_PMUXEN(1)
		| PORT_WRCONFIG_WRPINCFG(1)
		| PORT_WRCONFIG_HWSEL(1)
		| ((1 << 1) & 0xffff);
}