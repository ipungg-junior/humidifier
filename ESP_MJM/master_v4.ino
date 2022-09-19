#include <Arduino.h>
#include <Wire.h>
#include <WirePacker.h>
#include <WireSlaveRequest.h>
#define MAX_SLAVE_RESPONSE_LENGTH 4
#define I2C_SLAVE_ADDR 0x04

String str = "This is my string";
int val_1 = 0;
int val_2 = 0;
int trigger;
byte x = 0;
byte y = 5;
byte z = 8;
char char_array;
char char_array2;


String SSIDwifi = "poiuttr";
String PASSwifi = "djbfkjdnfdj";

int table[] = {0, 0, 0, 0, 0};
int analog1, analog2, analog3, analog4;
byte analog1_1, analog2_1, analog3_1, analog4_1;
int heaterplate = 0;
int heaterwire = 0;
float suhu1, suhu2, suhu3, suhu4;
byte setPointint = 25;
byte targetpid;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Wire.begin(); // join i2c bus (address optional for master)
  pinMode(val_1, INPUT_PULLUP);//pinio
  pinMode(val_2, INPUT_PULLUP);//pinio
  pinMode(heaterplate, INPUT_PULLUP);//triger heater plate
  pinMode(heaterwire, INPUT_PULLUP);//trigger heater wire
}

void loop() {
  val_1 = digitalRead(5);
  val_2 = digitalRead(6);
  heaterplate = digitalRead(8);
  heaterwire = digitalRead(9);
  if (val_1 == LOW && val_2 == LOW) {
    trigger = 0; // mengirim ssid
  }
  else if (val_1 == HIGH && val_2 == LOW) {
    trigger = 1; // mengirim password
  }
  else if (val_1 == LOW && val_2 == HIGH) {
    trigger = 2; // mengirim data sensor
  }
  else if (val_1 == HIGH && val_2 == HIGH) {
    trigger = 3;
  }
  Serial.println(trigger);
  komunikasinextion();
  delay(100);

}

void komunikasinextion() {

  switch (trigger) {
    case 0:
      if (Serial.available()) {
        String SSIDwifi2 = "";
        delay(50);
        while (Serial.available() > 0) {
          SSIDwifi2 += char(Serial.read());
        }
        SSIDwifi = SSIDwifi2;
        Serial.print("SSID: ");
        Serial.println(SSIDwifi);
        int str_len = SSIDwifi.length() + 1;
        char char_array[str_len];
        SSIDwifi.toCharArray(char_array, str_len);
        Serial.println(char_array);
        WirePacker packer;
        packer.write(char_array);
        packer.end();
        Wire.beginTransmission(0x04); // transmit to device #4
        while (packer.available()) {    // write every packet byte
          Wire.write(packer.read());
        }
        Wire.endTransmission();    // stop transmitting
      }
      break;

    case 1:
      if (Serial.available()) {
        String PASSwifi2 = "";
        delay(50);
        while (Serial.available() > 0) {
          PASSwifi2 += char(Serial.read());
        }
        PASSwifi = PASSwifi2;
        Serial.print("PASS: ");
        Serial.println(PASSwifi);
        int str_lenn = PASSwifi.length() + 1;
        char char_arrayy[str_lenn];
        PASSwifi.toCharArray(char_arrayy, str_lenn);
        Serial.println(char_arrayy);
        WirePacker packer2;
        packer2.write(char_arrayy);
        packer2.end();
        Wire.beginTransmission(0x04); // transmit to device #4
        while (packer2.available()) {    // write every packet byte
          Wire.write(packer2.read());
        }
        Wire.endTransmission();    // stop transmitting
        WireSlaveRequest slaveReq(Wire, I2C_SLAVE_ADDR, MAX_SLAVE_RESPONSE_LENGTH);
        slaveReq.setRetryDelay(5);
        bool success = slaveReq.request();
        if (success) {
          int x = slaveReq.read();    // receive byte as an integer
        }
        Serial.print("bt10.val=\"");
        Serial.print(int(x));
        Serial.print("\"");
        Serial.write(0xff);
        Serial.write(0xff);
        Serial.write(0xff);
        delay(50);
      }
      break;

    case 2:
      if (Serial.available()) {
        String Setpoint = "";
        delay(50);
        while (Serial.available()) {
          Setpoint += char(Serial.read());
        }
        setPointint = Setpoint.toInt();
        targetpid = setPointint;
      }
      Wire.beginTransmission(7); // transmit to device #8
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
      delay(50);
      analog1_1 = analog1 / 4;
      analog2_1 = analog2 / 4;
      analog3_1 = analog3 / 4;
      analog4_1 = analog4 / 4;
      table[0] = analog1_1;
      table[1] = analog2_1;
      table[2] = analog3_1;
      table[3] = heaterplate;
      table[4] = heaterwire;
      uint8_t Buffer[5];
      Buffer[0] = table[0];
      Buffer[1] = table[1];
      Buffer[2] = table[2];
      Buffer[3] = table[3];
      Buffer[4] = table[4];
      Serial.println(analog1_1);
      Serial.println(analog2_1);
      Serial.println(analog3_1);
      Serial.println(analog4_1);
      WirePacker packer_1;
      packer_1.write(Buffer, 5);
      packer_1.end();
      Wire.beginTransmission(0x04); // transmit to device #4
      while (packer_1.available()) {    // write every packet byte
        Wire.write(packer_1.read());
      }
      Wire.endTransmission();    // stop transmitting
      //
      /*Wire.write(setPointint);              // sends one byte
        Wire.write(analog1_1);              // sends one byte
        Wire.write(analog2_1);              // sends one byte
        Wire.write(analog3_1);              // sends one byte


        while (packer_2.available()) {    // write every packet byte
        Wire.write(packer_2.read());
        }
        while (packer_3.available()) {    // write every packet byte
        Wire.write(packer_3.read());
        }
      */
      //Wire.endTransmission();    // stop transmitting
      delay(50);
      break;

    case 3:
      Serial.println("case3");
      break;
  }
}


//
/*
  Serial.println(x);
  Serial.println(y);
  Serial.println(z);
  Wire.beginTransmission(8); // transmit to device #4
  Wire.write(x);              // sends one byte
  Wire.write(y);              // sends one byte
  Wire.write(z);              // sends one byte
  Wire.endTransmission();    // stop transmitting
  x++;
  y++;
  z++;
  break;


  case 3:
  Serial.println("case3");
  break;

  }
  }
*/
