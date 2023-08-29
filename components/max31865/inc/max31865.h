#pragma once

#include <string>
// #include <iostream>
// #include <sstream>

#include "spi.h"
// #include "log.h"

/**
 * @brief base class for max31865 register
 *
 */
struct max31865Register_t
{
    /**
     * @brief Permission to read or write to register
     */
    enum registerPermission_t
    {
        READ_ONLY = 0,
        WRITE_ONLY = 1,
        READ_WRITE = 2
    };

    max31865Register_t() {}
    virtual ~max31865Register_t() {}
    max31865Register_t::registerPermission_t registerPermission;
    virtual max31865Register_t::registerPermission_t getRegPermission(void) { return this->registerPermission; }
    uint8_t baseAddress = 0x20; // invalid address to avoid that this class is used directly
    uint8_t length = 0x00;      // invalid length to avoid that this class is used directly

protected:
    friend class max31865;
    
    virtual void getRawValue(uint8_t *value) { *value = 0; };
    virtual void updateRawValue(uint8_t *value) { *value = 0; };
};

/**
 * @brief Class for the max31865 configuration register
 *
 */
struct configRegister_t : public max31865Register_t
{
    configRegister_t()
    {
        uint8_t initValues[1] = {0x00};
        this->registerPermission = READ_WRITE;
        this->updateRawValue(initValues);
        this->baseAddress = 0x00;
        this->length = 1;
    }
    enum vBias_t
    {
        OFF = 0,
        ON = 1
    };

    enum conversionMode_t
    {
        AUTO = 1,
        MANUAL = 0
    };

    enum oneShot_t
    {
        SHOT_OFF = 0,
        SHOT_ON = 1
    };

    enum wire_t
    {
        WIRE_2 = 0,
        WIRE_3 = 1,
        WIRE_4 = 0
    };

    enum faultDetection_t
    {
        NO_FAULT = 0,
        AUTOMATIC_DELAY = 1,
        RUN_WITH_MANUAL_DELAY = 2,
        FINISH_WITH_MANUAL_DELAY = 3
    };

    enum faultStatusClear_t
    {
        AUTO_CLEAR_FAULT = 1,
        NO_CLEAR_FAULT = 0
    };

    enum filter_t
    {
        FILTER_50HZ = 1,
        FILTER_60HZ = 0
    };

    vBias_t vBias;
    conversionMode_t conversionMode;
    oneShot_t oneShot;
    wire_t wire;
    faultDetection_t faultDetection;
    faultStatusClear_t faultStatusClear;
    filter_t filter;

    void print(void);
    std::string toString(void);

#ifndef UNIT_TEST
protected:
#endif
    virtual void getRawValue(uint8_t *value) override;
    virtual void updateRawValue(uint8_t *value) override;

    friend class max31865;
};

/**
 * @brief Class for the max31865 RTD MSB register
 *
 */
struct rtdRegister_t : public max31865Register_t
{
    rtdRegister_t()
    {
        this->registerPermission = READ_ONLY;
        baseAddress = 0x01;
        length = 2;
    }

    bool rtdFault;
    uint16_t getResistance(void);

    void print(void);

    std::string toString(void);

#ifndef UNIT_TEST
protected:
#endif

    virtual void getRawValue(uint8_t *value)
    {
        *value = 0x00;
    };
    virtual void updateRawValue(uint8_t *value);

    friend class max31865;

private:
    uint16_t resistance;
};

/**
 * @brief Class for the max31865 high fault threshold register
 *
 */
struct faultThresholdRegister_t : public max31865Register_t
{
    uint16_t getThreshold(void);

    void setThreshold(uint16_t threshold);

    void print(void);

    std::string toString(void);

#ifndef UNIT_TEST
protected:
#endif
    friend class max31865;
    virtual void updateRawValue(uint8_t *value);
    virtual void getRawValue(uint8_t *value);

private:
    uint8_t getThresholdRawRegMSB(void);
    uint8_t getThresholdRawRegLSB(void);
    uint16_t threshold;
};

/**
 * @brief Class for the max31865 high fault threshold register
 *
 */
struct faultHighThresholdRegister_t : public faultThresholdRegister_t
{
    faultHighThresholdRegister_t()
    {
        this->registerPermission = READ_WRITE;
        length = 2;
        baseAddress = 0x03;
    }
};

/**
 * @brief Class for the max31865 high fault threshold register
 *
 */
struct faultLowThresholdRegister_t : public faultThresholdRegister_t
{
    faultLowThresholdRegister_t()
    {
        this->registerPermission = READ_WRITE;
        length = 2;
        baseAddress = 0x03;
    }
};

/**
 * @brief Class for the max31865 fault status register
 *
 */
struct faultStatus_t : public max31865Register_t
{
    faultStatus_t()
    {
        this->registerPermission = READ_ONLY;
        this->baseAddress = 0x07;
        this->length = 1;
    }

    bool rtdHighThreshold;
    bool rtdLowThreshold;
    bool refInHighThreshold;
    bool refInLowThreshold;
    bool rtdInLowThreshold;
    bool overUnderVoltage;

    void print(void);

    std::string toString(void);

#ifndef UNIT_TEST
protected:
#endif
    virtual void getRawValue(uint8_t *value)
    {
        *value = 0x00;
    };
    virtual void updateRawValue(uint8_t *value);
};

class max31865
{
public:
    enum errorCode_t
    {
        NO_ERROR = 0,
        CONVERSION_ALREADY_RUNNING,
        CONVERSION_STILL_RUNNING,
        CONVERSION_NOT_RUNNING,
        INVALID_REGISTER_LENGTH,
        INVALID_REGISTER_PERMISSION
    };

    max31865(spi *spiObj, uint8_t csPin, uint8_t rdyPin);
    max31865(){};
    ~max31865();

    errorCode_t setConfigRegister(configRegister_t config);
    errorCode_t getConfigRegister(configRegister_t *config);

    errorCode_t setHighFaultThreshold(uint16_t faultThreshold);
    errorCode_t setLowFaultThreshold(uint16_t faultThreshold);
    errorCode_t getHighFaultThreshold(faultThresholdRegister_t *faultThreshold);
    errorCode_t getLowFaultThreshold(faultThresholdRegister_t *faultThreshold);

    errorCode_t getRtdValue(uint16_t *rtd);
    faultStatus_t getFaultStatus(void);
    errorCode_t clearFaultStatus(void);

    errorCode_t startSingleConversion(void);
    errorCode_t startContinuousConversion(void);
    errorCode_t stopContinuousConversion(void);
    errorCode_t getConversionResult(uint16_t *rtd);

    bool isConversionRunning(void);
    bool isConversionResultReady(void);

    errorCode_t readRegister(max31865Register_t &reg);
    errorCode_t writeRegister(max31865Register_t &reg);

private:
    spi *spiDevice;
    uint8_t readyPin;
    uint8_t csPin;
    configRegister_t configRegister;
};
