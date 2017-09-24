#**************************************************************************************************************
#                        MICROCHIP PIC - CÁLCULO DE TEMPO DE INTERRUPÇÃO DO TIMER
#**************************************************************************************************************
# coding: utf-8
import sys

def sair():
    input('Precione ENTER para sair.')
    sys.exit(0)

def func_erro():
    try:
        return 100-((periodo/tempo)*100)
    except ZeroDivisionError:
        return 0

extra = 0
bit_falta = 0

print('CÁLCULO DE TEMPO DE INTERRUPÇÃO DO TIMER')
bit = int(input('Digite o número de bits do Timer:'))
clk = float(input('Digite o valor do clock[Hz]:'))
prc = float(input('Digite o valor do prescaler[adm]:'))
tempo = float(input('Digite o valor do período da interrupção[s]:'))

clk_maq = clk/4     #Ciclo de máquina
ciclo_maq = 1/clk_maq

bit = 2**bit
count = bit-1
while count > 0:
    periodo = (bit-count)*prc*ciclo_maq
    if periodo >= tempo:
        break
    count -= 1

if count==0 and periodo<tempo:
    periodo = (bit)*prc*ciclo_maq
    extra = tempo/periodo
    extra = int(extra)
    tempo_falta = tempo-(extra*periodo)
    tempo_um_bit = ciclo_maq*prc
    bit_falta = int(tempo_falta/tempo_um_bit)
    periodo = (periodo*extra)+(tempo_um_bit*bit_falta)
    erro = func_erro()
else:
    erro = func_erro()

valor_timer = bit-count

print('O valor para o timer é:%i' %valor_timer)
print('O período total é de:%.3g s' %periodo)
print('O timer vai estourar %i vezes.' %extra)
print('Na última, carregar o timer com %i.' %bit_falta)
print('O erro em porcentagem é de:%.1f' %erro)

sair()

