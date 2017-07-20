/* Schematic
 *  5v to 10k resistor
 *  Arduino pin to 1k resistor
 *  1k resistor to photoresistor positive leg
 *  Led negative leg to GND
 */
int lightPin = A0; //pin of photo resistor
int lightVal = 0.0; //value of photo resistor

void setup() {
  //intiallize photo resistor and serial com
  pinMode(lightPin,INPUT);
  Serial.begin(9600);
}

void loop() {
  //blink the led every 500ms
  lightVal = analogRead(lightPin);
  Serial.println(lightVal);
}
