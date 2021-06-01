#include <MsTimer2.h>

int ledr = 13;
int ledg = 12;
int btn = 6;
int pir = 2;
bool flag = 0;

void setup()
{
  pinMode(ledr, OUTPUT);
  pinMode(ledg, OUTPUT);
  pinMode(btn, INPUT);
  pinMode(pir, INPUT);
  Serial.begin(9600);
}

void loop()
{
  if(digitalRead(btn) == HIGH){
    //처음 버튼이 눌렸을 때 : 타이머 시작
    if(!flag){
      //공부시간을 설정했는지 확인
      if(analogRead(A0)){
        Serial.println("Study Start");
        studyTime();
        flag = 1;      
        delay(1000);
      }else{
        Serial.println("Please Set Study time");
        delay(1000);
      }
    }
    //실행 중 버튼이 눌렸을 때 : 타이머 중지
    else{
      Serial.println("Timer Stop");
      stopTimer();
      flag = 0;
      delay(1000);
    }
  }
}

void stopTimer(){
  //초기화
  MsTimer2::stop();
  digitalWrite(ledr,LOW);
  digitalWrite(ledg,LOW);
}

void studyTime(){
  //1024단계를 60분 단위로 맞춤 & 밀리초 맞춤
  double t = analogRead(A0) * 3519.0615835777; //(3600000/1023)
  Serial.print("Timer set about ");
  Serial.print(t/60000);
  Serial.print(" minutes\n");
  MsTimer2::stop();
  //공부시간 시작, 초록 LED 꺼짐, 빨간색 LED 켜짐.
  MsTimer2::set(t, breakTime);
  MsTimer2::start();
  digitalWrite(ledg,LOW);
  digitalWrite(ledr,HIGH);
}

void breakTime(){
  //쉬는시간 시작, 빨강 LED 꺼짐, PIR센서가 감지될 때까지 초록 LED 깜박거리기.
  Serial.println("Break time for 10 minutes! Move your body!");
  MsTimer2::stop();
  digitalWrite(ledr,LOW);
  bool state = false;
  while(digitalRead(pir) != HIGH){
    digitalWrite(ledg, state);
    state = !state;
    delay(1000);
  }
  
  //PIR센서 감지, 10분 타이머 시작, 끝나면 공부시간.
  digitalWrite(ledg, HIGH);
  MsTimer2::set(570000,breakTimeReady);
  MsTimer2::start();
}

void breakTimeReady(){
  //30초 타이머 시작, 30초 동안 깜박거리기.
  Serial.println("Study start in 30 seconds...");
  MsTimer2::stop();
  MsTimer2::set(30000,studyTime);
  MsTimer2::start();
  bool state = true;
  for(int i = 0; i < 30; i++){
    digitalWrite(ledg, state);
    state = !state;
    delay(1000);
  }
}

//한계점 : 깜박거리는 순간 (PIR대기시간 & 30초 준비시간)에는 버튼으로 중지 불가능
