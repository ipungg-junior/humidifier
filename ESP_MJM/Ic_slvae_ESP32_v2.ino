// WireSlave Receiver
// by Gutierrez PS <https://github.com/gutierrezps>
// ESP32 I2C slave library: <https://github.com/gutierrezps/ESP32_I2C_Slave>
// based on the example by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the WireSlave library for ESP32.
// Receives data as an I2C/TWI slave device; data must
// be packed using WirePacker.
// Refer to the "master_writer" example for use with this

#include <Arduino.h>
#include <Wire.h>
#include <WireSlave.h>

#define SDA_PIN 21
#define SCL_PIN 22
#define I2C_SLAVE_ADDR 0x04

int val_1 = 0; //vallue button
int val_2 = 0; //vallue button
int trigger;
String SSIDwifi = "abcdefg";
String PASSwifi = "123456";
void receiveEvent(int howMany);

void setup()
{
  Serial.begin(115200);

  bool success = WireSlave.begin(SDA_PIN, SCL_PIN, I2C_SLAVE_ADDR);
  if (!success) {
    Serial.println("I2C slave init failed");
    while (1) delay(100);
  }

  WireSlave.onReceive(komunikasiwifi);
  Serial.begin(115200);           // start serial for output
  pinMode(val_1, INPUT_PULLUP);
  pinMode(val_2, INPUT_PULLUP);
}

void loop()
{
  // the slave response time is directly related to how often
  // this update() method is called, so avoid using long delays
  // inside loop(), and be careful with time-consuming tasks
  Serial.println("test");
  WireSlave.update();
  komunikasimaster();
  // let I2C and other ESP32 peripherals interrupts work
  delay(1);
}

// function that executes whenever a complete and valid packet
// is received from master
// this function is registered as an event, see setup()

void komunikasimaster() {
  val_1 = digitalRead(25);
  val_2 = digitalRead(26);
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


void komunikasiwifi(int howMany)
{
  switch (trigger) {
    case 0:
      if (WireSlave.available()) {
        String SSIDwifi2 = "";
        delay(50);
        while (WireSlave.available() > 0) {
          SSIDwifi2 += char(WireSlave.read());
        }
        SSIDwifi = SSIDwifi2;
        Serial.print("SSID: ");
        Serial.println(SSIDwifi);
      }
      break;


    case 1:
      if (WireSlave.available()) {
        String PASSwifi2 = "";
        delay(50);
        while (WireSlave.available() > 0) {
          PASSwifi2 += char(WireSlave.read());
        }
        PASSwifi = PASSwifi2;
        Serial.print("PASSWORD: ");
        Serial.println(PASSwifi);
      }
      break;
    case 2:
      while (WireSlave.available()) // loop through all but the last
      {
        //int setpoint = Wire.read();
        int x = WireSlave.read();    // receive byte as an integer
        int y = WireSlave.read();    // receive byte as an integer
        int z = WireSlave.read();    // receive byte as an integer
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
