// Estagio 2
//
/* Replace "dll.h" with the name of your header */
#include <math.h>
#include <stdio.h>
#include "F28x_Project.h"
#include "structs.h"  // Changed to local load
#include "defines.h"

extern double t, delt;
extern double *in, *out;

extern void sincosf(float x, float *sinx, float *cosx);   // Calcula seno e cosseno de x
extern void initvars(void);

extern struct RETIFICADOR conv;
extern struct CONTROLE ctrl;
extern struct TRIGONOMETRICO trig;
extern struct PLL pll;
extern struct PI Cvz,CizID,CizIQ;


void stage2(void){ // Modo inversor
    // Geração do wt
    trig.w = 377;
    trig.wt+=trig.w*TS_PLL;

    if(trig.wt>PIX1) trig.wt = trig.wt - PIX2;

//*******************************************************//
// Geração dos sinais de referência Nominal
//*******************************************************//
    sincosf(trig.wt, &(trig.senoA), &(trig.coseA));

    conv.id = (trig.senoA+trig.coseA*cte1)*conv.ia+cte2*trig.coseA*conv.ic;
    conv.iq = (trig.coseA-trig.senoA*cte1)*conv.ia-cte2*trig.senoA*conv.ic;


// Controle de corrente id
   conv.idref = -3.5; // Corrente de referência
   CizID.e0 = conv.idref-conv.id;  // Erro atual
   CizID.u0= (CizID.e0*Cib0z+CizID.e1*Cib1z+CizID.u1); // Calcula saída atual
   CizID.u1=CizID.u0; // Atualiza saída anterior
   CizID.e1=CizID.e0; // Atualiza erro anterior

// Controle de corrente iq

   conv.iqref = 0; // Corrente de referência
   CizIQ.e0 = conv.iqref-conv.iq;  // Erro atual
   CizIQ.u0= (CizIQ.e0*Cib0z+CizIQ.e1*Cib1z+CizIQ.u1); // Calcula saída atual
   CizIQ.u1=CizIQ.u0; // Atualiza saída anterior
   CizIQ.e1=CizIQ.e0; // Atualiza erro anterior

// Desacoplamento
   conv.dd = CizID.u0 + kdq*conv.iq;
   conv.dq = CizIQ.u0 - kdq*conv.id;


    // Modulação
    conv.da = trig.senoA*conv.dd + trig.coseA*conv.dq;
    conv.db = -(cte3*trig.coseA + trig.senoA*0.5)*conv.dd + (-trig.coseA*0.5 + cte3*trig.senoA)*conv.dq;
    conv.dc = (cte3*trig.coseA - trig.senoA*0.5)*conv.dd - (trig.coseA*0.5 + cte3*trig.senoA)*conv.dq;

    //ctrl.ENABLE = 1;

    return;
}
