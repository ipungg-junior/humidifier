#ifndef MJMWifi_h
#define MJMWifi_h

class MJMWifiClass
{

public:  
    String SSID, PASW;
    bool WIFI_STATUS = false;
    int totalWifi;
    String wifiList[7];
    void scan();
    int connect(String ssid, String password);
    String getIP();
    bool isConnected();
    //String httpRequest(String uri="", String method="GET", String headers="", String body="");
};

extern MJMWifiClass MJMWifi;
#endif
