#include <WiFi.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Arduino.h>
#include <Wire.h>
#include <WireSlave.h>

#define SDA_PIN 21
#define SCL_PIN 22
#define I2C_SLAVE_ADDR 0x04

int val_1 ; //vallue button
int val_2 ; //vallue button
int trigger;

String mac;
String SSIDwifi;
String finalString;
const char* SSIDwifi_fix;

String mac2;
String PASSwifi;
String finalString2;
const char* PASSwifi_fix;
int lcd = 0;
int table[] = {0, 0, 0, 0, 0};
float suhu[] = {0, 0, 0, 0, 0};

String mainUrl = "http://0.tcp.ap.ngrok.io:13424/";
String URL_REGISTER_DEVICE = "service/register-machine-code/";
String URL_SESSION = "service/register-session/";
String URL_PUBLISH = "service/publish/";
String sessionID = "0", deviceID = "10107";

WiFiMulti wifiMulti;
HTTPClient http;


void setup()
{
  Serial.begin(115200);           // start serial for output
  bool success = WireSlave.begin(SDA_PIN, SCL_PIN, I2C_SLAVE_ADDR);
  if (!success) {
    Serial.println("I2C slave init failed");
    while (1) delay(100);
  }
  WireSlave.onReceive(komunikasiwifi); // register event

  WireSlave.onRequest(requestEventON);
  pinMode(val_1, INPUT_PULLUP);
  pinMode(val_2, INPUT_PULLUP);

}

void loop()
{

  WireSlave.update();
  komunikasimaster();
  delay(100);

}

void komunikasimaster() {
  val_1 = digitalRead(2);
  val_2 = digitalRead(4);

  if (val_1 == LOW && val_2 == LOW) {
    trigger = 0; // menerima ssid
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
  Serial.print("lcd: ");
  Serial.print(lcd);
}


void komunikasiwifi(int howMany) {

  switch (trigger) {
    case 0:
      if (WireSlave.available()) {
        String SSIDwifi2 = "";
        delay(50);
        while (WireSlave.available() > 0) {
          SSIDwifi2 += char(WireSlave.read());
        }
        SSIDwifi = SSIDwifi2;
        mac = WiFi.softAPmacAddress().c_str();
        finalString = SSIDwifi + mac;
        SSIDwifi_fix = finalString.c_str();
        Serial.print("SSID: ");
        Serial.println(SSIDwifi_fix);
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
        mac2 = WiFi.softAPmacAddress().c_str();
        finalString2 = PASSwifi + mac2;
        PASSwifi_fix = finalString2.c_str();
        Serial.print("PASSWORD: ");
        Serial.println(PASSwifi_fix);
        wifiMulti.addAP(SSIDwifi_fix, PASSwifi_fix)  ;
        Serial.println("wifi LOGIN");
        if (wifiMulti.run() == WL_CONNECTED) {
          lcd = 1;
          Serial.println("");
          Serial.println("WiFi connected");
          Serial.println("IP address: ");
          Serial.println(WiFi.localIP());
        }
        if (wifiMulti.run() != WL_CONNECTED) {
          Serial.println("wifi NOT conected");
          lcd = 0;
        }

        delay(500);
      }
      break;
    case 2:
      while (WireSlave.available()) // loop through all but the last byte
      {
        for (int i = 0; i < 5; i++) //organizes the data from the slave in the table
        {
          uint8_t c = WireSlave.read(); // receive a byte as character
          table[i] = c;
        }       // print the integer  // print the character
        Serial.print('\n');
        Serial.print(table[0]);
        Serial.print('\t');
        Serial.print(table[1]);
        Serial.print('\t');
        Serial.print(table[2]);
        Serial.print('\t');
        Serial.print(table[3]);
        Serial.print('\t');
        Serial.print(table[4]);
        Serial.print('\n');
        suhu[0] = 60 - (table[0] * 4 * 0.0586 );
        suhu[1] = 60 - (table[1] * 4 * 0.0586 );
        suhu[2] = 60 - (table[2] * 4 * 0.0586 );
        suhu[3] = table[3];
        suhu[4] = table[4];
        Serial.print('\n');
        Serial.print(suhu[0]);
        Serial.print('\t');
        Serial.print(suhu[1]);
        Serial.print('\t');
        Serial.print(suhu[2]);
        Serial.print('\t');
        Serial.print(suhu[3]);
        Serial.print('\t');
        Serial.print(suhu[4]);
        Serial.print('\n');
      }
      break;
  }

}
void requestEventON()
{
  WireSlave.write(lcd);
}
