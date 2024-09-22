//for the ASIS Integration 


char talker[4]; // 用來存放 傳送者
char listener[4]; // 用來存放 接收者
char command[3]; // 用來存放 指令
void setup() {
  // put your setup code here, to run once:


}

void loop() {
  // put your main code here, to run repeatedly:
  printf("請輸入指令: ");
  scanf("%2s", command); // 讀取輸入的指令，最多兩個字元

  //切指令
  char input[] = "ASUDLSST"; // 原始輸入字串

  // 使用 strncpy 來複製字串的部分
  strncpy(talker, input, 3); // 複製前3個字元 ("ASU")
  talker[3] = '\0';          // 確保字串結尾有 null terminator

  strncpy(listener, input + 3, 3); // 複製從第4個字元開始的3個字元 ("DLS")
  listener[3] = '\0';              // 確保字串結尾有 null terminator

  strncpy(command, input + 6, 2); // 複製從第7個字元開始的2個字元 ("ST")
  command[2] = '\0';              // 確保字串結尾有 null terminator

  // 輸出結果
  printf("talker: %s\n", talker);
  printf("listener: %s\n", listener);
  printf("command: %s\n", command);

  //確認收訊者
  if (strcmp(listener, "ASU") == 0||strcmp(listener, "ALL") == 0){
    //
    if (strcmp(command, "ST") == 0) {
      printf("執行指令 ST\n");
    } 
    else if (strcmp(command, "QS") == 0) {
      printf("執行指令 QS\n");
    } 
    else if (strcmp(command, "GS") == 0) {
      printf("執行指令 GS\n");
    } 
    else if (strcmp(command, "RT") == 0) {
      printf("執行指令 RT\n");
    } 
    else if (strcmp(command, "DA") == 0) {
      printf("執行指令 DA\n");
    } 
    else if (strcmp(command, "ED") == 0) {
      printf("執行指令 ED\n");
    } 
    else if (strcmp(command, "FS") == 0) {
      printf("執行指令 FS\n");
    } 
    else {
    printf("無效的指令\n");
    }
  }
}


