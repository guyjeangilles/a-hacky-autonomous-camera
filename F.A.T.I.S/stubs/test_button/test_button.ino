/* Schmatic
 *  5v to 10k resistor
 *  Node: 10k resistor, pin wire to arduino, button
 *  Button leg to GND
 */
int buttonPin = 13;
int buttonState = 1; //intialize the buttonState as not pressed
void setup() {
  pinMode(buttonPin, INPUT);
  Serial.begin(9600);
}

void loop() {
  //display "0" when button is pressed and "1" otherwise
  buttonState = digitalRead(buttonPin);
  Serial.println(buttonState);
}
