#include "LSM6.h"
#include <math.h>
#include <cassert>

// Defines ////////////////////////////////////////////////////////////////

// The Arduino two-wire interface uses a 7-bit number for the address,
// and sets the last bit correctly based on reads and writes
#define DS33_SA0_HIGH_ADDRESS 0b1101011
#define DS33_SA0_LOW_ADDRESS  0b1101010


#define DS33_WHO_ID    0x69

// Constructors ////////////////////////////////////////////////////////////////

LSM6::LSM6(const char * i2cDeviceName):
  i2c(i2cDeviceName)
{
  _device = device_auto;
  timestamp = 0;
  assert(init());
}

// Public Methods //////////////////////////////////////////////////////////////


bool LSM6::init(deviceType device, sa0State sa0)
{
  // perform auto-detection unless device type and SA0 state were both specified
  if (device == device_auto || sa0 == sa0_auto)
  {
    // check for LSM6DS33 if device is unidentified or was specified to be this type
    if (device == device_auto || device == device_DS33)
    {
      // check SA0 high address unless SA0 was specified to be low
      if (sa0 != sa0_low && testReg(DS33_SA0_HIGH_ADDRESS, WHO_AM_I) == DS33_WHO_ID)
      {
        sa0 = sa0_high;
        if (device == device_auto) { device = device_DS33; }
      }
      // check SA0 low address unless SA0 was specified to be high
      else if (sa0 != sa0_high && testReg(DS33_SA0_LOW_ADDRESS, WHO_AM_I) == DS33_WHO_ID)
      {
        sa0 = sa0_low;
        if (device == device_auto) { device = device_DS33; }
      }
    }

    // make sure device and SA0 were successfully detected; otherwise, indicate failure
    if (device == device_auto || sa0 == sa0_auto)
    {
      return false;
    }
  }

  _device = device;

  switch (device)
  {
    case device_DS33:
      address = (sa0 == sa0_high) ? DS33_SA0_HIGH_ADDRESS : DS33_SA0_LOW_ADDRESS;
      break;
  }

  return true;
}


void LSM6::enable(void) {
  enableDefault();
}
/*
Enables the LSM6's accelerometer and gyro. Also:
- Sets sensor full scales (gain) to default power-on values, which are
  +/- 2 g for accelerometer and 245 dps for gyro
- Selects 1.66 kHz (high performance) ODR (output data rate) for accelerometer
  and 1.66 kHz (high performance) ODR for gyro. (These are the ODR settings for
  which the electrical characteristics are specified in the datasheet.)
- Enables automatic increment of register address during multiple byte access
Note that this function will also reset other settings controlled by
the registers it writes to.
*/
void LSM6::enableDefault(void)
{
  if (_device == device_DS33)
  {
    // Accelerometer

    // 0x40 = 0b01000000
    // ODR = 0100 (104 Hz (normal mode)); FS_XL = 00 (+/-2 g full scale)
    writeReg(CTRL1_XL, 0x40);

    // Gyro

    // 0x48 = 0b001001000
    // ODR = 0100 (104 Hz (normal mode)); FS_XL = 10 (1000 dps)
    writeReg(CTRL2_G, 0x48);

    // Common

    // 0x04 = 0b00000100
    // IF_INC = 1 (automatically increment register address)
    writeReg(CTRL3_C, 0x04); 

    writeReg(WAKE_UP_DUR, 0x10); //sets timer resolution to 25us
  }
}

void LSM6::writeReg(uint8_t reg, uint8_t value)
{
  i2c.writeByte(reg, value);
}

uint8_t LSM6::readReg(uint8_t reg)
{
  return i2c.readByte(reg);
}

// Reads the 3 accelerometer channels and stores them in vector a
void LSM6::readAcc(void)
{

  uint8_t status = readReg(STATUS_REG);
  uint8_t mask = 0x01;

  if (status & mask) {
    uint8_t block[6];
    i2c.readBlock(OUTX_L_XL, sizeof(block), block);

    // combine high and low bytes
    a[0] = (int16_t)(block[1] << 8 | block[0]);
    a[1] = (int16_t)(block[3] << 8 | block[2]);
    a[2] = (int16_t)(block[5] << 8 | block[4]);
  }
}

// Reads the 3 gyro channels and stores them in vector g
void LSM6::readGyro(void)
{
  uint8_t status = readReg(STATUS_REG);
  uint8_t mask = 0x02;
  
  if (status & mask) {
    uint8_t block[6];
    i2c.readBlock(OUTX_L_G, sizeof(block), block);

    // combine high and low bytes
    g[0] = (int16_t)(block[1] << 8 | block[0]);
    g[1] = (int16_t)(block[3] << 8 | block[2]);
    g[2] = (int16_t)(block[5] << 8 | block[4]);
  }
}

void LSM6::readTime() 
{
  uint8_t block[3];
  i2c.readBlock(TIMESTAMP0_REG, sizeof(block), block);
  timestamp = (long)(block[2] << 16 | block[1] << 8 | block[0]);
}

// Reads all 6 channels of the LSM6 and stores them in the object variables
void LSM6::read(void)
{
  readAcc();
  readGyro();
}

// Private Methods //////////////////////////////////////////////////////////////

int16_t LSM6::testReg(uint8_t devaddress, regAddr reg)
{
  i2c.addressSet(devaddress);
  return i2c.tryReadByte(reg);
}