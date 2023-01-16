#include <Wire.h>
int val_1 = 0; //vallue button
int val_2 = 0; //vallue button
int trigger;
String SSIDwifi = "abcdefg";
String PASSwifi = "123456";

void setup()
{
  Wire.begin(8);                // join i2c bus with address #4
  Wire.onReceive(komunikasiwifi); // register event
  Serial.begin(9600);           // start serial for output
  pinMode(val_1, INPUT_PULLUP);
  pinMode(val_2, INPUT_PULLUP);

}

void loop()
{
  komunikasimaster();
  delay(100);
}

void komunikasimaster() {
  val_1 = digitalRead(PB1);
  val_2 = digitalRead(PB1);
  trigger = 2;
  if (val_1 == LOW && val_2 == LOW) {
    trigger = 2; // menerima ssid
  }
  else if (val_1 == HIGH && val_2 == LOW) {
    trigger = 1; // menerima password
  }
  else if (val_1 == LOW && val_2 == HIGH) {
    trigger = 2; // menerima data sensor
  }
  else if (val_1 == HIGH && val_2 == HIGH) {
    trigger = 3;
  }
}


void komunikasiwifi(int howMany) {

  switch (trigger) {
    case 0:
      if (Wire.available()) {
        String SSIDwifi2 = "";
        delay(50);
        while (Wire.available() > 0) {
          SSIDwifi2 += char(Wire.read());
        }
        SSIDwifi = SSIDwifi2;
        Serial.print("SSID: ");
        Serial.println(SSIDwifi);
      }
      break;


    case 1:
      if (Wire.available()) {
        String PASSwifi2 = "";
        delay(50);
        while (Wire.available() > 0) {
          PASSwifi2 += char(Wire.read());
        }
        PASSwifi = PASSwifi2;
        Serial.print("PASSWORD: ");
        Serial.println(PASSwifi);
      }
      break;
    case 2:
      while (1 < Wire.available()) // loop through all but the last
      {
        //int setpoint = Wire.read();
        int x = Wire.read();    // receive byte as an integer
        int y = Wire.read();    // receive byte as an integer
        int z = Wire.read();    // receive byte as an integer
        //Serial.print("suhu target: ");
        //Serial.println(setpoint);         // print the integer
        Serial.print("suhu x: ");
        Serial.println(x);         // print the integer
        Serial.print("suhu y: ");
        Serial.println(y);         // print the integer
        Serial.print("suhu z: ");
        Serial.println(z);         // print the integer

      }
      break;
  }

}
