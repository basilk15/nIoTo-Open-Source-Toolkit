/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-microsd-card-arduino/
  
  This sketch can be found at: Examples > SD(esp32) > SD_Test
*/

#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include <WebServer.h> 

void listDir(fs::FS &fs, const char * dirname, uint8_t levels){
  Serial.printf("Listing directory: %s\n", dirname);

  File root = fs.open(dirname);
  if(!root){
    Serial.println("Failed to open directory");
    return;
  }
  if(!root.isDirectory()){
    Serial.println("Not a directory");
    return;
  }

  File file = root.openNextFile();
  while(file){
    if(file.isDirectory()){
      Serial.print("  DIR : ");
      Serial.println(file.name());
      if(levels){
        listDir(fs, file.name(), levels -1);
      }
    } else {
      Serial.print("  FILE: ");
      Serial.print(file.name());
      Serial.print("  SIZE: ");
      Serial.println(file.size());
    }
    file = root.openNextFile();
  }
}

void createDir(fs::FS &fs, const char * path){
  Serial.printf("Creating Dir: %s\n", path);
  if(fs.mkdir(path)){
    Serial.println("Dir created");
  } else {
    Serial.println("mkdir failed");
  }
}

void removeDir(fs::FS &fs, const char * path){
  Serial.printf("Removing Dir: %s\n", path);
  if(fs.rmdir(path)){
    Serial.println("Dir removed");
  } else {
    Serial.println("rmdir failed");
  }
}

void readFile(fs::FS &fs, const char * path){
  Serial.printf("Reading file: %s\n", path);

  File file = fs.open(path);
  if(!file){
    Serial.println("Failed to open file for reading");
    return;
  }

  Serial.print("Read from file: ");
  while(file.available()){
    Serial.print(file.read());
  }
  file.close();
}

void writeFile(fs::FS &fs, const char * path, const char * message){
  Serial.printf("Writing file: %s\n", path);

  File file = fs.open(path, FILE_WRITE);
  if(!file){
    Serial.println("Failed to open file for writing");
    return;
  }
  if(file.print(message)){
    Serial.println("File written");
  } else {
    Serial.println("Write failed");
  }
  file.close();
}

void appendFile(fs::FS &fs, const char * path, const char * message){
  Serial.printf("Appending to file: %s\n", path);

  File file = fs.open(path, FILE_APPEND);
  if(!file){
    Serial.println("Failed to open file for appending");
    return;
  }
  if(file.print(message)){
      Serial.println("Message appended");
  } else {
    Serial.println("Append failed");
  }
  file.close();
}

void renameFile(fs::FS &fs, const char * path1, const char * path2){
  Serial.printf("Renaming file %s to %s\n", path1, path2);
  if (fs.rename(path1, path2)) {
    Serial.println("File renamed");
  } else {
    Serial.println("Rename failed");
  }
}

void deleteFile(fs::FS &fs, const char * path){
  Serial.printf("Deleting file: %s\n", path);
  if(fs.remove(path)){
    Serial.println("File deleted");
  } else {
    Serial.println("Delete failed");
  }
}

void testFileIO(fs::FS &fs, const char * path){
  File file = fs.open(path);
  static uint8_t buf[512];
  size_t len = 0;
  uint32_t start = millis();
  uint32_t end = start;
  if(file){
    len = file.size();
    size_t flen = len;
    start = millis();
    while(len){
      size_t toRead = len;
      if(toRead > 512){
        toRead = 512;
      }
      file.read(buf, toRead);
      len -= toRead;
    }
    end = millis() - start;
    Serial.printf("%u bytes read for %u ms\n", flen, end);
    file.close();
  } else {
    Serial.println("Failed to open file for reading");
  }


  file = fs.open(path, FILE_WRITE);
  if(!file){
    Serial.println("Failed to open file for writing");
    return;
  }

  size_t i;
  start = millis();
  for(i=0; i<2048; i++){
    file.write(buf, 512);
  }
  end = millis() - start;
  Serial.printf("%u bytes written for %u ms\n", 2048 * 512, end);
  file.close();
}

int getLineCount(fs::FS &fs, const char * path) {
  int lineCount = 0;

  Serial.printf("Counting lines in file: %s\n", path);

  File file = fs.open(path);
  if (!file) {
    Serial.println("Failed to open file for reading");
    return -1;  // Return -1 to indicate an error opening the file
  }

  // Read the file line by line
  while (file.available()) {
    char c = file.read();
    if (c == '\n') {  // Newline character indicates the end of a line
      lineCount++;
    }
  }

  // If the file ends without a newline, ensure the last line is counted
  if (file.size() > 0 && file.read() == EOF) {
    lineCount++;  // Count the last line if there's no newline after it
  }

  file.close();
  return lineCount;
}

char* readLine(fs::FS &fs, const char * path, int lineNum){
  static char lineBuffer[512];  // Static buffer to hold the line (ensure it's big enough)
  memset(lineBuffer, 0, sizeof(lineBuffer));  // Clear the buffer

  Serial.printf("Reading line %d from file: %s\n", lineNum, path);

  File file = fs.open(path);
  if (!file) {
    Serial.println("Failed to open file for reading");
    return nullptr;  // Return null if file can't be opened
  }

  int currentLine = 1;
  int bufferIndex = 0;
  
  // Read the file line by line
  while (file.available()) {
    char c = file.read();
    if (c == '\n') {  // Newline character indicates the end of a line
      if (currentLine == lineNum) {
        lineBuffer[bufferIndex] = '\0';  // Null-terminate the string
        file.close();
        Serial.println("Sending read");
        return lineBuffer;  // Return the requested line as a char*
      }
      currentLine++;
      bufferIndex = 0;  // Reset the buffer index for the next line
      memset(lineBuffer, 0, sizeof(lineBuffer));  // Clear the buffer for the next line
    } else {
      // Add character to the buffer
      if (bufferIndex < sizeof(lineBuffer) - 1) {  // Avoid buffer overflow
        lineBuffer[bufferIndex++] = c;
      }
    }
  }

  // If the line was not found (file ended)
  if (currentLine < lineNum) {
    Serial.printf("File has only %d lines. Line %d not found.\n", currentLine - 1, lineNum);
  }

  file.close();
  Serial.println("Sending read none");
  return nullptr;  // Return null if the line is not found
}


void setupSD(){
  while (!SD.begin(5)){
    Serial.println("Card Mount Failed");
    lcdPrintAt(0, 0, "Card Fail");
    delay(1000);
  }
  uint8_t cardType = SD.cardType();

  if(cardType == CARD_NONE){
    Serial.println("No SD card attached");
    return;
  }

  Serial.print("SD Card Type: ");
  if(cardType == CARD_MMC){
    Serial.println("MMC");
  } else if(cardType == CARD_SD){
    Serial.println("SDSC");
  } else if(cardType == CARD_SDHC){
    Serial.println("SDHC");
  } else {
    Serial.println("UNKNOWN");
  }

  uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  Serial.printf("SD Card Size: %lluMB\n", cardSize);

  // listDir(SD, "/", 0);
  // createDir(SD, "/mydir");
  // listDir(SD, "/", 0);
  // removeDir(SD, "/mydir");
  // listDir(SD, "/", 2);
  // writeFile(SD, "/hello.txt", "Hello ");
  // appendFile(SD, "/hello.txt", "World!\n");
  // readFile(SD, "/hello.txt");
  // deleteFile(SD, "/foo.txt");
  // renameFile(SD, "/hello.txt", "/foo.txt");
  // readFile(SD, "/foo.txt");
  // testFileIO(SD, "/test.txt");
  // Serial.printf("Total space: %lluMB\n", SD.totalBytes() / (1024 * 1024));
  // Serial.printf("Used space: %lluMB\n", SD.usedBytes() / (1024 * 1024));
}

void saveData(fs::FS &fs, const char * path, const char * message){
  if (fs.exists(path)) appendFile(fs, path, message); //changed csv name here just to 2, 1 etc
  else writeFile(fs, path, message);
}
void getLastData(fs::FS &fs, const char * path){
  Serial.println(readLine(fs, path, getLineCount(fs, path)-1));
}

void saveImuData(fs::FS &fs, const char * message){
  if (fs.exists("/imu.csv")) appendFile(fs, "/imu.csv", message);
  else writeFile(fs, "/imu.csv", message);
}
void getLastImuData(fs::FS &fs){
  Serial.println(readLine(fs, "/imu.csv", getLineCount(fs, "/imu.csv")-1));
}

void saveInmpData(fs::FS &fs, const char * message){
  if (fs.exists("/inmp.csv")) appendFile(fs, "/inmp.csv", message);
  else writeFile(fs, "/inmp.csv", message);
}
void getLastInmpData(fs::FS &fs){
  Serial.println(readLine(fs, "/inmp.csv", getLineCount(fs, "/inmp.csv")-1));
}

//adding this function for js plotting
void handleChartJS(WebServer &server) {
  File file = SD.open("/const/chart.js", FILE_READ);
  if (!file) {
    server.send(404, "text/plain", "chart.js not found on SD");
    return;
  }
  server.streamFile(file, "application/javascript");
  file.close();
}
