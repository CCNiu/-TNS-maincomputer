// void setup() {
//   // 初始化串口
//   Serial.begin(9600); // 用於監控
//   Serial1.begin(9600); // 用於UART通訊

//   // 等待串口初始化穩定
//   delay(2000);

//   // 確認串口初始化
//   Serial.println("UART Communication Initialized");
// }

// void loop() {
//   // 檢查是否有來自Serial1的數據
//   if (Serial1.available()) {
//     // 讀取數據，直到換行符結束
//     String receivedData = Serial1.readStringUntil('\n');
    
//     // 顯示接收到的數據
//     Serial.print("Received: ");
//     Serial.println(receivedData);

//     // 回應數據
//     Serial1.print("Data received: ");
//     Serial1.println(receivedData);
//   }

//   // 如果從串口監控器輸入數據，將其發送到Serial1
//   if (Serial.available()) {
//     // 讀取數據，直到換行符結束
//     String dataToSend = Serial.readStringUntil('\n');
//     Serial1.print(dataToSend);
//     Serial.print("KOKOK");
//     Serial1.println(); // 添加換行符
//   }
// }
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
    // Serial.println("2");
    // 顯示接收到的數據
    Serial.print("Received: ");
    Serial.println(receivedData);
  }
  delay(1000);
  if (Serial.available()) {
    // 讀取數據，直到換行符結束
    String dataToSend = Serial.readStringUntil('\n');
    Serial2.print(dataToSend);
    Serial.print(dataToSend);
  }
}