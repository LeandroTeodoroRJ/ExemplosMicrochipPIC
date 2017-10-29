//**********************************************************
//INCLUDE DE MAPEAMENTO DE MEÓRIA DOS REGISTRADORES
//ESPECIAIS E BITS DOS MESMOS PARA O mC 18F4520
//**********************************************************

#byte status = 0xFD8       
#byte tris_a = 0xF92        //#byte - define o endereco do
#byte tris_b = 0xF93        //registrador no banco de memoria
#byte tris_c = 0xF94        //de acordo com o datasheet
#byte tris_d = 0xF95        //de acordo com o datasheet
#byte tris_e = 0xF96        //de acordo com o datasheet
#byte port_a = 0xF80
#byte port_b = 0xF81
#byte port_c = 0xF82
#byte port_d = 0xF83
#byte port_e = 0xF84
#byte intcon = 0xFF2
#byte pir1 =   0xF9E
#byte pie1 =   0xF9D

//registrador pir1
#bit  pspif  = pir1.7
#bit  adif   = pir1.6
#bit  rcif   = pir1.5
#bit  txif   = pir1.4
#bit  sspif  = pir1.3
#bit  ccp1if = pir1.2
#bit  tmr2if = pir1.1
#bit  tmr1if = pir1.0

//registrador pie1
#bit  pspie  = pie1.7
#bit  adie   = pie1.6
#bit  rcie   = pie1.5
#bit  txie   = pie1.4
#bit  sspie  = pie1.3
#bit  ccp1ie = pie1.2
#bit  tmr2ie = pie1.1
#bit  tmr1ie = pie1.0

//USART
#byte txsta = 0xFAC
#byte rcsta = 0xFAB
#byte spbrg = 0xFAF
#byte spbrgh= 0xFB0
#byte txreg = 0xFAD
#byte rcreg = 0xFAE 

//registrador txsta
#bit  csrc  = txsta.7
#bit  tx9   = txsta.6
#bit  txen  = txsta.5
#bit  sync  = txsta.4
#bit  sendb = txsta.3
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
#bit  gie  = 0x0b.7
#bit  peie = 0x0b.6
#bit  t0ie = 0x0b.5
#bit  inte = 0x0b.4
#bit  rbie = 0x0b.3
#bit  t0if = 0x0b.2
#bit  intf = 0x0b.1
#bit  rbif = 0x0b.0



