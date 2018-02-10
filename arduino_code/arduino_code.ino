#include <Encoder.h>
#include "PID_v1.h"

#define Encoder_A 2
#define Encoder_B 3
#define PWM_A 5
#define PWM_B 6
#define Addr1 9
#define Addr2 10
#define Addr3 11

long randNumber;
unsigned long last_time = 0;
double Input;
double Setpoint, Output;
double Kp = 0.1, Ki = 0.5, Kd = 0;
int Output1, Output2;
int DisplayTime = 500;
int MotorNumber = 0;
String inputString = "";
int pidmode = AUTOMATIC;
long oldPosition  = 0;
long newPosition = 0;

unsigned long thisTime = 0;
unsigned long lastTime = 0;
unsigned long printTime = 0;

Encoder myEnc(Encoder_A, Encoder_B);
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);


void setup() {

  Serial.begin(9600);
  last_time = millis();
  pinMode (PWM_A, OUTPUT);
  pinMode (PWM_B, OUTPUT);
  // no need to setup encoder a and b, they will be setup in the Encoder class
  pinMode (Addr1, INPUT);
  pinMode (Addr2, INPUT);
  pinMode (Addr3, INPUT);
  // make sure the output is limited to what the PWM is setup for
  myPID.SetOutputLimits(-255, 255);
  // turn on the pid
  myPID.SetMode(pidmode);
  MotorNumber = 1+ digitalRead(Addr1)+digitalRead(Addr2)*2+digitalRead(Addr3)*4;
  
  
}

void loop() {
  
  thisTime = millis();
  newPosition = myEnc.read();
  Input = (oldPosition - newPosition); // This will generate the difference in steps over the delay, this will be the velocity
  myPID.Compute();
  
  Output1 = Output>0 ? 0 : (int)-Output;
  Output2 = Output>0 ? (int)Output : 0;
  
  Output1 = abs(Setpoint)<0.1 ? 0 : Output1;
  Output2 = abs(Setpoint)<0.1 ? 0 : Output2;
  
  analogWrite(PWM_A, Output1);
  analogWrite(PWM_B, Output2);
  
   if (thisTime - printTime > DisplayTime) {
    Serial.print(MotorNumber);
    Serial.print(",");
    Serial.print(Input);
    Serial.print(",");
    Serial.print(Output);
    Serial.print(",");
    Serial.println(Setpoint);
    printTime = thisTime;
  }
  oldPosition = newPosition;
  lastTime = thisTime;
  delay(50);


}
void serialEvent() {
  static int temp = 0;
  if(Serial.available()>=2){
    unsigned char mode = Serial.read();
    unsigned char value = Serial.read();
    
    Serial.print("mode: ");
    Serial.print(mode);
    Serial.print(" , value: ");
    Serial.println(value);


    switch (mode) {
    case 0:
      Setpoint = value;
      break;
    case 1:
      Setpoint = -value;
      break;
    case 2:
      temp = 0;
      temp = value;
      temp = temp << 8;
      Setpoint = temp + Serial.read();
      break;
    case 3:
      temp = 0;
      temp = (unsigned char) Serial.read();
      temp = temp << 8;
      Setpoint = temp + (unsigned char) Serial.read();
      Setpoint = -Setpoint;
      break;
    };
  }

}




