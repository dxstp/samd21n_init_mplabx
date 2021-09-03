// DOM-IGNORE-BEGIN
/*
    (c) 2021 Microchip Technology Inc. and its subsidiaries. 
    
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
#include "adc.h"

void ADC_init() {
    // connect GLCK2 with ADC module (4MHz clock)
	GCLK_REGS->GCLK_CLKCTRL =
        GCLK_CLKCTRL_GEN(GCLK_CLKCTRL_GEN_GCLK2_Val) |
        GCLK_CLKCTRL_ID(GCLK_CLKCTRL_ID_ADC_Val) |
        GCLK_CLKCTRL_CLKEN(1);

    // connect APBC clock for ADC module
    PM_REGS->PM_APBCMASK |= PM_APBCMASK_ADC(1);
    
  	// do a software reset of the module (write-synchronized)
    ADC_REGS->ADC_CTRLA = ADC_CTRLA_SWRST(1);
    while (ADC_REGS->ADC_STATUS & ADC_STATUS_SYNCBUSY(1));
    while (ADC_REGS->ADC_CTRLA & ADC_CTRLA_SWRST(1));
    
    // maximum CLK_ADC is 2100 kHz
    // set to 1MHz to have same clock as in temperature log row (37.11.8.2.1)
    // set resolution to 16 bit for averaging mode
    // set to free running mode
    ADC_REGS->ADC_CTRLB = 
        ADC_CTRLB_PRESCALER(ADC_CTRLB_PRESCALER_DIV4_Val) |
        ADC_CTRLB_RESSEL(ADC_CTRLB_RESSEL_16BIT_Val) |
        ADC_CTRLB_FREERUN(1);
    
    // average 4 samples
    ADC_REGS->ADC_AVGCTRL = ADC_AVGCTRL_SAMPLENUM(ADC_AVGCTRL_SAMPLENUM_4_Val);
    
    // select internal temperature sensor as input channel
    ADC_REGS->ADC_INPUTCTRL = 
        ADC_INPUTCTRL_MUXPOS(ADC_INPUTCTRL_MUXPOS_TEMP_Val) |
        ADC_INPUTCTRL_MUXNEG(ADC_INPUTCTRL_MUXNEG_GND_Val);
    
    // enable ADC 
    ADC_REGS->ADC_CTRLA = ADC_CTRLA_ENABLE(1);
    while(ADC_REGS-> ADC_STATUS & ADC_STATUS_SYNCBUSY_Msk);
    
    // start first conversion and wait for resultS
    ADC_REGS->ADC_INTFLAG = ADC_INTFLAG_RESRDY_Msk;
    ADC_REGS->ADC_SWTRIG = ADC_SWTRIG_START_Msk;
    while(ADC_REGS->ADC_INTFLAG & ADC_INTFLAG_RESRDY_Msk);
    
}