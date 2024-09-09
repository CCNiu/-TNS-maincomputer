#include <HardwareSerial.h>
String receivedData = "";
void setup() {
  Serial2.begin(115200,SERIAL_8N1); // 設定波特率
  Serial.begin(115200); // 用於調試
}

void loop() {

  // Serial2.write("S00"); // 發送數據
  // Serial.println("S00"); // 發送數據
  
  if (Serial2.available()) {
    // 讀取數據，直到換行符結束
    receivedData = Serial2.readStringUntil('\n');
    // 顯示接收到的數據
    delay(10);
    Serial.print("Received: ");
    Serial.println(receivedData);
  }
  delay(10);
  if (Serial.available()) {
    // 讀取數據，直到換行符結束
    String dataToSend = Serial.readStringUntil('\n');
    delay(10);
    Serial2.print(dataToSend+'\n');
    // size_t temp = Serial2.print(dataToSend);
    Serial.println(dataToSend);
    // Serial.println(temp);
    //Serial.println(dataToSend.length());
  }
}