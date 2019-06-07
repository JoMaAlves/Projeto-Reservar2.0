//----BIBLIOTECAS-------------------------------------------------------------------------------------------------------------------------------------------------------

#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
#include <RDM6300.h>

//----DEFINES-----------------------------------------------------------------------------------------------------------------------------------------------------------

#define linhas 4
#define colunas 3
#define vermelho A2
#define verde A1
#define azul A0
#define buzzer 12

//----FAZ FUNCIONAR O RFID----------------------------------------------------------------------------------------------------------------------------------------------

SoftwareSerial RFID(2, 3);

uint8_t Payload[6];                                                                          // SERVE PARA FAZER AS COMPARAÇÕES DOS CARTÕES

RDM6300 RDM6300(Payload);

//----PARTE FIXA DO LCD-------------------------------------------------------------------------------------------------------------------------------------------------

LiquidCrystal_I2C lcd(0x3F,2,1,0,4,5,6,7,3, POSITIVE);                                       // FAZ O LCD COM SHIELD FUNCIONAR

//----ALGUMAS CONFIGURAÇÕES---------------------------------------------------------------------------------------------------------------------------------------------

bool leuCartao = 0;
String conteudo = "";
char mapaTeclas[linhas][colunas]={{'1','2','3'},{'4','5','6'},{'7','8','9'},{'*','0','#'}};  // DEFINE O QUE CADA TECLA EM CADA LINHA VAI FAZER
char tecla;
String senha="1234";                                                                         // A SENHA
String digitada;
String textoEntrada = "";                                                                    // PYSERIAL COMPARA CARTÕES
int estado=0;
byte pinos_linha[linhas]={4,5,6,7};
byte pinos_coluna[colunas]={8,9,10};
long ultimoResetLeituraCartao = millis();                                                    // VOLTA A LER O CARTÃO
Keypad teclado = Keypad(makeKeymap(mapaTeclas),pinos_linha,pinos_coluna,linhas,colunas);

//----CHECAGEM DA SENHA-------------------------------------------------------------------------------------------------------------------------------------------------

bool verifcaSenha(String sa, String sd){
  bool resultado=false;
  if(sa.compareTo(sd)==0){
    resultado=true;
  }else{
    resultado=false;
  }
  return resultado;
}

 //----MENSAGEM QUE SEMPRE VAI APARECER NO LCD--------------------------------------------------------------------------------------------------------------------------
 
 void mensageminicial()
{
  lcd.clear();
  lcd.print("Coloque o cartao");  
  lcd.setCursor(0,1);
  lcd.print("ou insira o pin!");
  
  digitalWrite(verde,HIGH);
}

//----FUNCIONAMENTO DO LED JUNTO AO TECLADO-----------------------------------------------------------------------------------------------------------------------------

void leds(int e){                                                                            // 0=AGUARDANDO / 1=DIGITANDO / 2=NEGADO / 3=ACEITO 
  if(e==0){                                                                                  // ENQUANTO AGUARDA
    digitalWrite(vermelho,LOW);
    digitalWrite(verde,HIGH);
    digitalWrite(azul,LOW);
  }else if(e==1){                                                                            // SE ESTIVER DIGITANDO
    digitalWrite(vermelho,LOW);
    digitalWrite(verde,LOW);
    digitalWrite(azul,HIGH);
  }else if(e==2){                                                                            // SE A RESERVA FOR NEGADA
    mensagemnegadasenha();
  }else if(e==3){                                                                            // SE A RESERVA FOR CONFIRMADA
    mensagemaprovada(); 
  }
}

//----QUANDO A RESERVA É APROVADA---------------------------------------------------------------------------------------------------------------------------------------

void mensagemaprovada()
{
    Serial.println();
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Ola Joma!");
    lcd.setCursor(0,1);
    lcd.print("Seja bem vindo!");
    delay(1500);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(" Sua reserva de ");
    lcd.setCursor(0,1);
    lcd.print("30s foi iniciada");
    digitalWrite(verde,LOW);
    digitalWrite(vermelho,HIGH);
    delay(1500);
    lcd.clear();
    lcd.setCursor(6,0);
    lcd.print("Sala");
    lcd.setCursor(5,1);
    lcd.print("OCUPADA");
    delay(17000);
    digitalWrite(vermelho,LOW);
    digitalWrite(azul,HIGH); 
    lcd.clear();
    lcd.setCursor(4,0);
    lcd.print("10 seg");
    lcd.setCursor(3,1);
    lcd.print("restantes");
    digitalWrite(buzzer,HIGH);
    delay(1000);
    digitalWrite(buzzer,LOW);
    delay(9000);
    digitalWrite(azul,LOW);
    mensageminicial();
}

//----QUANDO A RESERVA É NEGADA PARA O CARTÃO---------------------------------------------------------------------------------------------------------------------------

  void mensagemnegada()
{
    Serial.println();
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Cartao detectado!");
    lcd.setCursor(0,1);
    lcd.print(" Acesso Negado! ");
    digitalWrite(vermelho,HIGH);
    digitalWrite(buzzer,HIGH);
    delay(1000);
    digitalWrite(buzzer,LOW);
    delay(2000);
    digitalWrite(vermelho,LOW);
    digitalWrite(azul,LOW);
    mensageminicial();
}

//----QUANDO A RESERVA É NEGADA PARA A SENHA----------------------------------------------------------------------------------------------------------------------------

  void mensagemnegadasenha()
{
    Serial.println();
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(" Senha errada! ");
    lcd.setCursor(0,1);
    lcd.print(" Acesso Negado! ");
    digitalWrite(vermelho,HIGH);
    digitalWrite(buzzer,HIGH);
    delay(1000);
    digitalWrite(buzzer,LOW);
    delay(2000);
    digitalWrite(vermelho,LOW);
    digitalWrite(azul,LOW);
    mensageminicial();
}

//----SETUP----SETUP----SETUP----SETUP----SETUP----SETUP----SETUP----SETUP----SETUP----SETUP----SETUP----SETUP----SETUP----SETUP----SETUP----SETUP----SETUP----SETUP----

void setup() {
  tecla=teclado.getKey();
  Serial.begin(9600);                                                                        // INICIA A SERIAL
  RFID.begin(9600);
  Serial.println();
  pinMode(buzzer,OUTPUT);
  pinMode(azul,OUTPUT);
  pinMode(verde,OUTPUT);
  pinMode(vermelho,OUTPUT); 
  lcd.begin(16, 2);                                                                          //DEFINE O NÚMERO DE COLUNAS E LINHAS NO LCD
  mensageminicial();
}

//-----LOOP----LOOP----LOOP----LOOP----LOOP----LOOP----LOOP----LOOP----LOOP----LOOP----LOOP----LOOP----LOOP----LOOP----LOOP----LOOP----LOOP----LOOP----LOOP----LOOP-----

void loop() {
  char tecla=teclado.getKey();
  if(tecla != NO_KEY){                                                                       // SE A FUNÇÃO TECLA COMEÇAR A RECEBER VALORES A VARIAVEL "estado" VAI SER IGUAL A 1
    lcd.clear();
    Serial.print(tecla);
    lcd.print(tecla); 
    estado=1;
    if(tecla=='#'){
      lcd.clear();
      if(verifcaSenha(senha,digitada)){
        estado=3;
        leds(estado);
        delay(3000);
        estado=0;
      }
      else{
        estado=2;
        leds(estado);
        delay(3000);
        estado=0;
      }
      digitada="";
    }else{
      digitada+=tecla;
    }
    leds(estado);
  }

  while (RFID.available() > 0)                                                               //PROCURA POR NOVOS CARTÕES
  {
    uint8_t c = RFID.read();
    if (RDM6300.decode(c))
    {
      Serial.println(" passou ");
      conteudo = "";
      for (int i = 0; i < 5; i++) {
        Serial.print(Payload[i], HEX);
        conteudo.concat(String(Payload[i], HEX));
        Serial.print(" ");
        if (i < 4){
          conteudo.concat(" ");
        }    
      }
      leuCartao = 1;
    }
  }
  if (millis() -  ultimoResetLeituraCartao > 10000){
    ultimoResetLeituraCartao = millis();
    for (int i = 0; i< 5 ; i++){
      Payload[i] = '0'; 
    }
  }
  conteudo.toUpperCase();

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------

  char byteEntrada;
  // put your main code here, to run repeatedly:
  textoEntrada = "";
  while (Serial.available()>0){
    byteEntrada = Serial.read();
    textoEntrada = String( textoEntrada + String(byteEntrada));
    //Serial.println(textoEntrada);
  } 
  
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------- 
  if (leuCartao) {
    Serial.print("textoEntrada=>");
    Serial.println(textoEntrada);
    Serial.print("conteudo=>");
    Serial.println(conteudo);
    if (conteudo == textoEntrada)                                                           // UID 1 - Cartão Joma
    {
      mensagemaprovada();
    }
    else                                                           //UID 2 - Cartao Enrico
    {
      mensagemnegada();
    }
    leuCartao = 0;
  }
}
