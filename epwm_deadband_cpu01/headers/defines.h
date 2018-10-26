/*
 * defines.h
 *
 *  Created on: 4 de out de 2018
 *      Author: Bruno
 */

#ifndef HEADERS_DEFINES_H_
#define HEADERS_DEFINES_H_

//TODO constantes numéricas
#define PIX1        (float)3.1415926535897932384626433832795
#define PIX2        (float)3.1415926535897932384626433832795*2
#define PIDiv2      (float)(3.1415926535897932384626433832795/2)
#define cte1        (float) 0.577350269189626   // sqrt(3)/3
#define cte2        (float) 1.154700538379251   // 2*sqrt(3)/3
#define sqrt3       (float) 1.732050807568877   // sqrt(3)
#define cte3        (float) 0.866025403784439   // sqrt(3)/2

//TODO constantes PLL
#define B0_PLL  61.8034088702226
#define B1_PLL  -61.7218092663225
#define CI_PLL  376.991118430775
#define TS_PLL  5e-005

//TODO constantes controle modo Retificador
/*
#define C0  1.66000000e-04
#define Hica    -2.59937108e-01
#define HicaInv 3.847084426283607
#define Cib1z   -4.61321049e+03
#define Cib0z   -2.59937108e-01
#define CiT 5.63462494e-05
#define Hvcc    5.10713250e-02
#define HvccInv 0.0019580459289043
#define Cvb1z   1.38366755e+01
#define Cvb0z   5.10713250e-02
#define CvT 3.69101126e-03
#define Dd  4.49073120e-01
#define Dq  -1.39914527e-02
#define Hi  1.00000000e+00
#define Hv  1.00000000e+00
#define Id  3.71134810e+00
#define Ipf 3.71134810e+00
#define Iq  0.00000000e+00
#define Lin 4.00000000e-03
#define P0  1.00000000e+03
#define Pa  3.33333333e+02
#define R0  1.60000000e+02
#define Rin 4.84000000e+01
#define Rse 5.00000000e-02
#define V0  4.00000000e+02
#define Vf  1.27017059e+02
#define Vll 2.20000000e+02
#define Vpf 1.79629248e+02
#define fc  4.80000000e+03
#define fr  6.00000000e+01
#define fs  10.000000e+03
#define kdq 3.76991118e-03
#define wc  3.76991118e+02
#define wr  3.76991118e+02
#define Rout    100
#define Cout    180e-6
*/

//TODO constantes modo inversor

#define C0  1.66000000e-04
#define Cia0z   1.00000000e+00
#define Cia1z   -1.00000000e+00
#define Cib1z   4.12368880e-01
#define Cib0z   -6.65910738e-01
#define CiT 1.06321657e-04
#define Cvb1z   -4.11973688e-02
#define Cvb0z   4.21952812e-02
#define CvT 2.08917758e-03
#define Dd  5.52729802e-01
#define Dq  -6.28318531e-02
#define Hi  1.00000000e+00
#define Hv  1.00000000e+00
#define Idpk    4.16666667e+00
#define Ieff    2.40562612e+00
#define Ipf 3.40206909e+00
#define Ipkf    3.40206909e+00
#define Iq  0.00000000e+00
#define Lin 4.00000000e-03
#define P0  2.50000000e+02
#define Pa  8.33333333e+01
#define R0  4.00000000e+01
#define Rin 1.44000000e+01
#define Rse 5.00000000e-02
#define V0  1.00000000e+02
#define Vf  3.46410162e+01
#define Vll 6.00000000e+01
#define Vpf 4.89897949e+01
#define dI  5.10310363e-01
#define dV    5.00000000e+00
#define deltaI  1.50000000e-01
#define deltaV  5.00000000e-02
#define fa  2.00000000e+04
#define Ta  5.00000000e-05
#define fc  2.50000000e+03
#define fr  6.00000000e+01
#define fs  1.00000000e+04
#define Ts  1.00000000e-04
#define kdq 1.50796447e-02
#define wc  3.76991118e+02
#define wr  3.76991118e+02


//TODO constantes de ganho dos sensores e ADC
#define Hvca 0.004232013940752
#define HvcaInv 2.362941176470588e+02

#define Hvcc 7.59373517e-03
#define HvccInv 131.6875

#define Hica 0.325600000000000
#define HicaInv 3.071253071253071

#define GADCinv 7.324218750000000e-04 // 3/4096
#define ADCoffset 2048

//TODO constantes para manipulação da CPU
/*
#define FCLK        200e6
#define FS_CONV     1.00000e+04
#define TIMERPWM   (unsigned int)(FCLK/(2*FS_CONV))

#define PHASE_PWM1  0
#define PHASE_PWM2  0
#define PHASE_PWM3  0
#define PHASE_PWM4  0
#define PHASE_PWM5  0   //(unsigned int)((float)TIMER_RET*0.6666666666666) // in falling edge PHSDIR=0
#define PHASE_PWM6  0   //(unsigned int)((float)TIMER_RET*0.6666666666666) // in rising edge  PHSDIR=1

#define DEAD_TIMEs  0.000001    // entrar com o tempo morto desejado em segundos
#define DEAD_TIME   (unsigned int) ((float)DEAD_TIMEs*FCLK)
*/
#endif /* HEADERS_DEFINES_H_ */
