//****************************************************************************
//                    M�DULO RF - PLACA TRANSMISSORA
//****************************************************************************
//Compilador CCS PCWH Ver 4.104

#include <16F877.h>              
#include <string.h>
#include <16f8x_registradores.h>//MAPEAMENTO DE ME�RIA DOS REGISTRADORES
                                                    

//****************************************************************************
//                    CONFIGURA��ES DO COMPILADOR
//****************************************************************************

#use fast_io(a)                  //modo de entrada e saida de dados que
#use fast_io(b)                  //o compilador nao configura o tris

#use delay(clock=20000000)        //clock usual
#fuses HS,NOLVP,NOWDT,PUT,NOPROTECT //


//****************************************************************************
//                                BIOS
//****************************************************************************
//****************************  ENTRADAS  ************************************

//*****************************  SA�DAS  *************************************


//****************************************************************************
//                        CONSTANTES E VARI�VEIS
//****************************************************************************
#define  tamanho_int     7

int   w_temp;
int   status_temp;
short int   dado_recebido;    //flag de novo dado recebido
int   count;
char  dado_tx;


//***************************************************************************
//                               DRIVES
//***************************************************************************
#include "C:\DADOS\projeto\INCLUDES\usart.h"

//****************************************************************************
//                       Rotinas da interrupcao
//****************************************************************************
//#inline                     //descomentar se for usar o in_line
recebe_dado_usart(){
   if (usart_rx()==novo_dado){  //Novo dado recebido
   dado_recebido=true;
   }
   else{
   //Se for erro reseta a USART
   usart_config(modo9bits, assincrono, tx_on, usart_off, rx_continuo);
   usart_config(modo9bits, assincrono, tx_on, usart_on, rx_continuo);
   }
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
      
   if (interrupt_rx_usart()==true){  //Foi interrup�o da USART?
   recebe_dado_usart();
   }

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
trata_dado_serial(){    
   if (dado_recebido==true){    //Recebeu um novo dado?
//   usart_tx(usart_rx_dado);     //Devolve o dado recebido
   }
}

short int gera_paridade(int dado){
int count = tamanho_int;
short int paridade = bit_test(dado,tamanho_int);
   while(count > 0){
   count = count-1;
   paridade = paridade^(bit_test(dado,count));
   }
return paridade;
}

//****************************************************************************
//                          Rotina Principal
//****************************************************************************

void main(){
//********************** CONFIGURA��ES INICIAIS *****************************
      setup_timer_0(rtcc_internal); //clock interno, prescaler 1:2
      setup_wdt(wdt_288ms);
            
      //Configura��es da USART
      usart_baud_rate(high_speed, 25);
      usart_config(modo9bits, assincrono, tx_on, usart_on, rx_continuo);

      bit_set(option_reg,7);    //sem pull-up

//    Configura��o do tris
//    Bit porta: 76543210
      tris_a = 0b00000000;
      tris_b = 0b00000000;
      tris_c = 0b00000000;
      tris_d = 0b00000000;
      
      enable_interrupts(GLOBAL); //Habilita interrup��o de recep��o da USART
      usart_int(tx_int_off, rx_int_on, int_9bit_on);

//******************** INICIALIZA��O DE VARI�VEIS ***************************
      
//****************************************************************************
      //dado_tx = "T";
      dado_tx = 0b11101101;
      usart_tx_9bit(gera_paridade(dado_tx),dado_tx);

//****************************************************************************
//                             INICIO DO LOOP
//****************************************************************************
      while(1){  
      restart_wdt();
      trata_dado_serial();
      }
//********************************* FIM **************************************
}


















































