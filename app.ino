#include <Adafruit_NeoPixel.h>
#include<Wire.h>
#include <math.h>
const int MPU=0x68;
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;
double pitch,roll;

#define PIN            7
#define NUMPIXELS      100

int LEDS = 8;
int latest = 0;

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  pixels.begin();
  //starting
  Serial.println("Successfully started!");
  flashing(2, 200);
  
  gyroSetup();
}

void loop() {
  ovserveGyro();
}

void ovserveGyro() {
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU,14,true);
  
  int AcXoff,AcYoff,AcZoff,GyXoff,GyYoff,GyZoff;
  int temp,toff;
  double t,tx,tf;
  
  //Acceleration data correction
  AcXoff = -950;
  AcYoff = -300;
  AcZoff = 0;
  
  //Temperature correction
  toff = -1600;
  
  //Gyro correction
  GyXoff = 480;
  GyYoff = 170;
  GyZoff = 210;
  
  //read accel data
  AcX=(Wire.read()<<8|Wire.read()) + AcXoff;
  AcY=(Wire.read()<<8|Wire.read()) + AcYoff;
  AcZ=(Wire.read()<<8|Wire.read()) + AcYoff;
  
  //read temperature data
  temp=(Wire.read()<<8|Wire.read()) + toff;
  tx=temp;
  t = tx/340 + 36.53;
  tf = (t * 9/5) + 32;
  
  //read gyro data
  GyX=(Wire.read()<<8|Wire.read()) + GyXoff;
  GyY=(Wire.read()<<8|Wire.read()) + GyYoff;
  GyZ=(Wire.read()<<8|Wire.read()) + GyZoff;
  
  //get pitch/roll
  getAngle(AcX,AcY,AcZ);
  
  //send the data out the serial port
//  Serial.print("Angle: ");
//  Serial.print("Pitch = "); Serial.print(pitch);
//  Serial.print(" | Roll = "); Serial.println(roll);
  
//  Serial.print("Temp: ");
//  Serial.print("Temp(F) = "); Serial.print(tf);
//  Serial.print(" | Temp(C) = "); Serial.println(t);
  
  //Serial.print("Accelerometer: ");
  //Serial.print("X = "); Serial.print(AcX);
  //Serial.print(" | Y = "); Serial.print(AcY);
  //Serial.print(" | Z = "); Serial.println(AcZ);
  
  Serial.print("Gyroscope: ");
  Serial.print("X = "); Serial.print(GyX);
  //Serial.print(" | Y = "); Serial.print(GyY);
  //Serial.print(" | Z = "); Serial.println(GyZ);
  Serial.println(" ");

  Serial.print("Latest = "); Serial.print(GyX);
  Serial.println(" ");
  if(latest > GyX + 500) {
    Serial.println("BREAKING");
    flashing(5, 250);  
  }
  
  latest = GyX;
  
  delay(2000);
}

void gyroSetup() {
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
  Serial.begin(9600);
}

void showUp() {
  for(int i = 0; i <= LEDS; i++) {
    pixels.setPixelColor(i, pixels.Color(100, 0, 0));  
  }
  
  pixels.show();
}

void shutDown() {
  for(int i = 0; i <= LEDS; i++) {
    pixels.setPixelColor(i, pixels.Color(0, 0, 0));  
  }
  
  pixels.show();
}

void flashing(int times, int delays) {
  for(int i = 0; i < times; i++) {
    showUp();
    delay(delays);
    shutDown();
    delay(delays);
  }
}

void getAngle(int Vx,int Vy,int Vz) {
  double x = Vx;
  double y = Vy;
  double z = Vz;
  
  pitch = atan(x/sqrt((y*y) + (z*z)));
  roll = atan(y/sqrt((x*x) + (z*z)));
  //convert radians into degrees
  pitch = pitch * (180.0/3.14);
  roll = roll * (180.0/3.14) ;
}

void breakSimulation() {
  Serial.println("BREAK SIMULATION!");
}
