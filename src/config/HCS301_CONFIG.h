/*
*	Header file for HCS301 configuration
*
*	Author: Josh Perry <josh.perry245@gmail.com>
*	Created: 10/23/2019
*   Copyright 2019
*/

// Timing (all in micro seconds)
#define TPS     4000
#define TPH1    4000
#define TPH2     100
#define TPBW   45000
#define TCLKH    100  
#define TCLKL  TCLKH        // Make symetric
#define TWC    75000

#define CRYPT_KEY       std::string("1010101010101010101010101010101010101010101010101010101010101010")      // 64 bits || Try generate using serial number + manufacturers code (micro chip)
#define SYNC_COUNTER    std::string("0000000000000000")                                                      // 16 bits
#define RESERVED_16     std::string("0000000000000000")                                                      // 16 bits
#define SERIAL_NUMBER   std::string("00000000000000000000000000000000")                                      // 32 bits
#define SEED_NUMBER     std::string("10101100101011011010110010101101")                                      // 32 bits
#define OVERFLOW        std::string("00")
#define VLOW            std::string("0")
#define HCS_BAUD        std::string("00")
#define RESERVED_S      std::string("0")

#define CONFIG_BITSTREAM_LENGTH 192
#define WORD_LENGTH 16

#define READ_TIMEOUT    10000 // ms