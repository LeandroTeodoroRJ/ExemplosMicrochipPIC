//**********************************************************
//INCLUDE DE MAPEAMENTO DE MEÓRIA DOS REGISTRADORES
//ESPECIAIS E BITS DOS MESMOS PARA OS mC 16f87X
//**********************************************************

#byte option_reg = 0x81
#byte tmr0   = 0x01
#byte status = 0x03
#byte tris_a = 0x85        //#byte - define o endereco do
#byte tris_b = 0x86        //registrador no banco de memoria
#byte tris_c = 0x87        //de acordo com o datasheet
#byte tris_d = 0x88        //de acordo com o datasheet
#byte tris_e = 0x89        //de acordo com o datasheet
#byte port_a = 0x05
#byte port_b = 0x06
#byte port_c = 0x07
#byte port_d = 0x08
#byte port_e = 0x09
#byte intcon = 0x0b

//USART
#byte txsta = 0x98
#byte rcsta = 0x18
#byte spbrg = 0x99
#byte txreg = 0x19
#byte rcreg = 0x1a   

#byte pir1 = 0x0c
#byte pie1 = 0x8c

//registrador pie1
#bit  eeie   = pie1.7
#bit  cmie   = pie1.6
#bit  rcie   = pie1.5
#bit  txie   = pie1.4
#bit  ccp1ie = pie1.2
#bit  tmr2ie = pie1.1
#bit  tmr1ie = pie1.0

//registrador pir1
#bit  eeif   = pir1.7
#bit  cmif   = pir1.6
#bit  rcif   = pir1.5
#bit  txif   = pir1.4
#bit  ccp1if = pir1.2
#bit  tmr2if = pir1.1
#bit  tmr1if = pir1.0

//registrador txsta
#bit  csrc  = txsta.7
#bit  tx9   = txsta.6
#bit  txen  = txsta.5
#bit  sync  = txsta.4
#bit  brgh =  txsta.2
#bit  trmt =  txsta.1
#bit  tx9d =  txsta.0

//registrador rcsta
#bit  spen  = rcsta.7
#bit  rx9   = rcsta.6
#bit  sren  = rcsta.5
#bit  cren  = rcsta.4
#bit  adden = rcsta.3
#bit  ferr  = rcsta.2
#bit  oerr  = rcsta.1
#bit  rx9d  = rcsta.0


//registrador intcon
#bit  gie = 0x0b.7
#bit  peie = 0x0b.6
#bit  t0ie = 0x0b.5
#bit  inte = 0x0b.4
#bit  rbie = 0x0b.3
#bit  t0if = 0x0b.2
#bit  intf = 0x0b.1
#bit  rbif = 0x0b.0

#bit  trigger = port_b.3
#bit  trigger_inv =  port_b.4
