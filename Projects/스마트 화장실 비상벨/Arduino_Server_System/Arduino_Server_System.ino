#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <SoftwareSerial.h>

//SSID와 패스워드 설정
#define STASSID "U+Net2493"
#define STAPSK  "k1025423s."
//#define STASSID "iptime"
//#define STAPSK  ""

// 웹 서버 선언
ESP8266WebServer server(80);
SoftwareSerial mySerial(10,11);

const char *ssid = STASSID;
const char *password = STAPSK;
char state;
int rPin=13;
int piezo=12;
unsigned long past = 0;
int ledState = LOW;

/////////////////////
//wifi, 웹 관련 함수들//
/////////////////////
void server_setting() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // WIFI 연결될 때까지 대기
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });
  server.onNotFound(handleNotFound);

  //서버 시작
  server.begin();
  Serial.println("HTTP server started");  
}

void handleRoot() {
  char temp[400];

  snprintf(temp, 400,
    "<html>\
      <head>\
        <meta http-equiv='refresh' content='5'/>\
        <title>ESP8266 Demo</title>\
      </head>\
      <body>\
        <h1>Hello from ESP8266!</h1>\
      </body>\
    </html>"
  );
    
  server.send(200, "text/html", temp);
}

// 페이지가 없을 때
void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }

  server.send(404, "text/plain", message);
}
//////////
//끝///////
//////////

void setup(void) {
  server_setting();
  Serial.begin(9600);
  mySerial.begin(2400);
  pinMode(rPin, OUTPUT);
  pinMode(piezo, OUTPUT);
}

void loop(void) {
  server.handleClient();
  if(mySerial.available() > 0) { //수신되었는지 상태 확인
    state = mySerial.read();
    
    unsigned long now = millis();
    
    if(state == '1') {
      digitalWrite(piezo, HIGH);
      if(now - past >= 100){
        past = now;
        
        if(ledState == LOW){
          ledState = HIGH;
        }
        else{
          ledState = LOW;
        }
        digitalWrite(rPin, ledState);
      }
      Serial.println(now);
    }
    
    else if(state == '2'){
      digitalWrite(piezo, LOW);
    }
  }
}
