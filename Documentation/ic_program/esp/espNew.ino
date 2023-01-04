#include "common.h"

Wifi wifi;
Web web;

int rando = 1;
String dt[12];
String c = "";

int sensor3, sensor6, instruksiesp32, instruksistm32, targetsuhu, stmHeaterPlate, stmHeaterWire, stmMatchingTemp, stmCompareTemp;
String stmAlarm, password, ssid;

void setup()
{
    Serial.begin(9600);
    bool success = WireSlave.begin(SDA_PIN, SCL_PIN, I2C_SLAVE_ADDR);
    while (!success)
    {
        bool tmp = WireSlave.begin(SDA_PIN, SCL_PIN, I2C_SLAVE_ADDR);
        if (tmp)
        {
            success = tmp;
            break;
        }
    }
    wifi.setAP("Wi-Fi", "sigemoyyy");
    wifi.connectWifi()
    delay(3000);
    bool sts = web.getSession();
    if (sts == false){
        if (web.isEstablished()){
            web.getSession();
        }else{
            wifi.connectWifi();
            delay(2000);
            web.getSession();
        }
    }
}

void loop()
{
    WireSlave.onReceive(receiveEvent);
    WireSlave.onRequest(requestEvent);
    WireSlave.update();
    
    if (web.isEstablished()){
        web.setSensorChamber(rando);
        web.setSensorOutputPasien((rando+1));
        web.setSuhuHeaterPlate((rando+4));
        web.setArusHeaterPlate((rando+7));
        web.publish();
    }
    
    else{
        setup();
    }

    rando = rando + 2;
    if (rando > 80){
        rando = 1;
    }

}

void receiveEvent(int howMany)
{
  while (WireSlave.available()) // loop through all but the last byte
  {
    c += char(WireSlave.read());
  }
  parsingdata();
  c = "";
}

void parsingdata()
{
  int j;
  dt[j] = "";
  for (int i = 1; i < c.length(); i++)
  {
    if ((c[i] == '#') || (c[i] == ':'))
    {
      j++;
      dt[j] = "";
    }
    else
    {
      dt[j] = dt[j] + c[i];
    }
  }
  Switchdata();
}

void Switchdata()
{

  switch (dt[0].toInt())
  {
  case 0:
    web.sensorOutputPasien = dt[1].toInt();
    break;
  case 1:
    web.sensorChamber = dt[1].toInt();
    break;
  case 2:
    sensor3 = dt[1].toInt();
    break;
  case 3:
    web.suhuHeaterPlate = dt[1].toInt();
    break;
  case 4:
    web.arusHeaterPlate = dt[1].toInt();
    break;
  case 5:
    sensor6 = dt[1].toInt();
    break;
  case 6:
    ssid = dt[1];
    if (ssid == ""){
      // Nothing
    }else{
      wifi.defaultSSID = ssid;
    }
    break;
  case 7:
    password = dt[1];
    if (password == ""){
      // Nothing
    }else{
      wifi.defaultPASSWORD = password;
    }
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

void requestEvent()
{
  WireSlave.write("*");
  ReplydataATMEGA(web.deviceID);
  ReplydataATMEGA(wifi.defaultSSID);
  ReplydataATMEGA(wifi.defaultPASSWORD);
  ReplydataATMEGA("INFO");
  WireSlave.write("#");
}

void ReplydataATMEGA(String val)
{
  String dataframe = val + ":";
  WireSlave.write(dataframe.c_str());
}