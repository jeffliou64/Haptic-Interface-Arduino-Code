//From bildr article: http://bildr.org/2012/08/rotary-encoder-arduino/
//these pins can not be changed 2/3 are special pins

#include <Wire.h>
int encoderPin1 = 2;
int encoderPin2 = 3;
const int pwmS1_1 = 4;
const int pwmS1_2 = 5;
const int pwmS2_1 = 6;
const int pwmS2_2 = 7;
int delaytime = 0;
int onflag1_2 = LOW;
int onflag1_1 = LOW;
int onflag2_2 = LOW;
int onflag2_1 = LOW;

int upperbound1 = 9999;
int lowerbound1 = -9999;
byte incomingByte = 0;
int upperbound2 = 9999;
int lowerbound2 = -9999;
byte incomingByte2 = 0;

volatile int lastEncoded = 0;
volatile long encoderValue = 0;
long lastencoderValue = 0;
int lockboth = 500;
int lastMSB = 0;
int lastLSB = 0;

volatile long slave_val = 0;
volatile long slave_encoderValue = 0;

void setup() {
  Wire.begin(8);
  Wire.onReceive(receiveEvent);
  Serial.begin (9600);
  pinMode(pwmS1_1, OUTPUT);
  pinMode(pwmS1_2, OUTPUT);
  pinMode(pwmS2_1, OUTPUT);
  pinMode(pwmS2_2, OUTPUT);
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
  digitalWrite(pwmS1_2, onflag1_2);
  digitalWrite(pwmS1_1, onflag1_1);
  digitalWrite(pwmS2_2, onflag2_2);
  digitalWrite(pwmS2_1, onflag2_1);
  delayMicroseconds(500);
  digitalWrite(pwmS1_2, LOW);
  digitalWrite(pwmS1_1, LOW);
  digitalWrite(pwmS2_2, LOW);
  digitalWrite(pwmS2_1, LOW);
  delayMicroseconds(delaytime);
    
  //Serial.print(encoderValue);
  //Serial.print("   ");
  //Serial.println(slave_encoderValue);
  
  
  if ((encoderValue) >= 400) {
    upperbound2 = slave_encoderValue+2;
    lowerbound2 = slave_encoderValue-2;
  }
//  else if ((slave_encoderValue + encoderValue) <= 800){
//    upperbound1 = 9999;
//    lowerbound1 = -9999;
//    upperbound2 = 9999;
//    lowerbound2 = -9999;
//  }


    if ((encoderValue >= lowerbound1) && (encoderValue <= upperbound1)) {
      onflag1_1 = LOW;
      onflag1_2 = LOW;
    }
    else if (encoderValue < lowerbound1 -1) {
      onflag1_1 = LOW;
      onflag1_2 = HIGH;
    }
    else if (encoderValue > upperbound1 +1) {
      onflag1_1 = HIGH;
      onflag1_2 = LOW;
    }

    if ((slave_encoderValue >= lowerbound2) && (slave_encoderValue <= upperbound2)) {
    onflag2_1 = LOW;
    onflag2_2 = LOW;
    }
    else if (slave_encoderValue < lowerbound2 -1) {
      onflag2_1 = LOW;
      onflag2_2 = HIGH;
    }
    else if (slave_encoderValue > upperbound2 +1) {
      onflag2_1 = HIGH;
      onflag2_2 = LOW;
    }
}

void receiveEvent(int howMany) {
  slave_val = Wire.read();
  if (slave_val == 0){
    slave_encoderValue = slave_encoderValue;
  }
  else if (slave_val == 1){
    slave_encoderValue++;
  }
  else if (slave_val == 2) {
    slave_encoderValue--;
  }
}

void updateEncoder() {
  int MSB = digitalRead(encoderPin1); //MSB = most significant bit
  int LSB = digitalRead(encoderPin2); //LSB = least significant bit

  int encoded = (MSB << 1) | LSB; //converting the 2 pin value to single number
  int sum  = (lastEncoded << 2) | encoded; //adding it to the previous encoded value

  if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) encoderValue ++;
  if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) encoderValue --;

  lastEncoded = encoded; //store this value for next time
}
