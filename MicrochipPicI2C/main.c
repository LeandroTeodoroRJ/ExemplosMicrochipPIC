//****************************************************************************
//              EXEMPLO INTERFACE I2C COM MEM�RIA EEPROM SERIAL
//****************************************************************************
//Compilador CCS PCWH Ver 4.104
//****************************************************************************
/*
OBS: 
O primeiro byte enviado para a mem�ria 24C02 segue a seguinte l�gica:
BITS: |7|6|5|4|3|2|1|0|
      |1|0|1|0|R|S|T|U|
      Onde:
      R,S,T s�o os n�veis l�gicos de A2,A1 e A0 respectivamente
      U -> 0 para escrita e 1 para leitura
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
#use i2c (master, sda=PIN_C4, scl=PIN_C3,fast=100000)   //Habilita o i2c com velocidade de 100KHz

#use delay(clock=20000000)        //clock usual
#fuses HS,NOWDT,NOLVP,PUT,NOPROTECT

//****************************************************************************
//                                BIOS
//****************************************************************************
//****************************  ENTRADAS  ************************************

//*****************************  SA�DAS  *************************************
#define  saida       port_d

//****************************************************************************
//                        CONSTANTES E VARI�VEIS
//****************************************************************************
#define  DADO                 0b11110000
#define  EEPROM_ADRESS_WRITE  0b10100000
#define  EEPROM_ADRESS_READ   0b10100001
#define  ENDERECO_INTERNO     0x05

int   w_temp;
int   status_temp;

//***************************************************************************
//                               DRIVES
//***************************************************************************
//#include <usart.h>



//****************************************************************************
//                       Rotinas da interrupcao
//****************************************************************************
//#inline                     //descomentar se for usar o in_line



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
      setup_wdt(wdt_288ms);
      disable_interrupts(global);
      port_b_pullups(false);    //sem pull-up

//    Configura��o do tris
//    Bit porta: 76543210
      tris_a = 0b00000000;
      tris_b = 0b00000000;
      tris_c = 0b00000000;
      tris_d = 0b00000000;
      tris_e = 0b00000000;
      
      
//******************** INICIALIZA��O DE VARI�VEIS ***************************
      saida = 0;
      
//****************************************************************************
//                             INICIO DO LOOP
//****************************************************************************
      while(1){  
      restart_wdt();
      
      //Escrita 24C02
      i2c_start();               //Coloca o barramento em condi��o de inicializar a comunica��o
      i2c_write(EEPROM_ADRESS_WRITE);       //Endere�o do dispositivo (escrita)
      i2c_write(ENDERECO_INTERNO);       //Endere�o interno da mem�ria para aloca��o do dado
      i2c_write(DADO);           //Grava o dado na mem�ria
      i2c_stop();                //Finaliza a comunica��o com o barramento
      delay_ms(11);
      
      //Leitura 24C02
      i2c_start();               //Coloca o barramento em condi��o de inicializar a comunica��o
      i2c_write(EEPROM_ADRESS_WRITE);     //Device select
      i2c_write(ENDERECO_INTERNO);     //Endere�o interno da mem�ria para leitura do dado
      i2c_start();               //Faz um restart
      i2c_write(EEPROM_ADRESS_READ);     //Agora como leitura
      saida = i2c_read(0);       //L� o valor do bus I2C e mostra o valor nos leds
      i2c_stop();                //Finaliza a comunica��o com o barramento
      
      }
//********************************* FIM **************************************
}


















































