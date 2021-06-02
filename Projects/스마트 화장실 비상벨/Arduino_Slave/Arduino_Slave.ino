#include <SoftwareSerial.h>

SoftwareSerial mySerial(10,11); //시리얼통신 핀

int ledGreen = 6;
int ledRed = 13;
int PIR = 12;
int PIRstate = LOW;
int val;
int piezo = 8;
int sw = 2;
bool check = false;
bool setLoop = false;
unsigned long past = 0;
unsigned long now = 0;
int ledState = LOW;

void setup()
{
  pinMode(ledGreen, OUTPUT);
  pinMode(ledRed, OUTPUT);
  pinMode(PIR, INPUT);
  pinMode(piezo, OUTPUT);
  pinMode(sw, INPUT_PULLUP);
  Serial.begin(9600);
  mySerial.begin(2400); //기존 9600이여서 다른 통신속도를 사용해야함
  attachInterrupt(digitalPinToInterrupt(sw), warning, FALLING);
}

void warning(){
  if(check == false){
    digitalWrite(piezo, HIGH);
    check = true;
    setLoop = true;
  }
  else{
    digitalWrite(piezo, LOW);
    check = false;
    setLoop = false;
  }
}

void loop()
{
  val = digitalRead(PIR);
  now = millis();
  
  if(!check){
    if(val == HIGH){
    digitalWrite(ledRed, HIGH);
    digitalWrite(ledGreen, LOW);
    if(PIRstate == LOW){
      PIRstate = HIGH;
    }
  }
  
  else{
    if(setLoop){
      digitalWrite(ledGreen, LOW);
    }
    else{
      digitalWrite(ledGreen, HIGH);
    }
    digitalWrite(ledRed, LOW);
    if(PIRstate == HIGH){
      PIRstate == LOW;
    }
  }
  }
  
  if(setLoop){
    mySerial.write('1');
    digitalWrite(ledGreen, LOW);
    if(now - past >= 100){
      past = now;
      
      if(ledState == LOW){
        ledState = HIGH;
      }
      else{
        ledState = LOW;
      }
      digitalWrite(ledRed, ledState);
    }
  }
  
  else if(!setLoop){
    mySerial.write('2');
  }
  
  //mySerial.write('1'); // 2번 아두이노에 '1'값 전송
  //delay(1000);
  
  //mySerial.write('0');  // 2번 아두이노에 '1'값 전송
  //delay(1000);

}
