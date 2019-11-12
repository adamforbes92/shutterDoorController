#include "Arduino.h"
#include "Sim800l.h"
#include <SoftwareSerial.h>

SoftwareSerial SIM(RX_PIN, TX_PIN);
//String _buffer;

void Sim800l::begin() {
  SIM.begin(19200);
  _buffer.reserve(250); //reserve memory to prevent intern fragmention
}

String Sim800l::_readSerial() {
  _timeout = 0;
  while  (!SIM.available() && _timeout < 12000  )
  {
    delay(13);
    _timeout++;
  }
  if (SIM.available()) {
    return SIM.readString();
  }
}

bool Sim800l::sendSms(String number, String text) {

  SIM.print (F("AT+CMGF=1\r")); //set sms to text mode
  _buffer = _readSerial();
  SIM.print (F("AT+CMGS=\""));  // command to send sms
  SIM.print (number);
  SIM.print(F("\"\r"));
  _buffer = _readSerial();
  SIM.print (text);
  //Serial.println(text + "\r");
  SIM.print ("\r");
  //change delay 100 to readserial
  _buffer = _readSerial();
  SIM.print((char)26);
  _buffer = _readSerial();
  //expect CMGS:xxx   , where xxx is a number,for the sending sms.
  if (((_buffer.indexOf("CMGS") ) != -1 ) ) {
    return true;
  }
  else {
    return false;
  }
}

String Sim800l::getNumberSms(uint8_t index) {
  _buffer = readSms(index);
  if (_buffer.length() > 10) //avoid empty sms
  {
    uint8_t _idx1 = _buffer.indexOf("+CMGR:");
    _idx1 = _buffer.indexOf("\",\"", _idx1 + 1);
    return _buffer.substring(_idx1 + 3, _buffer.indexOf("\",\"", _idx1 + 4));
  } else {
    return "";
  }
}

String Sim800l::readSms(uint8_t index) {
  SIM.print (F("AT+CMGF=1\r"));
  if (( _readSerial().indexOf("ER")) == -1) {
    SIM.print (F("AT+CMGR="));
    SIM.print (index);
    SIM.print("\r");
    _buffer = _readSerial();
    if (_buffer.indexOf("CMGR:") != -1) {
      return _buffer;
    }
    else return "";
  }
  else
    return "";
}

bool Sim800l::delAllSms() {
  SIM.print(F("at+cmgda=\"del all\"\n\r"));
  _buffer = _readSerial();
  if (_buffer.indexOf("OK") != -1) {
    return true;
  } else {
    return false;
  }
}
