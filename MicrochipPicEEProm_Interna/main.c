//****************************************************************************
//                    EXEMPLO USO DA EEPROM INTERNA
//****************************************************************************
//Compilador CCS PCWH Ver 4.104

#include <16F877.h>              //include do 16F628
#include <string.h>
#include <16f8x_registradores.h> //MAPEAMENTO DE ME�RIA DOS REGISTRADORES
                                                    
//****************************************************************************
//                    CONFIGURA��ES DO COMPILADOR
//****************************************************************************

#use fast_io(a)                  //modo de entrada e saida de dados que
#use fast_io(b)                  //o compilador nao configura o tris

#use delay(clock=20000000)        //clock usual
#fuses HS,NOWDT,NOLVP,PUT,NOPROTECT

#rom 0x2100={'P','I','C',' '}    //programa os primeiros endere�os da eeprom
                                 
//****************************************************************************
//                                BIOS
//****************************************************************************
//****************************  ENTRADAS  ************************************


//*****************************  SA�DAS  *************************************

// As defini��es a seguir s�o utilizadas para acesso aos pinos do display
// caso o pino RW n�o seja utilizado, comente a defini��o lcd_rw
#ifndef lcd_enable
   #define lcd_enable     pin_b5      // pino enable do LCD
   #define lcd_rs         pin_b4      // pino rs do LCD
   //#define lcd_rw      pin_b2      // pino rw do LCD
   #define lcd_d4         pin_b0      // pino de dados d4 do LCD
   #define lcd_d5         pin_b1      // pino de dados d5 do LCD
   #define lcd_d6         pin_b2      // pino de dados d6 do LCD
   #define lcd_d7         pin_b3      // pino de dados d7 do LCD
#endif

//****************************************************************************
//                        CONSTANTES E VARI�VEIS
//****************************************************************************

int   w_temp;
int   status_temp;
int   count;
char  texto[16];

//***************************************************************************
//                               DRIVES
//***************************************************************************
#include "c:\dados\projeto\includes\lcd.h"

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
      
//    Inicializa��o do LCD
      lcd_ini();
      delay_ms(100);
      lcd_escreve('\f');   //Limpa o lcd
      lcd_escreve('\b');
//******************** INICIALIZA��O DE VARI�VEIS ***************************
      count = 0;
      texto = "16F877A";
      
//****************************************************************************
//                             INICIO DO LOOP
//****************************************************************************
      while(1){  
      restart_wdt();
     
         while(count<sizeof(texto)){       //Faz at� o tamanho do array
            write_eeprom((count+4), texto[count]);  //Grava na EEPROM (endere�o, dado)
            delay_ms(2);
            count++;
         }
     
      //Imprimindo uma vari�vel string
      lcd_pos_xy(1,1);           
      lcd_escreve("EEPROM INTERNA");
      lcd_pos_xy(1,2);
      count=0;
         while(count<sizeof(texto)){         //Faz at� o tamanho do array
            lcd_escreve(read_eeprom(count));  //L� a EEPROM interna, par�metro: endere�o
            count++;
         }
      delay_ms(5000);
      lcd_escreve('\f');   //Limpa o lcd
      lcd_escreve('\b');
      }
//********************************* FIM **************************************
}


















































