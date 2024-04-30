/**
 * @file BQ27621.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2024-04-25
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef BQ27621_H
#define BQ27621_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "BQ27621_defs.h"

/**
 * @brief BQ27621 testing Macro. Not used in production
 *
 */
#define BQ27621_TESTING 1

#if defined(BQ27621_TESTING) && BQ27621_TESTING != 0
#include <iostream>
using std::cout;
using std::endl;
using std::hex;
using std::string;
using std::uppercase;
#endif

class BQ27621
{
private:
    I2C_device &_i2c_device; // Change for I2C class or structure from application MCU
    uint8_t _i2c_address;
    uint16_t _device_type;
    bool _seal_flag;
    bool _userConfigControl;

    static uint16_t byte_swap(uint16_t word);

    BQ27621_error_code isSealed(bool *isSealed);
    BQ27621_error_code seal(void);
    BQ27621_error_code unseal(void);

    BQ27621_error_code getOpConfig(uint16_t *opConfig);
    BQ27621_error_code setOpConfig(uint16_t opConfig);

    BQ27621_error_code softReset(void);

    BQ27621_error_code readWord(uint16_t subAdress, uint16_t *word);
    BQ27621_error_code readControlWord(uint16_t function, uint16_t *word);

    BQ27621_error_code executeControlWord(uint16_t function );

    BQ27621_error_code blockDataControl(void);
    BQ27621_error_code blockDataClass(uint8_t id);
    BQ27621_error_code blockDataOffset(uint8_t offset);
    BQ27621_error_code blockDataChecksum(uint8_t *checksum);

    BQ27621_error_code readBlockData(uint8_t offset, uint8_t *data);
    BQ27621_error_code writeBlockData(uint8_t offset, uint8_t data);

    BQ27621_error_code computeBlockChecksum(uint8_t *checksum);
    BQ27621_error_code writeBlockChecksum(uint8_t checksum);

    BQ27621_error_code writeExtendedData(uint8_t classId, uint8_t offset, uint8_t *data, size_t len);
    BQ27621_error_code readExtendedData(uint8_t classId, uint8_t offset, uint8_t *data);


    //... More to add

public:
    BQ27621(I2C_device &i2cDevice);
    ~BQ27621();

    BQ27621_error_code init();
    BQ27621_error_code setCapacity(uint16_t capacity);
    BQ27621_error_code setDesignenergy(uint16_t energy);
    BQ27621_error_code setTerminateVoltage(uint16_t voltage);

    // Battery characteristics
    BQ27621_error_code getVoltage(uint16_t *voltage);
    BQ27621_error_code getCurrent(int16_t *current);
    BQ27621_error_code getCapacity(CapacityMeasure type = C_MEASURE_REMAIN,uint16_t *capacity);
    BQ27621_error_code getPower(int16_t *power);
    BQ27621_error_code getSOC(SocMeasure type = FILTERED, uint16_t *soc);
    BQ27621_error_code getTemperature(TempMeasure type = T_MEASURE_BATTERY, uint16_t *temperature);

    // GPOUT commands
    BQ27621_error_code getGpoutPolarity(bool *polarity);
    BQ27621_error_code setGpoutPolarity(bool polarity);

    BQ27621_error_code getGpoutFunction(GpoutFunction *function);
    BQ27621_error_code setGpoutFunction(GpoutFunction function);

    // Thresholds
    BQ27621_error_code setThresoldSOC1(uint8_t *soc);
    BQ27621_error_code clearThresoldSOC1(uint8_t *soc);
    BQ27621_error_code setSOC1Thresholds(uint8_t set, uint8_t clear);

    BQ27621_error_code setThresoldSOCF(uint8_t *soc);
    BQ27621_error_code clearThresoldSOCF(uint8_t *soc);
    BQ27621_error_code setSOCFThresholds(uint8_t set, uint8_t clear);

    BQ27621_error_code getSoc1Flag(bool *soc1);
    BQ27621_error_code getSocfFlag(bool *socf);
    BQ27621_error_code getItPorFlag(bool *itpor);
    BQ27621_error_code getFcFlag(bool *fc);
    BQ27621_error_code getChgFlag(bool *chg);
    BQ27621_error_code getDsgFlag(bool *dsg);
    BQ27621_error_code getSociDelta(uint8_t *sociDelta);
    BQ27621_error_code setSociDelta(uint8_t sociDelta);
    BQ27621_error_code pulseGpout(void);

    BQ27621_error_code getDeviceType(uint16_t *deviceType);

};

#endif /*BQ27621_h*/