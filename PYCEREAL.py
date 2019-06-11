import serial
import time
import sys


file = open("cartao.txt", "a+") 
file.close()

file = open('pin.txt', 'a+')
file.close()


op = 0
x = 0
COM = 16
log = 0
ccard = ''
cuser=''
lsenha= ''
usuario = {}


def erro():
    global op
    print('opção inválida')
    op= int(input('Deseja registrar(1), entrar(2) ou sair(3)?: \n'))

def erro2():
    global carsen
    print('opção inválida')
    carsen=int(input('Quer entrar com cartão(1) ou senha(2)? \n'))



    
carsen=int(input('Deseja entrar com cartão(1) ou senha(2)? \n'))

while  carsen != 1 and carsen != 2:
  erro2()

if carsen == 1:
    op= int(input('Deseja registrar(1), entrar(2) ou sair(3)?: \n'))
    while op != 1 and op != 2 and op != 3:
        erro()
    log = 1


if carsen == 2:
    op= int(input('Deseja registrar(1), entrar(2) ou sair(3)?: \n'))
    while op != 1 and op != 2 and op != 3:
        erro()
    log = 2




def criasenha():
        global op
        while op == 1:
            cuser=input('digite o novo usuário: \n')
            
            ccard=input('digite a id do cartão: \n')
            csenha =str(cuser)+','+str(ccard)
            c = open("cartao.txt", "a+") 
            c.write(str(csenha +'\n'))
            c.close()
            print('cartão registrado')
            op= int(input('Deseja registrar(1), entrar(2) ou sair(3)?: \n'))
            while op != 1 and op != 2 and op != 3:
                erro()						
def lesenha():
    global x
    global lsenha
    lsenha= input('digite o usuário: \n')

    L= open('cartao.txt', 'r')
    Le= L.read()
    L.close()

    if lsenha in str(Le):
         print('pass')
         x = 1
    else:
        print('usuário incorreto')
        lesenha()
def pyreal():
    ser = serial.Serial('COM'+str(COM), 9600)
    time.sleep(3)
    textoSaida = (str(usuario[str(lsenha)]))
    ser.write(textoSaida.encode())
    textoEntrada = "" 
    while textoEntrada != textoSaida:
        textoEntrada = ser.readline()
    ser.close()


def criadicard():
    global usuario
    with open ('cartao.txt') as arq:
        for pessoa in arq:
            pessoa = pessoa.strip().split(',')
            nome = pessoa[0]
            senha= pessoa[1]
            usuario[nome] = senha

def criapin():
    global op
    while op == 1:
            cuser=input('digite o novo usuário: \n')
            
            cpin=input('digite o pin: \n')
            
            cspin =str(cuser)+','+str(cpin)
            c = open("pin.txt", "a+") 
            c.write(str(cspin +'\n'))
            c.close()
            print('pin registrado')
            op= int(input('Deseja registrar(1), entrar(2) ou sair(3)?: \n'))
            while op != 1 and op != 2 and op != 3:
                erro()              
   
                


def criadipin():
    global usuario
    with open ('pin.txt') as arq:
        for pessoa in arq:
            pessoa = pessoa.strip().split(',')
            nome = pessoa[0]
            pin= pessoa[1]
            usuario[nome] = pin



if op == 1:
    if log == 1:
        criasenha()
    if log ==2:
        criapin()

if op == 2:
    if log ==1:
        lesenha()
        criadicard()
        if x == 1:
            pyreal()
    if log ==2:
        print('debugiin')
if op == 3:
    print('saindo...')
