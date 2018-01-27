long randNumber;
unsigned long last_time = 0;
int input, output, setpoint;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  last_time = millis();
}

void loop() {
  if (millis() - last_time > 500) {
    Serial.print(0);
    Serial.print(",");
    Serial.print(input);
    Serial.print(",");
    Serial.print(1);
    Serial.print(",");
    Serial.print(output);
    Serial.print(",");
    Serial.print(2);
    Serial.print(",");
    Serial.print(setpoint);
    Serial.println(" ");
    last_time = millis();
  }


}
void serialEvent() {
  unsigned char mode = Serial.read();
  switch (mode) {
    case 0:
      input = (unsigned char) Serial.read();
      break;
    case 1:
      input = -(unsigned char) Serial.read();
      break;
    case 2:
      setpoint = 0;
      setpoint = (unsigned char) Serial.read();
      setpoint = setpoint << 8;
      setpoint = setpoint + (unsigned char) Serial.read();
      break;
    case 3:
      setpoint = 0;
      setpoint = (unsigned char) Serial.read();
      setpoint = setpoint << 8;
      setpoint = setpoint + (unsigned char) Serial.read();
      setpoint = -setpoint;
      break;
  };

}

