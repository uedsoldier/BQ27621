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

BQ27621::BQ27621(I2C_device& i2cDevice):
_i2c_device(i2cDevice), _i2c_address(BQ27621_DEFAULT_I2C_ADDRESS)
{
}

BQ27621::~BQ27621()
{
}