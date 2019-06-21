//----BIBLIOTECAS-------------------------------------------------------------------------------------------------------------------------------------------------------

#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
#include <MFRC522.h>

//----DEFINES-----------------------------------------------------------------------------------------------------------------------------------------------------------

#define linhas 4
#define colunas 3
#define vermelho A2
#define verde A1
#define azul A0
#define buzzer A3
#define SS_PIN 10
#define RST_PIN 9

//----FAZ FUNCIONAR O RFID----------------------------------------------------------------------------------------------------------------------------------------------

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

//----PARTE FIXA DO LCD-------------------------------------------------------------------------------------------------------------------------------------------------

LiquidCrystal_I2C lcd(0x3F,2,1,0,4,5,6,7,3, POSITIVE);                                       // FAZ O LCD COM SHIELD FUNCIONAR

//----ALGUMAS CONFIGURAÇÕES---------------------------------------------------------------------------------------------------------------------------------------------

char mapaTeclas[linhas][colunas]={{'1','2','3'},{'4','5','6'},{'7','8','9'},{'*','0','#'}};  // DEFINE O QUE CADA TECLA EM CADA LINHA VAI FAZER
char tecla;
char byteEntrada;
char st[20];
String textoEntrada = "";                                                                    // PYSERIAL COMPARA CARTÕES
//String senha = textoEntrada;                                                                 // A SENHA
String digitada;
//String conteudo = "";
int estado=0;
byte pinos_linha[linhas]={2,3,4,5};
byte pinos_coluna[colunas]={6,7,8};                                                  // VOLTA A LER O CARTÃO

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
    lcd.print("Ola!");
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
  SPI.begin();      // Inicia  SPI bus
  mfrc522.PCD_Init();   // Inicia MFRC522
  pinMode(buzzer,OUTPUT);
  pinMode(azul,OUTPUT);
  pinMode(verde,OUTPUT);
  pinMode(vermelho,OUTPUT); 
  lcd.begin(16, 2);                                                                          //DEFINE O NÚMERO DE COLUNAS E LINHAS NO LCD
  mensageminicial();
}

//-----LOOP----LOOP----LOOP----LOOP----LOOP----LOOP----LOOP----LOOP----LOOP----LOOP----LOOP----LOOP----LOOP----LOOP----LOOP----LOOP----LOOP----LOOP----LOOP----LOOP-----

void loop() {
//----TECLADO-----------------------------------------------------------------------------------------------------------------------------------------------------------

//----PYSERIAL----------------------------------------------------------------------------------------------------------------------------------------------------------   
    
    while (Serial.available()>0){
    byteEntrada = Serial.read();
    textoEntrada = String( textoEntrada + String(byteEntrada));
  }
  
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------   
  
  char tecla=teclado.getKey();
  if(tecla != NO_KEY){                                                                       // SE A FUNÇÃO TECLA COMEÇAR A RECEBER VALORES A VARIAVEL "estado" VAI SER IGUAL A 1
    lcd.clear();
    Serial.print(tecla);
    lcd.print(tecla); 
    estado=1;
    if(tecla=='#'){
      lcd.clear();
      if(verifcaSenha(textoEntrada,digitada)){
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
//----RFID--------------------------------------------------------------------------------------------------------------------------------------------------------------
  
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  String conteudo= "";
  byte letra;
  //Serial.println(textoEntrada);
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     //Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     //Serial.print(mfrc522.uid.uidByte[i], HEX);
     conteudo.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     conteudo.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  conteudo.toUpperCase();
  conteudo.remove(0,1);
  
  //Serial.print("Txt: ");
  //Serial.println(textoEntrada.length());
  //Serial.println(textoEntrada);
  //Serial.print("Cont: ");
  //Serial.println(conteudo.length());
  //Serial.println(conteudo);

//----PYSERIAL----------------------------------------------------------------------------------------------------------------------------------------------------------

  while (Serial.available()>0){
    byteEntrada = Serial.read();
    textoEntrada = String(textoEntrada + String(byteEntrada));
  } 
  
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------- 

  //Serial.println(textoEntrada.startsWith(conteudo));
  //Serial.println(textoEntrada.length());
  //Serial.print(conteudo.equals(textoEntrada));
  //Serial.print(conteudo.endsWith(textoEntrada));
  if (textoEntrada.startsWith(conteudo))                         //Compara a entrada com a leitura do RFID
  {
    mensagemaprovada();
  }
 
  else
  {
    mensagemnegada();
  }
   textoEntrada = "";
}
