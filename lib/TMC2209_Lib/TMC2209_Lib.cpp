/*
 * TMC2209_Lib.cpp
 *
 *  Created on: 13.07.2021
 *      Author: AB
 */

#include "TMC2209_Lib.hpp"

#define TMC2209_VERSION_BYTE    0x21  /* Chip version. */

TMC2209::TMC2209(HardwareSerial& serialPort, uint32_t baudrate, uint8_t chipAddress, uint8_t enablePin)
    : TMCSerial(serialPort, baudrate, chipAddress), _enablePin(enablePin), _isConfigured(false)
{

}

bool TMC2209::begin()
{
    /* Call base class begin() to set up UART peripheral. */
    TMCSerial::begin();

    /* Check that chip is alive. */
    if (isChipAlive() != true)
        return false;

    /* Set up enable pin. */
    pinMode(_enablePin, OUTPUT);

    /* Disable bridges.
     * On TMC2209 the input is active LOW. */
    digitalWrite(_enablePin, HIGH);

    /* Write fundamental parameters. */
    TMCSerial::writeField(TMC22XX_PDN_DISABLE, true);       // We will always use the UART interface here.
    TMCSerial::writeField(TMC22XX_MSTEP_REG_SELECT, true);  // Pins MS1 and MS2 select the chip ADDRESS.
    TMCSerial::writeField(TMC22XX_TEST_MODE, false);        // Normal operation setting.

    return true;
}

bool TMC2209::isChipAlive()
{
    if (_isConfigured != true)
    {
        /* If driver has not been configured, first check that we can read registers. */
        TMCSerial::enableReadChecksum(true);
        int8_t serialErr;

        if ((TMCSerial::readField(TMC22XX_VERSION, serialErr) != TMC2209_VERSION_BYTE) || serialErr < 0)
        {
            /* If we have the wrong chip, or a checksum error, fail here. */
            return false;
        }

        /* If checksum just passed, chances are it should be fine most of the time.
         * Disable here to save on computing. */
        TMCSerial::enableReadChecksum(false);
    }

    /* If this passes, we check some more parameters. */
    if (TMCSerial::readField(TMC22XX_DRV_ERR) == true)
    {
        /* Overtemperature or short circuit protection was tripped, yikes! */
        return false;
    }

    if (TMCSerial::readField(TMC22XX_UV_CP) == true)
    {
        /* Undervoltage on charge pump input, this disables the driver. */
        return false;
    }

    return true;
}