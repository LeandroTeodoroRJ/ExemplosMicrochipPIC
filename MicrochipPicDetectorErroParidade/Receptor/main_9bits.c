//****************************************************************************
//                        MÓDULO RF - PLACA RECEPTORA
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
// As definições a seguir são utilizadas para acesso aos pinos do display
// caso o pino RW não seja utilizado, comente a definição lcd_rw
#ifndef lcd_enable
   #define lcd_enable     pin_b1      // pino enable do LCD
   #define lcd_rs         pin_b0      // pino rs do LCD
   //#define lcd_rw      pin_e2      // pino rw do LCD
   #define lcd_d4         pin_b2      // pino de dados d4 do LCD
   #define lcd_d5         pin_b3      // pino de dados d5 do LCD
   #define lcd_d6         pin_b4      // pino de dados d6 do LCD
   #define lcd_d7         pin_b5      // pino de dados d7 do LCD
#endif
//****************************************************************************
//                        CONSTANTES E VARIÁVEIS
//****************************************************************************
#define  tamanho_int 7

int   w_temp;
int   status_temp;
short int   dado_recebido;    //flag de novo dado recebido
int   count;
short int   paridade; 

//***************************************************************************
//                               DRIVES
//***************************************************************************
#include "C:\DADOS\projeto\INCLUDES\usart.h"
#include "c:\dados\projeto\includes\lcd.h"

//****************************************************************************
//                       Rotinas da interrupcao
//****************************************************************************
//#inline                     //descomentar se for usar o in_line
recebe_dado_usart(){
   if ((usart_rx_9bit()==adress)||(usart_rx()==data)){  //Novo dado recebido
   dado_recebido=true;
      if(usart_rx_9bit()==adress){
      paridade = true;
      }
      else{
      paridade = false;
      }
   }
   else{
   //Se for erro reseta a USART
   usart_config(modo9bits, assincrono, tx_on, usart_off, rx_continuo);
   usart_config(modo9bits, assincrono, tx_on, usart_on, rx_continuo);
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
short int gera_paridade(int dado){
int count = tamanho_int;
short int paridade = bit_test(dado,tamanho_int);
   while(count > 0){
   count = count-1;
   paridade = paridade^(bit_test(dado,count));
   }
return paridade;
}

trata_dado_serial(){
   if (dado_recebido==true){    //Recebeu um novo dado?

      lcd_escreve('\f');   //Limpa o lcd
      lcd_escreve('\b');
      lcd_pos_xy(1,1);
      printf(lcd_escreve, "Rx: %u", usart_rx_dado);
      lcd_pos_xy(1,2);
      printf(lcd_escreve, "Paridade: %u", paridade);

/*
   if (gera_paridade(usart_rx_dado)==paridade){
      lcd_escreve('\f');   //Limpa o lcd
      lcd_escreve('\b');
      lcd_pos_xy(1,1);
      printf(lcd_escreve, "Rx: %c", usart_rx_dado);
   }
   else{
      lcd_escreve('\f');   //Limpa o lcd
      lcd_escreve('\b');
      lcd_pos_xy(1,1);
      lcd_escreve("Erro Paridade");
   }
*/

   dado_recebido=false;
   }
}


//****************************************************************************
//                          Rotina Principal
//****************************************************************************

void main(){
//********************** CONFIGURAÇÕES INICIAIS *****************************
      setup_timer_0(rtcc_internal); //clock interno, prescaler 1:2
      setup_wdt(wdt_288ms);
            
      //Configurações da USART
      usart_baud_rate(high_speed, 25);
      usart_config(modo9bits, assincrono, tx_on, usart_on, rx_continuo);


      bit_set(option_reg,7);    //sem pull-up

//    Configuração do tris
//    Bit porta: 76543210
      tris_a = 0b00000000;
      tris_b = 0b00000000;
      tris_c = 0b10000000;
      tris_d = 0b00000000;
      
      enable_interrupts(GLOBAL); //Habilita interrupção de recepção da USART
      usart_int(tx_int_off, rx_int_on, int_9bit_off);
      
//    Inicialização do LCD
      lcd_ini();
      delay_ms(100);
      lcd_escreve('\f');   //Limpa o lcd
      lcd_escreve('\b');
//******************** INICIALIZAÇÃO DE VARIÁVEIS ***************************
      
//****************************************************************************
//                             INICIO DO LOOP
//****************************************************************************
      while(1){  
      restart_wdt();
      trata_dado_serial();
         
      }
//********************************* FIM **************************************
}


















































