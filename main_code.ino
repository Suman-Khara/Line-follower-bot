#include <L298NX2.h>
#define MAXSPEED 255
#define MINSPEED -255
#define basespeed 155
const int numSensors = 8;
const int sensorPins[] = {A5, A4, A3, A2, A1, A0, 2, 3};
int sensorValues[numSensors];
const double Kp = 0.95;
const double Ki = 0.0001;
const double Kd = 6.3;
#define s sensorValues
double P = 0.0;
double I = 0.0;
double D = 0.0;
double lasterror = 0.0;
const int EN_A = 6;
const int IN1_A = 7;
const int IN2_A = 8;


const int EN_B = 11;
const int IN1_B = 9;
const int IN2_B = 10;

L298NX2 motors(EN_A, IN1_A, IN2_A, EN_B, IN1_B, IN2_B);
void setup() {
  Serial.begin(9600);
  for (int i = 0; i < numSensors; i++) 
  {
    pinMode(sensorPins[i], INPUT);
  }
  // motors.setSpeedA(100);
  // motors.setSpeedB(100);
}                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              

void PIDControl() {
  int pos = line_pos(sensorValues);
  // Serial.print("Position: ");
  // Serial.print(pos);
  // Serial.println();
  if(pos == -1) return;
  double error = 350-pos;

  // error = error*2;
  P  = error;
  I = I + error;
  D = error-lasterror;
  // Serial.print(" P: "); Serial.print(P);
  // Serial.print(" I: "); Serial.print(I);
  // Serial.print(" D: "); Serial.print(D);
  lasterror = error;
  
  float speed = P*Kp + D*Kd + I*Ki;
  // Serial.print(" Speed: "); Serial.println(speed);
  double speedA = (basespeed + speed);
  double speedB = (basespeed - speed);
  if(speedA > MAXSPEED) speedA = MAXSPEED;
  if(speedB > MAXSPEED) speedB = MAXSPEED;
  if (speedA < MINSPEED) {
    speedA = MINSPEED;
  }
  if (speedB < MINSPEED) {
    speedB = MINSPEED;

  }
  
  motors.setSpeedA((int)abs(speedA));
  motors.setSpeedB((int)abs(speedB));

  if(speedA>=0) motors.forwardA();
  else motors.backwardA();

  if(speedB>=0) motors.forwardB();
  else motors.backwardB();
}

int line_pos(int sensorarr[])
{
  int pos = 0 , cnt = 0;
  for(int i = 0; i<numSensors; i++)
  {
    if(sensorarr[i] == 0)
    {
      pos += 100*i;
      cnt++;
    }
  }
  if(cnt == 0) //All white
  {
    motors.stop();
    delay(300);
    motors.setSpeedA(80);
    motors.setSpeedB(80);
    motors.backwardA();
    motors.backwardB();
    delay(300);
    return -1;
  }
  else
  {
    
  return pos/cnt;
  }
}


void loop() {
  // motors.setSpeedA(MAXSPEED+27);
  // motors.setSpeedB(MAXSPEED);
  for (int i = 0; i < numSensors; i++) {
    sensorValues[i] = digitalRead(sensorPins[i]);
  }
  // Serial.print(motors.getSpeedA());
  // Serial.print(" ");
  // Serial.println(motors.getSpeedB());
  // for(int i = 0; i<numSensors; i++)
  // {
  //   Serial.print(sensorValues[i]);
  // }
  // Serial.println();
  PIDControl();
  // delay(2);
  //motors.forward();
  delay(22);
}