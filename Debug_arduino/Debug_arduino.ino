void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

}

void loop() {
  if(Serial.available()){
    Serial.print((char)Serial.read());
  }
  // put your main code here, to run repeatedly: 
  
}
