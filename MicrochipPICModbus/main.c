//****************************************************************************
//                          EXEMPLO PIC MODBUS
//****************************************************************************
//Compilador CCS PCWH Ver 4.104

#include <16F877.h>              //include do 16F877
#include <string.h>
#include <16f8x_registradores.h>//MAPEAMENTO DE MEÓRIA DOS REGISTRADORES
                                                    

//****************************************************************************
//                    CONFIGURAÇÕES DO COMPILADOR
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
#define  TX_ENABLE_PIN  port_c,2    //Habilitação de transmissão do MAX485

//PORTAS DO TECLADO
#define  tris_tec    tris_b   //indica qual o port do teclado
#define  port_tec    port_b


//*****************************  SAÍDAS  *************************************
// As definições a seguir são utilizadas para acesso aos pinos do display
// caso o pino RW não seja utilizado, comente a definição lcd_rw
#ifndef lcd_enable
   #define lcd_enable     pin_d5      // pino enable do LCD
   #define lcd_rs         pin_d4      // pino rs do LCD
   //#define lcd_rw      pin_d2      // pino rw do LCD
   #define lcd_d4         pin_d0      // pino de dados d4 do LCD
   #define lcd_d5         pin_d1      // pino de dados d5 do LCD
   #define lcd_d6         pin_d2      // pino de dados d6 do LCD
   #define lcd_d7         pin_d3      // pino de dados d7 do LCD
#endif
//****************************************************************************
//                        CONSTANTES E VARIÁVEIS
//****************************************************************************
#define  debouce     10

short int   estado_porta;

//***************************************************************************
//                               DRIVES
//***************************************************************************
#include "c:\dados\projeto\includes\lcd.h"
#include <teclado_4x4.h>
#include <pic_modbus.c>

//****************************************************************************
//                       Rotinas da interrupcao
//****************************************************************************

/*
//#inline                     //descomentar se for usar o in_line
void trata_serial(){

}
*/

//****************************************************************************
//                           INTERRUPÇÕES
//****************************************************************************
#int_rda
void interrupcao(){
   disable_interrupts(GLOBAL);
   modbus_buffer_rx_len++;
   modbus_rx_buffer[modbus_buffer_rx_len-1] = getc();
   modbus_receive_data = true;
   if (modbus_buffer_rx_len == TAMANHO_MAX_BUFFER){
      modbus_buffer_rx_len = TAMANHO_MAX_BUFFER-1; //Não extrapolar o buffer
   }
   enable_interrupts(GLOBAL);
}


//****************************************************************************
//                             Sub-rotinas
//****************************************************************************
void modbus_implementa_master(){
   lcd_escreve('\f');   //Limpa o lcd
   lcd_escreve('\b');
   lcd_pos_xy(1,1);           //Posiciona o cursor x e
   printf(lcd_escreve, "Valor: %i", modbus_rx_buffer[5]);
}

void modbus_implementa_slave(){
   //Modo Slave
}

void modbus_implementa_dado(){
   if (modbus_trata_dado == true){
      if (modbus_check_crc(modbus_buffer_rx_len-2)==true){ //Checa erro CRC
         //implementa a mensagem Modbus
         if (modbus_mode == MODBUS_MASTER){
            modbus_implementa_master();   //Implementa modo master
         }
         else{
            modbus_implementa_slave();    //Implementa modo slave
         }
      }
      modbus_trata_dado = false;    //Resseta o flag de mensagem recebida
      modbus_buffer_rx_len = 0;     //Resseta a contagem do buffer de recepção
   }
}

trata_tecla(){
   if (key_trata){      
      lcd_escreve('\f');   //Limpa o lcd
      lcd_escreve('\b');
      lcd_pos_xy(1,1);           //Posiciona o cursor x e
      lcd_escreve("Tecla digitada:");
      lcd_pos_xy(1,2);           //Coluna 3 linha 2
      printf(lcd_escreve,"Tecla: %X",new_key);
      if (new_key == 01){
         lcd_escreve('\f');   //Limpa o lcd
         lcd_escreve('\b');
         lcd_pos_xy(1,1);           //Posiciona o cursor x e
         lcd_escreve("MSG Enviada.");
//       Enviando a mensagem de teste Modbus
         modbus_add_byte(0x09);
         modbus_add_byte(PRESET_SINGLE_REGISTER);
         modbus_add_byte(0x00);
         modbus_add_byte(0x07);
         modbus_add_byte(0x00);
         modbus_add_byte(0x05);
         modbus_send_message();
      }
      key_trata = false;
   }
}

//****************************************************************************
//                          Rotina Principal
//****************************************************************************

void main(){
//********************** CONFIGURAÇÕES INICIAIS *****************************
      setup_timer_0(rtcc_internal); //clock interno
      setup_timer_1(t1_internal|t1_div_by_8); 
      setup_wdt(wdt_288ms);
      port_b_pullups(true);   //Ativa os pullups do port B   
      
          //Do pic18 boilerplate
       setup_psp(PSP_DISABLED);
       setup_adc_ports(NO_ANALOGS);
       setup_adc(ADC_OFF);
       setup_spi(SPI_SS_DISABLED);
       setup_timer_2(T2_DISABLED,0,1);

//    Configuração do tris
//    Bit porta: 76543210
      tris_a = 0b00000000;
//    tris_b = 0b00000000;  //Definido no arquivo drive do teclado
      tris_c = 0b10000000;  //TTl to RS485 on-board
      tris_d = 0b00000000;
      tris_e = 0b00000000;
      
      enable_interrupts(global);
      enable_interrupts(int_rda);

//    Inicialização do LCD
      lcd_ini();
      delay_ms(100);
      lcd_escreve('\f');   //Limpa o lcd
      lcd_escreve('\b');

//    Inicialização do Modbus
      modbus_init(); 
      
//******************** INICIALIZAÇÃO DE VARIÁVEIS ***************************
      key_trata = false;
      estado_porta = false;
      modbus_mode = MODBUS_MASTER;
//*************************** TELA INICIAL **********************************
      lcd_pos_xy(1,1);           //Posiciona o cursor x e
      lcd_escreve("PIC Modbus");
      lcd_pos_xy(1,2);           //Coluna 3 linha 2
      lcd_escreve("1- Envia MSG");

//****************************************************************************
//                            INICIO DO LOOP
//****************************************************************************
      while(1){  
      restart_wdt();
      scan_tec();       //Verifica se alguma tecla foi digitada.
      trata_tecla();    //Realiza o tratamento da tecla.      
      modbus_receive_delay();
      modbus_implementa_dado();
      }
//********************************* FIM **************************************
}


















































