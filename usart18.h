//************************************************************************
//             DRIVE PARA O HARDWARE DA USART INTERNA - PIC18
//                            ATZ:29.10.17
//                           AUTOR: LEANDRO
//************************************************************************
//***************************************************************************
//       CHECA SE OCORREU ERRO NO RECEBIMENTO DO BYTE NA USART
//***************************************************************************
//A FUN��O RETORNA:
//    0 SE A USART N�O RECEBEU UM NOVO DADO
//    1 SE A USART RECEBEU UM NOVO DADO E CARREGA EM usart_rx_dado
//    2 PARA ERRO DE FRAME
//    3 PARA ERRO DE OVERRUN (BUFFER OVERLOAD)

int usart_error(){
   if (ferr==1){
   return 1; //erro de frame (stop bit n�o recebido ou fora de hora)
   ferr=0;
   }
   if (oerr==1){
   return 3; //erro de overrun (tr�s caracteres foram recebidos pela usart sem ser lidos pela CPU)
   oerr=0;
   }
   if (rcif==0) return 0;
   if (rcif==1) return 1;
   
}


//***************************************************************************
//        CHECA SE UM DADO FOI TOTALMENTE TRANSMITIDO PELA USART
//***************************************************************************
//Retorna TRUE se todos os bytes j� foram transmitidos ou FALSE se
//ainda existem bytes no registrador de deslocamento
short int tx_usart_complete(){
return(trmt);
}


//**************************************************************************
//                      DETEC��O DE INTERRUP��O
//**************************************************************************
short int interrupt_rx_usart(){
   return(rcif);
}

//***************************************************************************
//                            FIM DO INCLUDE
//***************************************************************************






