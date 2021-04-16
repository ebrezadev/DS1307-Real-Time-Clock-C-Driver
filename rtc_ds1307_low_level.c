#include "rtc_ds1307.h"

/*function to transmit one byte of data to register_address on ds1307*/
void time_i2c_write_single(uint8_t device_address, uint8_t register_address, uint8_t *data_byte)
{
}

/*function to transmit an array of data to device_address, starting from start_register_address*/
void time_i2c_write_multi(uint8_t device_address, uint8_t start_register_address, uint8_t *data_array, uint8_t data_length)
{
}

/*function to read one byte of data from register_address on ds1307*/
void time_i2c_read_single(uint8_t device_address, uint8_t register_address, uint8_t *data_byte)
{
}

/*function to read an array of data from device_address*/
void time_i2c_read_multi(uint8_t device_address, uint8_t start_register_address, uint8_t *data_array, uint8_t data_length)
{
}

/*function to initialize I2C peripheral in 100khz*/
void DS1307_I2C_init()
{
}
