//****************************************************************************
//                           EXEMPLO COM LCD 20x4 
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
#fuses HS,NOWDT,NOLVP,PUT,NOPROTECT

//#rom 0x2100={0b00010010,0x00}    //programa os primeiros dois endereços da eeprom
                                 

//****************************************************************************
//                                BIOS
//****************************************************************************
//****************************  ENTRADAS  ************************************


//*****************************  SAÍDAS  *************************************

// As definições a seguir são utilizadas para acesso aos pinos do display
#define LCD_DATA_PORT getenv("SFR:PORTb")  //Define o Port B para o LCD
//Definição dos pinos do LCD 
#define LCD_ENABLE_PIN  PIN_b2         
#define LCD_RS_PIN      PIN_b0         
#define LCD_RW_PIN      PIN_b1         
#define LCD_DATA4       PIN_b4        
#define LCD_DATA5       PIN_b5        
#define LCD_DATA6       PIN_b6        
#define LCD_DATA7       PIN_b7        


//****************************************************************************
//                        CONSTANTES E VARIÁVEIS
//****************************************************************************
int   w_temp;
int   status_temp;

//***************************************************************************
//                               DRIVES
//***************************************************************************
#include <lcd.c>

//****************************************************************************
//                       Rotinas da interrupcao
//****************************************************************************
//#inline                     //descomentar se for usar o in_line

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
      setup_timer_0(rtcc_internal); //clock interno, prescaler 1:2
      setup_timer_1(t1_internal|t1_div_by_8); 
      setup_wdt(wdt_288ms);
      disable_interrupts(global);
      bit_set(option_reg,7);    //sem pull-up

//    Configuração do tris
//    Bit porta: 76543210
      tris_a = 0b00000000;
      tris_b = 0b00000000;
      
//    Inicialização do LCD
      lcd_init();
      delay_ms(100);
 
      
//******************** INICIALIZAÇÃO DE VARIÁVEIS ***************************

      
//****************************************************************************
//                             INICIO DO LOOP
//****************************************************************************
      while(1){  
      restart_wdt();
      
      //EXEMPLO: Escrita simples de um dado da ROM
      printf(lcd_putc,"\f");     //Limpa o LCD
      lcd_gotoxy(1,1);           //Posiciona o cursor (linha,coluna)
      printf (lcd_putc,"Linha 1");    
      lcd_gotoxy(1,2);           //Coluna 1 linha 2
      printf (lcd_putc,"Linha 2");    
      lcd_gotoxy(21,1);           //Coluna 1 linha 3
      printf (lcd_putc,"Linha 3");    
      lcd_gotoxy(21,2);           //Coluna 1 linha 4
      printf (lcd_putc,"Linha 4");    

      delay_ms(5000);

      }
//********************************* FIM **************************************
}


















































