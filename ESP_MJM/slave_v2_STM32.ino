// Wire Slave Receiver
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Receives data as an I2C/TWI slave device
// Refer to the "Wire Master Writer" example for use with this

// Created 29 March 2006

// This example code is in the public domain.

int analog;
const int pinio = PB5;
const int trigeerpin = PB0; //heater plate
const int led = PB10;
int val = 0; //vallue button
int trigger;
int val_1;
#include <Wire.h>

#include <PID_v1.h>
double targetpid, suhu, Output;

//Specify the links and initial tuning parameters
double Kp = 2, Ki = 5, Kd = 1;
PID myPID(&suhu, &Output, &targetpid, Kp, Ki, Kd, DIRECT);

int WindowSize = 5000;
unsigned long windowStartTime;

void setup()
{
  Wire.begin(7);                // join i2c bus with address #4
  // // register event
  Serial.begin(9600);           // start serial for output
  pinMode(trigeerpin, OUTPUT);
  pinMode(led, OUTPUT);
  digitalWrite(trigeerpin, HIGH);
  pinMode(val_1, INPUT_PULLUP);

  //PID
  windowStartTime = millis();

  //initialize the variables we're linked to

  //tell the PID to range between 0 and the full window size
  myPID.SetOutputLimits(0, WindowSize);

  //turn the PID on
  myPID.SetMode(AUTOMATIC);
}

void loop()
{
  val_1 = digitalRead(PB1);
  if (val_1 == HIGH) {
    trigger = 0;
  }
  else {
    trigger = 1;
  }

  switch (trigger) {
    case 0:
      analog = analogRead(PA0);
      suhu = 60 - (0.03800 * analog);
      Serial.print("analog: ");
      Serial.println(analog);
      Serial.print("Temp1: ");
      Serial.println(suhu);
      digitalWrite(led, HIGH);
      // turn LED on:
      myPID.Compute();
      if (millis() - windowStartTime > WindowSize)
      { //time to shift the Relay Window
        windowStartTime += WindowSize;
      }
      if (Output < millis() - windowStartTime) digitalWrite(trigeerpin, HIGH);
      else digitalWrite(trigeerpin, LOW);


      //delay(100);
      break;
    // function that executes whenever data is received from master
    // this function is registered as an event, see setup()
    //
    case 1:
      Wire.onReceive(receiveEvent);
      delay(100);
      break;
  }
}

void receiveEvent(int howMany)
{
  while (1 < Wire.available()) // loop through all but the last
  {
    char c = Wire.read();    // receive byte as an integer
    targetpid = c;
    Serial.println(c);         // print the integer
  }
  int x = Wire.read();    // receive byte as an integer
  targetpid = x;
  Serial.println(x);         // print the integer
}
