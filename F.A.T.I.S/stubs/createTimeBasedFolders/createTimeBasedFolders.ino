/*
  SD card read/write

 This example shows how to read and write data to and from an SD card file
 The circuit:
 * SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 10 (for MKRZero SD: SDCARD_SS_PIN)

 created   Nov 2010
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe
 modified 8 Jul 2017
 by Guyrandy Jean-Gilles

 This example code is in the public domain.

 */

//include required libraries
#include <SPI.h>
#include <SD.h>

//define chip select pin and power source pin on arduino
#define SD_CHIP_SELECT 10
int sdPowerPin = 6;

//intialize variables used to track time elapsed
unsigned long runMillis= 0;
unsigned long allSeconds=0;
int runHours= 0;
int secsRemaining=0;
int runMinutes=0;
int runSeconds=0;

File myFile; // create file variable
String folderName = "";

//open serial communitcaiton and turn on SD module
void setup() {
  pinMode(sdPowerPin,OUTPUT);
  digitalWrite(sdPowerPin,HIGH);
  Serial.begin(9600);

  while(!Serial){
    ;//wait for serial port to connect
  }

  //check if the SD module is properly attached and working
  Serial.print("Initializing SD card...");
  if (!SD.begin(SD_CHIP_SELECT)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");

  //calculate time elapsed since the sketch started running
  runMillis = millis();
  allSeconds=millis()/1000;
  runHours= allSeconds/3600;
  secsRemaining=allSeconds%3600;
  runMinutes=secsRemaining/60;
  runSeconds=secsRemaining%60;

  folderName = String(runHours) + "_" + String(runMinutes) + "_" + String(runSeconds);
  Serial.print("Folder Name: ");
  Serial.println(folderName);
  Serial.println("");

  if(SD.mkdir(folderName)){
    SD.mkdir(folderName);
    Serial.print("Made folder named: ");
    Serial.println(folderName);
  }
  else{
    Serial.println("Failed to make new directory.");
  }
}

void loop() {
  


  //folderName =  String(runHours + '_' + runMinutes + '_' + runSeconds);
  //Serial.println(folderName);
  //folderName = "";

//  folderName = String(runHours) + "_" + String(runMinutes) + "_" + String(runSeconds);
//  Serial.print("Folder Name: ");
//  Serial.println(folderName);
//  Serial.println("");
//  
//  Serial.print("Seconds: ");
//  Serial.println(runSeconds);
//  Serial.print("Minutess: ");
//  Serial.println(runMinutes);
//  Serial.print("Hours: ");
//  Serial.println(runHours);
//  Serial.println("");
  

}
