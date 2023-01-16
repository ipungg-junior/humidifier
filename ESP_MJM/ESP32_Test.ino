#include "MJMWifi.h"

void setup()
{
    Serial.begin(9600);
    MJMWifi.scan();

    //Show Available Network
    Serial.println("Available Network : ");
    for (int x=0; x<7; x++) {
      Serial.println(MJMWifi.wifiList[x]);
    }

    Serial.println("===========");
    delay(2000);
}

void loop()
{   
    
    if (MJMWifi.isConnected()) {
      Serial.println("Konek");
      
    }else {
      Serial.println("Connecting to Wi-Fi...");
      MJMWifi.connect("realme 9 Pro+", "11111111");
    }

    delay(1000);
    
}
