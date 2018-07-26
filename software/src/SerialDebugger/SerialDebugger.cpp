#include "SerialDebugger.hpp"
#include "HashMap.h"

// Create the global instance

SerialDebugger_ SerialDebugger = SerialDebugger_();

SerialDebugger_::SerialDebugger_() : SerialDisplay(SerialDisplayType::ansi_vt100) {
  mNextPrintMillis = millis();
  mStatusValues = HashMap<String, String, MAX_DEBUG_VALUES>();
}

void SerialDebugger_::updateValue(String variable, String value) {
  mStatusValues[variable] = value;
  printUpdate();
}

void SerialDebugger_::printUpdate() {
  if (millis() > mNextPrintMillis) {
    mNextPrintMillis = millis() + 200;
    
    clearSerialDisplay();

    Serial.println("------ Now: " + String(millis()) + " ---------");

    for (unsigned int i=0; i<mStatusValues.size(); i++) {
      Serial.println(mStatusValues.keyAt(i) + ": " + mStatusValues.valueAt(i));
    }
  }  
}