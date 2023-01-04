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

struct Wifi
{
    int counterReconnect = 0;
    bool wifiStatus = false;
    String defaultSSID = "Wi-Fi", defaultPASSWORD = "sigemoyyy";
    void setAP(String ss, String ps);
    bool connectWifi();
};

struct Web
{
    int sensorChamber, sensorOutputPasien, suhuHeaterPlate, arusHeaterPlate;
    String sessionID = "0";
    String deviceID = "10101";
    String protocol = "http://";
    String domain = "0.tcp.ap.ngrok.io";
    String port = ":18041/";
    String URL_REGISTER_DEVICE = "service/register-machine-code/";
    String URL_SESSION = "service/register-session/";
    String URL_PUBLISH = "service/publish/";
    bool isEstablished();
    bool getSession();
    void setSensorChamber(int a);
    void setSensorOutputPasien(int a);
    void setSuhuHeaterPlate(int a);
    void setArusHeaterPlate(int a);
    bool publish();
};

WiFiMulti wifiMulti;
HTTPClient http;

void Wifi::setAP(String ss, String ps)
{
    Wifi::defaultSSID = ss;
    Wifi::defaultPASSWORD = ps;
}

bool Wifi::connectWifi()
{

    wifiMulti.addAP((Wifi::defaultSSID).c_str(), (Wifi::defaultPASSWORD).c_str());
    for (uint8_t t = 3; t > 0; t--)
    {
        if (!(wifiMulti.run() == WL_CONNECTED))
        {
            wifiMulti.run();
        }
        else
        {
        }
    }
    return true;
}

bool Web::isEstablished()
{
    http.begin("http://www.google.co.id/");
    int httpCode = http.GET();
    if (httpCode < 0)
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool Web::getSession()
{
    String input = "{\"deviceID\":" + Web::deviceID + "}";
    const size_t capacity = JSON_OBJECT_SIZE(3) + JSON_ARRAY_SIZE(2) + 60;
    DynamicJsonDocument doc(capacity);
    deserializeJson(doc, input);
    JsonObject obj = doc.as<JsonObject>();

    http.begin(Web::protocol + Web::domain + Web::port + Web::URL_SESSION);
    int httpCode = http.POST(input);
    if (httpCode < 0)
    {
        // Gagal
        return false;
    }
    else
    {
        String payload = http.getString();
        DeserializationError error = deserializeJson(doc, payload);
        http.end();
        Web::sessionID = (doc["sessionID"].as<String>());
        return true;
    }
}

void Web::setSensorChamber(int a)
{
    Web::sensorChamber = a;
}

void Web::setSensorOutputPasien(int a)
{
    Web::sensorOutputPasien = a;
}

void Web::setSuhuHeaterPlate(int a)
{
    Web::suhuHeaterPlate = a;
}

void Web::setArusHeaterPlate(int a)
{
    Web::arusHeaterPlate = a;
}

bool Web::publish()
{
    String input = "{\"deviceID\":" + Web::deviceID + "," +
                 "\"sessionID\":" + Web::sessionID + "," +
                 "\"suhu_chamber_a\":" + Web::sensorChamber + "," +
                 "\"suhu_chamber_b\":" + Web::sensorChamber + "," +
                 "\"suhu_output_pasien\":" + Web::sensorOutputPasien + "," +
                 "\"suhu_heater_plate\":" + Web::suhuHeaterPlate + "," +
                 "\"arus_heater_plate\":" + Web::arusHeaterPlate + "," +
                 "\"arus_heater_wire\":" + Web::arusHeaterPlate + "," +
                 "\"code\":" + 0 + "}";
    const size_t capacity = JSON_OBJECT_SIZE(3) + JSON_ARRAY_SIZE(2) + 60;
    DynamicJsonDocument doc(capacity);
    deserializeJson(doc, input);
    JsonObject obj = doc.as<JsonObject>();

    http.begin(Web::protocol + Web::domain + Web::port + Web::URL_PUBLISH);
    int httpCode = http.POST(input);
    if (httpCode < 0)
    {
        // Gagal
        http.end();
        return false;
    }
    else
    {
        http.end();
        return true;
    }
}

