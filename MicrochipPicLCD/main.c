//****************************************************************************
//                            EXEMPLOS COM LCD
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
#define  botao       PIN_B0

//*****************************  SAÍDAS  *************************************

// As definições a seguir são utilizadas para acesso aos pinos do display
// caso o pino RW não seja utilizado, comente a definição lcd_rw
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
//                        CONSTANTES E VARIÁVEIS
//****************************************************************************
#define  debouce     10
#define alarme_off_flag alarme_status,1

int   w_temp;
int   status_temp;
char  var1[10]={"Ola Mundo!"};
int   count;
int   var3;
signed int var4;
float var5;
char  var6;

//***************************************************************************
//                               DRIVES
//***************************************************************************
#include "c:\dados\projeto\includes\lcd.h"



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
      
      //EXEMPLO1: Escrita simples de um dado da ROM
      lcd_pos_xy(1,1);           //Posiciona o cursor x e
      lcd_escreve("EXEMPLO 1");
      lcd_pos_xy(3,2);           //Coluna 3 linha 2
      lcd_escreve("Texto");
      delay_ms(5000);
      lcd_escreve('\f');   //Limpa o lcd
      lcd_escreve('\b');

      //EXEMPLO2: Imprimindo uma variável string
      lcd_pos_xy(1,1);           
      lcd_escreve("EXEMPLO 2");
      lcd_pos_xy(3,2);
      count=0;
      while(count<sizeof(var1)){
         lcd_escreve(var1[count]);
         count++;
      }
      delay_ms(5000);
      lcd_escreve('\f');   //Limpa o lcd
      lcd_escreve('\b');

     //EXEMPLO3: Imprime um inteiro sem sinal com dois algarismos
      lcd_pos_xy(1,1);
      lcd_escreve("EXEMPLO 3");
      lcd_pos_xy(1,2);
      var3=57;
      printf(lcd_escreve,"Numero: %2u",var3);
      delay_ms(5000);
      lcd_escreve('\f');   //Limpa o lcd
      lcd_escreve('\b');

      //EXEMPLO4: Imprime um inteiro com sinal com três algarismos
      lcd_pos_xy(1,1);
      lcd_escreve("EXEMPLO 4");
      lcd_pos_xy(1,2);
      var4=-14;
      printf(lcd_escreve,"Numero: %d",var4);
      delay_ms(5000);
      lcd_escreve('\f');   //Limpa o lcd
      lcd_escreve('\b');

      //EXEMPLO5: Concatenando
      lcd_pos_xy(1,1);
      lcd_escreve("EXEMPLO 5");
      lcd_pos_xy(1,2);
      var4=-14;
      printf(lcd_escreve,"Numeros: %d e %d",var4, var3);
      delay_ms(5000);
      lcd_escreve('\f');   //Limpa o lcd
      lcd_escreve('\b');

      //EXEMPLO6: Imprime um número float
      lcd_pos_xy(1,1);
      lcd_escreve("EXEMPLO 6");
      lcd_pos_xy(1,2);
      var5=43e-3;
      printf(lcd_escreve,"Numero: %1.3f",var5); //1 casa antes 3 depois da vírgula
      delay_ms(5000);
      lcd_escreve('\f');   //Limpa o lcd
      lcd_escreve('\b');

     //EXEMPLO7: Imprime um número float em formato exponencial
      lcd_pos_xy(1,1);
      lcd_escreve("EXEMPLO 7");
      lcd_pos_xy(1,2);
      var5=43e-3;
      printf(lcd_escreve,"Numero: %2.2e",var5); //2 casas antes 2 depois da vírgula
      delay_ms(5000);
      lcd_escreve('\f');   //Limpa o lcd
      lcd_escreve('\b');
    
       /*
      EXEMPLO DE OUTROS FORMATOS:
      %s:   string
      %x:   hexadecimal com letras minúsculas
      %X:   hexadecimal com letras maiúsculas
      %L*:  Formato longo(16 ou 32 bits),
            sendo * qualquer umas das letras 
            referentes a números já tratadas
      */

      }
//********************************* FIM **************************************
}


















































