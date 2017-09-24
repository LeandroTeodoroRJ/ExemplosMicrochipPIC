//****************************************************************************
//                   EXEMPLO DISPLAYS DE 7 SEGMENTOS
//****************************************************************************
//Compilador CCS PCWH Ver 4.104

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


//****************************************************************************
//                                BIOS
//****************************************************************************
//****************************  ENTRADAS  ************************************


//*****************************  SAÍDAS  *************************************
#define  DISPLAY_BUS    port_b
#define  DISPLAY_A      0
#define  DISPLAY_B      1
#define  DISPLAY_C      2
#define  DISPLAY_D      3
#define  DISPLAY_COM    port_c
#define  PONTO          port_b,7

//****************************************************************************
//                        CONSTANTES E VARIÁVEIS
//****************************************************************************
int   w_temp;
int   status_temp;
long int   count;
unsigned long int   count_timer1;
int   count_disp_control;
int   count_disp;
const int   CONV_DISP[25]={                   
                   0b00111111,
                   0b00000110,
                   0b01011011,
                   0b01001111,
                   0b01100110,
                   0b01101101,
                   0b01111100,
                   0b00000111,
                   0b01111111,
                   0b01101111,
                   0b01110111,
                   0b00111001,
                   0b01011110,
                   0b01111001,
                   0b01110001,
                   0b01111101,
                   0b01110110,
                   0b00011110,
                   0b00111000,
                   0b01010100,
                   0b01011100,
                   0b01110011,
                   0b01010000,
                   0b01111000,
                   0b00011100
                           };
/*
      //Conversão para o display de 7 segmentos
      //      LEDS:  PGFEDCBA        Onde P=ponto
      CONV_DISP[0]=0b00111111; //0
      CONV_DISP[1]=0b00000110; //1
      CONV_DISP[2]=0b01011011; //2
      CONV_DISP[3]=0b01001111; //3
      CONV_DISP[4]=0b01100110; //4
      CONV_DISP[5]=0b01101101; //5
      CONV_DISP[6]=0b01111100; //6
      CONV_DISP[7]=0b00000111; //7
      CONV_DISP[8]=0b01111111; //8
      CONV_DISP[9]=0b01101111; //9
     CONV_DISP[10]=0b01110111; //A
     CONV_DISP[11]=0b00111001; //C
     CONV_DISP[12]=0b01011110; //d
     CONV_DISP[13]=0b01111001; //E
     CONV_DISP[14]=0b01110001; //F
     CONV_DISP[15]=0b01111101; //G
     CONV_DISP[16]=0b01110110; //H
     CONV_DISP[17]=0b00011110; //J
     CONV_DISP[18]=0b00111000; //L
     CONV_DISP[19]=0b01010100; //n
     CONV_DISP[20]=0b01011100; //o
     CONV_DISP[21]=0b01110011; //P
     CONV_DISP[22]=0b01010000; //r
     CONV_DISP[23]=0b01111000; //t
     CONV_DISP[24]=0b00011100; //u
*/


//***************************************************************************
//                               DRIVES
//***************************************************************************


//****************************************************************************
//                       Rotinas da interrupcao
//****************************************************************************
//#inline                     //descomentar se for usar o in_line
void mostra_display(int disp, short int dot, int value);    

void estouro_timer0(){
   if (count == 50){          //Base de aprox 20ms
      mostra_display(count_disp_control, false, count_disp);
      count_disp_control = count_disp_control+1;
      if (count_disp_control > 3) count_disp_control = 0;
      count = 0;
   }
   else{
      count = count+1;
   }
t0if = false;                //Reseta o flag de interrupção do timer0
}

void estouro_timer1(){
   if (count_timer1 == 20){              //Base de aprox 1s.
      count_disp = count_disp+1;
         if (count_disp > 24) count_disp = 0;
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
void mostra_display(int disp, short int dot, int value){    
DISPLAY_BUS = CONV_DISP[value];
if (dot == true) bit_set(PONTO);
if (dot==true){
   bit_set(PONTO);
}
else{
   bit_clear(PONTO);
}
bit_clear(DISPLAY_COM,DISPLAY_A);
bit_clear(DISPLAY_COM,DISPLAY_B);
bit_clear(DISPLAY_COM,DISPLAY_C);
bit_clear(DISPLAY_COM,DISPLAY_D);
bit_set(DISPLAY_COM,disp);
}


//****************************************************************************
//                          Rotina Principal
//****************************************************************************

void main(){
//********************** CONFIGURAÇÕES INICIAIS *****************************
      setup_timer_0(rtcc_internal); //clock interno
      setup_timer_1(t1_internal|t1_div_by_8); 
//      setup_wdt(wdt_288ms);
      setup_ccp1(ccp_off); //Desliga os modos de captura
      setup_ccp2(ccp_off);
      enable_interrupts(global);
      enable_interrupts(int_timer0);
      enable_interrupts(int_timer1);
      port_b_pullups(false);    //sem pull-up

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
      count_disp_control = 0;
      count_disp = 0;
      
//****************************************************************************
//                             INICIO DO LOOP
//****************************************************************************
      while(1){  
      restart_wdt();
      
      }
//********************************* FIM **************************************
}


















































