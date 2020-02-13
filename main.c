// DOM-IGNORE-BEGIN
/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
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

#include <samd21j18a.h>
#include <stdio.h>
#include "my_init/sysctrl.h"
#include "my_init/nvmctrl.h"
#include "my_init/clockgen.h"
#include "my_init/port.h"
#include "my_init/sercom.h"
#include "my_init/dsu.h"
#include "utils/print.h"
#include "utils/delay.h"
#include "my_init/sysctrl.h"

int main(void) {
    NVMCTRL_init();
	SYSCTRL_init();
    CLOCKGEN_init();
	PORT_init();
	SERCOM3_init();
	print_init();
    DSU_init();
	
    printf("Hello D21 World!\r\n");
	
    while (1) {	
		
        PORT_REGS->GROUP[1].PORT_OUTTGL = (1 << 30);
		delay_ms(1000);
        
    }
}

void HardFault_Handler() {
    while(1);
}