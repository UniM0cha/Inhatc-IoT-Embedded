void setup() {
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  Serial.begin(9600);
}
 
void loop() {
  if(Serial.available()) {
    char c = Serial.read();
    if(c == 'A') {
      digitalWrite(13, HIGH);
      digitalWrite(12, LOW);
      Serial.write("A");
      return;
    }
    if(c == 'B') {
      digitalWrite(13, LOW);
      digitalWrite(12, HIGH);
      Serial.write("B");
      return;
    }
  }
}
