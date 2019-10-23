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
    pinMode(this->switch_led, OUTPUT);
    digitalWrite(this->switch_led, LOW);
    setProgramMode();

    LOG_DEBUG("DIGITAL IO: Initialized pins\n");
    this->initialized = true;
}

void DigitalIO::setProgramMode() {
    LOG_DEBUG("DIGITAL IO: Setting IO to program mode (output)");

    // Data transfer
    pinMode(this->data_line, OUTPUT);
    digitalWrite(this->data_line, LOW);
    pinMode(this->clock, OUTPUT);
    digitalWrite(this->clock, LOW);
}

void DigitalIO::setVerifyMode() {
    LOG_DEBUG("DIGITAL IO: Setting IO to verify mode (input)");

    // Data verification
    pinMode(this->data_line, INPUT);
    pinMode(this->clock, INPUT);
}

void DigitalIO::updateSwitchLED() {
    const int previous_state = digitalRead(this->switch_led);
    const int switch_state = digitalRead(this->switch_input);
    
    // Switch is pulled up to HIGH, if pressed, goes LOW
    // LED should mimick switch state (active/passive)
    if (previous_state == switch_state) digitalWrite(this->switch_led, !switch_state);          
    
    // If LED was off, and switch is now low, then this is a new press
    if (previous_state == LOW && switch_state == LOW) {
        LOG_DEBUG("DIGITAL IO: Button press detected\n");
        this->button_callback();
    }
}

void DigitalIO::set_button_callback(ButtonCallback callback){
    this->button_callback = callback;
}

// Verification
bool DigitalIO::getClockValue() {
    return digitalRead(clock);
}

bool DigitalIO::getDataLineValue() {
    return digitalRead(data_line);
}