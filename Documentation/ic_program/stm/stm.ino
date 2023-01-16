#include <Wire_slave.h>
#include <PID_v1.h>
#define NYALA LOW
#define MATI HIGH
#define PIN_PROBE_1 PA1
const int numReadings = 20;

int readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // the average

//parsing data
String dt[12];
String c = "";

//minta data dari arduino
double sensor1, sensor2, sensor3, sensor4, sensor5, sensor6;
double  sensorSTM;  // 1 thermistor, 2 dan 3 temp probe, 4 dan 5 sensor arus
String ssid, password;
int intruksiesp32, intruksistm32 = 0;
double targetsuhu;  //pid

//kirim data ke arduino
int heaterwire = 1, heaterplate = 9, matchingtemp = 5, comparetemp = 3;
String alarmstm = "stmworkssss";

//digital
int digitalheaterplate = PB11;

//PID
double Output;
double Kp = 1500, Ki = 0.098, Kd = 2.56;
PID myPID(&sensorSTM, &Output, &targetsuhu, Kp, Ki, Kd, DIRECT);
int WindowSize = 5000;
unsigned long windowStartTime;

void setup(){
  Serial.begin(9600);
  
  pinMode(digitalheaterplate, OUTPUT);
  digitalWrite(digitalheaterplate, MATI);
  Wire.begin(0x03);  // join i2c bus with address #4
  //pid
  windowStartTime = millis();
  myPID.SetOutputLimits(0, WindowSize);
  myPID.SetMode(AUTOMATIC);
    for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
}
}

void loop(){
  total = total - readings[readIndex];
  readings[readIndex] = analogRead(PIN_PROBE_1);
  total = total + readings[readIndex];
  readIndex = readIndex + 1;
  if (readIndex >= numReadings) {
    readIndex = 0;
  }
  average = total / numReadings;
  sensorSTM = ((-0.031)*average)+109.62; //kalibrasi y = -0,1231x + 109,62

  Wire.onRequest(requestEvent);
  Wire.onReceive(receiveEvent);

  if (intruksistm32 == 0) {
    digitalWrite(digitalheaterplate, MATI);
    heaterplate = 0;
    heaterwire = 0;
    alarmstm = "PID off";
  } 
  
  else {
    PIDrun();
    heaterplate = 1;
    heaterwire = 1;
    alarmstm = "PID heat";
  }
  
  c = "";
}


void receiveEvent(int howMany) {
  while (Wire.available()) {  // loop through all but the last
    c += char(Wire.read());   // receive byte as a character
  }
  parsingdata();
}

void parsingdata() {
  int j = 0;
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
    case 1:
      sensor1 = double(dt[1].toFloat());
      break;
    case 2:
      sensor2 = double(dt[1].toFloat());
      break;
    case 3:
      sensor3 = double(dt[1].toFloat());
      break;
    case 4:
      sensor4 = double(dt[1].toFloat());
      break;
    case 5:
      sensor5 = double(dt[1].toFloat());
      break;
    case 6:
      sensor6 = double(dt[1].toFloat());
      break;
    case 7:
      targetsuhu = double((dt[1].toFloat()) + 1.5);
      break;
    case 8:
      intruksistm32 = dt[1].toInt();
      break;
  }
}

void PIDrun() {
  myPID.Compute();
  if (millis() - windowStartTime > WindowSize) {
    windowStartTime += WindowSize;
  }
  if (Output < millis() - windowStartTime) {
    digitalWrite(digitalheaterplate, MATI);
  }
  else {
    digitalWrite(digitalheaterplate, NYALA);
  }
  if (targetsuhu <= sensorSTM) matchingtemp = 1;
  else matchingtemp = 0;
  Serial.println(sensorSTM);
}

void requestEvent() {
  Wire.write("*");
  ReplydataATMEGA(heaterwire);
  ReplydataATMEGA(heaterplate);
  ReplydataATMEGA(matchingtemp);
  ReplydataATMEGA(comparetemp);
  ReplydataATMEGA(alarmstm);
  ReplydataATMEGA(sensorSTM);
  Wire.write("#");  // as expected by master
}

void ReplydataATMEGA(int val2) {
  String dataframe = String(val2) + ":";
  Wire.write(dataframe.c_str());
}

void ReplydataATMEGA(String val) {
  String dataframe = val + ":";
  Wire.write(dataframe.c_str());
}