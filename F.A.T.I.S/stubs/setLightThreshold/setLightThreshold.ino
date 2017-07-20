/* Schematic
 * //Photo Resitor
 *  5v to 10k resistor
 *  Arduino pin to 1k resistor
 *  10k resistor to photoresistor positive leg
 *  Led negative leg to GND
 *  
 *  //Button
 *  5v to 10k resistor
 *  Node: 10k resistor, pin wire to arduino, button
 *  Button leg to GND
 */
int lightPin = A0; //pin of photo resistor
int lightVal = 0.0; //value of photo resistor
int lightThreshold = 1000; // initialize light threshold

int buttonPin = 2; // pin of button
int buttonState = 1; //intialize the buttonState as not pressed

//Intialize pins and serial com
void setup() {
  pinMode(lightPin,INPUT);
  pinMode(buttonPin, INPUT);
  
  Serial.begin(9600);
  Serial.println("Starting...");
}

void loop() {
  //read sensor values
  lightVal = analogRead(lightPin);
  buttonState = digitalRead(buttonPin);

  // when the button is pressed update the light threshold
  if (buttonState == 0){
    while(buttonState == 0){
      //wait until the buttons isn't pressed anymore
      buttonState = digitalRead(buttonPin);
    }
    Serial.println("Button Pressed");
    Serial.print("Old Light Threshold: ");
    Serial.println(lightThreshold);
    
    lightThreshold = lightVal;

    Serial.print("New Threshold: ");
    Serial.println(lightThreshold);
  }  
  else
  {
    //Serial.print(".");
  }
}
