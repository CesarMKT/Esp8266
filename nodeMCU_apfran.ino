//hora


//Programa: Relogio com NodeMCU ESP8266 e display Oled - NTP
//Referencia: A Beginner's Guide to the ESP8266 - Pieter P.
//Adaptacoes e alteracoes: Arduino e Cia

#include <ESP8266WiFiMulti.h>
#include <WiFiUdp.h>
#include <Wire.h>

//Cria uma instancia da classe ESP8266WiFiMulti, chamada 'wifiMulti'
ESP8266WiFiMulti wifiMulti;
//Cria uma instancia da classe WifiUDP para enviar e receber dados
WiFiUDP UDP;

IPAddress timeServerIP;
//Define o servidor NTP utilizado
const char* NTPServerName = "b.ntp.br";
//Time stamp do NTP se encontra nos primeiros 48 bytes da mensagem
const int NTP_PACKET_SIZE = 48;
//Buffer para armazenar os pacotes transmitidos e recebidos
byte NTPBuffer[NTP_PACKET_SIZE];
//Pinos do NodeMCU - Interface I2C: SDA => D1, SCL => D2
//Inicializa o display Oled


//Requisita horario do servidor NTP a cada minuto
unsigned long intervalNTP = 60000;
unsigned long prevNTP = 0;
unsigned long lastNTPResponse = millis();
uint32_t timeUNIX = 0;
unsigned long prevActualTime = 0;



//en hora


#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

// Altere com as suas credenciais
const char *ssid = "ap fran";
const char *password = "franOcosta11";

ESP8266WebServer server(80); //instanciar servidor no porto 80

String page = "";

void setup(void)
{

 


  //Pagina HTML
  page = "<html><head><title>Controle Sinaleiro</title></head>"//barra de titulo
  "<body>    <h1>      <em><strong>Sinaleiro programavel</strong></em></h1>    <p>"
  "<a href=\"LEDOn\"><button style=\"background: #069cc2; border-radius: 6px; padding: 15px; cursor: pointer; color: #fff; border: none; font-size: 26px;\">Ligar o Led Azul </button></a>"  // botão extra
  "<p>   <strong><a href=\"LEDOn\"><button>ON</button></a></strong></p>  " // botão pequeno
  "<p> <p> <p> <p>          <p>      "
  "<strong>&nbsp;<a href=\"LEDOff\"><button>OFF</button></a></strong></p>  </body></html>"

  "<p> <p> <p> <p>          "

  "<a href=\"LEDOff\"><button style=\"background: #111213; border-radius: 6px; padding: 15px; cursor: pointer; color: #fff; border: none; font-size: 26px;\">Desligar o Led Azul </button></a>"  // botão extra

  ;  //segundo botão


  //DEsligar PINs
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  delay(1000);
  Serial.begin(9600);
  WiFi.begin(ssid, password); //Iniciar ligação à rede Wi-Fi
  Serial.println("");

  // Aguardar por ligação
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Ligado a: ");
  Serial.println(ssid);
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());

  server.on("/", []() {
    server.send(200, "text/html", page);
  });
  server.on("/LEDOn", []() {
    server.send(200, "text/html", page);
    digitalWrite(LED_BUILTIN, LOW);
    delay(1000);
  });
    server.on("/LEDOff", []() {
    server.send(200, "text/html", page);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);
   
  });
  server.on("/ATHora", []() {
    server.send(200, "text/html", page);
    digitalWrite(LED_BUILTIN, LOW);
    delay(1000);
     digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);
  });
  server.begin();
  Serial.println("Web server Inicializado!");


///hora UDP

   startUDP();
    if (!WiFi.hostByName(NTPServerName, timeServerIP))
    {
        //Obtem o endereco IP do servidor NTP
        Serial.println("DNS lookup failed. Rebooting.");
        Serial.flush();
        ESP.reset();
    }
    Serial.print("IP do servidor NTP:t");
    Serial.println(timeServerIP);
    Serial.println("rnEnviando requisicao NTP...");
    sendNTPpacket(timeServerIP);



/// end hora UPD
}

void loop(void)
{
  server.handleClient();


/// hora CERTA
 unsigned long currentMillis = millis();
    if (currentMillis - prevNTP > intervalNTP)
    {
        //Verificar se passou um minuto da ultima requisicao
        prevNTP = currentMillis;
        Serial.println("rnEnviando requisicao NTP ...");
        sendNTPpacket(timeServerIP);
    }
    uint32_t time = getTime();
    if (time)
    {
        timeUNIX = time - 10800;
        Serial.print("Resposta NTP:t");
        Serial.println(timeUNIX);
        lastNTPResponse = currentMillis;
    }
    else if ((currentMillis - lastNTPResponse) > 3600000) {
        Serial.println("Mais de 1 hora desde a ultima resposta NTP. Reiniciando.");
        Serial.flush();
        ESP.reset();
    }
    uint32_t actualTime = timeUNIX + (currentMillis - lastNTPResponse) / 1000;
    if (actualTime != prevActualTime && timeUNIX != 0)
    {
        //Verifica se passou um segundo desde a ultima impressao de valores no serial monitor
        prevActualTime = actualTime;
        Serial.printf("rUTC time:t%d:%d:%d   ", getHours(actualTime), getMinutes(actualTime), getSeconds(actualTime));
        Serial.println();
    }
 //Pagina HTML
  page = "<html><head><title>Controle Sinaleiro</title></head>"//barra de titulo
  "<body>    <h1>      <em><strong>Sinaleiro programavel</strong></em></h1>    <p>"
  
    "<p> <p> <p>    <p><body>    <h1>      linha </strong></em></h1>    <p> "
  "<a href=\"LEDOn\"><button style=\"background: #069cc2; border-radius: 6px; padding: 15px; cursor: pointer; color: #fff; border: none; font-size: 26px;\">Ligar o Led Azul </button></a>"  // botão extra
  "<p>   <strong><a href=\"LEDOn\"><button>ON</button></a></strong></p>  " // botão pequeno
   "<p> <p> <p>    <p><body>    <h1>      linha </strong></em></h1>    <p> "
  "<strong>&nbsp;<a href=\"LEDOff\"><button>OFF</button></a></strong></p>  </body></html>"

  "<p> <p> <p>    <p><body>    <h1>      linha </strong></em></h1>    <p> "

  "<a href=\"LEDOff\"><button style=\"background: #111213; border-radius: 6px; padding: 15px; cursor: pointer; color: #fff; border: none; font-size: 26px;\">Desligar o Led Azul </button></a>"  // botão extra
 
    "<p> <p> <p>    <p><body>    <h1>      linha </strong></em></h1>    <p> "
  "<p><a href=\"ATHora\"><button style=\"background: #111213; border-radius: 6px; padding: 15px; cursor: pointer; color: #fff; border: none; font-size: 26px;\"> Hora Atual piscar led </button></a>"  // botão extra
"<p><body>    <h1>      <em><strong>Ultima atuaização:  "
  ;  //segundo botão
  page=page+(String(getHours(actualTime)));
  page=page+":";
  page=page+(String(getMinutes(actualTime)));
  page=page+":";
  page=page+(String(getSeconds(actualTime)));

   page=page+" </strong></em></h1>    <p>";
//    //Mostra o horario atualizado
//       Serial.println("");
//        Serial.print( String(getHours(actualTime)));
//         Serial.print( ":");
//          Serial.print(String(getMinutes(actualTime)));
//         Serial.print( ":");
//       Serial.print( String(getSeconds(actualTime)));



/// em hora certa

  
}




///funções hora certa


void startUDP()
{
  Serial.println("Iniciando UDP");
  //Inicializa UDP na porta 23
  UDP.begin(123);
  Serial.print("Porta local:t");
  Serial.println(UDP.localPort());
  Serial.println();
}
uint32_t getTime()
{
  if (UDP.parsePacket() == 0)
  {
    return 0;
  }
  UDP.read(NTPBuffer, NTP_PACKET_SIZE); // read the packet into the buffer
  //Combina os 4 bytes do timestamp em um numero de 32 bits
  uint32_t NTPTime = (NTPBuffer[40] << 24) | (NTPBuffer[41] << 16) | (NTPBuffer[42] << 8) | NTPBuffer[43];
  //Converte o horario NTP para UNIX timestamp
  //Unix time comeca em 1 de Jan de 1970. Sao 2208988800 segundos no horario NTP:
  const uint32_t seventyYears = 2208988800UL;
  //Subtrai setenta anos do tempo
  uint32_t UNIXTime = NTPTime - seventyYears;
  return UNIXTime;
}
void sendNTPpacket(IPAddress& address)
{
  //Seta todos os bytes do buffer como 0
  memset(NTPBuffer, 0, NTP_PACKET_SIZE);
  //Inicializa os valores necessarios para formar a requisicao NTP
  NTPBuffer[0] = 0b11100011;   // LI, Version, Mode
  //Envia um pacote requisitando o timestamp
  UDP.beginPacket(address, 123);
  UDP.write(NTPBuffer, NTP_PACKET_SIZE);
  UDP.endPacket();
}
inline int getSeconds(uint32_t UNIXTime) 
{
  return UNIXTime % 60;
}
inline int getMinutes(uint32_t UNIXTime) 
{
  return UNIXTime / 60 % 60;
}
inline int getHours(uint32_t UNIXTime) 
{
  return UNIXTime / 3600 % 24;
}
