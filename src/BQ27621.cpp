/**
 * @file BQ27621.cpp
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2024-04-25
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "BQ27621.h"

BQ27621::BQ27621(I2C_device &i2cDevice) : _i2c_device(i2cDevice), _i2c_address(BQ27621_I2C_ADDRESS), _seal_flag(false), _userConfigControl(false)
{
}

BQ27621::~BQ27621()
{
}

/**
 * @brief
 *
 * @param word Which control word to read
 * @param function Pointer to uint16_t that will hold the control word value
 * @return BQ27621_error_code
 */
BQ27621_error_code BQ27621::readControlWord(uint16_t word, uint16_t *function)
{
    uint16_t swapped_function = byte_swap(*function);

    // TODO I2c write bytes address = CONTROL, data = swapped, len = 2
    // TODO I2C read bytes address = CONTROL, data = swapped, len = 2
}

BQ27621_error_code BQ27621::init()
{
    BQ27621_error_code retVal;
    uint16_t deviceId = 0;

    // TODO I2c transaction begin
    retVal = getDeviceType(&deviceId);

    if (retVal != OK)
        return retVal;
    if (deviceId == BQ27621_DEVICE_TYPE)
        return OK;
    else
        return INCORRECT_DEVICE_TYPE;
}

/**
 * @brief
 *
 * @param capacity
 * @return BQ27621_error_code
 */
BQ27621_error_code BQ27621::setCapacity(uint16_t capacity)
{
    uint16_t *capacity_data = (uint16_t *)(byte_swap(capacity));
    return writeExtendedData(ID_STATE, STATE_DESIGN_CAPACITY, (uint8_t *)capacity_data, sizeof(uint16_t));
}

/**
 * @brief
 *
 * @param energy
 * @return BQ27621_error_code
 */
BQ27621_error_code BQ27621::setDesignenergy(uint16_t energy)
{
    uint16_t *capacity_data = (uint16_t *)(byte_swap(energy));
    return writeExtendedData(ID_STATE, STATE_DESIGN_ENERGY, (uint8_t *)capacity_data, sizeof(uint16_t));
}

/**
 * @brief
 *
 * @param voltage
 * @return BQ27621_error_code
 */
BQ27621_error_code BQ27621::setTerminateVoltage(uint16_t voltage)
{
    uint16_t *capacity_data = (uint16_t *)(byte_swap(voltage));
    return writeExtendedData(ID_STATE, STATE_TERMINATE_VOLTAGE, (uint8_t *)capacity_data, sizeof(uint16_t));
}

/**
 * @brief
 *
 * @param voltage
 * @return BQ27621_error_code
 */
BQ27621_error_code BQ27621::getVoltage(uint16_t *voltage)
{
    return readWord(COMMAND_VOLTAGE, voltage);
}

/**
 * @brief Reads and returns the specified current measurement
 *
 * @param current
 * @return BQ27621_error_code
 */
BQ27621_error_code BQ27621::getCurrent(int16_t *current)
{
    return readWord(COMMAND_EFFECTIVE_CURRENT, (uint16_t *)current);
}

/**
 * @brief Reads and returns the specified capacity measurement
 *
 * @param type
 * @param capacity
 * @return BQ27621_error_code
 */
BQ27621_error_code BQ27621::getCapacity(CapacityMeasure type, uint16_t *capacity)
{
    uint8_t capacityCmd;
    switch (type)
    {
    case C_MEASURE_FULL:
        capacityCmd = COMMAND_FULL_CHARGE_CAPACITY;
        break;
    case C_MEASURE_AVAIL:
        capacityCmd = COMMAND_NOMINAL_AVAILABLE_CAPACITY;
        break;
    case C_MEASURE_AVAIL_FULL:
        capacityCmd = COMMAND_FULL_AVAILABLE_CAPACITY;
        break;
    case C_MEASURE_REMAIN_F:
        capacityCmd = COMMAND_REMAINING_CAPACITY_FILTERED;
        break;
    case C_MEASURE_REMAIN_UF:
        capacityCmd = COMMAND_REMAINING_CAPACITY_UNFILTERED;
        break;
    case C_MEASURE_FULL_F:
        capacityCmd = COMMAND_FULL_CHARGE_CAPACITY_FILTERED;
        break;
    case C_MEASURE_FULL_UF:
        capacityCmd = COMMAND_FULL_CHARGE_CAPACITY_UNFILTERED;
        break;
    case C_MEASURE_DESIGN:
        capacityCmd = EXTENDED_DESIGN_CAPACITY;
        break;
    case C_MEASURE_REMAIN:
    default:
        capacityCmd = COMMAND_REMAINING_CAPACITY;
        break;
    }
    return readWord(capacityCmd, capacity);
};

/**
 * @brief Reads and returns measured average power
 *
 * @param power
 * @return BQ27621_error_code
 */
BQ27621_error_code BQ27621::getPower(int16_t *power)
{
    return readWord(COMMAND_AVERAGE_POWER, (uint16_t *)power);
};

/**
 * @brief Reads and returns specified state of charge measurement
 *
 * @param type
 * @param soc
 * @return BQ27621_error_code
 */
BQ27621_error_code BQ27621::getSOC(SocMeasure type, uint16_t *soc)
{
    Commands socCmd;
    switch (type)
    {
    case UNFILTERED:
        socCmd = COMMAND_STATE_OF_CHARGE_UNFILTERED;
        break;

    case FILTERED:
    default:
        socCmd = COMMAND_STATE_OF_CHARGE;
        break;
    }
    return readWord(socCmd, soc);
};

/**
 * @brief Reads and returns specified temperature measurement
 *
 * @param type
 * @param temperature
 * @return BQ27621_error_code
 */
BQ27621_error_code BQ27621::getTemperature(TempMeasure type, uint16_t *temperature)
{

    Commands temperatureCmd = (type == T_MEASURE_BATTERY) ? COMMAND_TEMPERATURE : COMMAND_INTERNAL_TEMPERATURE;
    return readWord(temperatureCmd, temperature);
};

/**
 * @brief  Get GPOUT polarity setting (active-high or active-low)
 *
 * @param polarity
 * @return BQ27621_error_code
 */
BQ27621_error_code BQ27621::getGpoutPolarity(bool *polarity)
{
    uint16_t opConfig;
    BQ27621_error_code retVal = getOpConfig(&opConfig);
    *polarity = opConfig & OPCONFIG_GPIOPOL;
    return retVal;
};

/**
 * @brief Set GPOUT polarity to active-high or active-low
 *
 * @param polarity
 * @return BQ27621_error_code
 */
BQ27621_error_code BQ27621::setGpoutPolarity(bool polarity)
{
    uint16_t oldOpConfig;
    BQ27621_error_code retVal = getOpConfig(&oldOpConfig);

    if ((polarity && (oldOpConfig & OPCONFIG_GPIOPOL)) || (!polarity && !(oldOpConfig & OPCONFIG_GPIOPOL)))
        return OK;

    uint16_t newOpConfig = oldOpConfig;
    if (polarity)
    {
        newOpConfig |= OPCONFIG_GPIOPOL;
    }
    else
    {
        newOpConfig &= ~(OPCONFIG_GPIOPOL);
    }

    return setOpConfig(newOpConfig);
};

/**
 * @brief Get GPOUT function (BAT_LOW or SOC_INT)
 * 
 * @param function 
 * @return BQ27621_error_code 
 */
BQ27621_error_code BQ27621::getGpoutFunction(GpoutFunction *function){
    uint16_t opConfig;
    BQ27621_error_code retVal = getOpConfig(&opConfig);
    *function = GpoutFunction(opConfig & OPCONFIG_BATLOWEN);
    return retVal;
};

/**
 * @brief 
 * 
 * @param function 
 * @return BQ27621_error_code 
 */
BQ27621_error_code BQ27621::setGpoutFunction(GpoutFunction function){
    uint16_t oldOpConfig;
    BQ27621_error_code retVal = getOpConfig(&oldOpConfig);

    if ((function && (oldOpConfig & OPCONFIG_GPIOPOL)) || (!function && !(oldOpConfig & OPCONFIG_GPIOPOL)))
        return OK;

    uint16_t newOpConfig = oldOpConfig;
    if (function == GPOUT_F_BAT_LOW)
    {
        newOpConfig |= OPCONFIG_BATLOWEN;
    }
    else
    {
        newOpConfig &= ~(OPCONFIG_BATLOWEN);
    }

    return setOpConfig(newOpConfig);
};



/**
 * @brief
 *
 * @param deviceType Pointer to uint16_t that will hold
 * @return BQ27621_error_code
 */
BQ27621_error_code BQ27621::getDeviceType(uint16_t *deviceType)
{
    return readControlWord(BQ27621_DEVICE_TYPE, deviceType);
}

/**
 * @brief
 *
 * @param word
 * @return uint16_t Swapped word
 */
uint16_t BQ27621::byte_swap(uint16_t word)
{
    uint16_t msb = (word & 0xFF00) >> 8;
    uint16_t lsb = (word & 0x00FF);
    return (lsb << 8) | msb;
}