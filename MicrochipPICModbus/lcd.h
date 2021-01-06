
/************************************************************************
        MOD_LCD.C - Biblioteca de manipula��o de m�dulo LCD
                        Autor: F�bio Pereira                                                

FUN��ES:
         void lcd_ini(void) ->  rotina de inicializa��o do display
         void lcd_pos_xy( byte x, byte y) -> posiciona o cursor 
         void lcd_escreve( char c) ->  envia caractere para o display ou
                                       uma string ex. lcd_escreve("R.Johnson");
                                       argumentos ex. lcd_escreve('\f');
                                       \f -> apaga o display e retorna a primeira posi��o da primeira linha
                                       \r -> passa para linha de baixo "enter"   
                                       \n -> sem uso  
ATUALIZADO: 14.08.10                                              
/************************************************************************/



#define lcd_type 2           // 0=5x7, 1=5x10, 2=2 linhas
#define lcd_seg_lin 0x40    // Endere�o da segunda linha na RAM do LCD

// a constante abaixo define a seq��ncia de inicializa��o do m�dulo LCD
byte CONST INI_LCD[4] = {0x20 | (lcd_type << 2), 0xf, 1, 6};

/* FUN��O DESABILITADA DEVIDO AO DESUSO

byte lcd_le_byte()
// l� um byte do LCD (somente com pino RW)
{
   byte dado;
   // configura os pinos de dados como entradas
   input(lcd_d4);
   input(lcd_d5);
   input(lcd_d6);
   input(lcd_d7);
   // se o pino rw for utilizado, coloca em 1
   #ifdef lcd_rw
      output_high(lcd_rw);
   #endif
   output_high(lcd_enable); // habilita display
   dado = 0;   // zera a vari�vel de leitura
   // l� os quatro bits mais significativos
   if (input(lcd_d7)) bit_set(dado,7);
   if (input(lcd_d6)) bit_set(dado,6);
   if (input(lcd_d5)) bit_set(dado,5);
   if (input(lcd_d4)) bit_set(dado,4);
   // d� um pulso na linha enable
   output_low(lcd_enable);
   output_high(lcd_enable);
   // l� os quatro bits menos significativos
   if (input(lcd_d7)) bit_set(dado,3);
   if (input(lcd_d6)) bit_set(dado,2);
   if (input(lcd_d5)) bit_set(dado,1);
   if (input(lcd_d4)) bit_set(dado,0);
   output_low(lcd_enable);   // desabilita o display
   return dado;   // retorna o byte lido
}
*/


void lcd_envia_nibble( byte dado )
// envia um dado de quatro bits para o display
{
   // coloca os quatro bits nas saidas
   output_bit(lcd_d4,bit_test(dado,0));
   output_bit(lcd_d5,bit_test(dado,1));
   output_bit(lcd_d6,bit_test(dado,2));
   output_bit(lcd_d7,bit_test(dado,3));
   // d� um pulso na linha enable
   output_high(lcd_enable);
   output_low(lcd_enable);
}


void lcd_envia_byte( boolean endereco, byte dado )
{
   // coloca a linha rs em 0
   output_low(lcd_rs);
   // aguarda o display ficar desocupado
   //while ( bit_test(lcd_le_byte(),7) ) ;
   // configura a linha rs dependendo do modo selecionado
   output_bit(lcd_rs,endereco);
   delay_us(100);   // aguarda 100 us
   // caso a linha rw esteja definida, coloca em 0
   #ifdef lcd_rw
      output_low(lcd_rw);
   #endif
   // desativa linha enable
   output_low(lcd_enable);
   // envia a primeira parte do byte
   lcd_envia_nibble(dado >> 4);
   // envia a segunda parte do byte
   lcd_envia_nibble(dado & 0x0f);
}


void lcd_ini()
// rotina de inicializa��o do display
{
   byte conta;
   output_low(lcd_d4);
   output_low(lcd_d5);
   output_low(lcd_d6);
   output_low(lcd_d7);
   output_low(lcd_rs);
   #ifdef lcd_rw
      output_high(lcd_rw);
   #endif
   output_low(lcd_enable);
   delay_ms(15);
   // envia uma seq��ncia de 3 vezes 0x03
   // e depois 0x02 para configurar o m�dulo
   // para modo de 4 bits
   for(conta=1;conta<=3;++conta)
   {
      lcd_envia_nibble(3);
      delay_ms(5);
   }
   lcd_envia_nibble(2);
   // envia string de inicializa��o do display
   for(conta=0;conta<=3;++conta) lcd_envia_byte(0,INI_LCD[conta]);
}

void lcd_pos_xy( byte x, byte y)
{
   byte endereco;
   if(y!=1)
      endereco = lcd_seg_lin;
   else
      endereco = 0;
   endereco += x-1;
   lcd_envia_byte(0,0x80|endereco);
}

void lcd_escreve( char c)
// envia caractere para o display
{
   switch (c)
   {
     case '\f'    :   lcd_envia_byte(0,1);
              delay_ms(2);
            break;
     case '\n'   :
     case '\r'    :   lcd_pos_xy(1,2);
              break;
     case '\b'    :   lcd_envia_byte(0,0x10);
              break;
     default   :   lcd_envia_byte(1,c);
              break;
   }
}


/* FUN��O DESABILITADA DEVIDO AO DESUSO

char lcd_le( byte x, byte y)
// le caractere do display
{
   char valor;
   // seleciona a posi��o do caractere
   lcd_pos_xy(x,y);
   // ativa rs
   output_high(lcd_rs);
   // l� o caractere
   valor = lcd_le_byte();
   // desativa rs
   output_low(lcd_rs);
   // retorna o valor do caractere
   return valor;
}
*/
