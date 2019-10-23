/*
*	Interface for digital IO for the ESP8266 NodeMCU board
*
*	Author: Josh Perry <josh.perry245@gmail.com>
*	Created: 09/20/2019
*   Copyright 2019
*/

#pragma once

#include <Arduino.h>
#include "./config/Configuration.h"

typedef void (*ButtonCallback)();

class DigitalIO {
    private:
        // Singleton handling
        DigitalIO();
        void operator=(DigitalIO const&) = delete;
        DigitalIO(DigitalIO const&) = delete; 

        // Data members
        bool initialized = false;
        const int switch_input = SWITCH_INPUT;
        const int switch_led = SWITCH_LED_INDICATOR;
        const int data_line = DATA_LINE;
        const int clock = CLOCK_LINE;
        ButtonCallback button_callback;

        // Methods
        void initPins();

    public:
        static DigitalIO& GetInstance();
        ~DigitalIO() { LOG_DEBUG("DIGITAL IO: Singleton deleted"); };
        void set_button_callback(ButtonCallback callback);
        void updateSwitchLED();
        void setProgramMode();
        void setVerifyMode();

        // GPIO logic levels
        void setClockLow();
        void setClockHigh();
        void setData(const bool& bit);
        bool getClockValue();
        bool getDataLineValue();
};
