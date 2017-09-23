//****************************************************************************
//                       BASE DE TEMPO COM TIMERS
//****************************************************************************
//Compilador CCS PCWH Ver 4.104
//****************************************************************************
/*
Obs:
1) O bit de interrup��o t0if deve ser apagado pelo usu�rio.
2) Se o prescaler estiver configurado para uso com watchdog, o fator
de divis�o para o timer0 ser� 1.
3) O timer1 � um contador de 16bits com prescaler de 1, 2, 4 ou 8.
4) O timer1 � ligado ou desligado pelo bit tmr1on (t1con bit 0).
5) Possui possibilidade de entrada externa de clock com frequ�ncia m�xima
de 200Khz podendo ser sincronizado com o clock do mCU ou n�o, sendo recomen-
dado o modo sincrozinado.
6) O flag de interrup��o tmr1if(pir bit 0) deve ser apagado pelo usu�rio.
7) Quando utilizado o timer1 com o m�dulo CCP deve-se ter cuidado, pois algumas
fun��es do CCP podem resetar o timer1.

Clock = 20e6Hz
Ciclo de maq = 20e6Hz/4 = 5e6 => 200ns
Timer 0:
Estouro = TMR0 x prescaler x ciclo de maq
        = 256 x 1 x 200ns
        = 51us

Timer 1:
Estouro = TMR1 x prescaler x ciclo de maq
        = 65536 x 2 x 200ns
        = 26ms

*/
//****************************************************************************

#include <16F877.h>              //include do 16F628
#include <string.h>
#include <16f8x_registradores.h>//MAPEAMENTO DE ME�RIA DOS REGISTRADORES
                                                    

//****************************************************************************
//                    CONFIGURA��ES DO COMPILADOR
//****************************************************************************

#use fast_io(a)                  //modo de entrada e saida de dados que
#use fast_io(b)                  //o compilador nao configura o tris

#use delay(clock=20000000)        //clock usual
#fuses HS,NOWDT,NOLVP,PUT,NOPROTECT

//#rom 0x2100={0b00010010,0x00}    //programa os primeiros dois endere�os da eeprom
                                 

//****************************************************************************
//                                BIOS
//****************************************************************************
//****************************  ENTRADAS  ************************************


//*****************************  SA�DAS  *************************************
#define  sinal_curto       port_a,0
#define  sinal_longo       port_a,1

//****************************************************************************
//                        CONSTANTES E VARI�VEIS
//****************************************************************************
int   w_temp;
int   status_temp;
int   count;
int   count_timer1;
short int   sinal_curto_flag;
short int   sinal_longo_flag;

//***************************************************************************
//                               DRIVES
//***************************************************************************


//****************************************************************************
//                       Rotinas da interrupcao
//****************************************************************************
//#inline                     //descomentar se for usar o in_line
estouro_timer0(){
   if (count == 20){          //Base de aprox 1ms
      if (sinal_curto_flag == true){   //Inverte a sa�da
         bit_clear(sinal_curto);
         sinal_curto_flag = false;
      }
      else{
         bit_set(sinal_curto);
         sinal_curto_flag = true;
      }
//      t0if = false;                //Reseta o flag de interrup��o do timer0
      count = 0;
   }
   else{
      count = count+1; 
   }
t0if = false;                //Reseta o flag de interrup��o do timer0
}

estouro_timer1(){
   if (count_timer1 == 1){              //Base de aprox 200ms.
      if (sinal_longo_flag == true){    //Inverte a sa�da
         bit_clear(sinal_longo);
         sinal_longo_flag = false;
      }
      else{
         bit_set(sinal_longo);
         sinal_longo_flag = true;
      }
//      tmr1if = false;           //Reseta o flag de interrup��o do timer1
      count_timer1 = 0;
   }
   else{
      count_timer1 = count_timer1+1; 
   }
tmr1if = false;           //Reseta o flag de interrup��o do timer1
}


//****************************************************************************
//                           INTERRUP��ES
//****************************************************************************
#int_global
void interrupcao(){
restart_wdt();
disable_interrupts(global);
#asm
movwf w_temp
swapf status,w
movwf status_temp
#endasm
if (intcon,t0if == true) estouro_timer0(); //Trata interrup��o timer0
if (pir1,tmr1if == true) estouro_timer1(); //Trata interrup��o timer1

//***************************************************************************
//                          FIM DA INTERRUPCAO
//***************************************************************************
enable_interrupts(global);
#asm
swapf status_temp,w
movwf status
swapf w_temp,f
swapf w_temp,w
#endasm
}

//****************************************************************************
//                             Sub-rotinas
//****************************************************************************



//****************************************************************************
//                          Rotina Principal
//****************************************************************************

void main(){
//********************** CONFIGURA��ES INICIAIS *****************************
      setup_timer_0(rtcc_internal); //clock interno
      setup_timer_1(t1_internal|t1_div_by_8);//clock interno, prescaler 2
      setup_ccp1(ccp_off); //Desliga os modos de captura
      setup_ccp2(ccp_off);
//      setup_wdt(wdt_288ms);
      enable_interrupts(global);
      enable_interrupts(int_timer0);
      enable_interrupts(int_timer1);

      bit_set(option_reg,7);    //sem pull-up

//    Configura��o do tris
//    Bit porta: 76543210
      tris_a = 0b00000000;
      tris_b = 0b00000000;
      tris_c = 0b00000000;
      tris_d = 0b00000000;
      tris_e = 0b00000000;


//******************** INICIALIZA��O DE VARI�VEIS ***************************
      count = 0;
      count_timer1 = 0;
      
//****************************************************************************
//                             INICIO DO LOOP
//****************************************************************************
      while(1){  
      restart_wdt();
      }
//********************************* FIM **************************************
}


















































