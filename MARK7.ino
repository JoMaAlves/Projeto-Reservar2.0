#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
#include <RDM6300.h>

#define linhas 4
#define colunas 3
#define vermelho A2
#define verde A1
#define azul A0

SoftwareSerial RFID(2, 3);

uint8_t Payload[6]; // used for read comparisons

RDM6300 RDM6300(Payload);

LiquidCrystal_I2C lcd(0x3F,2,1,0,4,5,6,7,3, POSITIVE); 

char mapaTeclas[linhas][colunas]={{'1','2','3'},{'4','5','6'},{'7','8','9'},{'*','0','#'}};
String senha="1234";
String digitada;
int estado=0;
byte pinos_linha[linhas]={4,5,6,7};
byte pinos_coluna[colunas]={8,9,10};
char tecla;
Keypad teclado = Keypad(makeKeymap(mapaTeclas),pinos_linha,pinos_coluna,linhas,colunas);

void setup() {
  tecla=teclado.getKey();
  Serial.begin(9600);   // Inicia a serial
  RFID.begin(9600);
  Serial.println("Aproxime o seu cartao do leitor...");
  Serial.println();
  pinMode(azul,OUTPUT);
  pinMode(verde,OUTPUT);
  pinMode(vermelho,OUTPUT); 
  lcd.begin(16, 2);  //Define o número de colunas e linhas do LCD:
  mensageminicial();
}

void loop() {
  char tecla=teclado.getKey();
    if(tecla != NO_KEY){  // ######## SE A FUNÇÃO TECLA COMEÇAR A RECEBER VALORES A VARIAVEL "estado" VAI SER IGUAL A 1 ########
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
      }else{
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
  // Look for new cards
    String conteudo = "";
    while (RFID.available() > 0)
  {
    uint8_t c = RFID.read();
    if (RDM6300.decode(c))
    {
      Serial.println("ID TAG: ");
      //Mostra os dados no serial monitor
      for (int i = 0; i < 5; i++) {
        Serial.print(Payload[i], HEX);
        conteudo.concat(String(Payload[i], HEX));
        Serial.print(" ");
        if (i < 4){
          conteudo.concat(" ");
        }
      }
    }
  }
  
  //Serial.println();
  conteudo.toUpperCase();
  
  if (conteudo == "29 0 9A 91 F0") //UID 1 - Chaveiro
  {
    mensagemaprovada();
  }
 
  if (conteudo.substring(0) == "29 0 9A 90 92") //UID 2 - Cartao
  {
    mensagemnegada();
  }
}    

bool verifcaSenha(String sa, String sd){
  bool resultado=false;
  if(sa.compareTo(sd)==0){
    resultado=true;
  }else{
    resultado=false;
  }
  return resultado;
}



//----------------------------------------------------------------------------------------------------------------------------------------------------------------------


void leds(int e){// ########## 0=AGUARDANDO / 1=DIGITANDO / 2=NEGADO / 3=ACEITO ########## //
  if(e==0){
    digitalWrite(vermelho,LOW);
    digitalWrite(verde,HIGH);
    digitalWrite(azul,LOW);
  }else if(e==1){
    digitalWrite(vermelho,LOW);
    digitalWrite(verde,LOW);
    digitalWrite(azul,HIGH);
  }else if(e==2){
    mensagemnegadacard();
  }else if(e==3){   // ############ SE A RESERVA FOR CONFIRMADA ############ //
    mensagemaprovada(); 
  }
}


void mensagemaprovada()   //###### QUANDO É APROVADA A SALA ######
{
    Serial.println("Ola Joma!");
    Serial.println();
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Ola Joma!");
    lcd.setCursor(0,1);
    lcd.print("Seja bem vindo!"); //Começa o timer
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
    delay(10000);
    digitalWrite(azul,LOW);
    mensageminicial();
  }

  void mensagemnegada()   //###### QUANDO A SALA É NEGADA ######
{
    Serial.println("Ola Cartao !");
    Serial.println();
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Cartao detectado!");
    lcd.setCursor(0,1);
    lcd.print(" Acesso Negado! ");
    digitalWrite(vermelho,HIGH);
    delay(3000);
    digitalWrite(vermelho,LOW);
    digitalWrite(azul,LOW);
    mensageminicial();
  }

  void mensageminicial()
{
  lcd.clear();
  lcd.print("Coloque o cartao");  
  lcd.setCursor(0,1);
  lcd.print("ou insira o pin!");
  
  digitalWrite(verde,HIGH);
}

  void mensagemnegadacard()
{
  Serial.println("Ola Senha!");
    Serial.println();
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(" Senha errada! ");
    lcd.setCursor(0,1);
    lcd.print(" Acesso Negado! ");
    digitalWrite(vermelho,HIGH);
    delay(3000);
    digitalWrite(vermelho,LOW);
    digitalWrite(azul,LOW);
    mensageminicial();
}
