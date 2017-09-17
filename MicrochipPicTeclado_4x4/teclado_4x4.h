/****************************************************************
      ROTINA DE SCANEAMENTO DE TECLAS DE UMA MATRIZ
      4X4 USANDO UM PORT DO mC  09.05.06

VARIAVEIS:
         KEY_TRATA = 1 INDICA QUE EXISTE UMA TECLA NOVA
         KEY_TRATA = 0 INDICA QUE NAO EXISTE UMA TECLA NOVA
         NEW_KEY => RECEBE O VALOR DA TECLA DIGITADA, AS TECLAS VARIAM NO INTERVALO DE 01 ATÉ 0F
         KEY_PRESS = 0 NENHUMA TECLA PRECIONADA
         KEY_PRESS = 1 TECLA PRECIONADA

FUNÇÕES:
         void scan_tec(void) ->  INICIA A BUSCA POR UMA TECLA PRECIONADA                                 

ATUALIZADO: 14.08.10
*****************************************************************/

//********************************************************
//            constantes e variaveis
//********************************************************

const int linha1[4]={0x01,0x02,0x03,0x0a};
const int linha2[4]={0x04,0x05,0x06,0x0b};
const int linha3[4]={0x07,0x08,0x09,0x0c};
const int linha4[4]={0x0d,0x00,0x0e,0x0f};
short int key_trata=0;     //flag: 0-já tratou a tecla, 1-não tratou ainda
short int key_press = 0;   //flag: 0-não está segurando a tecla, 1-está segurando
int   new_key;
int   temp_key;
//void scan_tec();


//********************************************************
//                      FUNÇÕES
//********************************************************

scan_tec(){
       tris_tec=0b11110000;
       port_tec=0b11110000;
       if (port_tec == 0b11110000){
       key_press = 0;         //sai sem nenhuma pressionada
       return;
       }

       delay_ms(20);                   //debouce

       if (port_tec == 0b11110000){
       key_press = 0;         //verifica se não foi ruido.
       return;
       }

       if (!key_trata & key_press){       //já tratou a tecla ou esta
       return;
       }

       port_tec=0b11111110;         //primeira coluna
       if (!bit_test(port_tec,4)) new_key=linha1[0];
       if (!bit_test(port_tec,5)) new_key=linha2[0];
       if (!bit_test(port_tec,6)) new_key=linha3[0];
       if (!bit_test(port_tec,7)) new_key=linha4[0];

      port_tec=0b11111101;         //segunda coluna
       if (!bit_test(port_tec,4)) new_key=linha1[1];
       if (!bit_test(port_tec,5)) new_key=linha2[1];
       if (!bit_test(port_tec,6)) new_key=linha3[1];
       if (!bit_test(port_tec,7)) new_key=linha4[1];

       port_tec=0b11111011;         //terceira coluna
       if (!bit_test(port_tec,4)) new_key=linha1[2];
       if (!bit_test(port_tec,5)) new_key=linha2[2];
       if (!bit_test(port_tec,6)) new_key=linha3[2];
       if (!bit_test(port_tec,7)) new_key=linha4[2];

       port_tec=0b11110111;         //quarta coluna
       if (!bit_test(port_tec,4)) new_key=linha1[3];
       if (!bit_test(port_tec,5)) new_key=linha2[3];
       if (!bit_test(port_tec,6)) new_key=linha3[3];
       if (!bit_test(port_tec,7)) new_key=linha4[3];

      key_press = 1;
       key_trata=1;
}

