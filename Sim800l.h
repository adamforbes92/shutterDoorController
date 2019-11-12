#ifndef Sim800l_h
#define Sim800l_h
#include <SoftwareSerial.h>
#include "Arduino.h"

#define RX_PIN 10
#define TX_PIN 11

class Sim800l
{
  private:
    int _timeout;
    String _buffer;
    String _readSerial();

  public:

    void begin();
    bool sendSms(String number, String text);
    String readSms(uint8_t index); //return all the content of sms
    String getNumberSms(uint8_t index); //return the number of the sms..
    bool delAllSms();     // return :  OK or ERROR ..
};
#endif
