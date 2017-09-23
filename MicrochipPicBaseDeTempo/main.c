//****************************************************************************
//                       BASE DE TEMPO COM TIMERS
//****************************************************************************
//Compilador CCS PCWH Ver 4.104
//****************************************************************************
/*
Obs:
1) O bit de interrupção t0if deve ser apagado pelo usuário.
2) Se o prescaler estiver configurado para uso com watchdog, o fator
de divisão para o timer0 será 1.
3) O timer1 é um contador de 16bits com prescaler de 1, 2, 4 ou 8.
4) O timer1 é ligado ou desligado pelo bit tmr1on (t1con bit 0).
5) Possui possibilidade de entrada externa de clock com frequência máxima
de 200Khz podendo ser sincronizado com o clock do mCU ou não, sendo recomen-
dado o modo sincrozinado.
6) O flag de interrupção tmr1if(pir bit 0) deve ser apagado pelo usuário.
7) Quando utilizado o timer1 com o módulo CCP deve-se ter cuidado, pois algumas
funções do CCP podem resetar o timer1.

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
#include <16f8x_registradores.h>//MAPEAMENTO DE MEÓRIA DOS REGISTRADORES
                                                    

//****************************************************************************
//                    CONFIGURAÇÕES DO COMPILADOR
//****************************************************************************

#use fast_io(a)                  //modo de entrada e saida de dados que
#use fast_io(b)                  //o compilador nao configura o tris

#use delay(clock=20000000)        //clock usual
#fuses HS,NOWDT,NOLVP,PUT,NOPROTECT

//#rom 0x2100={0b00010010,0x00}    //programa os primeiros dois endereços da eeprom
                                 

//****************************************************************************
//                                BIOS
//****************************************************************************
//****************************  ENTRADAS  ************************************


//*****************************  SAÍDAS  *************************************
#define  sinal_curto       port_a,0
#define  sinal_longo       port_a,1

//****************************************************************************
//                        CONSTANTES E VARIÁVEIS
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
      if (sinal_curto_flag == true){   //Inverte a saída
         bit_clear(sinal_curto);
         sinal_curto_flag = false;
      }
      else{
         bit_set(sinal_curto);
         sinal_curto_flag = true;
      }
//      t0if = false;                //Reseta o flag de interrupção do timer0
      count = 0;
   }
   else{
      count = count+1; 
   }
t0if = false;                //Reseta o flag de interrupção do timer0
}

estouro_timer1(){
   if (count_timer1 == 1){              //Base de aprox 200ms.
      if (sinal_longo_flag == true){    //Inverte a saída
         bit_clear(sinal_longo);
         sinal_longo_flag = false;
      }
      else{
         bit_set(sinal_longo);
         sinal_longo_flag = true;
      }
//      tmr1if = false;           //Reseta o flag de interrupção do timer1
      count_timer1 = 0;
   }
   else{
      count_timer1 = count_timer1+1; 
   }
tmr1if = false;           //Reseta o flag de interrupção do timer1
}


//****************************************************************************
//                           INTERRUPÇÕES
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
if (intcon,t0if == true) estouro_timer0(); //Trata interrupção timer0
if (pir1,tmr1if == true) estouro_timer1(); //Trata interrupção timer1

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
//********************** CONFIGURAÇÕES INICIAIS *****************************
      setup_timer_0(rtcc_internal); //clock interno
      setup_timer_1(t1_internal|t1_div_by_8);//clock interno, prescaler 2
      setup_ccp1(ccp_off); //Desliga os modos de captura
      setup_ccp2(ccp_off);
//      setup_wdt(wdt_288ms);
      enable_interrupts(global);
      enable_interrupts(int_timer0);
      enable_interrupts(int_timer1);

      bit_set(option_reg,7);    //sem pull-up

//    Configuração do tris
//    Bit porta: 76543210
      tris_a = 0b00000000;
      tris_b = 0b00000000;
      tris_c = 0b00000000;
      tris_d = 0b00000000;
      tris_e = 0b00000000;


//******************** INICIALIZAÇÃO DE VARIÁVEIS ***************************
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


















































