//M4 is responsible for communicating with ADLS and RPi5, 
//cutting the instructions and passing them to M7 using RPC.

#include <HardwareSerial.h>
#include <RPC.h>
String receivedData_outside = "";//example ALSASUST
char talker[4]; // 用來存放 傳送者
char listener[4]; // 用來存放 接收者
char command[3]; // 用來存放 指令

//ASUALLST
//ASU DLS DA 0026 00100040 EDEDED
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial1.begin(115200,SERIAL_8N1); // set baud rate to ADLS D1  D2
  Serial3.begin(115200,SERIAL_8N1); // set baud rate to RPi5 D16 D17

  RPC.bind("getTalker", getTalker);
  RPC.bind("getListener", getListener);
  RPC.bind("getCommand", getCommand);

  RPC.bind("sendtoADLS", sendtoADLS);
  RPC.bind("sendtopico", sendtopico);
  RPC.bind("sendtoRPI5", sendtoRPI5);

}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available()) { 
    receivedData_outside = Serial.readStringUntil('\n');  // 讀取來自 Serial 的輸入
    receivedData_outside.trim();  // 去除首尾空白

    //切指令
    // 使用 strncpy 來複製字串的部分
    if (receivedData_outside.length() >= 8) {
      strncpy(talker, receivedData_outside.c_str(), 3);
      talker[3] = '\0';

      strncpy(listener, receivedData_outside.c_str() + 3, 3);
      listener[3] = '\0';

      strncpy(command, receivedData_outside.c_str() + 6, 2);
      command[2] = '\0';
    } else {
      Serial.println("Error: Received data is too short.");
    }
    // 輸出結果
    Serial.println("talker: " + String(talker));
    Serial.println("listener: " + String(listener));
    Serial.println("command: " + String(command));
  }
}
// RPC用來訪問變數的getter函數
String getTalker() {
  return String(talker);
}

String getListener() {
  return String(listener);
}

String getCommand() {
  return String(command);
}

void sendToDevice(HardwareSerial &deviceSerial, const char* message) {
  String dataToSend = String(message) + '\n';
  delay(10);
  deviceSerial.print(dataToSend);
  Serial.println(dataToSend); // 調試用輸出
}

void sendtoADLS(const char* message) {
    sendToDevice(Serial1, message);
}

void sendtopico(const char* message) {
    sendToDevice(Serial2, message);
}

void sendtoRPI5(const char* message) {
    sendToDevice(Serial3, message);
}
