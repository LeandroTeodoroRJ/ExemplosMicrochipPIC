//****************************************************************************
//                      PIC 18 - ASYNC USART MODE
//****************************************************************************
//Compilador CCS PCWH Ver 4.104

#include <18F4520.h>              //include do 16F628
#include <string.h>
#include <18F4520_registradores.h>//MAPEAMENTO DE ME�RIA DOS REGISTRADORES
                                                    

//****************************************************************************
//                    CONFIGURA��ES DO COMPILADOR
//****************************************************************************

#use fast_io(a)                  //modo de entrada e saida de dados que
#use fast_io(b)                  //o compilador nao configura o tris

#use delay(clock=20000000)        //clock usual
#use rs232(baud=9600, xmit=PIN_C6, rcv=PIN_C7, parity=N, bits=8)
#fuses HS,NOWDT,NOLVP,PUT,NOPROTECT


//****************************************************************************
//                                BIOS
//****************************************************************************
//****************************  ENTRADAS  ************************************


//*****************************  SA�DAS  *************************************
#define  led       port_d,7

//****************************************************************************
//                        CONSTANTES E VARI�VEIS
//****************************************************************************
int8  usart_rx_buffer[1]; //Buffer de recebimento da usart
int1  trata_serial_flag;


//***************************************************************************
//                               DRIVES
//***************************************************************************
#include <usart18.h>



//****************************************************************************
//                       Rotinas da interrupcao
//****************************************************************************
//#inline                     //descomentar se for usar o in_line

recebe_dado_usart(){
  usart_rx_buffer[0] = getc();
  trata_serial_flag = true;
}

//****************************************************************************
//                           INTERRUP��ES
//****************************************************************************
#int_rda
void rda_isr(void) 
{
disable_interrupts(int_rda);
   switch(usart_error()){
   case 2: //ERRO DE FRAME
   //Trata erro
   break;
   case 3: //ERRO DE OVERRUN (BUFFER OVERLOAD)
   //Trata erro
   break;
   default: //Recebeu um novo dado
   recebe_dado_usart();
   }
enable_interrupts(int_rda);
}


//***************************************************************************
//                          FIM DA INTERRUPCAO
//***************************************************************************

//****************************************************************************
//                             Sub-rotinas
//****************************************************************************
trata_dado_serial(){    
   if (usart_rx_buffer[0] == 's'){
      bit_set(led);
   }
trata_serial_flag = false;
}

aguarda_transmissao(){
   while(tx_usart_complete() == true){
   delay_ms(1);
   }
}
//****************************************************************************
//                          Rotina Principal
//****************************************************************************

void main(){
//********************** CONFIGURA��ES INICIAIS *****************************
      setup_timer_0(rtcc_internal); //clock interno
      setup_timer_1(t1_internal|t1_div_by_8); 
      setup_wdt(wdt_off);
      port_b_pullups(false);    //sem pull-up

//    Configura��o do tris
//    Bit porta: 76543210
      tris_a = 0b00000000;
      tris_b = 0b00000000;
      tris_c = 0b10000000;
      tris_d = 0b00000000;
      tris_e = 0b00000000;
      
      enable_interrupts(global);
      enable_interrupts(int_rda);

//******************** INICIALIZA��O DE VARI�VEIS ***************************
      trata_serial_flag = false;
//****************************************************************************
      putc('L');  //Transmite um byte pela usart
      aguarda_transmissao();
      putc('e');  //Transmite um byte pela usart
      aguarda_transmissao();
      putc('a');  //Transmite um byte pela usart
      aguarda_transmissao();
      putc('n');  //Transmite um byte pela usart
      aguarda_transmissao();
      putc('d');  //Transmite um byte pela usart
      aguarda_transmissao();
      putc('r');  //Transmite um byte pela usart
      aguarda_transmissao();
      putc('o');  //Transmite um byte pela usart
      aguarda_transmissao();
      
//****************************************************************************
//                             INICIO DO LOOP
//****************************************************************************
      while(1){  
      restart_wdt();
      if (trata_serial_flag = true) trata_dado_serial();
      }
//********************************* FIM **************************************
}










