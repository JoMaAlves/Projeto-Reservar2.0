import serial
import time


#-------------------------------------------------------criar os arquivos-------------------------------------------------------
file = open("cartao.txt", "a+") 
file.close()

file = open('pin.txt', 'a+')
file.close()

#-----------------------------------------------------------vsrisveis-----------------------------------------------------------
op = 0
x = 0
COM = 16
log = 0
ccard = ''
cuser=''
lcard= ''
usuario = {}
lpin = ''


#-------------------------------------------------------------erros-------------------------------------------------------------

def erro():
    global op
    print('opção inválida')
    op= int(input('Deseja registrar(1), entrar(2) ou sair(3)?: \n'))

def erro2():
    global carsen
    print('opção inválida')
    carsen=int(input('Quer entrar com cartão(1) ou senha(2)? \n'))


#------------------------------------------------------------opções-------------------------------------------------------------
    
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



#--------------------------------------------------------------card------------------------------------------------------------
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
    global lcard
    lcard= input('digite o usuário: \n')

    L= open('cartao.txt', 'r')
    Le= L.read()
    L.close()

    if lcard in str(Le):
         x = 1
    else:
        print('usuário incorreto')
        lesenha()

def pyreal():
    ser = serial.Serial('COM'+str(COM), 9600)
    p = 0
    time.sleep(3)
    textoSaida = (str(usuario[str(lcard)]))
    ser.write(textoSaida.encode())
    textoEntrada = "" 
    while p == 0:
        textoEntrada = ser.readline()
        p += 1
    ser.close()

def criadicard():
    global usuario
    with open ('cartao.txt') as arq:
        for pessoa in arq:
            pessoa = pessoa.strip().split(',')
            nome = pessoa[0]
            senha= pessoa[1]
            usuario[nome] = senha


#-----------------------------------------------------------------pin-----------------------------------------------------------
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
   
def lepin():
	global x
	global lpin
	lpin= input('digite o usuário: \n')

	L= open('pin.txt', 'r')
	Le= L.read()
	L.close()

	if lpin in str(Le):
		x = 2
	else:
		print('usuário incorreto')
		lesenha()

def criadipin():
    global usuario
    with open ('pin.txt') as arq:
        for pessoa in arq:
            pessoa = pessoa.strip().split(',')
            nome = pessoa[0]
            pin= pessoa[1]
            usuario[nome] = pin

def pyrealpin():
	ser = serial.Serial('COM'+str(COM), 9600)
	p = 0
	time.sleep(3)
	textoSaida = (str(usuario[str(lpin)]))
	ser.write(textoSaida.encode())
	textoEntrada = "" 
	while p == 0:
		textoEntrada = ser.readline()
		p += 1
	ser.close()



#-------------------------------------------------------------faz acontecer-----------------------------------------------------
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
    	lepin()
    	criadipin()
    	if x ==2:
    		pyrealpin()
        
if op == 3:
    print('saindo...')
