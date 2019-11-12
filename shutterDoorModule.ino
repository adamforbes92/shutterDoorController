// Date and time functions using a DS1307 RTC connected via I2C and Wire lib
#include <Wire.h>
#include "RTClib.h"
#include "Sim800l.h"
#include <SoftwareSerial.h> //is necesary for the library!! 

#define smsWelcome F("Hello Everyone. I've just woken up!")
#define smsHelp F("Blah, Blah...")

//Interrupt pins: 2, 3, 18, 19, 20, 21
#define input_PIR_1 2
#define input_PIR_2 3
#define input_lights 18
#define input_doorUP 19
#define input_doorDOWN 20
#define input_additionalInterrupt 21

#define output_doorUP 4
#define output_doorDOWN 5
#define output_lights 6
#define output_alarm 7
#define output_strobe 8
#define output_additional 9

#define GSMrx 10                                                                                              // GSM RX Pin
#define GSMtx 11                                                                                              // GSM RX Pin

#define mainsPower 12

#define DEBUG                                                                                                // If debug is defined, allow serial
#ifdef DEBUG
#define DEBUG_PRINT(x)     Serial.print (x)
#define DEBUG_PRINTDEC(x)  Serial.print (x, DEC)
#define DEBUG_PRINTLN(x)   Serial.println (x)
#else
#define DEBUG_PRINT(x)
#define DEBUG_PRINTDEC(x)
#define DEBUG_PRINTLN(x)
#endif

RTC_DS1307 rtc;                                                                                               // Connect A4 to SDA. Connect A5 to SCL.
Sim800l GSM;                                                                                                  // Sim800L GSM Module

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};   // Define day "text"

String smsNumbers[] = {"+447866178122", "+447927090241"};
String smsRead = "";

unsigned long lightOffLimit = 180000;//  1800000                                                                        // the duration to run for once activated.  1500000 = 1000 * 60 * 15 (1000ms, 60 sec, 18 mins)
unsigned long lightOffDuration = 0;//  1800000

unsigned long doorUpLimit = 180000;
unsigned long doorUpDuration = 0;

unsigned long previousMillis = 0;                                                                             // will store last time Millis was called (better than delay
unsigned long currentMillis = 0;
int numberPosition = -1;

bool smsError;                                                                                                // bool for smsError
bool lightsOn = false;

String doorStatus = "NULL";

void setup () {
  pinMode(output_doorUP, OUTPUT); pinMode(output_doorDOWN, OUTPUT);
  pinMode(output_lights, OUTPUT); pinMode(output_alarm, OUTPUT); pinMode(output_strobe, OUTPUT);
  pinMode(output_additional, OUTPUT); pinMode(mainsPower, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(input_PIR_1), alarm, RISING);
  attachInterrupt(digitalPinToInterrupt(input_PIR_2), alarm, RISING);
  attachInterrupt(digitalPinToInterrupt(input_lights), toggleLights, RISING);
  attachInterrupt(digitalPinToInterrupt(input_doorUP), doorUP, RISING);
  attachInterrupt(digitalPinToInterrupt(input_doorDOWN), doorDOWN, RISING);
  attachInterrupt(digitalPinToInterrupt(input_additionalInterrupt), alarm, RISING);

  Serial.begin(57600);                                                                                        // begin serial

  previousMillis = currentMillis;
  DEBUG_PRINTLN(F("Preparing GSM Module"));
  GSM.begin(); delay(150); GSM.delAllSms();                                                                               // prepare GSM module

  rtc.begin();
  //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
}

void loop ()
{
  DateTime nowTime = rtc.now();
  currentMillis = millis();
}

void smsCheck() {
  if ((GSM.getNumberSms(1).toInt() > 0)) {
    DEBUG_PRINTLN("Reading SMS");
    smsRead = GSM.readSms(1); smsRead.toUpperCase(); GSM.delAllSms();
    DEBUG_PRINTLN(smsRead);

    if (containsText(smsRead, "HELP") && smsRead.indexOf(smsNumbers) >= 0) {
      //asking for help
      if (numberPosition != -1) {
        smsError = GSM.sendSms(smsNumbers[numberPosition], smsHelp);
        GSM.delAllSms();
      }
    }
  }
}

bool containsText(String smsRead, String containsWord) {
  return smsRead.indexOf(containsWord) >= 0;
}

bool containsNumber(String smsRead) {
  bool isFound = false;
  numberPosition = -1;

  for (byte index = 0; index < sizeof(smsNumbers); index++) {
    isFound = containsText(smsRead, smsNumbers[index]);
    if (isFound) {
      numberPosition = index;
      break;
    }
  }
  return isFound;
}

void alarm() {
  return;
}

void toggleLights() {
  return;
}

void doorUP() {
  return;
}

void doorDOWN() {
  return;
}
