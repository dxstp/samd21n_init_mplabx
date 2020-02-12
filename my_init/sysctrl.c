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
#include "sysctrl.h"

void SYSCTRL_init() {
    // OSC8M is running by default at 1 MHz, clock source for CPU after POR
    // OSCULP32K is running by default at 32 kHz
    
    // configure XOSC32K to work with an external crystal oszillator
    SYSCTRL_REGS->SYSCTRL_XOSC32K =
        SYSCTRL_XOSC32K_STARTUP(SYSCTRL_XOSC32K_STARTUP_CYCLE65536_Val) |
        SYSCTRL_XOSC32K_ONDEMAND(0) |
        SYSCTRL_XOSC32K_RUNSTDBY(0) |
        SYSCTRL_XOSC32K_AAMPEN(0) |
        SYSCTRL_XOSC32K_EN32K(1) |
        SYSCTRL_XOSC32K_XTALEN(1);

    // enable XOSC32K
    SYSCTRL_REGS->SYSCTRL_XOSC32K |= SYSCTRL_XOSC32K_ENABLE(1);
    
    // load calibration for internal OSC32K from calibration memory
    uint64_t reg = *(uint64_t *) OTP4_ADDR;
    reg &= 0x00001FC000000000;
    reg >>= 38;
    
    // configure and enable internal OSC32K
    SYSCTRL_REGS->SYSCTRL_OSC32K =
        SYSCTRL_OSC32K_CALIB(reg) |
        SYSCTRL_OSC32K_STARTUP(SYSCTRL_OSC32K_STARTUP_CYCLE3_Val) |
        SYSCTRL_OSC32K_ONDEMAND(0) |
        SYSCTRL_OSC32K_RUNSTDBY(0) |
        SYSCTRL_OSC32K_EN32K(1);
    
    // enable OSC32K
     SYSCTRL_REGS->SYSCTRL_OSC32K |= SYSCTRL_OSC32K_ENABLE(1);

    
     
    // wait until OSC32K is ready
    while((SYSCTRL_REGS->SYSCTRL_PCLKSR & SYSCTRL_PCLKSR_OSC32KRDY_Msk) == 0);
    
    // wait for XOSC32K to stabilize
    while((SYSCTRL_REGS->SYSCTRL_PCLKSR & SYSCTRL_PCLKSR_XOSC32KRDY_Msk) == 0);
}
