//From bildr article: http://bildr.org/2012/08/rotary-encoder-arduino/
//int angle90 = 40;   //40,40
//int angle180 = 67;  //67,67
//int angle360 = 116; //116,111
//these pins can not be changed 2/3 are special pins
int encoderPin1 = 2;
int encoderPin2 = 3;
//const int pwmS1 = 5;
//const int pwmS2 = 6;
//int delayseconds = 500E-6;
//int speedduration = 1000;
//int stopduration = 10000;
//int count = 0;
//int count2 = 0;
int posnegflag = LOW;

volatile int lastEncoded = 0;
volatile long encoderValue = 0;

long lastencoderValue = 0;

int lastMSB = 0;
int lastLSB = 0;

void setup() {
  Serial.begin (9600);
  //pinMode(pwmS1, OUTPUT);
  //pinMode(pwmS2, OUTPUT);
  pinMode(encoderPin1, INPUT); 
  pinMode(encoderPin2, INPUT);

  digitalWrite(encoderPin1, HIGH); //turn pullup resistor on
  digitalWrite(encoderPin2, HIGH); //turn pullup resistor on

  //call updateEncoder() when any high/low changed seen
  //on interrupt 0 (pin 2), or interrupt 1 (pin 3) 
  attachInterrupt(0, updateEncoder, CHANGE); 
  attachInterrupt(1, updateEncoder, CHANGE);

}

void loop(){
//  if (count < speedduration && posnegflag == LOW) {
//    digitalWrite(pwmS1, HIGH);
//    digitalWrite(pwmS2, LOW);
//    count++;
//  }
//  else if (count2 < stopduration && posnegflag == HIGH) {
//    digitalWrite(pwmS1, LOW); 
//    count2++;
//  }
//  else if (count >= speedduration || count2 >= stopduration) {
//    count = 0;
//    count2 = 0;
//    posnegflag = ~posnegflag;
//  }
  //Serial.println(posnegflag);
  Serial.println(encoderValue);
}


void updateEncoder(){
  int MSB = digitalRead(encoderPin1); //MSB = most significant bit
  int LSB = digitalRead(encoderPin2); //LSB = least significant bit

  int encoded = (MSB << 1) |LSB; //converting the 2 pin value to single number
  int sum  = (lastEncoded << 2) | encoded; //adding it to the previous encoded value

  if(sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) encoderValue ++;
  if(sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) encoderValue --;
  
  lastEncoded = encoded; //store this value for next time
}
