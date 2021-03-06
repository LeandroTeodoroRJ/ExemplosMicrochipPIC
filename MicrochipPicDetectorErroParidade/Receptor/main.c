//****************************************************************************
//                           PLACA RECEPTORA
//****************************************************************************
//Compilador CCS PCWH Ver 4.104

#include <16F877.h>              
#include <string.h>
#include <16f8x_registradores.h>//MAPEAMENTO DE ME�RIA DOS REGISTRADORES
                                                    

//****************************************************************************
//                    CONFIGURA��ES DO COMPILADOR
//****************************************************************************

#use fast_io(a)                  //modo de entrada e saida de dados que
#use fast_io(b)                  //o compilador nao configura o tris

#use delay(clock=20000000)        //clock usual
#fuses HS,NOLVP,NOWDT,PUT,NOPROTECT //


//****************************************************************************
//                                BIOS
//****************************************************************************
//****************************  ENTRADAS  ************************************

//*****************************  SA�DAS  *************************************
// As defini��es a seguir s�o utilizadas para acesso aos pinos do display
// caso o pino RW n�o seja utilizado, comente a defini��o lcd_rw
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
//                        CONSTANTES E VARI�VEIS
//****************************************************************************
#define  tamanho_int 7
#define  header_mask 0b10100000
#define  ACK   6
#define  NAK   21

int   w_temp;
int   status_temp;
short int   dado_recebido;    //flag de novo dado recebido
int   count;
int   dado_rx[2];
int   count_dado_rx;
int   count_timer1;
short int time_out_flag;
//***************************************************************************
//                               DRIVES
//***************************************************************************
#include "C:\DADOS\projeto\INCLUDES\usart.h"
#include "c:\dados\projeto\includes\lcd.h"


//****************************************************************************
//                             Sub-rotinas
//****************************************************************************
tela_time_out(){
lcd_escreve('\f');   //Limpa o lcd
lcd_escreve('\b');
lcd_pos_xy(1,1);
lcd_escreve("Time-out...");
time_out_flag = false;
}

tela1(){
lcd_escreve('\f');   //Limpa o lcd
lcd_escreve('\b');
lcd_pos_xy(1,1);
lcd_escreve("Aguardando...");
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

trata_dado_serial(){
   if (dado_recebido==true){    //Recebeu um novo dado?
      if (gera_paridade(dado_rx[1]) == bit_test(dado_rx[0],0)){
         lcd_escreve('\f');   //Limpa o lcd
         lcd_escreve('\b');
         lcd_pos_xy(1,1);
         printf(lcd_escreve, "Rx: %u", dado_rx[1]);
         lcd_pos_xy(1,2);
         printf(lcd_escreve, "Paridade: %u", bit_test(dado_rx[0],0));
      }
      else{
         lcd_escreve('\f');   //Limpa o lcd
         lcd_escreve('\b');
         lcd_pos_xy(1,1);
         lcd_escreve("Erro Paridade.");
      }
   dado_recebido=false;
   }
}

reseta_usart(){
count_dado_rx = 0;
usart_config(modo8bits, assincrono, tx_on, usart_off, rx_continuo);
usart_config(modo8bits, assincrono, tx_on, usart_on, rx_continuo);
}

short int check_paridade(int dado){
short int paridade;
paridade = bit_test(dado,0);     //Bit de paridade
   if (gera_paridade(dado_rx[1]) == paridade){
   return true;
   }
   else{
   return false;
   }
}


//****************************************************************************
//                       Rotinas da interrupcao
//****************************************************************************
//#inline                     //descomentar se for usar o in_line

estouro_timer1(){
   if (count_timer1 == 4){              //Usart time-out
         if (count_dado_rx != 0){
         reseta_usart();
         time_out_flag = true;
         }
      count_timer1 = 0;
   }
   else{
      count_timer1 = count_timer1+1; 
   }
tmr1if = false;           //Reseta o flag de interrup��o do timer1
}

recebe_dado_usart(){
int header;
   if (usart_rx()==novo_dado){  //Novo dado recebido

         if (count_dado_rx != 0){                     //Aqui recebe um dado depois do header
         dado_rx[count_dado_rx] = usart_rx_dado;
         count_dado_rx = 0;         //Reseta o contador de bytes do protocolo
         dado_recebido = true;
         }
   
         header = usart_rx_dado;         //Aqui recebe o header
         bit_clear(header,0);            //Reseta o bit 0      
         if ((header == header_mask)&&(count_dado_rx == 0)){   //S� entra se for detectado o header e o contador estiver no come�o
         dado_rx[0] = usart_rx_dado;
         count_dado_rx = count_dado_rx + 1;
         }
//   usart_tx(ACK);
   }
   else{
   //Se for erro reseta a USART
   reseta_usart();
//   usart_tx(NAK);
   }
}

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
      
   if (interrupt_rx_usart()==true){  //Foi interrup�o da USART?
   recebe_dado_usart();
   }

   if (pir1,tmr1if == true) estouro_timer1(); //Trata interrup��o timer1
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
//                          Rotina Principal
//****************************************************************************

void main(){
//********************** CONFIGURA��ES INICIAIS *****************************
      setup_timer_0(rtcc_internal); //clock interno
      setup_timer_1(t1_internal|t1_div_by_8);//clock interno, prescaler 2
      setup_ccp1(ccp_off); //Desliga os modos de captura
      setup_ccp2(ccp_off);

      setup_wdt(wdt_288ms);
            
      //Configura��es da USART
      usart_baud_rate(low_speed, 255);
      usart_config(modo8bits, assincrono, tx_on, usart_on, rx_continuo);
      enable_interrupts(int_timer1);

      bit_set(option_reg,7);    //sem pull-up

//    Configura��o do tris
//    Bit porta: 76543210
      tris_a = 0b00000000;
      tris_b = 0b00000000;
      tris_c = 0b10000000;
      tris_d = 0b00000000;
      
      enable_interrupts(GLOBAL); //Habilita interrup��o de recep��o da USART
      usart_int(tx_int_off, rx_int_on, int_9bit_off);
     
      
//    Inicializa��o do LCD
      lcd_ini();
      delay_ms(100);
      lcd_escreve('\f');   //Limpa o lcd
      lcd_escreve('\b');
      tela1();
      
//******************** INICIALIZA��O DE VARI�VEIS ***************************
      count_dado_rx = 0;
      count = 0;
      count_timer1 = 0;
      time_out_flag = false;

//****************************************************************************
//                             INICIO DO LOOP
//****************************************************************************
      while(1){  
      restart_wdt();
      trata_dado_serial();
      if (time_out_flag == true) tela_time_out();
      }
//********************************* FIM **************************************
}


















































