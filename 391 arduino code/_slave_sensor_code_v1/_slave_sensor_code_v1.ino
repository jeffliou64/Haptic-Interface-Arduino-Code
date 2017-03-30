#include <Wire.h>
int encoderPin1 = 2;
int encoderPin2 = 3;

volatile int lastEncoded = 0;
volatile byte encoderValue = 0;
long lastencoderValue = 0;
int encodeflag = 0;
int change = 0;

int lastMSB = 0;
int lastLSB = 0;

void setup() {
  Wire.begin();
  
  Serial.begin (9600);
  pinMode(encoderPin1, INPUT);
  pinMode(encoderPin2, INPUT);

  digitalWrite(encoderPin1, HIGH); //turn pullup resistor on
  digitalWrite(encoderPin2, HIGH); //turn pullup resistor on

  //call updateEncoder() when any high/low changed seen
  //on interrupt 0 (pin 2), or interrupt 1 (pin 3)
  attachInterrupt(0, updateEncoder, CHANGE);
  attachInterrupt(1, updateEncoder, CHANGE);
}

void loop() {
  Wire.beginTransmission(8);
  if(change == 0){
    Wire.write(0);
  }
  else if (encodeflag == 1) {
    Wire.write(1);
    change = 0;
  }
  else if (encodeflag == 2) {
    Wire.write(2);
    change = 0;
  }
  Wire.endTransmission(); 
  //encodeflag = 0;
  //Serial.println(encoderValue);
}

void updateEncoder() {
  int MSB = digitalRead(encoderPin1); //MSB = most significant bit
  int LSB = digitalRead(encoderPin2); //LSB = least significant bit
  int encoded = (MSB << 1) | LSB; //converting the 2 pin value to single number
  int sum  = (lastEncoded << 2) | encoded; //adding it to the previous encoded value
  

  change = 1;

  if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) {
    encoderValue ++;
    encodeflag = 1;
  }
  if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) {
    encoderValue --;
    encodeflag = 2;
  }
  lastEncoded = encoded; //store this value for next time
}
