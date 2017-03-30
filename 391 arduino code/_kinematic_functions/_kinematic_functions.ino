#include <stdio.h>
#include <math.h>

const float pi = 3.14159265359;
const float d1 = 0.70710678;
const float d2 = 0.70710678;
volatile float x = 0;
volatile float y = 0;
volatile float theta1 = 45.0;
volatile float theta2 = 0.0;

void setup() {
  Serial.begin(9600);
  Serial.print(d1);
  Serial.print(" ");
  Serial.print(d2);
  Serial.print(" ");
  Serial.print(x);
  Serial.print(" ");
  Serial.print(y);
  Serial.print(" ");
  Serial.print(theta1);
  Serial.print(" ");
  Serial.println(theta2);
}

void loop() {
  //theta2 = inverseKin2(x, y);
  //theta1 = inverseKin1(x, y, theta2);
  //Serial.print(theta1);
  //Serial.print(" ");
  //Serial.println(theta2);
  x = forwardKinx(theta1, theta2);
  y = forwardKiny(theta1, theta2);
  Serial.print(x);
  Serial.print(" ");
  Serial.println(y);
}

float inverseKin2(float x, float y) {
  float theta;
  theta = acos((square(x) + square(y) - square(d1) - square(d2))/(2*d1*d2));
  if (isnan((double)theta)){
    theta = 0.0;
  }
  theta = (theta * 180)/ pi;
  return theta;
}

float inverseKin1(float x, float y, float theta2) {
  float theta;
  theta = atan2(y*(d1+d2*cos(theta2))-x*(d2*sin(theta2)) , x*(d1+d2*cos(theta2)) + y*(d2*sin(theta2)));
  if (isnan((double)theta)){
    theta = 0.0;
  }
  theta = (theta * 180)/ pi;
  return theta;
}

float forwardKinx(float theta1, float theta2) {
  float x1;
  theta1 = (theta1 * pi)/180;
  theta2 = (theta2 * pi)/180;
  x1 = (d1 * cos(theta1) + d2*cos(theta1+theta2));
  return x1;
}

float forwardKiny(float theta1, float theta2) {
  float y1;
  theta1 = (theta1 * pi)/180;
  theta2 = (theta2 * pi)/180;
  y1 = (d1 * sin(theta1) + d2*sin(theta1+theta2));
  return y1;
}


