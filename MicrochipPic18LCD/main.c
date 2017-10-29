//****************************************************************************
//                            EXEMPLOS COM LCD
//****************************************************************************
//Compilador CCS PCWH Ver 4.104

#include <18F4520.h>              //include do 16F628
#include <string.h>
#include <18F4520_registradores.h>//MAPEAMENTO DE MEÓRIA DOS REGISTRADORES
                                                    

//****************************************************************************
//                    CONFIGURAÇÕES DO COMPILADOR
//****************************************************************************

#use fast_io(a)                  //modo de entrada e saida de dados que
#use fast_io(b)                  //o compilador nao configura o tris

#use delay(clock=4000000)        //clock usual
#fuses HS,NOWDT,NOLVP,PUT,NOPROTECT


//****************************************************************************
//                                BIOS
//****************************************************************************
//****************************  ENTRADAS  ************************************
#define  botao       PIN_B0

//*****************************  SAÍDAS  *************************************

// As definições a seguir são utilizadas para acesso aos pinos do display
#define LCD_DATA_PORT getenv("SFR:PORTb")  //Define o Port B para o LCD
//Definição dos pinos do LCD 
#define LCD_ENABLE_PIN  PIN_b5         
#define LCD_RS_PIN      PIN_b4         
#define LCD_RW_PIN      PIN_b6         
#define LCD_DATA4       PIN_b0        
#define LCD_DATA5       PIN_b1        
#define LCD_DATA6       PIN_b2        
#define LCD_DATA7       PIN_b3        

//****************************************************************************
//                        CONSTANTES E VARIÁVEIS
//****************************************************************************
int8  w_temp;
int8   status_temp;
int8  var1[10]={"Ola Mundo!"};
int8   count;
int8   var3;
signed int8 var4;
float var5;
int8  var6;

//***************************************************************************
//                               DRIVES
//***************************************************************************
#include <lcd.c>



//****************************************************************************
//                       Rotinas da int8errupcao
//****************************************************************************
//#inline                     //descomentar se for usar o in_line



//****************************************************************************
//                           int8ERRUPÇÕES
//****************************************************************************
#int_global
void int8errupcao(){
restart_wdt();
disable_interrupts(global);
#asm
movwf w_temp
swapf status,w
movwf status_temp
#endasm


//***************************************************************************
//                          FIM DA int8ERRUPCAO
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
      setup_timer_0(rtcc_internal); //clock int8erno, prescaler 1:2
      setup_timer_1(t1_internal|t1_div_by_8); 
      setup_wdt(wdt_off);
      disable_interrupts(global);
      port_b_pullups(false);    //sem pull-up

//    Configuração do tris
//    Bit porta: 76543210
      tris_a = 0b00000000;
      tris_b = 0b00000000;
      tris_c = 0b00000000;
      tris_d = 0b00000000;
      tris_e = 0b00000000;
      
//    Inicialização do LCD
      lcd_init();
      delay_ms(100);
      
//******************** INICIALIZAÇÃO DE VARIÁVEIS ***************************

      
//****************************************************************************
//                             INICIO DO LOOP
//****************************************************************************
      while(1){  
      restart_wdt();
      //EXEMPLO1: Escrita simples de um dado da ROM
      lcd_gotoxy(1,1);           //Posiciona o cursor x e
      lcd_putc("EXEMPLO 1");
      lcd_gotoxy(3,2);           //Coluna 3 linha 2
      lcd_putc("Texto");
      delay_ms(5000);

      //EXEMPLO2: Imprimindo uma variável string
      lcd_gotoxy(1,1);           
      lcd_putc("EXEMPLO 2");
      lcd_gotoxy(3,2);
      count=0;
      while(count<sizeof(var1)){
         lcd_putc(var1[count]);
         count++;
      }
      delay_ms(5000);

     //EXEMPLO3: Imprime um int8eiro sem sinal com dois algarismos
      lcd_gotoxy(1,1);
      lcd_putc("EXEMPLO 3");
      lcd_gotoxy(1,2);
      var3=57;
      printf(lcd_putc,"Numero: %2u",var3);
      delay_ms(5000);

      //EXEMPLO4: Imprime um int8eiro com sinal com três algarismos
      lcd_gotoxy(1,1);
      lcd_putc("EXEMPLO 4");
      lcd_gotoxy(1,2);
      var4=-14;
      printf(lcd_putc,"Numero: %d",var4);
      delay_ms(5000);

      //EXEMPLO5: Concatenando
      lcd_gotoxy(1,1);
      lcd_putc("EXEMPLO 5");
      lcd_gotoxy(1,2);
      var4=-14;
      printf(lcd_putc,"Numeros: %d e %d",var4, var3);
      delay_ms(5000);

      //EXEMPLO6: Imprime um número float
      lcd_gotoxy(1,1);
      lcd_putc("EXEMPLO 6");
      lcd_gotoxy(1,2);
      var5=43e-3;
      printf(lcd_putc,"Numero: %1.3f",var5); //1 casa antes 3 depois da vírgula
      delay_ms(5000);

     //EXEMPLO7: Imprime um número float em formato exponencial
      lcd_gotoxy(1,1);
      lcd_putc("EXEMPLO 7");
      lcd_gotoxy(1,2);
      var5=43e-3;
      printf(lcd_putc,"Numero: %2.2e",var5); //2 casas antes 2 depois da vírgula
      delay_ms(5000);
    
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


















































