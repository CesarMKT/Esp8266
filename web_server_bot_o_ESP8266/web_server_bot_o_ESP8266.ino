#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

// Altere com as suas credenciais
const char *ssid = "CongrafVisitante";
const char *password = "VivaCor1";

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
  server.begin();
  Serial.println("Web server Inicializado!");
}

void loop(void)
{
  server.handleClient();
}
