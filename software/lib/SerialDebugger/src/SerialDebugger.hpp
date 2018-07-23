#ifndef __SERIALDEBUGGER_H_INCLUDED__
#define __SERIALDEBUGGER_H_INCLUDED__

#include <Arduino.h>
#include "HashMap.h"
#include "SerialDisplay.hpp"

const unsigned int MAX_DEBUG_VALUES = 30;

class SerialDebugger_ : public SerialDisplay {
public:
  SerialDebugger_();

  void updateValue(String variable, String value);

  void updateValue(String variable, float value) {updateValue(variable, String(value));}
  void updateValue(String variable, int value) {updateValue(variable, String(value));}
  void updateValue(String variable, long value) {updateValue(variable, String(value));}
  void updateValue(String variable, unsigned long value) {updateValue(variable, String(value));}  
  void updateValue(String variable, uint8_t value) {updateValue(variable, String(value));}
  void updateValue(String variable, uint16_t value) {updateValue(variable, String(value));}

  void printUpdate();

  unsigned long mNextPrintMillis;
  HashMap<String, String, MAX_DEBUG_VALUES> mStatusValues;

};
// Make the instance globally accessible
extern const SerialDebugger_ SerialDebugger;

#endif // __SERIALDEBUGGER_H_INCLUDED__