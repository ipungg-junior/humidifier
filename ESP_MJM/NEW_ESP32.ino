#include <Arduino.h>
#include <WiFiMulti.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

#define USE_SERIAL Serial
String mainUrl = "http://0.tcp.ap.ngrok.io:12928/";
String URL_REGISTER_DEVICE = "service/register-machine-code/";
String URL_SESSION = "service/register-session/";
String URL_PUBLISH = "service/publish/";
String sessionID, deviceID;

WiFiMulti wifiMulti;
HTTPClient http;

int chamberA, chamberB, suhuPasien, suhuPlate, arusPlate, arusWire, code;


void setup()
{

  USE_SERIAL.begin(9600);

  USE_SERIAL.println();
  USE_SERIAL.println();
  USE_SERIAL.println();

  for (uint8_t t = 4; t > 0; t--)
  {
    USE_SERIAL.printf("[SETUP] WAIT %d...\n", t);
    USE_SERIAL.flush();
    delay(1000);
  }

  wifiMulti.addAP("realme 9 Pro+", "11111111");

  deviceID = "10107";
  sessionID = getSession();
}

void loop()
{

  if ((wifiMulti.run() == WL_CONNECTED))
  {

    Serial.println(deviceID + sessionID);
    int statusCode = publishData();

    delay(5000);
  }
}

String getSession()
{
  String input = "{\"deviceID\":" + deviceID + "}";
  const size_t capacity = JSON_OBJECT_SIZE(3) + JSON_ARRAY_SIZE(2) + 60;
  DynamicJsonDocument doc(capacity);
  deserializeJson(doc, input);
  JsonObject obj = doc.as<JsonObject>();

  Serial.print("Request session for device : ");
  Serial.println(deviceID);
  Serial.print("Body : ");
  Serial.println(doc.as<String>());
  http.begin(mainUrl + URL_SESSION);
  int httpCode = http.POST(input);
  Serial.print("Session response code : ");
  Serial.println(httpCode);
  String payload = http.getString();
  DeserializationError error = deserializeJson(doc, payload);
  http.end();

  // Extract values
  return (doc["sessionID"].as<String>());
}

int publishData()
{
  String input = "{\"deviceID\":" + deviceID + "," + 
                 "\"sessionID\":" + sessionID + "," +
                 "\"suhu_chamber_a\":" + chamberA + "," +
                 "\"suhu_chamber_b\":" + chamberB + "," +
                 "\"suhu_output_pasien\":" + suhuPasien + "," +
                 "\"suhu_heater_plate\":" + suhuPlate + "," +
                 "\"arus_heater_plate\":" + arusPlate + "," +
                 "\"arus_heater_wire\":" + arusWire + "," +
                 "\"code\":" + code + "}";
  const size_t capacity = JSON_OBJECT_SIZE(3) + JSON_ARRAY_SIZE(2) + 60;
  DynamicJsonDocument doc(capacity);
  deserializeJson(doc, input);
  JsonObject obj = doc.as<JsonObject>();

  http.begin(mainUrl + URL_PUBLISH);
  int httpCode = http.POST(input);

  String payload = http.getString();
  http.end();
  return httpCode;
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
    USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }

  http.end();
}
