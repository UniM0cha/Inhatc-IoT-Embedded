#include <SoftwareSerial.h>
SoftwareSerial mySerial(10,11);

char state;
int rPin=13;
int piezo=12;
unsigned long past = 0;
int ledState = LOW;

void setup()
{
  Serial.begin(9600);
  mySerial.begin(2400);
  pinMode(rPin, OUTPUT);
  pinMode(piezo, OUTPUT);
}

void loop()
{ 
  if(mySerial.available() > 0) { //수신되었는지 상태 확인
    state = mySerial.read();
    
    unsigned long now = millis();
    
    if(state == '1'){
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
    
    
    
    
    //state = mySerial.read(); //수신된 데이터를 1byte 읽어와서 state 변수에 저장함
    
    //if(state=='1') digitalWrite(rPin, HIGH); //읽어온 값이 '1'이면 참으로 Red에 HIGH로 불이 들어옴
    //else digitalWrite(rPin, LOW); //'0'이면 1이 아니기 때문에 거짓으로 Red에 LOW로 불이 꺼짐
    
    //Serial.println(state);  //정상적으로 읽은 값이 들어왔는지 아두이노 IDE 시리얼모니터로 출력
  }
  
}
