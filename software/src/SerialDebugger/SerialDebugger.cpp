#include "SerialDebugger.hpp"
#include "HashMap.h"

// Create the global instance
SerialDebugger_ SerialDebugger = SerialDebugger_();
// SerialDebugger_ SerialDebugger = SerialDebugger_(SerialDisplayType::serialMonitor);

/********************
 * Constructors
 ********************/
SerialDebugger_::SerialDebugger_(SerialDisplayType displayType) : SerialDisplay(displayType) {
  mNextPrintMillis = millis();
  mStatusValues = HashMap<String, String, MAX_DEBUG_VALUES>();
}

SerialDebugger_::SerialDebugger_() : SerialDebugger_(SerialDisplayType::ansi_vt100) {}

/********************
 * Methods
 ********************/
void SerialDebugger_::updateValue(String variable, void* value) {
  // Convert the pointer into a memory address String
  char memAddressBuffer[65];
  sprintf(memAddressBuffer, "%p", (void *) value);
  if (MAINTAINMAP) {
    mStatusValues[variable] = String(memAddressBuffer);
    printUpdate();
  } else {
    Serial.println(variable + ": " + String(memAddressBuffer));
  }
}

void SerialDebugger_::updateValue(String variable, String value) {
  if (MAINTAINMAP) {
    mStatusValues[variable] = value;
    printUpdate();
  } else {
    Serial.println(variable + ": " + value);
  }
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