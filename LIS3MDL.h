#ifndef LIS3MDL_h
#define LIS3MDL_h


class LIS3MDL
{
  public:

    enum deviceType { device_LIS3MDL, device_auto };
    enum sa1State { sa1_low, sa1_high, sa1_auto };

    // register addresses
    enum regAddr
    {
      WHO_AM_I    = 0x0F,

      CTRL_REG1   = 0x20,
      CTRL_REG2   = 0x21,
      CTRL_REG3   = 0x22,
      CTRL_REG4   = 0x23,
      CTRL_REG5   = 0x24,

      STATUS_REG  = 0x27,
      OUT_X_L     = 0x28,
      OUT_X_H     = 0x29,
      OUT_Y_L     = 0x2A,
      OUT_Y_H     = 0x2B,
      OUT_Z_L     = 0x2C,
      OUT_Z_H     = 0x2D,
      TEMP_OUT_L  = 0x2E,
      TEMP_OUT_H  = 0x2F,
      INT_CFG     = 0x30,
      INT_SRC     = 0x31,
      INT_THS_L   = 0x32,
      INT_THS_H   = 0x33,
    };

    int m[3]; // magnetometer readings

    uint8_t last_status; // status of last I2C transmission

    LIS3MDL(void);

    bool init(deviceType device = device_auto, sa1State sa1 = sa1_auto);
    deviceType getDeviceType(void) { return _device; }

    void enable(void);
    void enableDefault(void);

    void writeReg(uint8_t reg, uint8_t value);
    uint8_t readReg(uint8_t reg);

    void read(void);



  private:
    I2CBus i2c;
    deviceType _device; // chip type
    uint8_t address;

    int16_t testReg(uint8_t address, regAddr reg);
};


#endif