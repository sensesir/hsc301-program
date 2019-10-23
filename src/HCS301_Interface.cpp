/*
*	Interface for the KeeLoQ HCS301 chip
*
*	Author: Josh Perry <josh.perry245@gmail.com>
*	Created: 10/23/2019
*   Copyright 2019
*/

#include "HCS301_Interface.hpp"

HCS301Interface::HCS301Interface() {}

void HCS301Interface::sendCode() {
    // Send code from config
    const std::string new_code = codeFromConfig();
    if (new_code.length()) sendCode(new_code);
}

// Generates string code from config and validates elements
const std::string HCS301Interface::codeFromConfig() {
    const int start_index = SERIAL_NUMBER.length() - 10;
    const std::string config = SERIAL_NUMBER.substr(start_index, 10) + OVERFLOW + VLOW + HCS_BAUD + RESERVED_S;

    // Validations
    if (CRYPT_KEY.length() != 64)       LOG_RELEASE("Crypt key incorrect length\n");
    if (SYNC_COUNTER.length() != 16)    LOG_RELEASE("Sync counter incorrect length\n"); 
    if (RESERVED_16.length() != 16)      LOG_RELEASE("Reserved buffer incorrect length\n");
    if (SERIAL_NUMBER.length() != 32)   LOG_RELEASE("Serial number incorrect length\n");
    if (SEED_NUMBER.length() != 32)     LOG_RELEASE("Seed incorrect length\n");
    if (config.length() != 16)          LOG_RELEASE("Config incorrect length\n");

    const std::string full_code = CRYPT_KEY + SYNC_COUNTER + RESERVED_16 + SERIAL_NUMBER + SEED_NUMBER + RESERVED_16 + config;
    if (full_code.length() != 192) {
        LOG_RELEASE("Full code incorrect length\n");
        return "";
    }

    return full_code;
}

void HCS301Interface::sendCode(const std::string bits) {
    LOG_RELEASE("HCS: Sending new code to HCS301 chip\n");
    loadNewBitstream(bits);
    sendStartSignal();

    int word_counter {0};

    // Go through the bitstream until completion
    while(!bitstream.empty()) {
        sendNextBit(bitstream.front());
        bitstream.pop();
        word_counter ++;

        if (word_counter >= WORD_LENGTH) {
            word_counter = 0;
            delayMicroseconds(TWC); 
        }
    }

    // Complete
    LOG_RELEASE("HCS: Completed send\n");
    verifySignal();
}

void HCS301Interface::sendStartSignal() {
    DigitalIO& digital_interface = DigitalIO::GetInstance();
    digital_interface.setData(LOW);

    digital_interface.setClockHigh();
    delayMicroseconds(TPS);
    digital_interface.setData(HIGH);
    delayMicroseconds(TPH1);
    digital_interface.setData(LOW);
    delayMicroseconds(TPH2);
    digital_interface.setClockLow();
    delayMicroseconds(TPBW);
}

void HCS301Interface::sendNextBit(const bool& next_bit) {
    DigitalIO& digital_interface = DigitalIO::GetInstance();
    digital_interface.setClockLow();
    digital_interface.setData(next_bit);
    digital_interface.setClockHigh();
    delayMicroseconds(TCLKH);               // Verify, this is critical
    digital_interface.setClockLow();
    delayMicroseconds(TCLKL);               // Verify, this is critical
}

void HCS301Interface::loadNewBitstream(const std::string new_stream){
    LOG_DEBUG("HCS: Loading new input stream");
    bitstream = std::queue<bool>();
    for (char bit : new_stream) {
        if (bit == '0')      bitstream.emplace(false);
        else if (bit == '1') bitstream.emplace(true);
        else LOG_RELEASE("HCS: ERROR => UNKNOWN BIT TYPE !!!");
    }

    // Temp
    printCurrentBitstream();
}

void HCS301Interface::printCurrentBitstream() {
    LOG_DEBUG("HCS: Current bitstream = ");
    std::queue<bool> print_queue(bitstream); 
    
    while (!print_queue.empty()) {
        bool bit = print_queue.front();
        print_queue.pop();
        LOG_DEBUG(bit);
        LOG_DEBUG(" ");
    }

    LOG_DEBUG("\n");
}

/*
 *      Code verification
 */

void HCS301Interface::verifySignal() {
    LOG_RELEASE("HCS: Preparing for verification");
    DigitalIO::GetInstance().setVerifyMode();
    verified_bitstream = "";

    while (verified_bitstream.length() < CONFIG_BITSTREAM_LENGTH) {
        std::string next_bit = nextVerifiedBit();
        if (next_bit.length()) {
            verified_bitstream += next_bit;
        } else {
            return;
        }
    }

    // Complete - print read stream
    LOG_RELEASE("HCS: Verified bitstream = ");
    LOG_RELEASE(verified_bitstream.c_str());
    LOG_RELEASE("\n");
}

const std::string HCS301Interface::nextVerifiedBit() {
    // Look for rising edge on the clock
    DigitalIO& digital_interface = DigitalIO::GetInstance();
    
    bool read_timeout = false;
    bool clock_val = false;
    unsigned long start_millis = millis();

    while (!clock_val) {
        clock_val = digital_interface.getClockValue();
        
        // Ensure no hang
        unsigned long current_millis = millis();
        if ((current_millis - start_millis) > READ_TIMEOUT) {
            read_timeout = true;
            break;
        }
    }

    if (read_timeout) {
        LOG_RELEASE("HCS: ERROR => Read timeout");
        const std::string next_bit{""};
        return next_bit;
    }

    // Found rising edge, read data bit
    bool nextBit = digital_interface.getDataLineValue();
    if (nextBit) return std::string("1");
    else return std::string("0");
}


HCS301Interface::~HCS301Interface() {
    LOG_DEBUG("HCS: Destroyed interface\n");
}