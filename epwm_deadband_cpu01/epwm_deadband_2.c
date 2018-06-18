//###########################################################################
//
// FILE:    epwm_deadband_c28.c
//
// TITLE:   Check PWM Dead-Band
//
//! \addtogroup cpu01_example_list
//! <h1> EPWM dead band control (epwm_deadband)</h1>
//!
//! During the test, monitor ePWM1, ePWM2, and/or ePWM3 outputs
//! on a scope.
//!
//! - ePWM6A is on GPIO10
//! - ePWM6B is on GPIO11
//! - ePWM7A is on GPIO12
//! - ePWM7B is on GPIO13
//! - ePWM8A is on GPIO14
//! - ePWM8B is on GPIO15
//!
//! This example configures ePWM1, ePWM2 and ePWM3 for:
//! - Count up/down
//! - Deadband
//!
//! 3 Examples are included:
//! - ePWM6: Active low PWMs
//! - ePWM7: Active low complementary PWMs
//! - ePWM8: Active high complementary PWMs
//!
//! Each ePWM is configured to interrupt on the 3rd zero event.
//! When this happens the deadband is modified such that
//! 0 <= DB <= DB_MAX.  That is, the deadband will move up and
//! down between 0 and the maximum value.
//!
//! View the EPWM6A/B, EPWM7A/B and EPWM8A/B waveforms
//! via an oscilloscope
//
//
//###########################################################################
// $TI Release: F2837xS Support Library v210 $
// $Release Date: Tue Nov  1 15:35:23 CDT 2016 $
// $Copyright: Copyright (C) 2014-2016 Texas Instruments Incorporated -
//             http://www.ti.com/ ALL RIGHTS RESERVED $
//###########################################################################

//
// Included Files
//
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <file.h>
#include "F28x_Project.h"

//RET_VARS ret1 = RET_DEFAULTS;
//
// Defines
//

#define RESULTS_BUFFER_SIZE 256
//
// Globals
//
Uint16 resultsIndex;
Uint16 AdcaResults[RESULTS_BUFFER_SIZE];

//
// Function Prototypes
//
void InitEPwm1Example(void);
void InitEPwm6Example(void);
void InitEPwm7Example(void);
void InitEPwm8Example(void);
__interrupt void epwm1_isr(void);
__interrupt void epwm6_isr(void);
__interrupt void epwm7_isr(void);
__interrupt void epwm8_isr(void);

interrupt void adcal_isr(void); //CANAL A

void ConfigureADC(void);
void SetupADCContinuous(void);//(Uint16 channel);

//void InitEPwmGpio_DB(void);
//
// Main
//
void main(void)
{
//
// Step 1. Initialize System Control:
// PLL, WatchDog, enable Peripheral Clocks
// This example function is found in the F2837xS_SysCtrl.c file.
//
    InitSysCtrl();

//
// Step 2. Initialize GPIO:
// This example function is found in the F2837xS_Gpio.c file and
// illustrates how to set the GPIO to its default state.
//
    //InitGpio();
    //InitEPwmGpio_DB()
//
// enable PWM1, PWM2 and PWM3
//
    CpuSysRegs.PCLKCR2.bit.EPWM1=1;
    CpuSysRegs.PCLKCR2.bit.EPWM6=1;
    CpuSysRegs.PCLKCR2.bit.EPWM7=1;
    CpuSysRegs.PCLKCR2.bit.EPWM8=1;

//
// For this case just init GPIO pins for ePWM1, ePWM2, ePWM3
// These functions are in the F2837xS_EPwm.c file
//
    InitEPwm1Gpio();
    InitEPwm6Gpio();
    InitEPwm7Gpio();
    InitEPwm8Gpio();

//
// Step 3. Clear all interrupts and initialize PIE vector table:
// Disable CPU interrupts
//
    DINT;

//
// Initialize the PIE control registers to their default state.
// The default state is all PIE interrupts disabled and flags
// are cleared.
// This function is found in the F2837xS_PieCtrl.c file.
//
    InitPieCtrl();

//
// Disable CPU interrupts and clear all CPU interrupt flags:
//
    IER = 0x0000;
    IFR = 0x0000;

//
// Initialize the PIE vector table with pointers to the shell Interrupt
// Service Routines (ISR).
// This will populate the entire table, even if the interrupt
// is not used in this example.  This is useful for debug purposes.
// The shell ISR routines are found in F2837xS_DefaultIsr.c.
// This function is found in F2837xS_PieVect.c.
//
    InitPieVectTable();

    EALLOW; // This is needed to write to EALLOW protected registers

    PieVectTable.EPWM6_INT = &epwm1_isr;

    PieVectTable.EPWM6_INT = &epwm6_isr;
    PieVectTable.EPWM7_INT = &epwm7_isr;
    PieVectTable.EPWM8_INT = &epwm8_isr;

    PieVectTable.ADCA1_INT = &adcal_isr;

    EDIS;   // This is needed to disable write to EALLOW protected registers

    EALLOW;
    CpuSysRegs.PCLKCR0.bit.TBCLKSYNC =0;
    EDIS;

    InitEPwm1Example();
    InitEPwm6Example();
    InitEPwm7Example();
    InitEPwm8Example();

    ConfigureADC();

    SetupADCContinuous();

    EALLOW;
    CpuSysRegs.PCLKCR0.bit.TBCLKSYNC =1;
    EDIS;

    IER |= M_INT1;
    IER |= M_INT6;
    IER |= M_INT7;

    PieCtrlRegs.PIEIER2.bit.INTx1 = 1;
    PieCtrlRegs.PIEIER2.bit.INTx6 = 1;
    PieCtrlRegs.PIEIER2.bit.INTx7 = 1;
    PieCtrlRegs.PIEIER2.bit.INTx8 = 1;

    EINT;
    ERTM;

//  - IDLE loop. Just loop forever
// ***************************************************************************
        for(;;);  //infinite loop

} // END MAIN

__interrupt void epwm1_isr(void)
{
    //
    // Clear INT flag for this timer
    //
    EPwm1Regs.ETCLR.bit.INT = 1;

    //
    // Acknowledge this interrupt to receive more interrupts from group 3
    //
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;
}
//
// epwm6_isr - EPWM6 ISR
//
__interrupt void epwm6_isr(void)
{
    //
    // Clear INT flag for this timer
    //
    EPwm6Regs.ETCLR.bit.INT = 1;

    //
    // Acknowledge this interrupt to receive more interrupts from group 3
    //
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;
}

//
// epwm7_isr - EPWM7 ISR
//
__interrupt void epwm7_isr(void)
{
    //
    // Clear INT flag for this timer
    //
    EPwm7Regs.ETCLR.bit.INT = 1;

    //
    // Acknowledge this interrupt to receive more interrupts from group 3
    //
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;
}

//
// epwm8_isr - EPWM8 ISR
//
__interrupt void epwm8_isr(void)
{
    //
    // Clear INT flag for this timer
    //
    EPwm8Regs.ETCLR.bit.INT = 1;

    //
    // Acknowledge this interrupt to receive more interrupts from group 3
    //
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;
}

interrupt void adcal_isr(void)
{
    //AdcaRegs.ADCSOCFRC1.bit.SOC0 = 1;

    AdcaResults[resultsIndex++] = (AdcaResultRegs.ADCRESULT0);
    if(RESULTS_BUFFER_SIZE<=resultsIndex)
    {
       resultsIndex = 0;
    }

    AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; // clear INT1 flag
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP2;

}
//
// InitEPwm6Example - Initialize EPWM6 configuration
//
//TODO pwm examples

void InitEPwm1Example()
{
    EPwm1Regs.TBPRD = 5208/2; //6000;                       // Set timer period
    EPwm1Regs.TBPHS.bit.TBPHS = 0x0000;           // Phase is 0
    EPwm1Regs.TBCTR = 0x0000;                     // Clear counter

    //
    // Setup TBCLK
    //
    EPwm1Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; // Count up
    EPwm1Regs.TBCTL.bit.PHSEN = TB_DISABLE;        // Disable phase loading
    EPwm1Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1; //TB_DIV4;       // Clock ratio to SYSCLKOUT
    EPwm1Regs.TBCTL.bit.CLKDIV = TB_DIV1;    //TB_DIV4;
    EPwm1Regs.TBCTL.bit.SYNCOSEL = TB_CTR_ZERO; //Sync down-stream module
    EPwm1Regs.TBCTL.bit.PRDLD = TB_SHADOW;   //adicionado agora

    EPwm1Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;    // Load registers every ZERO
    EPwm1Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
    EPwm1Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
    EPwm1Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;

    //
    // Setup compare
    //
    EPwm1Regs.CMPA.bit.CMPA = 5208/4; //3000;

    //
    // Set actions
    //
    EPwm1Regs.AQCTLA.bit.CAU = AQ_SET;            // Set PWM1A on Zero
    EPwm1Regs.AQCTLA.bit.CAD = AQ_CLEAR;

    EPwm1Regs.AQCTLB.bit.CAU = AQ_CLEAR;          // Set PWM1A on Zero
    EPwm1Regs.AQCTLB.bit.CAD = AQ_SET;

    //
    // Active Low PWMs - Setup Deadband
    //
    EPwm1Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;
    EPwm1Regs.DBCTL.bit.POLSEL = DB_ACTV_LOC;
    EPwm1Regs.DBCTL.bit.IN_MODE = DBA_ALL;
    EPwm1Regs.DBRED.bit.DBRED = 0; //Rising Edge Dead time
    EPwm1Regs.DBFED.bit.DBFED = 0; //Falling Edge Dead time
    //EPwm6_DB_Direction = DB_UP;

    //
    // Interrupt where we will change the Deadband
    //
    EPwm1Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;    // Select INT on Zero event
    EPwm1Regs.ETSEL.bit.INTEN = 1;               // Enable INT
    EPwm1Regs.ETPS.bit.INTPRD = ET_1ST; //ET_3RD;          // Generate INT on 3rd event

}
void InitEPwm6Example()
{
    //TPRD = T_PWM/(2*TBCLK) = f_CLK/(2*f_PWM) / Change System Clock at F2837xS.SysCtrl.c
    // Check EPWMCLK register

    EPwm6Regs.TBPRD = 5208/2; //6000;                       // Set timer period
    EPwm6Regs.TBPHS.bit.TBPHS = 0x0000;           // Phase is 0
    EPwm6Regs.TBCTR = 0x0000;                     // Clear counter

    //
    // Setup TBCLK
    //
    EPwm6Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; // Count up
    EPwm6Regs.TBCTL.bit.PHSEN = TB_ENABLE;        // Disable phase loading
    EPwm6Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1; //TB_DIV4;       // Clock ratio to SYSCLKOUT
    EPwm6Regs.TBCTL.bit.CLKDIV = TB_DIV1;    //TB_DIV4;
    EPwm6Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN; //adicionado agora
    EPwm6Regs.TBCTL.bit.PRDLD = TB_SHADOW;   //adicionado agora

    EPwm6Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;    // Load registers every ZERO
    EPwm6Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
    EPwm6Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
    EPwm6Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;

    //
    // Setup compare
    //
    EPwm6Regs.CMPA.bit.CMPA = 5208/4; //3000;

    //
    // Set actions
    //
    EPwm6Regs.AQCTLA.bit.CAU = AQ_SET;            // Set PWM1A on Zero
    EPwm6Regs.AQCTLA.bit.CAD = AQ_CLEAR;

    EPwm6Regs.AQCTLB.bit.CAU = AQ_CLEAR;          // Set PWM1A on Zero
    EPwm6Regs.AQCTLB.bit.CAD = AQ_SET;

    //
    // Active Low PWMs - Setup Deadband
    //
    EPwm6Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;
    EPwm6Regs.DBCTL.bit.POLSEL = DB_ACTV_LOC;
    EPwm6Regs.DBCTL.bit.IN_MODE = DBA_ALL;
    EPwm6Regs.DBRED.bit.DBRED = 0; //EPWM6_MIN_DB;
    EPwm6Regs.DBFED.bit.DBFED = 0; //EPWM6_MIN_DB;
    //EPwm6_DB_Direction = DB_UP;

    //
    // Interrupt where we will change the Deadband
    //
    EPwm6Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;    // Select INT on Zero event
    EPwm6Regs.ETSEL.bit.INTEN = 1;               // Enable INT
    EPwm6Regs.ETPS.bit.INTPRD = ET_1ST; //ET_3RD;          // Generate INT on 3rd event
}

//
// InitEPwm7Example - Initialize EPWM7 configuration
//
void InitEPwm7Example()
{
    EPwm7Regs.TBPRD = 5208/2;                       // Set timer period
    EPwm7Regs.TBPHS.bit.TBPHS = 0x0000;           // Phase is 0
    EPwm7Regs.TBCTR = 0x0000;                     // Clear counter

    //
    // Setup TBCLK
    //
    EPwm7Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; // Count up
    EPwm7Regs.TBCTL.bit.PHSEN = TB_ENABLE;        // Disable phase loading
    EPwm7Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;
    EPwm7Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;       // Clock ratio to SYSCLKOUT
    EPwm7Regs.TBCTL.bit.CLKDIV = TB_DIV1;          // Slow just to observe on
    EPwm7Regs.TBCTL.bit.PRDLD = TB_SHADOW;   //adicionado agora

    EPwm7Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;    // Load registers every ZERO
    EPwm7Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
    EPwm7Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
    EPwm7Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;                                             // the scope

    //
    // Setup compare
    //
    EPwm7Regs.CMPA.bit.CMPA = 5208/4;

    //
    // Set actions
    //
    EPwm7Regs.AQCTLA.bit.CAU = AQ_SET;            // Set PWM2A on Zero
    EPwm7Regs.AQCTLA.bit.CAD = AQ_CLEAR;

    EPwm7Regs.AQCTLB.bit.CAU = AQ_CLEAR;          // Set PWM2A on Zero
    EPwm7Regs.AQCTLB.bit.CAD = AQ_SET;

    //
    // Active Low complementary PWMs - setup the deadband
    //
    EPwm7Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;
    EPwm7Regs.DBCTL.bit.POLSEL = DB_ACTV_LOC;
    EPwm7Regs.DBCTL.bit.IN_MODE = DBA_ALL;
    EPwm7Regs.DBRED.bit.DBRED = 0; //EPWM7_MIN_DB;
    EPwm7Regs.DBFED.bit.DBFED = 0; //EPWM7_MIN_DB;
    //EPwm7_DB_Direction = DB_UP;

    //
    // Interrupt where we will modify the deadband
    //
    EPwm7Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;     // Select INT on Zero event
    EPwm7Regs.ETSEL.bit.INTEN = 1;                // Enable INT
    EPwm7Regs.ETPS.bit.INTPRD = ET_1ST;           // Generate INT on 3rd event
}

//
// InitEPwm8Example - Initialize EPWM8 configuration
//
void InitEPwm8Example()
{
    EPwm8Regs.TBPRD = 5208/2;                        // Set timer period
    EPwm8Regs.TBPHS.bit.TBPHS = 0x0000;            // Phase is 0
    EPwm8Regs.TBCTR = 0x0000;                      // Clear counter

    //
    // Setup TBCLK
    //
    EPwm8Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; // Count up
    EPwm8Regs.TBCTL.bit.PHSEN = TB_ENABLE;        // Disable phase loading
    EPwm8Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;       // Clock ratio to SYSCLKOUT
    EPwm8Regs.TBCTL.bit.CLKDIV = TB_DIV1;          // Slow so we can observe on
    EPwm8Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;                                       // the scope
    EPwm8Regs.TBCTL.bit.PRDLD = TB_SHADOW;   //adicionado agora

    EPwm8Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;    // Load registers every ZERO
    EPwm8Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
    EPwm8Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
    EPwm8Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;
    //
    // Setup compare
    //
    EPwm8Regs.CMPA.bit.CMPA = 5208/4;

    //
    // Set actions
    //
    EPwm8Regs.AQCTLA.bit.CAU = AQ_SET;             // Set PWM3A on Zero
    EPwm8Regs.AQCTLA.bit.CAD = AQ_CLEAR;

    EPwm8Regs.AQCTLB.bit.CAU = AQ_CLEAR;           // Set PWM3A on Zero
    EPwm8Regs.AQCTLB.bit.CAD = AQ_SET;

    //
    // Active high complementary PWMs - Setup the deadband
    //
    EPwm8Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;
    EPwm8Regs.DBCTL.bit.POLSEL = DB_ACTV_LOC;
    EPwm8Regs.DBCTL.bit.IN_MODE = DBA_ALL;
    EPwm8Regs.DBRED.bit.DBRED = 0; //EPWM8_MIN_DB;
    EPwm8Regs.DBFED.bit.DBFED = 0; //EPWM8_MIN_DB;
    //EPwm8_DB_Direction = DB_UP;

    //
    // Interrupt where we will change the deadband
    //
    EPwm8Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;      // Select INT on Zero event
    EPwm8Regs.ETSEL.bit.INTEN = 1;                 // Enable INT
    EPwm8Regs.ETPS.bit.INTPRD = ET_1ST;            // Generate INT on 3rd event
}

void ConfigureADC(void)
{
    EALLOW;
    //
    //write configurations
    //
    AdcaRegs.ADCCTL2.bit.PRESCALE = 6; //set ADCCLK divider to /4
    AdcSetMode(ADC_ADCA, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);
    AdcaRegs.ADCCTL1.bit.INTPULSEPOS = 1;
    AdcaRegs.ADCCTL1.bit.ADCPWDNZ = 1;
    DELAY_US(1000);

    EDIS;

}

//TODO setup adc
void SetupADCContinuous()
{
    EALLOW;
    AdcaRegs.ADCSOC0CTL.bit.CHSEL  = 0;  //SOC will convert on channel
    AdcaRegs.ADCSOC0CTL.bit.ACQPS  = 14;    //sample window is acqps + 1 SYSCLK cycles
    AdcaRegs.ADCINTSEL1N2.bit.INT1E = 1; //disable INT1 flag
    //AdcaRegs.ADCINTSEL1N2.bit.INT1CONT = 0;
    AdcaRegs.ADCINTSEL1N2.bit.INT1SEL = 0;  //end of SOC0 will set INT1 flag
    AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; //make sure INT1 flag is cleared
    AdcaRegs.ADCSOC0CTL.bit.TRIGSEL = 15;
    EDIS;
}
//
// End of file
//


