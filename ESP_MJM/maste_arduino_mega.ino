#include <Wire.h>
int analog1, analog2, analog3, analog4;
float suhu1, suhu2, suhu3, suhu4;
const int pio7 = 11;
const int trigeerpin = 13; //heater plate
int val = 0; //vallue button
int setPointint;
byte targetpid;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  
  
}
void loop() {
  if (Serial.available()) {
    String Setpoint = "";
    delay(50);
    while (Serial.available()) {
      Setpoint += char(Serial.read());
    }
    setPointint = Setpoint.toInt();
    targetpid = setPointint;
  }
  Wire.beginTransmission(8); // transmit to device #8
  Wire.write(setPointint);              // sends one byte
  Wire.endTransmission();    // stop transmitting
  analog1 = analogRead(A0);
  analog2 = analogRead(A1);
  analog3 = analogRead(A2);
  analog4 = analogRead(A3);
  suhu1 = 60 - (0.0586 * analog1);
  suhu2 = 60 - (0.0586 * analog2);
  suhu3 = 60 - (0.0586 * analog3);
  suhu4 = 60 - (0.0586 * analog4);
  Serial.print("t10.txt=\"");
  Serial.print(int(suhu1));
  Serial.print("\"");
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);
  //Serial.println("");
  //Serial.print("Temp1: ");
  //Serial.println(suhu1);
  //Serial.print("Temp2: ");
  //Serial.println(suhu2);
  //Serial.print("Temp3: ");
  //Serial.println(suhu3);
  //Serial.print("Thermistor: ");
  //Serial.println(suhu4);
}
