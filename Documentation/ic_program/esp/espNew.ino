#include "common.h"

Wifi wifi;
Web web;

String dt[12];
String c = "";

int sensor2, sensor3, sensor6, instruksiesp32, instruksistm32, targetsuhu, stmHeaterPlate, stmHeaterWire, stmMatchingTemp, stmCompareTemp;
String stmAlarm, password, ssid;

void setup()
{
  Serial.begin(9600);
  WireSlave.onReceive(receiveEvent);
  WireSlave.onRequest(requestEvent);
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
  wifi.connectWifi();
      delay(3000);
  bool sts = web.getSession();
  if (sts == false)
  {
    if (web.isEstablished())
    {
      web.getSession();
    }
    else
    {
      wifi.connectWifi();
      delay(2000);
      web.getSession();
    }
  }
}

void loop()
{
  
  WireSlave.update();

  if (web.isEstablished())
  {
    web.setSensorChamber(web.sensorChamber);
    web.setSensorOutputPasien(web.sensorOutputPasien);
    web.setSuhuHeaterPlate(web.suhuHeaterPlate);
    web.setArusHeaterPlate(12);
    web.publish();
  }
  else
  {
    bool status = wifi.reconnect();
    if (status)
    {
      web.getSession();
    }
  }
}

void receiveEvent(int howMany)
{
  while (WireSlave.available()) // loop through all but the last byte
  {
    c += char(WireSlave.read());
  }
  parsingdata();
  Serial.print("DATA IN : ");
  Serial.println(c);
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
    sensor2 = dt[1].toInt();
    break;
  case 2:
    web.sensorChamber = dt[1].toInt();
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
    wifi.defaultSSID = ssid;
    break;
  case 7:
    password = dt[1];
    wifi.defaultPASSWORD = password;
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
  Serial.print(" - OutputPasien : ");
  Serial.println(web.sensorOutputPasien);
  Serial.print(" - Chamber : ");
  Serial.println(web.sensorChamber);
  Serial.print(" - OutputPasien : ");
  Serial.println(web.suhuHeaterPlate);
  Serial.print(" - SSID : ");
  Serial.println(wifi.defaultSSID);
  Serial.print(" - SSID (local) : ");
  Serial.println(ssid);
  if (web.isEstablished())
  {
  }
  else
  {
    wifi.setAP(ssid, password);
    wifi.connectWifi();
    delay(1000);
    bool isOnline = web.isEstablished();
    if (isOnline)
    {
      bool newSession = web.getSession();
      delay(10);
    }
    else
    {
      wifi.connectWifi();
      delay(100);
    }
  }
}

void requestEvent()
{
  WireSlave.write("*");
  ReplydataATMEGA(web.deviceID);
  ReplydataATMEGA(wifi.defaultSSID);
  ReplydataATMEGA(wifi.defaultPASSWORD);
  ReplydataATMEGA(String(web.sensorChamber));
  WireSlave.write("#");
}

void ReplydataATMEGA(String val)
{
  String dataframe = val + ":";
  WireSlave.write(dataframe.c_str());
}