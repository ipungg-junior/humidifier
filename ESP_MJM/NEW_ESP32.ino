// https://codecollab.io/@proj/JeansLanguagePrice
#include <Arduino.h>
#include <WiFiMulti.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

String mainUrl = "http://0.tcp.ap.ngrok.io:13424/";
String URL_REGISTER_DEVICE = "service/register-machine-code/";
String URL_SESSION = "service/register-session/";
String URL_PUBLISH = "service/publish/";
String sessionID = "0", deviceID = "10107";

WiFiMulti wifiMulti;
HTTPClient http;

int chamberA, chamberB, suhuPasien, suhuPlate, arusPlate, arusWire, code;


void setup()
{

  Serial.begin(9600);
  wifiMulti.addAP("realme 9 Pro+", "11111111");
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
 
  while (sessionID == "0"){
    sessionID = getSession();
  }
  
}

void loop(){

  if(WL_CONNECTED){
    //Wifi terhubung
    
    int statusPublish = publishData();
    if (statusPublish == 1){
      // Pusblish Data berhasil
      Serial.println("Published.");  
    }else{
      // Pusblish Data gagal
      Serial.println("Error.");
    }
    
  }else{
    // Wifi terputus
    Serial.println("Tidak ada koneksi . . .");
  }
  
  
  delay(3000);

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
