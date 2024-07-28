void setup(){
  Serial.begin(9600);
}
void loop() {
tone(7,659,500);
Serial.print("123");
delay(500);
}