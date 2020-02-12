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

#include <samd21j18a.h>
#include "clockgen.h"

void CLOCKGEN_init() {
    // reset GCLK module
    GCLK_REGS->GCLK_CTRL = GCLK_CTRL_SWRST(1);
    while((GCLK_REGS->GCLK_STATUS & GCLK_STATUS_SYNCBUSY_Msk) &&
        (GCLK_REGS->GCLK_CTRL & GCLK_CTRL_SWRST_Msk));
    
    // set GCLK1 division factor
    GCLK_REGS->GCLK_GENDIV =
        GCLK_GENDIV_DIV(1) |
        GCLK_GENDIV_ID(1);
    
    // select XOSC32K as generator for GCLK1
    GCLK_REGS->GCLK_GENCTRL =
        GCLK_GENCTRL_ID(1) |
        GCLK_GENCTRL_SRC_XOSC32K |
        GCLK_GENCTRL_GENEN(1);
    while(GCLK_REGS->GCLK_STATUS & GCLK_STATUS_SYNCBUSY_Msk);
    
    // set GCLK1 as reference clock for DFLL
    GCLK_REGS->GCLK_CLKCTRL =
        GCLK_CLKCTRL_GEN(GCLK_CLKCTRL_GEN_GCLK1_Val) |
        GCLK_CLKCTRL_ID(GCLK_CLKCTRL_ID_DFLL48_Val) |
        GCLK_CLKCTRL_CLKEN(1);
   
    // start DFLL in open loop mode
    while((SYSCTRL_REGS->SYSCTRL_PCLKSR & SYSCTRL_PCLKSR_DFLLRDY_Msk) == 0);
    SYSCTRL_REGS->SYSCTRL_DFLLCTRL =
        SYSCTRL_DFLLCTRL_ENABLE(1);
        
    // load DFLL calibration value for DFLL48M from calibration memory
    uint64_t reg = *(uint64_t *) OTP4_ADDR;
    reg &= 0xFC00000000000000;
    reg >>= 58;
    while((SYSCTRL_REGS->SYSCTRL_PCLKSR & SYSCTRL_PCLKSR_DFLLRDY_Msk) == 0);
    SYSCTRL_REGS->SYSCTRL_DFLLVAL = SYSCTRL_DFLLVAL_COARSE(reg);
    
    // set up DFLL
    while((SYSCTRL_REGS->SYSCTRL_PCLKSR & SYSCTRL_PCLKSR_DFLLRDY_Msk) == 0);
    SYSCTRL_REGS->SYSCTRL_DFLLMUL =
        SYSCTRL_DFLLMUL_CSTEP(0x3F / 4) |
        SYSCTRL_DFLLMUL_FSTEP(0x3FF / 4) |
        SYSCTRL_DFLLMUL_MUL(1464); 
    
    // enable DFLL closed loop mode
    while((SYSCTRL_REGS->SYSCTRL_PCLKSR & SYSCTRL_PCLKSR_DFLLRDY_Msk) == 0);
    SYSCTRL_REGS->SYSCTRL_DFLLCTRL =
        SYSCTRL_DFLLCTRL_MODE(1) |
        SYSCTRL_DFLLCTRL_WAITLOCK(1) |
        SYSCTRL_DFLLCTRL_BPLCKC(1) |
        SYSCTRL_DFLLCTRL_ENABLE(1);

    // wait for fine lock
    while((SYSCTRL_REGS->SYSCTRL_PCLKSR & SYSCTRL_PCLKSR_DFLLRDY_Msk) == 0);
    while((SYSCTRL_REGS->SYSCTRL_PCLKSR & SYSCTRL_PCLKSR_DFLLLCKF_Msk) == 0);
    
    // switch GCLK0 to DFLL48M to run core at 48 MHz
    GCLK_REGS->GCLK_GENCTRL =
        GCLK_GENCTRL_ID(0) |
        GCLK_GENCTRL_SRC_DFLL48M |
        GCLK_GENCTRL_GENEN(1);
    while(GCLK_REGS->GCLK_STATUS & GCLK_STATUS_SYNCBUSY_Msk);
}
