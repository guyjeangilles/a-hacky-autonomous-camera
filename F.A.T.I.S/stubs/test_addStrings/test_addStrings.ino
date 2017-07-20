void setup() {
  // put your setup code here, to run once:
int seconds = 10;
String stringOne = String(seconds);
String stringTwo = String(seconds)+ " Tits";

Serial.begin(9600);
Serial.print("Seconds: ");
Serial.println(seconds);

Serial.print("String One: ");
Serial.println(stringOne);

Serial.print("String Two: ");
Serial.println(stringTwo);
}

void loop() {
  // put your main code here, to run repeatedly:

}
