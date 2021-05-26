/*
  제목   : 토양 수분센서로 수분량 측정하기
  내용   : 토양 수분센서로 토양에 있는 수분량을 측정하고, 안드로이드 APP을 통해 모터를 제어해 물을 줍니다.

  700 ~ 1024 : 건조함
  300 ~ 700 : 습함
*/

#include <ESP8266WiFi.h>
#include <Servo.h>

#ifndef STASSID
//#define STASSID "jiwon"
//#define STAPSK "skawldnjs"

#define STASSID "U+NetE27B"
#define STAPSK  "671B0B1EM@"
#endif

// 연결할 WiFi id, pw
const char* ssid     = STASSID;
const char* password = STAPSK;
const char* server = "api.thingspeak.com";

// WiFiClient를 선언한다.
WiFiClient client;

// WiFiServer 포트선언
WiFiServer webServer(80);

// 자신의 thingspeak 채널의 API key 선언
String apiKey = "CJZPWXHQ045MUGE1";

// 토양 수분센서를 A0번 핀으로 선언
int soil = A0;

// 서보모터 이름 선언
Servo servo_2;

// RED LED를 디지털 D8번 핀에 연결합니다.
const int RED_PIN = D8;
// GREEN LED를 디지털 D10번 핀에 연결합니다.
const int GREEN_PIN = D10;
// BLUE LED를 디지털 D12번 핀에 연결합니다.
const int BLUE_PIN = D12;

/////////////////////////////////////////////////////////
void setup() {
  // 토양 수분센서의 동작 상태를 확인하기 위하여 시리얼 통신을 설정합니다. (전송속도 115200bps)
  Serial.begin(115200);

  // 11번 핀을 출력으로 설정 합니다.
  pinMode(RED_PIN, OUTPUT);    
  // 10번 핀을 출력으로 설정 합니다.
  pinMode(GREEN_PIN, OUTPUT); 
  // 9번 핀을 출력으로 설정 합니다.
  pinMode(BLUE_PIN, OUTPUT);
  
  // 2번 핀에 모터 등록
  servo_2.attach(D2);
  // 서보모터 값 0으로 setting
  servo_2.write(0);

  // WiFi연결
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);  

  // WiFi 연결이 되지않을때 ... 재연결
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected"); // WiFi 연결 성공!

  // 웹 서버 start
  webServer.begin();
  Serial.println("Web server running. Waiting for the ESP IP...");

  // IP 주소 출력
  Serial.println(WiFi.localIP());

}


///////////////////////////////////////////////////////////////
void loop() {
  
  ///////////////////////////////////////////////////////
  // webServer 작동
  
  // 클라이언트 연결되었는지 확인함
  if(WiFiClient Webclient = webServer.available()){ // 서버로부터 수신된 데이터의 길이를 반환
    if (!client) {
      return;
    }
    
    
    // 데이터 보내기전 기다림
    Serial.println("new client");
    while (!Webclient.available()) {
      delay(1); 
    }
    
    // 요청의 첫번째 라인을 읽는다
    String request = Webclient.readStringUntil('\r');
    Serial.println(request);
    Webclient.flush(); // 버퍼의 모든 문자가 전송될때까지 기다린다.
    
    // 요청 들어온값 확인해 모터 움직임
    int mo_value = LOW;
    if (request.indexOf("/MOTER=ON") != -1) {
      servo_2.write(180);
      mo_value = HIGH;
    } 
    if (request.indexOf("/MOTER=OFF") != -1){
      servo_2.write(0);
      mo_value = LOW;
    }
   
    // 응답 return ( html단 )
    Webclient.println("HTTP/1.1 200 OK");
    Webclient.println("Content-Type: text/html");
    Webclient.println(""); //  do not forget this one
    Webclient.println("<!DOCTYPE HTML>");
    Webclient.println("<html>");
   
    if(mo_value == HIGH) {
      Webclient.print("0");  
    } else {
      Webclient.print("45");
    }
    
    Webclient.println("<br><br>");
    Webclient.println("Click <a href=\"/MOTER=ON\">here</a> turn the LED on pin 5 ON<br>");
    Webclient.println("Click <a href=\"/MOTER=OFF\">here</a> turn the LED on pin 5 OFF<br>");
    Webclient.println("</html>");
  
    delay(1);
    Serial.println("Client disconnected");
    Serial.println("");
  }
  

  ////////////////////////////////////
  // 토양 수분센서 단
  
  // thingSpeak에 값 전송 하는데 필요한 대기시간 15초 이상 
  unsigned long currentMillis = millis();
  if((currentMillis % 16000) == 0){
    
    // 토양 수분센서로부터 측정된 값을 읽습니다.
    int value = analogRead(soil);
  
    // 토양 수분센서로부터 측정된 값를 시리얼 모니터에 출력합니다.
    Serial.print("read sensor value : ");
    Serial.println(value);

    if(value >= 900){// RED LED를 ON 시킵니다.
    digitalWrite(RED_PIN,HIGH);  
    digitalWrite(GREEN_PIN,LOW);
    digitalWrite(BLUE_PIN,LOW);
    } else { // 상태 양호하면 Green LED ON 시킵니다.
      digitalWrite(RED_PIN,LOW);  
    digitalWrite(GREEN_PIN,HIGH);
    digitalWrite(BLUE_PIN,LOW);
    }
    
    // server api.thingspeak.com의 포트 80에 접속한다.
    if(client.connect(server, 80)){ // 접속성공시 true, 실패시 false return
      // apiKey의 뒤에 field1값을 붙이서 보낼 값을 만들어준다.
      String postStr = apiKey;
      postStr += "&field1=";
      postStr += String(value);
      postStr += "\r\n\r\n";
      
      // POST로 값 전송
      client.print("POST /update HTTP/1.1\n");
      client.print("Host: api.thingspeak.com\n");
      client.print("Connection: close\n");
      client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
      client.print("Content-Type: application/x-www-form-urlencoded\n");
      client.print("Content-Length: ");
      client.print(postStr.length());
      client.print("\n\n");
      client.print(postStr);
    }
  }
}
