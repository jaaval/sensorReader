#include "LIS3MDL.h"
#include <math.h>
#include "I2CBus.h"
#include <cassert>

#define LIS3MDL_SA1_HIGH_ADDRESS  0b0011110
#define LIS3MDL_SA1_LOW_ADDRESS   0b0011100

#define LIS3MDL_WHO_ID  0x3D

LIS3MDL::LIS3MDL(const char * i2cDeviceName):
  i2c(i2cDeviceName)
{
  _device = device_auto;

  assert(init());
}

bool LIS3MDL::init(deviceType device, sa1State sa1)
{
  // perform auto-detection unless device type and SA1 state were both specified
  if (device == device_auto || sa1 == sa1_auto)
  {
    // check for LIS3MDL if device is unidentified or was specified to be this type
    if (device == device_auto || device == device_LIS3MDL)
    {
      // check SA1 high address unless SA1 was specified to be low
      if (sa1 != sa1_low && testReg(LIS3MDL_SA1_HIGH_ADDRESS, WHO_AM_I) == LIS3MDL_WHO_ID)
      {
        sa1 = sa1_high;
        if (device == device_auto) { device = device_LIS3MDL; }
      }
      // check SA1 low address unless SA1 was specified to be high
      else if (sa1 != sa1_high && testReg(LIS3MDL_SA1_LOW_ADDRESS, WHO_AM_I) == LIS3MDL_WHO_ID)
      {
        sa1 = sa1_low;
        if (device == device_auto) { device = device_LIS3MDL; }
      }
    }

    // make sure device and SA1 were successfully detected; otherwise, indicate failure
    if (device == device_auto || sa1 == sa1_auto)
    {
      return false;
    }
  }

  _device = device;

  switch (device)
  {
    case device_LIS3MDL:
      address = (sa1 == sa1_high) ? LIS3MDL_SA1_HIGH_ADDRESS : LIS3MDL_SA1_LOW_ADDRESS;
      break;
  }

  return true;
}

void LIS3MDL::enable(void) {
  enableDefault();
}

void LIS3MDL::enableDefault(void)
{
  if (_device == device_LIS3MDL)
  {
    // 0x78 = 0b01111000
    // OM = 11 (ultra-high-performance mode for X and Y); DO = 110 (40 Hz ODR)
    writeReg(CTRL_REG1, 0x78);

    // 0x00 = 0b01000000
    // FS = 40 (+/- 12 gauss full scale)
    writeReg(CTRL_REG2, 0x40);

    // 0x00 = 0b00000000
    // MD = 00 (continuous-conversion mode)
    writeReg(CTRL_REG3, 0x00);

    // 0x0C = 0b00001100
    // OMZ = 11 (ultra-high-performance mode for Z)
    writeReg(CTRL_REG4, 0x0C);
  }
}


// Writes a mag register
void LIS3MDL::writeReg(uint8_t reg, uint8_t value)
{
  i2c.writeByte(reg, value);
}

// Reads a mag register
uint8_t LIS3MDL::readReg(uint8_t reg)
{
  return i2c.readByte(reg);
}

// Reads the 3 mag channels and stores them in vector m
bool LIS3MDL::read()
{

  uint8_t status = readReg(STATUS_REG);

  if (status & 7) {
  //if (true) {
    uint8_t block[6];
    // (0x80 | OUT_X_L) when reading multiple bytes this sensor wants the MSB set to 1
    i2c.readBlock(0x80 | OUT_X_L, sizeof(block), block);
    // combine high and low bytes
    m[0] = (int16_t)(block[1] << 8 | block[0]);
    m[1] = (int16_t)(block[3] << 8 | block[2]);
    m[2] = (int16_t)(block[5] << 8 | block[4]);
    
    return true;
  }
  return false;
}

int16_t LIS3MDL::testReg(uint8_t devaddress, regAddr reg)
{
  i2c.addressSet(devaddress);
  return i2c.tryReadByte(reg);
}