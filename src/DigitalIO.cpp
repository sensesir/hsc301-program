/*
*	Interface for digital IO for the ESP8266 NodeMCU board
*
*	Author: Josh Perry <josh.perry245@gmail.com>
*	Created: 09/20/2019
*   Copyright 2019
*/

#include "DigitalIO.hpp"

DigitalIO& DigitalIO::GetInstance() {
    static DigitalIO instance;
    if (!(instance.initialized)) instance.initPins();
    return instance;
}

DigitalIO::DigitalIO() {}

void DigitalIO::initPins() {
    pinMode(this->switch_input, INPUT);
    // pinMode(this->switch_led, OUTPUT);
    // digitalWrite(this->switch_led, LOW);
    setProgramMode();

    LOG_DEBUG("DIGITAL IO: Initialized pins\n");
    this->initialized = true;
}

void DigitalIO::setProgramMode() {
    LOG_DEBUG("DIGITAL IO: Setting IO to program mode (output)\n");

    // Data transfer
    pinMode(this->data_line, OUTPUT);
    digitalWrite(this->data_line, LOW);
    pinMode(this->clock, OUTPUT);
    digitalWrite(this->clock, LOW);
}

void DigitalIO::setVerifyMode() {
    LOG_DEBUG("DIGITAL IO: Setting IO to verify mode (input)\n");

    // Data verification
    pinMode(this->data_line, INPUT);
    pinMode(this->clock, OUTPUT);
}

void DigitalIO::readButton() {
    const int new_switch_state = digitalRead(this->switch_input);

    // If LED was off, and switch is now low, then this is a new press
    if (switch_state == HIGH && new_switch_state == LOW) {
        LOG_DEBUG("DIGITAL IO: Button press detected [debounce delay]\n");
        delay(1000);
        this->button_callback();
    }

    switch_state = new_switch_state;
}

void DigitalIO::set_button_callback(ButtonCallback callback){
    this->button_callback = callback;
}

void DigitalIO::setClockLow() {
    digitalWrite(this->clock, LOW);
}
        
void DigitalIO::setClockHigh() {
    digitalWrite(this->clock, HIGH);
}
        
void DigitalIO::setData(const bool& bit) {
    digitalWrite(this->data_line, bit);
}

// Verification
bool DigitalIO::getClockValue() {
    return digitalRead(clock);
}

bool DigitalIO::getDataLineValue() {
    return digitalRead(data_line);
}