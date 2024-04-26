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

// TODO
typedef uint8_t error_t;    // TODO change error_t in application
typedef uint8_t I2C_device; // TODO change I2C_Device in application
// TODO

/**
 * @brief The fuel gauge uses a series of 2-byte standard commands to enable system reading and writing of battery information.
 * Each command has an associated sequential command-code pair.
 */
enum Commands : uint8_t
{                                           // Units obtained
    CONTROL = 0x00,                         // NA
    TEMPERATURE = 0x02,                     // 0.1 K
    VOLTAGE = 0x04,                         // mV
    FLAGS = 0x06,                           // NA
    NOMINAL_AVAILABLE_CAPACITY = 0x08,      // mAh
    FULL_AVAILABLE_CAPACITY = 0x0A,         // mAh
    REMAINING_CAPACITY = 0x0C,              // mAh
    FULL_CHARGE_CAPACITY = 0x0E,            // mAh
    EFFECTIVE_CURRENT = 0x10,               // mA
    AVERAGE_POWER = 0x18,                   // mW
    STATE_OF_CHARGE = 0x1C,                 // %
    INTERNAL_TEMPERATURE = 0x1E,            // 0.1 K
    REMAINING_CAPACITY_UNFILTERED = 0x28,   // mAh
    REMAINING_CAPACITY_FILTERED = 0x2A,     // mAh
    FULL_CHARGE_CAPACITY_UNFILTERED = 0x2C, // mAh
    FULL_CHARGE_CAPACITY_FILTERED = 0x2E,   // mAh
    STATE_OF_CHARGE_UNFILTERED = 0x30,      // mAh
    OPERATION_CONFIGURATION = 0x3A          // NA
};

/**
 * @brief Issuing a Control() command requires a subsequent 2-byte subcommand. These additional bytes specify
 * the particular control function desired. The Control() command allows the system to control specific features
 * of the fuel gauge during normal operation and additional features when the device is in different access modes.
 */
enum ControlSubCommands : uint16_t
{
    CONTROL_STATUS = 0x0000,  // Reports the status of device.
    DEVICE_TYPE = 0x0001,     // Reports the device type (0x0621).
    FW_VERSION = 0x0002,      // Reports the firmware version of the device.
    PREV_MACWRITE = 0x0007,   // Returns previous MAC command code.
    CHEM_ID = 0x0008,         // Reports the chemical identifier of the battery profile currently used by the fuel gauging algorithm
    BAT_INSERT = 0x000C,      // Forces the [BAT_DET] bit set when the [BIE] bit is 0.
    BAT_REMOVE = 0x000D,      // Forces the [BAT_DET] bit clear when the [BIE] bit is 0.
    TOGGLE_POWERMIN = 0x0010, // Set CONTROL_STATUS [POWERMIN] to 1.
    SET_HIBERNATE = 0x0011,   // Forces CONTROL_STATUS [HIBERNATE] to 1.
    CLEAR_HIBERNATE = 0x0012, // Forces CONTROL_STATUS [HIBERNATE] to 0.
    SET_CFGUPDATE = 0x0013,   // Force CONTROL_STATUS [CFGUPMODE] to 1 and gauge enters CONFIG UPDATE mode.
    SHUTDOWN_ENABLE = 0x001B, // Enables device SHUTDOWN mode.
    SHUTDOWN = 0x001C,        // Commands the device to enter SHUTDOWN mode.
    SEALED = 0x0020,          // Places the device in SEALED access mode.
    TOGGLE_GPOUT = 0x0023,    // Test the GPIO pin by sending a pulse signal
    ALT_CHEM1 = 0x0031,       // Selects alternate chemistry 1 (0x1210)
    ALT_CHEM2 = 0x0032,       // Selects alternate chemistry 2 (0x354)
    RESET = 0x0041,           // Performs a full device reset.
    SOFT_RESET = 0x0042,      // Gauge exits CONFIG UPDATE mode.
    EXIT_CFGUPDATE = 0x0043,  // Exits CONFIG UPDATE mode without an OCV measurement and without resimulating to update StateOfCharge().
    EXIT_RESIM = 0x0044       // Exits CONFIG UPDATE mode without an OCV measurement and resimulates with the updated configuration data to update StateOfCharge().
};

/**
 * @brief Extended data commands offer additional functionality beyond the standard set of commands. They are used in the same manner;
 * however, unlike standard commands, extended commands are not limited to 2-byte words.
 */
enum ExtendedCommands : uint8_t
{
    EXTENDED_OPCONFIG = 0x3A,
    EXTENDED_DESIGN_CAPACITY = 0x3C,
    EXTENDED_DATA_CLASS = 0x3E,
    EXTENDED_DATA_BLOCK = 0x3F,
    EXTENDED_BLOCK_DATA = 0x40,
    EXTENDED_BLOCK_DATA_CHECKSUM = 0x60,
    EXTENDED_BLOCK_DATA_CONTROL = 0x61,
    EXTENDED_RESERVED = 0x62,
};

/**
 * @brief  Bit positions for the 16-bit data of CONTROL_STATUS. CONTROL_STATUS instructs the fuel gauge to return status information
 * to Control() addresses 0x00 and 0x01. The read-only status word contains status bits that are set or cleared either automatically
 * as conditions warrant or through using specified subcommands.
 */
enum ControlStatus : uint16_t
{
    STATUS_SHUTDOWNEN = (1 << 15), // Indicates the fuel gauge has received the SHUTDOWN_ENABLE subcommand and is enabled for SHUTDOWN. Active when set.
    STATUS_WDRESET = (1 << 14),    // Indicates the fuel gauge has performed a Watchdog Reset. Active when set.
    STATUS_SS = (1 << 13),         // Indicates the fuel gauge is in the SEALED state. Active when set.
    STATUS_CALMODE = (1 << 12),    // Indicates the fuel gauge is in calibration mode. Active when set.
    STATUS_RSVD3 = (1 << 11),      // Reserved
    STATUS_RSVD2 = (1 << 10),      // Reserved
    STATUS_OCVCMDCOMP = (1 << 9),  // Indicates that the OCV measurement is complete
    STATUS_OCVFAIL = (1 << 8),     // If set indicates that the gauge measured a large jump of current while trying to determine SOC, therefore SOC may not be reliable.
    STATUS_INITCOMP = (1 << 7),    // Initialization completion bit indicating the initialization completed. True when set.
    STATUS_POWERMIN = (1 << 6),    // Indicates the fuel gauge is in minimum power mode. True when set. The state can be detected by monitoring the power used by the fuel gauge.
    STATUS_SLEEP = (1 << 4),       // Indicates the fuel gauge is in minimum power mode. True when set. The state can be detected by monitoring the power used by the fuel gauge.
    STATUS_LDMD = (1 << 3),        // Indicates the algorithm is using constant-power model. True when set. Default is 0.
    STATUS_RSVD1 = (1 << 2),       // Reserved
    STATUS_RSVD0 = (1 << 1),       // Reserved
    STATUS_CHEMCHNG = 1            // Bit is set by a successful MAC command for chemistry change and gets cleared when all of the RAM has been updated
};

/**
 * @brief Bit positions for the 16-bit data of Flags(). This read-word function returns the contents of the fuel gauging status
 * register, depicting the current operating status.
 */
enum FlagBits : uint16_t
{
    FLAG_OT = (1 << 15),       // Over-Temperature condition
    FLAG_UT = (1 << 14),       // Under-Temperature condition is detected
    FLAG_FC = (1 << 9),        // Full-charge is detected
    FLAG_CHG = (1 << 8),       // Fast charging allowed.
    FLAG_OCVTAKEN = (1 << 7),  // Cleared on entry to relax mode and set to 1 when OCV measurement is performed in relax.
    FLAG_ITPOR = (1 << 5),     // Indicates a power-on reset or RESET subcommand has occurred
    FLAG_CFGUPMODE = (1 << 4), // Fuel gauge is in CONFIG UPDATE mode
    FLAG_BAT_DET = (1 << 3),   // Battery insertion detected. True when set.
    FLAG_SOC1 = (1 << 2),      // If set, StateOfCharge( ) ≤ SOC1 Set Threshold. The [SOC1] bit will remain set until StateOfCharge( ) ≥ SOC1 Clear Threshold.
    FLAG_SOCF = (1 << 1),      // If set, StateOfCharge( ) ≤ SOCF Set Threshold. The [SOCF] bit will remain set until StateOfCharge( ) ≥ SOCF Clear Threshold.
    FLAG_DSG = (1 << 0),       // Discharging detected. True when set.
};

enum OpConfig : uint16_t
{
    OPCONFIG_RSVD5 = (1 << 15),   // Reserved
    OPCONFIG_ADOLP = (1 << 14),   // If cleared (default), the ADC conversion time is largest to achieve maximum accuracy.
    OPCONFIG_BIE = (1 << 13),     // Battery Insertion Enable. If set, the battery insertion is detection via the BIN pin input. If cleared, the detection relies on the host to issue BAT_INSERT subcommand to indicate battery presence in the system.
    OPCONFIG_RSVD4 = (1 << 12),   // Reserved
    OPCONFIG_GPIOPOL = (1 << 11), // GPOUT pin is active-high if set or active-low if cleared.
    OPCONFIG_ADMLP = (1 << 10),   // If set (default), the ADC is in low-power conversion mode.
    OPCONFIG_RSVD3 = (1 << 9),    // Reserved
    OPCONFIG_RSVD2 = (1 << 8),    // Reserved
    OPCONFIG_VCONSEN = (1 << 7),  // Enables voltage consistency check. True when set. Default is recommended.
    OPCONFIG_RSVD1 = (1 << 6),    // Reserved
    OPCONFIG_RSVD0 = (1 << 5),    // Reserved
    OPCONFIG_RMFCC = (1 << 4),    // RM is updated with the value from FCC on valid charge termination. True when set.
    OPCONFIG_FCONVEN = (1 << 3),  // Enables Fast SOC Convergence. True when set. Default is recommended.
    OPCONFIG_BATLOWEN = (1 << 2), // If set, the BAT_LOW function for GPOUT pin is selected. If cleared, the SOC_INT function is selected for GPOUT.
    OPCONFIG_NODVAVG = (1 << 1),  // xxx. True when set. Default is recommended.
    OPCONFIG_TEMPS = 1,           // Selects the temperature source. Enables the host to write Temperature( ) if set. If cleared, the internal temperature sensor is used for Temperature( ).

};

enum ConfigurationClass : uint8_t
{
    ID_SAFETY = 2,
    ID_CHARGE_TERMINATION = 36,
    ID_DATA = 48,
    ID_DISCHARGE = 49,
    ID_REGISTERS = 64,
    ID_POWER = 68,
};

enum GaugingClass : uint8_t
{
    ID_IT_CFG = 80,
    ID_CURRENT_THRESH = 81,
    ID_STATE = 82
};

enum RaTablesClass : uint8_t
{
    ID_R_A_RAM = 89,
};

enum SecurityClass : uint8_t
{
    ID_CODES = 112,
};


enum CapacityMeasure: uint8_t{
    REMAIN,     // Remaining Capacity (DEFAULT)
	FULL,       // Full Capacity
	AVAIL,      // Available Capacity
	AVAIL_FULL, // Full Available Capacity
	REMAIN_F,   // Remaining Capacity Filtered
	REMAIN_UF,  // Remaining Capacity Unfiltered
	FULL_F,     // Full Capacity Filtered
	FULL_UF,    // Full Capacity Unfiltered
	DESIGN      // Design Capacity
};

enum SohMeasure: uint8_t {
    PERCENT,  // State of Health Percentage (DEFAULT)
	SOH_STAT  // State of Health Status Bits
};

enum SocMeasure: uint8_t {
    FILTERED,  // State of Charge Filtered (DEFAULT)
	UNFILTERED // State of Charge Unfiltered
};

enum TempMeasure: uint8_t {
    BATTERY,      // Battery Temperature (DEFAULT)
	INTERNAL_TEMP // Internal IC Temperature
};

enum GpoutFunction: uint8_t {
    SOC_INT, // Set GPOUT to SOC_INT functionality
	BAT_LOW  // Set GPOUT to BAT_LOW functionality
};

class BQ27621
{
private:
    I2C_device &_i2c_device; // Change for I2C class or structure from application MCU

    static const uint16_t BQ27621_UNSEAL_KEY = 0x8000;

public:
    BQ27621(I2C_device &i2cDevice);
    ~BQ27621();

    uint8_t _i2c_address;
    uint16_t _device_type;

    static const uint16_t BQ27621_DEVICE_TYPE = 0x0621;
    static const uint8_t BQ27621_DEFAULT_I2C_ADDRESS = 0x55;

    error_t init();
    bool setCapacity(uint16_t capacity);
    bool setDesignenergy(uint16_t energy);
    bool setTerminateVoltage(uint16_t voltage);

    // Battery characteristics
    error_t getVoltage(uint16_t *voltage);
    error_t getCurrent(int16_t *current);
    error_t getCapacity(CapacityMeasure type = REMAIN,uint16_t *capacity);
    error_t getPower(int16_t *power);
    error_t getSOC(SocMeasure type = FILTERED, uint8_t *soc);
    error_t getSOH(SohMeasure type = PERCENT, uint8_t *soh);
    error_t getTemperature(TempMeasure type = BATTERY, uint16_t *temperature);

    error_t getGpoutPolarity(bool *polarity);
    error_t setGpoutPolarity(bool polarity);

    error_t getGpoutFunction(GpoutFunction *function);
    error_t setGpoutFunction(GpoutFunction function);

    error_t setThresoldSOC1(uint8_t *soc);
    error_t clearThresoldSOC1(uint8_t *soc);
    error_t setSOC1Thresholds(uint8_t set, uint8_t clear);

    error_t setThresoldSOCF(uint8_t *soc);
    error_t clearThresoldSOCF(uint8_t *soc);
    error_t setSOCFThresholds(uint8_t set, uint8_t clear);

    error_t getSoc1Flag(bool *soc1);
    error_t getSocfFlag(bool *socf);


};

#endif /*BQ27621_h*/