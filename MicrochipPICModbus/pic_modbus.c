
//   Include do Protocolo Modbus



//*********************** CONSTANTES E VARIÁVEIS *****************************

#define  TAMANHO_MAX_BUFFER   20
#define  TX_ENABLE_DELAY      10
#define  RX_PROCESSING_DELAY  1
#define  CRC_BYTES            2
#define  MODBUS_MASTER        1
#define  MODBUS_SLAVE         0

//Comandos(Function Codes):
#define  READ_COIL_STATUS        1
#define  READ_INPUT_STATUS       2
#define  READ_HOLDING_REGISTERS  3
#define  READ_INPUT_REGISTERS    4
#define  FORCE_SINGLE_COIL       5
#define  PRESET_SINGLE_REGISTER  6

int   modbus_rx_buffer[TAMANHO_MAX_BUFFER];     //Buffer recebido pelo Modbus
int   modbus_tx_buffer[TAMANHO_MAX_BUFFER];     //Buffer transmitido pelo ModBus
int   modbus_buffer_len;                        //Tamanho da mensagem tirando
                                                //os bytes de CRC
int   modbus_buffer_rx_len;
int1  modbus_receive_data;
int1  modbus_mode;                              //Modo master ou slave                            
int1  modbus_trata_dado;

// ***************************************************************************

void modbus_init(){
   modbus_buffer_len = 0;
   modbus_buffer_rx_len = 0;
   modbus_receive_data = 0;
   modbus_trata_dado = 0;

}


void transmite_buffer_tx(){
   int count = 0;
//   disable_interrupts(int_rda);
//   disable_interrupts(int_timer0);
   bit_set(TX_ENABLE_PIN);                 //Habilita a transmissão
   delay_ms(TX_ENABLE_DELAY);             //Delay para começo de transmissão         
   while(count < modbus_buffer_len + CRC_BYTES){
      putc(modbus_tx_buffer[count]);
      count++;
      delay_ms(RX_PROCESSING_DELAY);     //Espera processamento pelo receptor, para rotinas maiores de delay utilizar as interrupções dos timers
   }
   delay_ms(TX_ENABLE_DELAY); 
   bit_clear(TX_ENABLE_PIN);             //Habilita a leitura
   modbus_buffer_len = 0;                //Reseta o buffer de tx
//   enable_interrupts(int_rda);
//   enable_interrupts(int_timer0);
}

void calc_crc(unsigned int dataLenght){
   unsigned long checksum;
   unsigned long j;
   unsigned char lowCRC;
   unsigned char highCRC;
   unsigned char i;
   
   checksum = 0xFFFF;
   
   for (j=0; j<dataLenght; j++){
      checksum = checksum^(unsigned int)modbus_tx_buffer[j];
      for (i=8; i>0; i--){
         if ((checksum)&0x0001){
            checksum = (checksum>>1)^0xA001;
         }
         else{
            checksum>>=1;
         }
      }
   }
   
   highCRC = checksum>>8;
   checksum<<=8;
   lowCRC = checksum>>8;
   modbus_tx_buffer[dataLenght] = lowCRC;
   modbus_tx_buffer[dataLenght+1] = highCRC;
}

void modbus_add_byte(unsigned int8 dado){
   modbus_buffer_len++;
   modbus_tx_buffer[modbus_buffer_len-1] = dado;
}

void modbus_send_message(){
   calc_crc(modbus_buffer_len);
   transmite_buffer_tx();
}

void modbus_receive_delay(){
   if (modbus_receive_data == true){
      modbus_receive_data = false;
      delay_ms(4);
      if (modbus_receive_data == false){
         modbus_trata_dado = true;
      }
   }
}


int1 modbus_check_crc(unsigned int dataLenght){
   unsigned long checksum;
   unsigned long j;
   unsigned char lowCRC;
   unsigned char highCRC;
   unsigned char i;
   
   checksum = 0xFFFF;
   
   for (j=0; j<dataLenght; j++){
      checksum = checksum^(unsigned int)modbus_rx_buffer[j];
      for (i=8; i>0; i--){
         if ((checksum)&0x0001){
            checksum = (checksum>>1)^0xA001;
         }
         else{
            checksum>>=1;
         }
      }
   }
   
   highCRC = checksum>>8;
   checksum<<=8;
   lowCRC = checksum>>8;
  
   if ((modbus_rx_buffer[dataLenght] == lowCRC)&&(modbus_rx_buffer[dataLenght+1] == highCRC)){
      return true;
   }
   else{
      return false;
   }
}
