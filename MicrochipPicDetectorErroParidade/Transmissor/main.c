//****************************************************************************
//                    MÓDULO RF - PLACA TRANSMISSORA
//****************************************************************************
//Compilador CCS PCWH Ver 4.104

#include <16F877.h>              
#include <string.h>
#include <16f8x_registradores.h>//MAPEAMENTO DE MEÓRIA DOS REGISTRADORES
                                                    

//****************************************************************************
//                    CONFIGURAÇÕES DO COMPILADOR
//****************************************************************************

#use fast_io(a)                  //modo de entrada e saida de dados que
#use fast_io(b)                  //o compilador nao configura o tris

#use delay(clock=20000000)        //clock usual
#fuses HS,NOLVP,NOWDT,PUT,NOPROTECT //


//****************************************************************************
//                                BIOS
//****************************************************************************
//****************************  ENTRADAS  ************************************

//*****************************  SAÍDAS  *************************************


//****************************************************************************
//                        CONSTANTES E VARIÁVEIS
//****************************************************************************
#define  tamanho_int     7
#define  ACK   6
#define  NAK   21

int   w_temp;
int   status_temp;
short int   dado_recebido;    //flag de novo dado recebido
int   count;
int  dado_tx;


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
   usart_config(modo8bits, assincrono, tx_on, usart_off, rx_continuo);
   usart_config(modo8bits, assincrono, tx_on, usart_on, rx_continuo);
   }
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
      
   if (interrupt_rx_usart()==true){  //Foi interrupão da USART?
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
int gera_header(short int paridade){
int header = 0b10100000;   //tokken mais alto é fixo 1010
   if (paridade){
   bit_set(header,0);      //header bit 0 é o bit de paridade
   }
return header;
}

trata_dado_serial(){
   if (dado_recebido==true){    //Recebeu um novo dado?
      if (usart_rx_dado == ACK){
//    NOP
      }
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
//********************** CONFIGURAÇÕES INICIAIS *****************************
      setup_timer_0(rtcc_internal); //clock interno, prescaler 1:2
      setup_wdt(wdt_288ms);
            
      //Configurações da USART
      usart_baud_rate(low_speed, 255);
      usart_config(modo8bits, assincrono, tx_on, usart_on, rx_continuo);

      bit_set(option_reg,7);    //sem pull-up

//    Configuração do tris
//    Bit porta: 76543210
      tris_a = 0b00000000;
      tris_b = 0b00000000;
      tris_c = 0b00000000;
      tris_d = 0b00000000;
      
      enable_interrupts(GLOBAL); //Habilita interrupção de recepção da USART
      usart_int(tx_int_off, rx_int_off, int_9bit_off);

//******************** INICIALIZAÇÃO DE VARIÁVEIS ***************************
      
//****************************************************************************
      dado_tx = 0b00000111;      //dec. 7
      usart_tx(gera_header(gera_paridade(dado_tx)));  //Envia o header
      usart_tx(dado_tx);                              //Envia o dado

//****************************************************************************
//                             INICIO DO LOOP
//****************************************************************************
      while(1){  
      restart_wdt();
      trata_dado_serial();
      }
//********************************* FIM **************************************
}


















































