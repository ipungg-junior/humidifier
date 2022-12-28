#include <Arduino.h>
#include <Wire.h>
#include <WirePacker.h>
#include <WireSlaveRequest.h>

int totalC, readingsC[10], readIndexC = 0, numReadingsC = 10;
int counterGlobal = 0, counterAlarmOutputPasien = 0, counterAlarmChamber = 0, counterAirChamber = 0, prevAirChamber = 0;
bool getHeaterPlateStep = true, getOutputPasienStep = true;
int numReadings = 10;
int readings[10];  // the readings from the analog input
int readIndex = 0; // the index of the current reading
int total = 0;     // the running total
int average = 0;   // the average
const int numReadingsA = 10;
int readingsA[numReadingsA]; // the readings from the analog input
int readIndexA = 0;          // the index of the current reading
int totalA = 0;              // the running total
int averageA = 0;            // the average
int j, j2, j3;
int runningHeaterPlate = 15;

String mySSID = "MJM-WIFI", myPASS = "HMD2022";
int initSensorSTM = 0;
String dt[12], dt2[12], dt3[12], dataIn, dataInEsp;
#include "stdcomm.h"

void setup()
{
  Serial.begin(9600);
  Wire.begin();
  delay(5300);
  pinMode(PIN_RELAY_WIRE, OUTPUT);
  pinMode(PIN_RELAY_PLATE, OUTPUT);
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);
  for (int thisReading = 0; thisReading < numReadings; thisReading++)
  {
    readings[thisReading] = 0;
  }
  for (int thisReadingA = 0; thisReadingA < numReadingsA; thisReadingA++)
  {
    readingsA[thisReadingA] = 0;
  }
  delay(1);

  // Init OUtput Pasien
  total = total - readings[readIndex];
  readings[readIndex] = analogRead(PIN_PROBE_1);
  total = total + readings[readIndex];
  readIndex = readIndex + 1;
  if (readIndex >= numReadings)
  {
    readIndex = 0;
  }
  int hasilprobe = total / numReadings;

  initOutputPasien = (hasilprobe * (-0.1885)) + 109.62;

  total = 0;
  numReadings = 10;
  readings[10]; // the readings from the analog input
  readIndex = 0;

  requestFromStm();
  initSensorSTM += stm.sensorSTM;
  initSensors();
  beep(2, 250);
  // beep(1, 800);
  digitalWrite(PIN_BUZZER, LOW);
  digitalWrite(PIN_LED, LOW);
  delay(50);
}

void loop()
{

  // stabilkan Sensor STM (kalibrasi di STM)
  requestFromStm();
  requestFromEsp();

  // stabilkan Sensor probe
  total = total - readings[readIndex];
  readings[readIndex] = analogRead(PIN_PROBE_1);
  total = total + readings[readIndex];
  readIndex = readIndex + 1;
  if (readIndex >= numReadings)
  {
    readIndex = 0;
  }
  int hasilprobe = total / numReadings;

  sensor.outputPasien = (hasilprobe * (-0.1885)) + 109.62;
  sensor.outputPasien = sensor.outputPasien + 72;

  delay(1);
  // stabilkan Sensor termistor
  totalA = totalA - readingsA[readIndexA];
  readingsA[readIndexA] = analogRead(PIN_HEATER_PLATE);
  totalA = totalA + readingsA[readIndexA];
  readIndexA = readIndexA + 1;
  if (readIndexA >= numReadingsA)
  {
    readIndexA = 0;
  }
  int hasilplate = totalA / numReadingsA;
  sensor.heaterPlate = ((-0.3285) * hasilplate) + 227.25; // y = -0,1411x + 225,94

  // Update new Init heater plate
  if (getHeaterPlateStep)
  {
    prevAirChamber = sensor.heaterPlate;
    getHeaterPlateStep = false;
  }

  if (getOutputPasienStep)
  {
    initOutputPasien = sensor.outputPasien;
    getOutputPasienStep = false;
  }

  sensor.sensorDua = (analogRead(PIN_PROBE_2) * (-0.1885)) + 109.62;
  String nextBuff = "";

  /**
      NEXTION SEND DATA INSTRUCTION
  */
  if (Serial.available())
  {
    delay(50);
    while (Serial.available())
    {
      nextBuff += char(Serial.read());
    }

    parsingNextion(nextBuff);

    // STM32 Communication (send)
    transmitToSTM(0x03, 1, sensor.outputPasien);
    transmitToSTM(0x03, 2, sensor.sensorDua);
    transmitToSTM(0x03, 3, sensor.chamber);
    transmitToSTM(0x03, 4, sensor.heaterPlate);
    transmitToSTM(0x03, 5, sensor.sensorLima);
    transmitToSTM(0x03, 6, sensor.sensorEnam);
    transmitToSTM(0x03, 7, nextion.targetTemp);
    transmitToSTM(0x03, 8, nextion.instructionStm);

    if (nextion.instructionStm == 1)
    {
      digitalWrite(PIN_LED, LOW);
    }
    else
    {
      digitalWrite(PIN_LED, HIGH);
    }

    if (nextion.relay == 1)
    {
      digitalWrite(PIN_RELAY_WIRE, HIGH);
      digitalWrite(PIN_RELAY_PLATE, HIGH);
    }
    else
    {
      digitalWrite(PIN_RELAY_WIRE, LOW);
      digitalWrite(PIN_RELAY_PLATE, LOW);
    }

    // ESP Communication (send)
    transmitToEsp(0x04, 0, sensor.outputPasien);
    transmitToEsp(0x04, 1, sensor.sensorDua);
    transmitToEsp(0x04, 2, sensor.chamber);
    transmitToEsp(0x04, 3, sensor.heaterPlate);
    transmitToEsp(0x04, 4, sensor.sensorLima);
    transmitToEsp(0x04, 5, sensor.sensorEnam);
    transmitToEsp(0x04, 6, nextion.ssid);
    transmitToEsp(0x04, 7, nextion.passWifi);
    transmitToEsp(0x04, 8, nextion.instructionEsp);
    transmitToEsp(0x04, 9, nextion.instructionStm);
    transmitToEsp(0x04, 10, nextion.targetTemp);
    transmitToEsp(0x04, 11, stm.inHeaterPlateStatus);
    transmitToEsp(0x04, 12, stm.inHeaterWire);
    transmitToEsp(0x04, 13, stm.inMatchingTemp);
    transmitToEsp(0x04, 14, stm.inCompareTemp);
    transmitToEsp(0x04, 15, stm.inAlarmSTM);

  }

  transmitNextion("s1", stm.sensorSTM);
  transmitNextion("s3", stm.sensorSTM);
  transmitNextion("s21", stm.sensorSTM);
  transmitNextion("s2", (sensor.outputPasien - 79));
  transmitNextion("s4", sensor.heaterPlate);
  transmitNextion("s5", sensor.sensorLima);
  transmitNextion("s6", sensor.sensorEnam);
  transmitNextion("s7", nextion.ssid);
  transmitNextion("s8", nextion.passWifi);
  transmitNextion("s9", nextion.instructionEsp);
  transmitNextion("s10", nextion.instructionStm);
  transmitNextion("s11", nextion.targetTemp);
  transmitNextion("s12", esp.inSerialNum);
  transmitNextion("s13", esp.inKondisiWifi);
  transmitNextion("s14", esp.inAlarmESP);
  transmitNextion("s15", esp.inAlertWeb);
  transmitNextion("s16", stm.inHeaterWire);
  transmitNextion("s17", stm.inHeaterPlateStatus);
  transmitNextion("s18", stm.inMatchingTemp);
  transmitNextion("s19", stm.inCompareTemp);
  transmitNextion("s20", stm.inAlarmSTM);

  dataIn = "";
  dataInEsp = "";

  /**
        Kabel Probe Terlepas (ALARM)
  */
  if (analogRead(PIN_PROBE_2) > 550)
  {
    if (nextion.hold == false)
    {
      enableAlarm("C");
    }
    else
    {
      if (nextion.state == "C")
      {
        disableAlarm();
      }
    }
  }
  else
  {
    if (nextion.state == "C")
    {
      disableAlarm();
    }
  }

  /**
         Alarm Output Pasien
  */
  //  if (stm.inHeaterPlateStatus == 1)
  //  {
  //    if (counterAlarmOutputPasien == 75)
  //    { // 3 menit
  //      counterAlarmOutputPasien = 0;
  //      if (sensor.outputPasien - initOutputPasien < 1)
  //      {
  //        if (nextion.hold == false)
  //        {
  //          enableAlarm("E");
  //        }
  //        else
  //        {
  //          if (nextion.state == "E")
  //          {
  //            disableAlarm();
  //          }
  //        }
  //      }
  //      else
  //      {
  //        if (nextion.state == "E")
  //        {
  //          disableAlarm();
  //        }
  //      }
  //    }
  //    else
  //    {
  //      counterAlarmOutputPasien++;
  //    }
  //    if (counterAlarmOutputPasien > 100)
  //    { // 3 menit
  //      counterAlarmOutputPasien = 0;
  //      beep(1, 50);
  //    }
  //  }
  //  else
  //  {
  //    counterAlarmOutputPasien = 0;
  //    getOutputPasienStep = true;
  //  }

  /**
        Alarm Air chamber
  */
  //
  //  int totals, readingsB[10], readIndexB = 0, numReadingsB = 10;
  //  int newAirChamber, AirChamber;
  //  if (stm.inHeaterPlateStatus == 1)
  //  {
  //    if (counterAirChamber == 50)
  //    {
  //      counterAirChamber = 0;
  //      int s = analogRead(PIN_HEATER_PLATE);
  //      newAirChamber = s;
  //      AirChamber = ((-0.3285) * newAirChamber) + 227.25;
  //
  //      if ((AirChamber - prevAirChamber) < 2)
  //      {
  //        enableAlarm("A");
  //      }
  //      else
  //      {
  //        if (nextion.state == "A")
  //        {
  //          disableAlarm();
  //        }
  //      }
  //    }
  //    else
  //    {
  //      counterAirChamber++;
  //    }
  //
  //    if (counterAirChamber > 50)
  //    {
  //      counterAirChamber = 0;
  //    }
  //  }
  //  else
  //  {
  //    counterAirChamber = 0;
  //    getHeaterPlateStep = true;
  //  }

  if (nextion.counterHold == 35)
  {
    nextion.counterHold = 0;
    if (nextion.hold == true)
    {
      nextion.hold = false;
    }
    else
    {
      nextion.hold = true;
    }
  }
  else
  {
    if (nextion.counterHold > 35)
    {
      nextion.counterHold = 0;
    }
    nextion.counterHold++;
  }


  /**
        Beep while stm running (2minutes)
  */
  if (stm.inHeaterPlateStatus == 1)
  {
    if (counterGlobal == 200)
    {
      counterGlobal = 0;
      beep(1, 1000);
    }
    else
    {
      counterGlobal++;
    }
  }
  else
  {
    counterGlobal = 0;
  }

}