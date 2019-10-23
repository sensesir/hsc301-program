/*
*	Interface for the KeeLoQ HCS301 chip
*
*	Author: Josh Perry <josh.perry245@gmail.com>
*	Created: 10/23/2019
*   Copyright 2019
*/

#include <Arduino.h>
#include "./config/Configuration.h"
#include "./config/HCS301_CONFIG.h"
#include "DigitalIO.hpp"
#include <queue>
// #include <vector>
#include <string>

class HCS301Interface {
private:
    void sendStartSignal();
    void loadNewBitstream(const std::string new_stream);
    void printCurrentBitstream();
    const std::string nextVerifiedBit();
    const std::string codeFromConfig();
    void sendNextBit(const bool& next_bit);
    void verifySignal();

    std::queue<bool> bitstream;
    std::string verified_bitstream;

public:
    HCS301Interface();
    ~HCS301Interface();

    void sendCode();        
    void sendCode(const std::string bits);
};


