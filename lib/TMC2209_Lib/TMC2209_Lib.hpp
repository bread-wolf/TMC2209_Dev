/*
 * TMC2209_Lib.hpp
 *
 *  Created on: 13.07.2021
 *      Author: AB
 */

#ifndef TMC2209_LIB_H_
#define TMC2209_LIB_H_

#include<Arduino.h>
#include<stdint.h>
#include<stdbool.h>

#include"TMCSerial.hpp"
#include"TMC22XX/TMC22XX_Map.hpp"



class TMC2209 : private TMCSerial
{
    public:
        /* Creates instance, sets up default configuration. */
        TMC2209(HardwareSerial& serialPort, uint32_t baudrate, uint8_t chipAddress, uint8_t enablePin);

        /* Checks driver is present, put driver in a disabled safe state for configuration. */
        bool begin();

        /* Invert direction of shaft rotation. */
        void invertShaft();

        /* Switch between stealthchop and spreadcycle modes.
         * SpreadCycle is on by default. */
        void enableStealthChop(bool enable);

        /* Check chip is alive :
         *    - If chip is unconfigured, check version number
         *    - If chip is configured, check error flag
         */
        bool isChipAlive();

    private:
        /* Driver enable pin */
        uint8_t _enablePin;

        /* Driver state and statuses */
        bool _isConfigured;
};

#endif /* TMC2209_LIB_H_ */