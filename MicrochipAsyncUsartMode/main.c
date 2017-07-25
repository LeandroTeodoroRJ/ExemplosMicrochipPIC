//****************************************************************************
//                          ESTUDO DA PORTA SERIAL
//****************************************************************************
//Compilador CCS PCWH Ver 4.104

#include <16F628.h>              //include do 16F628
#include <string.h>
#include <16f628_registradores.h>//MAPEAMENTO DE MEÓRIA DOS REGISTRADORES
                                                    

//****************************************************************************
//                    CONFIGURAÇÕES DO COMPILADOR
//****************************************************************************

#use fast_io(a)                  //modo de entrada e saida de dados que
#use fast_io(b)                  //o compilador nao configura o tris

#use delay(clock=4000000)        //clock usual
#fuses HS,NOWDT,NOLVP,PUT,NOPROTECT //

//#rom 0x2100={0b00010010,0x00}    //programa os primeiros dois endereços da eeprom
                                 

//****************************************************************************
//                                BIOS
//****************************************************************************
//****************************  ENTRADAS  ************************************

//*****************************  SAÍDAS  *************************************
#define  led       port_a,0

//****************************************************************************
//                        CONSTANTES E VARIÁVEIS
//****************************************************************************
int   w_temp;
int   status_temp;
short int   dado_recebido;    //flag de novo dado recebido

//***************************************************************************
//                               DRIVES
//***************************************************************************
#include <usart.h>


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
trata_dado_serial(){    
   if (dado_recebido==true){    //Recebeu um novo dado?
   usart_tx(usart_rx_dado);     //Devolve o dado recebido
   usart_rx_dado=65;
      if (usart_rx_dado==65){   //
      bit_set(led);  //Acende o led se recebeu o decimal 65
      }
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
      usart_config(modo8bits, assincrono, tx_on, usart_on, rx_continuo);

      bit_set(option_reg,7);    //sem pull-up

//    Configuração do tris
//    Bit porta: 76543210
      tris_a = 0b00000000;
      tris_b = 0b00000010;
      
      enable_interrupts(GLOBAL); //Habilita interrupção de recepção da USART
      usart_int(tx_int_off, rx_int_on, int_9bit_off);
//******************** INICIALIZAÇÃO DE VARIÁVEIS ***************************
      bit_clear(led);
      usart_rx_dado=0;
      
//****************************************************************************
//                             INICIO DO LOOP
//****************************************************************************
      while(1){  
      restart_wdt();
      trata_dado_serial();
      }
//********************************* FIM **************************************
}


















































