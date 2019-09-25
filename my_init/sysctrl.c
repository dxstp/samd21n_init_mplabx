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

void SYSCTRL_init() {
    // load calibration for OSC32K from Calibration Area
    uint64_t reg = *(uint64_t *) OTP4_ADDR;
    reg &= 0x00001FC000000000;
    reg >>= 38;
    reg = 0x3F;
    
    // configure and enable OSC32K
    SYSCTRL_REGS->SYSCTRL_OSC32K =
            SYSCTRL_OSC32K_CALIB(reg) |
            SYSCTRL_OSC32K_STARTUP(SYSCTRL_OSC32K_STARTUP_CYCLE3_Val) |
            SYSCTRL_OSC32K_ONDEMAND(0) |
            SYSCTRL_OSC32K_RUNSTDBY(0) |
            SYSCTRL_OSC32K_EN32K(1) |
            SYSCTRL_OSC32K_ENABLE(1);

    // wait until OSC32K is ready
    while(!(SYSCTRL_REGS->SYSCTRL_PCLKSR & SYSCTRL_PCLKSR_OSC32KRDY_Msk));
    
    GCLK_REGS->GCLK_GENCTRL =
            GCLK_GENCTRL_OE(1) |
            GCLK_GENCTRL_IDC(1) |
            GCLK_GENCTRL_GENEN(1) |
            GCLK_GENCTRL_SRC(GCLK_GENCTRL_SRC_OSC32K_Val) |
            GCLK_GENCTRL_ID(4);
    
    while(GCLK_REGS->GCLK_STATUS & GCLK_STATUS_SYNCBUSY_Msk);
    
    PORT_REGS->GROUP[0].PORT_WRCONFIG = 
            PORT_WRCONFIG_HWSEL(0) |
            PORT_WRCONFIG_WRPINCFG(1) |
            PORT_WRCONFIG_WRPMUX(1) |
            PORT_WRCONFIG_PMUX(PORT_PMUX_PMUXE_H_Val) |
            PORT_WRCONFIG_PMUXEN(1) |
            PORT_WRCONFIG_PINMASK(1 << 10);
}
