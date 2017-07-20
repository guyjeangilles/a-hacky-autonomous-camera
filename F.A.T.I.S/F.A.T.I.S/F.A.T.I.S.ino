// ArduCAM demo (C)2017 Lee
// Web: http://www.ArduCAM.com
// This program is a demo of how to use most of the functions
// of the library with a supported camera modules, and can run on any Arduino platform.
//
// This demo was made for Omnivision 2MP/5MP sensor.
// It will run the ArduCAM 2MP/5MP as a real 2MP/5MP digital camera, provide both JPEG capture.
// The demo sketch will do the following tasks:
// 1. Set the sensor to JPEG mode.
// 2. Capture and buffer the image to FIFO every 5 seconds 
// 3. Store the image to Micro SD/TF card with JPEG format in sequential.
// 4. Resolution can be changed by myCAM.set_JPEG_size() function.
// This program requires the ArduCAM V4.0.0 (or later) library and ArduCAM 2MP/5MP shield
// and use Arduino IDE 1.6.8 compiler or above

//include all the appropiate files/libraries
#include <ArduCAM.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include "memorysaver.h"

//This demo can only work on OV2640_MINI_2MP or OV5642_MINI_5MP or OV5642_MINI_5MP_BIT_ROTATION_FIXED platform.
#if !(defined OV5642_MINI_5MP || defined OV5642_MINI_5MP_BIT_ROTATION_FIXED || defined OV2640_MINI_2MP || defined OV3640_MINI_3MP)
  #error Please select the hardware platform and camera module in the ../libraries/ArduCAM/memorysaver.h file
#endif

//intialize pin variables
#define SD_CS 10
const int SPI_CS = 7;
int sdPowerPin = 6;
int redLed = 2;
int greenLed = 3;
int blueLed = 4;

//intialize variables used to track time elapsed
unsigned long runMillis= 0;
unsigned long allSeconds=0;
int runHours= 0;
int secsRemaining=0;
int runMinutes=0;
int runSeconds=0;

//int lightPin = A0; //pin of photo resistor
//int lightVal = 0.0; //value of photo resistor
//int lightThreshold = 1000; // initialize light threshold

//int buttonPin = 2; // pin of button
//int buttonState = 1; //intialize the buttonState as not pressed

//create ArduCam variable
#if defined (OV2640_MINI_2MP)
  ArduCAM myCAM( OV2640, SPI_CS );
#elif defined (OV3640_MINI_3MP)
  ArduCAM myCAM( OV3640, SPI_CS );
#else
  ArduCAM myCAM( OV5642, SPI_CS );
#endif

/*********************************************************************************************************************************
 * Define image writing function
 ********************************************************************************************************************************/
void myCAMSaveToSDFile(){
  //white to signal capture process started
  digitalWrite(redLed,HIGH);
  digitalWrite(greenLed,HIGH);
  digitalWrite(blueLed,HIGH);
  
  //intialize variables  
  char str[8];
  byte buf[256];
  static int i = 0;
  static int k = 0;
  uint8_t temp = 0,temp_last=0;
  uint32_t length = 0;
  bool is_header = false;
  File outFile;
  String fileName = "";
  
  //Flush the FIFO
  myCAM.flush_fifo();
  
  //Clear the capture done flag
  myCAM.clear_fifo_flag();
  
  //Start capture
  myCAM.start_capture();
  Serial.println(F("start Capture"));

  unsigned long captureTime = millis();
  unsigned long captureStart = captureTime;
  while(!myCAM.get_bit(ARDUCHIP_TRIG , CAP_DONE_MASK)){
    captureTime = millis();
    //If it takes longer than 5 seconds to take a photo break out of the funciton
    if (captureTime - captureStart > 10000){
      Serial.println("Image Capture taking too long. Exiting Function");
        //cyan to show force out of capture
        digitalWrite(redLed,LOW);
        digitalWrite(greenLed,HIGH);
        digitalWrite(blueLed,HIGH);
      return;
    }
  }
  Serial.println(F("Capture Done."));  
  
  length = myCAM.read_fifo_length();
  Serial.print(F("The fifo length is :"));
  Serial.println(length, DEC);
  
  if (length >= MAX_FIFO_SIZE) //384K
  {
    Serial.println(F("Over size."));
    length = 384000;
    //return ;
  }
  if (length == 0 ) //0 kb
  {
    Serial.println(F("Size is 0."));
    length = 384000;
    //return ;
  }
  
  //Construct a file name
//  k = k + 1;
//  itoa(k, str, 10);
//  strcat(str, ".jpg");

//calculate time elapsed since the sketch started running
  runMillis = millis();
  allSeconds=millis()/1000;
  runHours= allSeconds/3600;
  secsRemaining=allSeconds%3600;
  runMinutes=secsRemaining/60;
  runSeconds=secsRemaining%60;

  fileName = String(runHours) + "_" + String(runMinutes) + "_" + String(runSeconds) + ".jpg";

  
  //Open the new file
  //digitalWrite(sdPowerPin,HIGH);
  //Serial.println("Turned On SD Card");
  //delay(5000);
  //outFile = SD.open(str, O_WRITE | O_CREAT | O_TRUNC);
  outFile = SD.open(fileName, O_WRITE | O_CREAT | O_TRUNC);
  if(!outFile){
    Serial.println(F("File open failed"));
    //digitalWrite(sdPowerPin,LOW);
    //Serial.println("Turned Off SD Card");
    return;
  }
  
  
  myCAM.CS_LOW();
  myCAM.set_fifo_burst();
  while ( length-- )
  {
    temp_last = temp;
    temp =  SPI.transfer(0x00);
    
    //Read JPEG data from FIFO
    if ( (temp == 0xD9) && (temp_last == 0xFF) ) //If find the end ,break while,
    {
      buf[i++] = temp;  //save the last  0XD9     
      //Write the remain bytes in the buffer
      myCAM.CS_HIGH();
      outFile.write(buf, i);    
      //Close the file
      outFile.close();
      //Green to signal image write ok
        digitalWrite(redLed,LOW);
        digitalWrite(greenLed,HIGH);
        digitalWrite(blueLed,LOW);
      Serial.println(F("Image save OK."));
      is_header = false;
      i = 0;
    }  
    if (is_header == true)
    { 
      //Write image data to buffer if not full
      if (i < 256)
      buf[i++] = temp;
      else
      {
        //Write 256 bytes image data to file
        myCAM.CS_HIGH();
        outFile.write(buf, 256);
        i = 0;
        buf[i++] = temp;
        myCAM.CS_LOW();
        myCAM.set_fifo_burst();
      }        
    }
    else if ((temp == 0xD8) & (temp_last == 0xFF))
    {
      is_header = true;
      buf[i++] = temp_last;
      buf[i++] = temp;   
    } 
  } 
}
/*********************************************************************************************************************************
 * Begin void setup()
 ********************************************************************************************************************************/
void setup(){
uint8_t vid, pid;
uint8_t temp;
Wire.begin();
Serial.begin(115200);
Serial.println(F("ArduCAM Start!"));

//set pin modes
pinMode(SPI_CS,OUTPUT);
//pinMode(lightPin,INPUT);
//pinMode(buttonPin, INPUT);
pinMode(redLed,OUTPUT);
pinMode(greenLed,OUTPUT);
pinMode(blueLed,OUTPUT);
//make LED red to show nothing Initialized yet
  digitalWrite(redLed,HIGH);
  digitalWrite(greenLed,LOW);
  digitalWrite(blueLed,LOW);
pinMode(sdPowerPin,OUTPUT);
digitalWrite(sdPowerPin,HIGH);



// initialize SPI:
SPI.begin();
while(1){
  //Check if the ArduCAM SPI bus is OK
  myCAM.write_reg(ARDUCHIP_TEST1, 0x55);
  temp = myCAM.read_reg(ARDUCHIP_TEST1);
  
  if (temp != 0x55){
    Serial.println(F("SPI interface Error!"));
    delay(1000);continue;
    //yellow LED to signify camera error
  digitalWrite(redLed,HIGH);
  digitalWrite(greenLed,HIGH);
  digitalWrite(blueLed,LOW);
  }else{
    Serial.println(F("SPI interface OK."));break;
  //magenta led to show SPI interface OK
  digitalWrite(redLed,HIGH);
  digitalWrite(greenLed,LOW);
  digitalWrite(blueLed,HIGH);
  }
}

//Initialize SD Card
//digitalWrite(sdPowerPin,HIGH);
while(!SD.begin(SD_CS)){
  Serial.println(F("SD Card Error!"));delay(1000);
  //red led to show SD card error
  digitalWrite(redLed,HIGH);
  digitalWrite(greenLed,LOW);
  digitalWrite(blueLed,LOW);
}
Serial.println(F("SD Card detected."));
  //Blue to show SD Card detected and Camera detected
  digitalWrite(redLed,LOW);
  digitalWrite(greenLed,LOW);
  digitalWrite(blueLed,HIGH);
//digitalWrite(sdPowerPin,LOW);

#if defined (OV2640_MINI_2MP)
  while(1){
    //Check if the camera module type is OV2640
    myCAM.wrSensorReg8_8(0xff, 0x01);
    myCAM.rdSensorReg8_8(OV2640_CHIPID_HIGH, &vid);
    myCAM.rdSensorReg8_8(OV2640_CHIPID_LOW, &pid);
    if ((vid != 0x26 ) && (( pid != 0x41 ) || ( pid != 0x42 ))){
      Serial.println(F("Can't find OV2640 module!"));
      delay(1000);continue;
    }
    else{
      Serial.println(F("OV2640 detected."));break;
    } 
  }
#elif defined (OV3640_MINI_3MP)
  while(1){
    //Check if the camera module type is OV3640
    myCAM.rdSensorReg16_8(OV3640_CHIPID_HIGH, &vid);
    myCAM.rdSensorReg16_8(OV3640_CHIPID_LOW, &pid);
    if ((vid != 0x36) || (pid != 0x4C)){
      Serial.println(F("Can't find OV3640 module!"));
      delay(1000);continue; 
    }else{
      Serial.println(F("OV3640 detected."));break;    
    }
 } 
#else
  while(1){
    //Check if the camera module type is OV5642
    myCAM.wrSensorReg16_8(0xff, 0x01);
    myCAM.rdSensorReg16_8(OV5642_CHIPID_HIGH, &vid);
    myCAM.rdSensorReg16_8(OV5642_CHIPID_LOW, &pid);
    if((vid != 0x56) || (pid != 0x42)){
      Serial.println(F("Can't find OV5642 module!"));
      delay(1000);continue;
    }
    else{
      Serial.println(F("OV5642 detected."));break;
    } 
  }
#endif
myCAM.set_format(JPEG);
myCAM.InitCAM();
#if defined (OV2640_MINI_2MP)
  myCAM.OV2640_set_JPEG_size(OV2640_320x240);
  //myCAM.OV2640_set_JPEG_size(OV2640_1280x1024);
  //myCAM.OV2640_set_JPEG_size(OV2640_1600x1200);
#elif defined (OV3640_MINI_3MP)
  myCAM.OV3640_set_JPEG_size(OV3640_320x240);
#else
  myCAM.write_reg(ARDUCHIP_TIM, VSYNC_LEVEL_MASK);   //VSYNC is active HIGH
  myCAM.OV5642_set_JPEG_size(OV5642_320x240);
#endif
delay(1000);
}
/*********************************************************************************************************************************
 * Begin void loop()
 ********************************************************************************************************************************/
void loop(){
delay(1000);//for some reason this is the best delay time
myCAMSaveToSDFile();

//  //read sensor values
//  lightVal = analogRead(lightPin);
//  buttonState = digitalRead(buttonPin);
//  
//  // update the light threshold if the button is pressed
//  if (buttonState == 0){
//  while(buttonState == 0){
//      //wait until the buttons isn't pressed anymore
//      buttonState = digitalRead(buttonPin);
//    }
//    Serial.println("Button Pressed");
//    Serial.print("Old Light Threshold: ");
//    Serial.println(lightThreshold);
//    
//    lightThreshold = lightVal;
//
//    Serial.print("New Threshold: ");
//    Serial.println(lightThreshold);
//  }  
//  
//  delay(1000);
//  //take a picture if it's bright enough
//  if (lightVal < lightThreshold){
//    myCAMSaveToSDFile();
//    //Serial.println("Smile for the camera");
//    //Serial.print("The Light Value is: ");
//    //Serial.println(lightVal);
//    //Serial.print("The Light Threshold is: ");
//    //Serial.println(lightThreshold);
//  } else {
//    Serial.println("Too Dark, Door is closed");
//  }
}


