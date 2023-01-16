// https://7733-103-23-224-244.jp.ngrok.io
#include "WiFi.h"
#include "MJMWifi.h"


void MJMWifiClass::scan(){

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  
  totalWifi = WiFi.scanNetworks();
  
  for (unsigned short int i = 0; i < totalWifi; i++) {
    wifiList[i] = WiFi.SSID(i);
  }
  
}


int MJMWifiClass::connect(String ssid, String password){
  WiFi.begin(ssid.c_str(), password.c_str());
  for (int x=0; x<10; x++){  
    if (WiFi.status() != WL_CONNECTED){
      delay(1000);
    }
  }
  if (WiFi.status() != WL_CONNECTED){
      return false;
    }
    SSID = ssid;
    PASW = password;
    WIFI_STATUS = true;
  return true;
}

String MJMWifiClass::getIP(){
  return (String)WiFi.localIP();
}


bool MJMWifiClass::isConnected(){
  if (WiFi.status() != WL_CONNECTED){
      WIFI_STATUS = false;
      return false;
  }else{
    WIFI_STATUS = true;
    return true;
  }
}




MJMWifiClass MJMWifi;
