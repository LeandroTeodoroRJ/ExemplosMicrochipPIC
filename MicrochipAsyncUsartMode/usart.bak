//************************************************************************
//             DRIVE PARA O HARDWARE DA USART INTERNA
//                            ATZ:23.09.11
//                           AUTOR: LEANDRO
//************************************************************************

//************************************************************************
//                      CONFIGURA O BAUD RATE
//************************************************************************
//Vide: PIC Técnicas Avançacas pág.321
//EXEMPLO DE CHAMADA: usart_baud_rate(high_speed, 25);

#define high_speed  1
#define low_speed   0

usart_baud_rate(short int vel, int baud_rate){
brgh=vel;
spbrg=baud_rate;
}

//************************************************************************
//                   CONFIGURA INTERRUPÇÃO DA USART
//************************************************************************
//PARA ATIVAÇÃO DA INTERRUPÇÃO NECESSITA DA ATIVAÇÃO DO BIT INTCON -> GIE=1
//EXEMPLO DE CHAMADA: usart_int(tx_int_off, rx_int_off, int_9bit_off);
//O BIT ADDEN HABILITA A INTERRUPÇÃO PELO RECEBIMENTO DO 9ºBIT


#define tx_int_on 1
#define tx_int_off 0
#define rx_int_on 1
#define rx_int_off 0
#define int_9bit_on 1
#define int_9bit_off 0

usart_int(short int tx_int, short int rx_int, short int int_9bit){
rcie=rx_int;
txie=tx_int;
adden=int_9bit;
}


//*************************************************************************
//                        CONFIGURA A USART
//*************************************************************************
//OBS.: Detecção de endereços (9bit) somente disponível no modo assincrono
//EXEMPLO DE CHAMADA DE FUNÇÃO
//usart_config(modo8bits, assincrono, tx_on, usart_on, rx_continuo);


#define modo8bits  0
#define modo9bits  1  //habilita recepção de endereço
#define tx_on  1  //tx habilitado
#define tx_off 0
#define sincrono 1
#define assincrono 0
#define usart_on  1
#define usart_off 0
#define rx_continuo  1
#define rx_unica     0


usart_config(short int bits, short int syc, short int tx_liga,
short int usart_liga, short int rx_config){
tx9=bits;
rx9=bits;
txen=tx_liga;
sync=syc;
spen=usart_liga;
cren=rx_config;
ferr=0;
oerr=0;
}

//***************************************************************************
//             TRANSMITE UM BYTE PELA USART (MODO 8 BITS)
//***************************************************************************
usart_tx(int dado){
txreg=dado;
}

//***************************************************************************
//             RECEBE UM BYTE PELA USART (MODO 8 BITS)
//***************************************************************************
//A FUNÇÃO RETORNA:
//    0 SE A USART NÃO RECEBEU UM NOVO DADO
//    1 SE A USART RECEBEU UM NOVO DADO E CARREGA EM usart_rx_dado
//    2 PARA ERRO DE FRAME
//    3 PARA ERRO DE OVERRUN (BUFFER OVERLOAD)

#define  novo_dado   0x01
int usart_rx_dado;

int usart_rx(){
   if (ferr==1){
   return (0x02); //erro de frame (stop bit não recebido ou fora de hora)
   ferr=0;
   }
   if (oerr==1){
   return (0x03); //erro de overrun (três caracteres foram recebidos pela usart sem ser lidos pela CPU)
   oerr=0;
   }
   if (rcif==0) return (0x00);
   if (rcif==1){
   usart_rx_dado=rcreg;
   return (0x01);
   }
}

//***************************************************************************
//          TRANSMITE UM BYTE PELA USART (MODO 9 BITS)
//***************************************************************************
//EXEMPLO DE CHAMADA:
//usart_tx_9bit(adress, 0x0F);
//usart_tx_9bit(data, 0x0c);

#define adress 1
#define data 0

usart_tx_9bit(short int bit_adress, int dado){
tx9d=bit_adress;
txreg=dado;
}

//***************************************************************************
//             RECEBE UM BYTE PELA USART (MODO 9 BITS)
//***************************************************************************
//A FUNÇÃO RETORNA:
//    0 SE A USART NÃO RECEBEU UM NOVO DADO
//    1 SE A USART RECEBEU UM NOVO ENDEREÇO E CARREGA EM usart_rx_dado
//    2 SE A USART RECEBEU UM NOVO DADO E CARREGA EM usart_rx_dado
//    3 PARA ERRO DE FRAME
//    4 PARA ERRO DE OVERRUN (BUFFER OVERLOAD)

int usart_rx_9bit(){
   if (rcif==1){
   usart_rx_dado=rcreg;
   }
   if (ferr==1){
   return (0x03); //erro de frame (stop bit não recebido ou fora de hora)
   ferr=0;
   }
   if (oerr==1){
   return (0x04); //erro de overrun (três caracteres foram recebidos pela usart sem ser lidos pela CPU)
   oerr=0;
   }
   if ((rcif==1)&(rx9d==1)) return (0x01); //recebido um endereço
   if (rcif==1) return (0x02);

}

//**************************************************************************
//                      DETECÇÃO DE INTERRUPÇÃO
//**************************************************************************
short int interrupt_rx_usart(){
   return(rcif);
}

//***************************************************************************
//                            FIM DO INCLUDE
//***************************************************************************






