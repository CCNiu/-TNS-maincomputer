//M7 is responsible for data processing and calculation, 
//classifying the data collected by PICO and changing it into a JSON-like format, 
//and executing foul sound effects, starting signals, etc. 
//according to the instructions from M4.

#include <Arduino_AdvancedAnalog.h>
#include <DigitalOut.h>
#include <Arduino_USBHostMbed5.h>
#include <FATFileSystem.h>
#include <RPC.h>

AdvancedDAC dac0(A12);
USBHostMSD msd;
mbed::FATFileSystem usb("USB_DRIVE");
FILE * file = nullptr;
int sample_size = 0;
int samples_count = 0;


char murmur = 'Z';
int ready = 0;

// WAV header 結構
struct wav_header_t {
  char chunkID[4];
  unsigned long chunkSize;
  char format[4];
  char subchunk1ID[4];
  unsigned long subchunk1Size;
  unsigned short audioFormat;
  unsigned short numChannels;
  unsigned long sampleRate;
  unsigned long byteRate;
  unsigned short blockAlign;
  unsigned short bitsPerSample;
};
struct chunk_t {
  char ID[4];
  unsigned long size;
};
void setup() {
  // put your setup code here, to run once:
  RPC.begin(); //boot M4
  Serial2.begin(115200,SERIAL_8N1); // set baud rate to PICO D18 D19
  
  //audio
  while (!Serial);

  pinMode(PA_15, OUTPUT);
  digitalWrite(PA_15, HIGH);

  if (!initializeUSB()) return;

  wav_header_t header;
  if (!readWavHeader(header)) return;

  chunk_t chunk;
  if (!findDataChunk(chunk)) return;

  sample_size = header.bitsPerSample / 8;
  samples_count = chunk.size * 8 / header.bitsPerSample;

  if (!dac0.begin(AN_RESOLUTION_12, header.sampleRate * 2, 256, 16)) {
    Serial.println("Failed to start DAC1 !");
    return;
  }
}


//core_0 m7 do thinking
void loop(){
  // String talker = RPC.call("getTalker");
  // String listener = RPC.call("getListener");
  // String command = RPC.call("getCommand");
  delay(1); 
  // executeCommand(talker,listener,command);

  switch (murmur) {
    case 'S':
      Serial.println("啟動功能：ST");
      RPC.call("sendtopico","S00");
      // sendtopico("S00");
      break;
    case 'Q':
      Serial.println("啟動功能：QS");
      // if without ADLS
      // if(ready == 1){
        
      //   sendtooutside("RD");
      // }
      break;
    case 'G':
      Serial.println("啟動功能：GS");
      // 問大家好不好
      break;
    case 'R':
      Serial.println("啟動功能：RT");
      RPC.call("sendtopico","c00");
      // sendtopico("C00");
      break;
    case 'D':
      Serial.println("啟動功能：DA");
      // 在這裡執行指令 'DA' 對應的動作
      RPC.call("sendtopico","D00");
      // sendtopico("D00");
      break;
    case 'F':
      Serial.println("啟動功能：FS");
      tone(7,659,500);//給裁判
      playAudio();
      break;
    case 'Z':
    default:
      Serial.println("無效或無動作");
      // 對於無效或未指定的 murmur，這裡執行預設動作
      break;
  }
}

void executeCommand(const char* talker, const char* listener, const char* command) {
  if (strcmp(listener, "ASU") == 0 || strcmp(listener, "ALL") == 0) {
    if (strcmp(command, "ST") == 0) {
      Serial.println("執行指令 ST");
      murmur = 'S';
    } else if (strcmp(command, "QS") == 0) {
      Serial.println("執行指令 QS");
      murmur = 'Q';
    } else if (strcmp(command, "GS") == 0) {
      Serial.println("執行指令 GS");
      murmur = 'G';
    } else if (strcmp(command, "RT") == 0) {
      Serial.println("執行指令 RT");
      murmur = 'R';
    } else if (strcmp(command, "DA") == 0) {
      Serial.println("執行指令 DA");
      murmur = 'D';
    } else if (strcmp(command, "FS") == 0) {
      Serial.println("執行指令 FS");
      murmur = 'F';
    } else {
      Serial.println("無效的指令");
      murmur = 'Z';
    }
  }
}

bool initializeUSB() {
  Serial.println("Please connect a USB stick to the GIGA's USB port ...");
  while (!msd.connect()) delay(100);

  Serial.println("Mounting USB device ...");
  int rc_mount = usb.mount(&msd);
  if (rc_mount) {
    Serial.print("Error mounting USB device: ");
    Serial.println(rc_mount);
    return false;
  }
  return true;
}

// 讀取 WAV 檔案 Header
bool readWavHeader(wav_header_t &header) {
  file = fopen("/USB_DRIVE/AUDIO_SAMPLE.wav", "rb");
  if (file == nullptr) {
    Serial.print("Error opening audio file: ");
    Serial.println(strerror(errno));
    return false;
  }

  Serial.println("Reading audio header ...");
  fread(&header, sizeof(header), 1, file);

  // 顯示 Header 資訊
  char msg[64];
  snprintf(msg, sizeof(msg), "File Type: %s", header.chunkID); Serial.println(msg);
  snprintf(msg, sizeof(msg), "File Size: %ld", header.chunkSize); Serial.println(msg);
  snprintf(msg, sizeof(msg), "WAV Marker: %s", header.format); Serial.println(msg);
  snprintf(msg, sizeof(msg), "Number of Channels: %hd", header.numChannels); Serial.println(msg);
  snprintf(msg, sizeof(msg), "Sample Rate: %ld", header.sampleRate); Serial.println(msg);
  snprintf(msg, sizeof(msg), "Bits per Sample: %hd", header.bitsPerSample); Serial.println(msg);

  return true;
}

// 尋找 WAV 檔案中的 Data 區塊
bool findDataChunk(chunk_t &chunk) {
  char msg[64];
  Serial.println("Finding data chunk...");
  while (true) {
    fread(&chunk, sizeof(chunk), 1, file);
    snprintf(msg, sizeof(msg), "%c%c%c%c\t%li", chunk.ID[0], chunk.ID[1], chunk.ID[2], chunk.ID[3], chunk.size);
    Serial.println(msg);
    if (*(unsigned int *)&chunk.ID == 0x61746164) {  // "data" chunk
      break;
    }
    fseek(file, chunk.size, SEEK_CUR);  // Skip 非 data 區塊
  }
  return true;
}

// 播放音樂
void playAudio() {
  if (dac0.available() && !feof(file)) {
    uint16_t sample_data[256] = {0};
    fread(sample_data, sample_size, 256, file);

    SampleBuffer buf = dac0.dequeue();
    for (size_t i = 0; i < buf.size(); i++) {
      uint16_t dac_val = ((static_cast<unsigned int>(sample_data[i]) + 32768) >> 4) & 0x0fff;
      buf[i] = dac_val;
    }
    dac0.write(buf);
  }
}
