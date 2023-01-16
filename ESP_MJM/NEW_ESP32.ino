// https://codecollab.io/@proj/JeansLanguagePrice
#include <Arduino.h>
#include <Wire.h>
#include <WireSlave.h>
#include <WiFiMulti.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#define SDA_PIN 21
#define SCL_PIN 22
#define I2C_SLAVE_ADDR 0x04

String mainUrl = "http://0.tcp.ap.ngrok.io:18041/";
String URL_REGISTER_DEVICE = "service/register-machine-code/";
String URL_SESSION = "service/register-session/";
String URL_PUBLISH = "service/publish/";
String sessionID = "0", deviceID = "10101";
String dt[12];
String c = "";

WiFiMulti wifiMulti;
HTTPClient http;

float sensor1=12.1, sensor2=11.2, sensor3=13.0, sensor4=16.6, sensor5, sensor6, code;
String ssid="rrrrrrrr", password="11111111";
String instruksiesp32, instruksistm32, targetsuhu, stmHeaterPlate, stmHeaterWire, stmMatchingTemp, stmCompareTemp, stmAlarm;
void receiveEvent(int howMany);

void setup()
{
  
  
  Serial.begin(9600);
  wifiMulti.addAP("rrrrrrrr", "11111111");
  bool success = WireSlave.begin(SDA_PIN, SCL_PIN, I2C_SLAVE_ADDR);
    if (!success) {
        Serial.println("I2C slave init failed");
        while(1) delay(100);
    }

    
  for (uint8_t t = 4; t > 0; t--)
  {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }

  
  for (uint8_t t = 3; t > 0; t--){
    if(!(wifiMulti.run() == WL_CONNECTED)){
      wifiMulti.run();
    }
  }
 
  if (sessionID == "0"){
    sessionID = getSession();
  }
  
}

void loop(){
  WireSlave.onReceive(receiveEvent);
  WireSlave.onRequest(requestEvent);
  WireSlave.update();
  if(WL_CONNECTED){
    //Wifi terhubung
    if (sessionID == "0"){
      sessionID = getSession();
    }else{
      int statusPublish = publishData();
      if (statusPublish == 1){
        // Pusblish Data berhasil
        Serial.println("Published.");  
      }else{
        // Pusblish Data gagal
        Serial.println("Error.");
      }
    }
    
  }else{
    wifiMulti.addAP(ssid.c_str(), password.c_str());
    for (uint8_t t = 3; t > 0; t--){
      if(!(wifiMulti.run() == WL_CONNECTED)){
        delay(2000);
        wifiMulti.run();
      }
    }
  }
  
  

}

String getSession()
{
  String input = "{\"deviceID\":" + deviceID + "}";
  const size_t capacity = JSON_OBJECT_SIZE(3) + JSON_ARRAY_SIZE(2) + 60;
  DynamicJsonDocument doc(capacity);
  deserializeJson(doc, input);
  JsonObject obj = doc.as<JsonObject>();

  http.begin(mainUrl + URL_SESSION);
  int httpCode = http.POST(input);
  if (httpCode < 0){
    //Gagal
    return String(0);
  }
  else{
    String payload = http.getString();
    DeserializationError error = deserializeJson(doc, payload);
    http.end();
    return (doc["sessionID"].as<String>());
  }
  
}

int publishData() {
  String input = "{\"deviceID\":" + deviceID + "," + 
                 "\"sessionID\":" + sessionID + "," +
                 "\"suhu_chamber_a\":" + sensor3 + "," +
                 "\"suhu_chamber_b\":" + sensor2 + "," +
                 "\"suhu_output_pasien\":" + sensor1 + "," +
                 "\"suhu_heater_plate\":" + sensor4 + "," +
                 "\"arus_heater_plate\":" + sensor5 + "," +
                 "\"arus_heater_wire\":" + sensor6 + "," +
                 "\"code\":" + code + "}";
  const size_t capacity = JSON_OBJECT_SIZE(3) + JSON_ARRAY_SIZE(2) + 60;
  DynamicJsonDocument doc(capacity);
  deserializeJson(doc, input);
  JsonObject obj = doc.as<JsonObject>();

  http.begin(mainUrl + URL_PUBLISH);
  int httpCode = http.POST(input);
  if (httpCode < 0){
    //Gagal
    http.end();
    return -1;
  }else{
    http.end();
    return 1;
  }

  
}

String getDeviceID()
{
  http.begin(mainUrl + URL_REGISTER_DEVICE); // HTTP
  int httpCode = http.GET();

  if (httpCode > 0)
  {

    // file found at server
    if (httpCode == HTTP_CODE_OK)
    {
      String payload = http.getString();
      const size_t capacity = JSON_OBJECT_SIZE(3) + JSON_ARRAY_SIZE(2) + 60;
      DynamicJsonDocument doc(capacity);
      DeserializationError error = deserializeJson(doc, payload);
      if (error)
      {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        http.end();
      }
      deviceID = (doc["machine-code"].as<String>());
    }
  }
  else
  {
    Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }

  http.end();
}


void receiveEvent(int howMany) {
  while (WireSlave.available())  // loop through all but the last byte
  {
    c += char(WireSlave.read());
  }
  parsingdata();
  c = "";
}

void parsingdata() {
  int j;
  dt[j] = "";
  for (int i = 1; i < c.length(); i++) {
    if ((c[i] == '#') || (c[i] == ':')) {
      j++;
      dt[j] = "";
    } else {
      dt[j] = dt[j] + c[i];
    }
  }
  Switchdata();
}

void Switchdata() {

    switch (dt[0].toInt()) {
      case 0:
        sensor1 = dt[1].toFloat();
        break;
      case 1:
        sensor2 = dt[1].toFloat();
        break;
      case 2:
        sensor3 = dt[1].toFloat();
        break;
      case 3:
        sensor4 = dt[1].toFloat();
        break;
      case 4:
        sensor5 = dt[1].toFloat();
        break;
      case 5:
        sensor6 = dt[1].toFloat();
        break;
      case 6:
        ssid = dt[1];
        break;
      case 7:
        password = dt[1];
        break;
      case 8:
        instruksiesp32 = dt[1].toInt();
        break;
      case 9:
        instruksistm32 = dt[1].toInt();
        break;
      case 10:
        targetsuhu = dt[1].toInt();
        break;
      case 11:
        stmHeaterPlate = dt[1].toInt();
        break;
      case 12:
        stmHeaterWire = dt[1].toInt();
        break;
      case 13:
        stmMatchingTemp = dt[1].toInt();
        break;
      case 14:
        stmCompareTemp = dt[1].toInt();
        break;
      case 15:
        stmAlarm = dt[1];
        break;
    }

}

void requestEvent() {
  WireSlave.write("*");
  ReplydataATMEGA(deviceID);
  ReplydataATMEGA(ssid);
  ReplydataATMEGA(password);
  ReplydataATMEGA("JUST INFO 3");
  WireSlave.write("#");
}

void ReplydataATMEGA(String val) {
  String dataframe = val + ":";
  WireSlave.write(dataframe.c_str());
}