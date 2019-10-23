/*
 * R&D project to get an ESP MCU to interface with an HCS301 
 * to work with a rolling code RF receiver.
 * 
 * Collaborator(s): Josh Perry <josh.perry245@gmail.com>
 * Created: 09/20/2019
 */

// Global includes
#include <memory>

// Local includes
#include "./src/config/Configuration.h"
#include "./src/DigitalIO.hpp"
#include "./src/HCS301_Interface.hpp"

std::unique_ptr<HCS301Interface> keeloq_interface_ptr;

void setup() {
  Serial.begin(BAUD_RATE);
  LOG_RELEASE("MAIN: Booting RF code...\n");

  keeloq_interface_ptr = std::unique_ptr<HCS301Interface>(new HCS301Interface());
  void (*callback_ptr)() = []() { keeloq_interface_ptr->sendCode(); };  // Should check if ptr is non-null
  DigitalIO::GetInstance().updateSwitchLED();
}

void loop() {
  DigitalIO::GetInstance().updateSwitchLED();
}
