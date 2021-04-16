#include "rtc_ds1307.h"
#include <Wire.h>       /*added for arduino*/

#define I2C_SPEED       100000

/*function to transmit one byte of data to register_address on ds1307*/
void time_i2c_write_single(uint8_t device_address, uint8_t register_address, uint8_t *data_byte)
{
  Wire.beginTransmission(device_address);
  Wire.write(register_address);
  Wire.write(*data_byte);
  Wire.endTransmission(device_address);
}

/*function to transmit an array of data to device_address, starting from start_register_address*/
void time_i2c_write_multi(uint8_t device_address, uint8_t start_register_address, uint8_t *data_array, uint8_t data_length)
{
  /*choose i2c device_address*/
  Wire.beginTransmission(device_address);
  /*choose the starting register on device*/
  Wire.write(start_register_address);
  /*transmit an array of data to the device*/
  for (; data_length; data_length--)
  {
    Wire.write(*data_array);
    data_array++;
  }
  Wire.endTransmission(device_address);
}

/*function to read one byte of data from register_address on ds1307*/
void time_i2c_read_single(uint8_t device_address, uint8_t register_address, uint8_t *data_byte)
{
    Wire.beginTransmission(device_address);
  Wire.write(start_register_address);
  Wire.endTransmission(device_address);
  Wire.requestFrom(uint16_t(device_address), 1, 0);
  while (Wire.available())
  {
    *data_byte = Wire.read();
  }
}

/*function to read an array of data from device_address*/
void time_i2c_read_multi(uint8_t device_address, uint8_t start_register_address, uint8_t *data_array, uint8_t data_length)
{
  /*setting the i2c device_address to read data*/
    Wire.beginTransmission(device_address);
  Wire.write(start_register_address);
  Wire.endTransmission(device_address);
  /*requesting data_length bytes of data from device_address*/
  Wire.requestFrom(uint16_t(device_address), uint16_t(data_length), 0);
  /*polling and reading the requested data*/
  while (Wire.available())
  {
    *data_array = Wire.read();
    data_array++;
  }
}

/*function to initialize I2C peripheral in 100khz*/
void DS1307_I2C_init()
{
  Wire.begin();
  Wire.setClock(I2C_SPEED);
}
