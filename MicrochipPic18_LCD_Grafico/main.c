//****************************************************************************
//                   DISPLAY GR�FICO 128x64 HDM64GS12
//****************************************************************************
//Compilador CCS PCWH Ver 4.104

#include <18F4520.h>              //include do 18F4520
#include <string.h>
#include <18F4520_registradores.h>//MAPEAMENTO DE ME�RIA DOS REGISTRADORES
                                                    

//****************************************************************************
//                    CONFIGURA��ES DO COMPILADOR
//****************************************************************************

#use fast_io(a)                  //modo de entrada e saida de dados que
#use fast_io(b)                  //o compilador nao configura o tris

#use delay(clock=20000000)        //clock usual
#fuses HS,NOWDT,NOLVP,PUT,NOPROTECT

//#rom 0x2100={0b00010010,0x00}    //programa os primeiros dois endere�os da eeprom
                                 

//****************************************************************************
//                                BIOS
//****************************************************************************
//****************************  ENTRADAS  ************************************

//*****************************  SA�DAS  *************************************
//Pinos LCD Gr�fico
//Obs: O port D � fixo
#define GLCD_CS1     PIN_C0   // Chip Selection 1
#define GLCD_CS2     PIN_C1   // Chip Selection 2
#define GLCD_DI      PIN_B4   // Data or Instruction input
#define GLCD_RW      PIN_B1   // Read/Write
#define GLCD_E       PIN_B2   // Enable
#define GLCD_RST     PIN_B3   // Reset

//****************************************************************************
//                        CONSTANTES E VARI�VEIS
//****************************************************************************

int   w_temp;
int   status_temp;
int   count;

long int *ponteiro_texto;
char texto[6]={"Hello"};   //Colocar sempre 1 posi��o a mais.
int num;
float num2;
char texto_2[15];
//***************************************************************************
//                            BIBLIOTECAS
//***************************************************************************
#include <string.h>

//***************************************************************************
//                               DRIVES
//***************************************************************************
#include <LIB_HDM64GS12.c>
#include <LIB_GRAPHICS.C>
#include <BMPImages.C>

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
void limpa_e_delay(){    
   delay_ms(3000);     
   glcd_fillScreen(0); //limpa display inteiro
}


//****************************************************************************
//                          Rotina Principal
//****************************************************************************

void main(){
//********************** CONFIGURA��ES INICIAIS *****************************
      setup_adc_ports(NO_ANALOGS);
      setup_adc(ADC_OFF);
      setup_psp(PSP_DISABLED);
      setup_spi(SPI_SS_DISABLED);
      setup_timer_0(rtcc_internal); //clock interno
      setup_timer_1(t1_internal|t1_div_by_8); 
      setup_timer_1(T1_DISABLED);
      setup_timer_2(T2_DISABLED,0,1);
      setup_timer_3(T3_DISABLED|T3_DIV_BY_1);     
      setup_wdt(wdt_off);
      disable_interrupts(global);
      port_b_pullups(false);    //sem pull-up
      

//    Configura��o do tris
//    Bit porta: 76543210
      tris_a = 0b00000000;
      tris_b = 0b00000000;
      tris_c = 0b00000000;
      tris_d = 0b00000000;
      tris_e = 0b00000000;
      
      glcd_init(ON);   //inicializa o display
      glcd_fillScreen(0); //limpa display inteiro

//******************** INICIALIZA��O DE VARI�VEIS ***************************
      
//****************************************************************************
//                             INICIO DO LOOP
//****************************************************************************
      while(1){  
      restart_wdt();
      glcd_imagem(1);     //Carrega imagem 1
      delay_ms(3000);     //Delay 3s
      glcd_fillScreen(0); //limpa display inteiro
      glcd_imagem(2);     //Carrega imagem 2
      limpa_e_delay();
 
   //Desenhos de figuras geom�tricas
   //Nota: O pixel de coordenada (0,0) est� no canto superior esquerdo
   //Resolu��o do LCD 128x64

    //Linha
    //glcd_line(x1, y1, x2, y2, ON)
      glcd_line(0, 0, 127, 63, ON);
      limpa_e_delay();
      
     //Ret�ngulo 
     //glcd_rect(x1, y1, x2, y2, fill, color) 
     glcd_rect(10, 10, 100, 50, OFF, ON);
     limpa_e_delay();
     glcd_rect(40, 20, 70, 30, ON, ON);
     limpa_e_delay();
    
     //C�rculo
     //glcd_circle(x, y, radius, fill, color)
     glcd_circle(64,32, 10, OFF, ON);
     limpa_e_delay();
     glcd_circle(64,32, 20, ON, ON);
     limpa_e_delay();
 
     //Texto
     //Os caracteres s�o 5 por 7 pixels e size � um multiplicador inteiro
     // x e y s�o as coordenadas do ponto superior do texto
     // textptr � o ponteiro para o array do texto.
     //glcd_text57(x, y, textptr, size, color)
     ponteiro_texto = &texto;
     glcd_text57(10, 10, ponteiro_texto, 1, ON);
     limpa_e_delay();
     glcd_text57(10, 10, ponteiro_texto, 3, ON);
     limpa_e_delay();
     
     //Imprimindo um inteiro
     num = 23;
     sprintf(texto_2, "Valor:%i", num);
     ponteiro_texto = &texto_2;
     glcd_text57(10, 10, ponteiro_texto, 1, ON);
     limpa_e_delay();
     
     //Imprimindo um float
     num2 = 35e-3;
     sprintf(texto_2, "Valor:%1.4f", num2);
     ponteiro_texto = &texto_2;
     glcd_text57(10, 10, ponteiro_texto, 1, ON);
     limpa_e_delay();

     }
//********************************* FIM **************************************
}


















































