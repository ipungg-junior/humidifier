#define PIN_RELAY_WIRE 7
#define PIN_RELAY_PLATE 5
#define PIN_PROBE_1 A0
#define PIN_PROBE_2 A1
#define PIN_PROBE_3 A8
#define PIN_HEATER_PLATE A2
#define PIN_ARUS_WIRE A3
#define PIN_ARUS_PLATE A7
#define PIN_BUZZER 8
#define PIN_LED 6
#define ESP_ADDR 0x04
#define STM_ADDR 0x03

float initOutputPasien, initSenDua, initChamber, initHeaterPlate, initSenLima, initSenEnam;

typedef struct {
    String state="";
    bool hold=false;
    int counterHold=0;
    String ssid, passWifi;
    int instructionEsp, instructionStm, targetTemp, relay=0;
} Nextion;

typedef struct {
    float outputPasien, sensorDua, chamber, heaterPlate, sensorLima, sensorEnam;
} Sensor;

typedef struct {
    // Data from Esp
    String inSerialNum, inKondisiWifi, inAlarmESP, inAlertWeb;
} Esp;

typedef struct{
    // Data from Stm
    int inHeaterWire, inHeaterPlateStatus, inMatchingTemp, inCompareTemp, sensorSTM;
    String inAlarmSTM;
} Stm;


Nextion nextion;
Sensor sensor;
Esp esp;
Stm stm;

void initSensors()
{
  delay(100);
  for (int count = 0; count < 80; count++)
  {
    
    initSenDua += analogRead(PIN_PROBE_2);
    delay(5);
    initChamber += analogRead(PIN_PROBE_3);
    delay(5);
    initHeaterPlate += analogRead(PIN_HEATER_PLATE);
    delay(5);
    initSenLima += analogRead(PIN_ARUS_WIRE);
    delay(5);
    initSenEnam += analogRead(PIN_ARUS_PLATE);
    delay(5);
  }
  initSenDua = (initSenDua / 80);
  initChamber = (initChamber / 80);
  initHeaterPlate = (initHeaterPlate / 80);
  initSenLima = (initSenLima / 80);
  initSenEnam = (initSenEnam / 80);

}

float getAnalogOutputPasien(){
  
  int syncSensor;
  for(short int i=0; i<80; i++){
    syncSensor += analogRead(PIN_PROBE_1);
  }
  syncSensor = (syncSensor/80);
  return syncSensor;
}

int getAnalogSenDua(){
  
  int syncSensor;
  for(short int i=0; i<30; i++){
    syncSensor += analogRead(PIN_PROBE_2);
  }
  syncSensor = (syncSensor/30);
  return syncSensor;
}

float getAnalogChamber(){
  int syncSensor;
  for(short int i=0; i<30; i++){
    syncSensor += analogRead(PIN_PROBE_3);
  }
  syncSensor = (syncSensor/30);
  return syncSensor;
}

void parsingDataEsp()
{
  int j2 = 0;
  dt2[j2] = "";
  for (int ii = 1; ii < dataInEsp.length(); ii++)
  {
    if ((dataInEsp[ii] == '#') || (dataInEsp[ii] == ':'))
    {
      j2++;
      dt2[j2] = "";
    }
    else
    {
      dt2[j2] = dt2[j2] + dataInEsp[ii];
    }
  }
  esp.inSerialNum = dt2[0];
  esp.inKondisiWifi = dt2[1];
  esp.inAlarmESP = dt2[2];
  esp.inAlertWeb = dt2[3];
}

void parsingDataStm()
{
  int j = 0;
  dt[j] = "";
  for (int i = 1; i < dataIn.length(); i++)
  {
    if ((dataIn[i] == '#') || (dataIn[i] == ':'))
    {
      j++;
      dt[j] = "";
    }
    else
    {
      dt[j] = dt[j] + dataIn[i];
    }
  }
  stm.inHeaterWire = dt[0].toInt();
  stm.inHeaterPlateStatus = dt[1].toInt();
  stm.inMatchingTemp = dt[2].toInt();
  stm.inCompareTemp = dt[3].toInt();
  stm.inAlarmSTM = dt[4];
  stm.sensorSTM = dt[5].toInt();
}

void parsingNextion(String nextBuff)
{
  int j = 0;
  dt3[j] = "";
  for (int i = 1; i < nextBuff.length(); i++)
  {

    if ((nextBuff[i] == '#') || (nextBuff[i] == ':'))
    {
      j++;
      dt3[j] = "";
    }

    else
    {
      dt3[j] = dt3[j] + nextBuff[i];
    }
  }

  nextion.instructionEsp = dt3[1].toInt();
  nextion.instructionStm = dt3[2].toInt();
  nextion.ssid = dt3[3];
  nextion.passWifi = dt3[4];
  nextion.targetTemp = dt3[5].toInt();
  nextion.relay = dt3[6].toInt();
}

void transmitNextion(String widget, String val)
{
  Serial.print(widget + ".txt=" + "\"" + String(val) + "\"");
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);
  delay(10);
}

void transmitNextion(String widget, int val)
{
  Serial.print(widget + ".txt=" + "\"" + String(val) + "\"");
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);
  delay(10);
}

void transmitNextion(String widget, float val)
{
  Serial.print(widget + ".txt=" + "\"" + String(val) + "\"");
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);
  delay(10);
}

void transmitToEsp(byte address, unsigned int header, String val)
{
  WirePacker packer;
  String dataFrame = "*" + String(header) + ":" + val + "#";
  packer.write(dataFrame.c_str());
  packer.end();
  Wire.beginTransmission(address);
  while (packer.available())
  { // write every packet byte
    Wire.write(packer.read());
  }
  Wire.endTransmission();
}

void transmitToEsp(byte address, unsigned int header, float val)
{
  WirePacker packer;
  String dataFrame = "*" + String(header) + ":" + String(val) + "#";
  packer.write(dataFrame.c_str());
  packer.end();
  Wire.beginTransmission(address);
  while (packer.available())
  { // write every packet byte
    Wire.write(packer.read());
  }
  Wire.endTransmission();
}

void transmitToEsp(byte address, unsigned int header, int val)
{
  WirePacker packer;
  String dataFrame = "*" + String(header) + ":" + String(val) + "#";
  packer.write(dataFrame.c_str());
  packer.end();
  Wire.beginTransmission(address);
  while (packer.available())
  { // write every packet byte
    Wire.write(packer.read());
  }
  Wire.endTransmission();
}

void transmitToSTM(byte address, unsigned int header, int val)
{
  String dataFrame = "*" + String(header) + ":" + String(val) + "#";
  Wire.beginTransmission(address);
  Wire.write(dataFrame.c_str());
  Wire.endTransmission();
  delay(5);
}

void transmitToSTM(byte address, unsigned int header, String val)
{
  String dataFrame = "*" + String(header) + ":" + String(val) + "#";
  Wire.beginTransmission(address);
  Wire.write(dataFrame.c_str());
  Wire.endTransmission();
  delay(5);
}

void transmitToSTM(byte address, unsigned int header, float val)
{
  String dataFrame = "*" + String(header) + ":" + String(val) + "#";
  Wire.beginTransmission(address);
  Wire.write(dataFrame.c_str());
  Wire.endTransmission();
  delay(5);
}

void requestFromEsp()
{
  WireSlaveRequest espReq(Wire, ESP_ADDR, 32);
  espReq.request();
  while (espReq.available())
  {
    dataInEsp += char(espReq.read());
  }
  parsingDataEsp();
  delay(10);
}

void requestFromStm()
{
  Wire.requestFrom(STM_ADDR, 32);
  while (Wire.available())
  {
    dataIn += char(Wire.read());
  }
  parsingDataStm();
}

void beep(int count, int dly)
{
  for (int i = 0; i < count; i++)
  {
    digitalWrite(PIN_BUZZER, HIGH);
    digitalWrite(PIN_LED, HIGH);
    delay(dly);
    digitalWrite(PIN_BUZZER, LOW);
    digitalWrite(PIN_LED, LOW);
    delay(dly);
  }
}

void enableAlarm(String code){
      nextion.state = code;
      transmitNextion("t22", code);
}


void disableAlarm(){
      transmitNextion("t22", "Z");
}
