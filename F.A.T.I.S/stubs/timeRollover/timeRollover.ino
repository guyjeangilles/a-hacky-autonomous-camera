unsigned long runMillis= 0;
unsigned long allSeconds=0;
int runHours= 0;
int secsRemaining=0;
int runMinutes=0;
int runSeconds=0;

void setup() {
  Serial.begin(9600);
  
}

void loop() {
runMillis= millis();
allSeconds=millis()/1000;
runHours= allSeconds/3600;
secsRemaining=allSeconds%3600;
runMinutes=secsRemaining/60;
runSeconds=secsRemaining%60;
  
/*
unsigned long runMillis= millis();
unsigned long allSeconds=millis()/1000;
int runHours= allSeconds/3600;
int secsRemaining=allSeconds%3600;
int runMinutes=secsRemaining/60;
int runSeconds=secsRemaining%60;
*/
char buf[21];
sprintf(buf,"Runtime%02d:%02d:%02d",runHours,runMinutes,runSeconds);
Serial.println(buf);
delay(1000);
}


