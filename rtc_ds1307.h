/*ds1307 driver header file - Reza Ebrahimi v1.0*/
#ifndef RTC_DS1307_H
#define RTC_DS1307_H

enum options {SECOND, MINUTE, HOUR, DAY_OF_WEEK, DATE, MONTH, YEAR, CONTROL, RAM, TIME, SNAPSHOT, ALL};
enum square_wave {WAVE_OFF, WAVE_1, WAVE_2, WAVE_3, WAVE_4};
enum ds1307_registers {
  DS1307_REGISTER_SECONDS, 
  DS1307_REGISTER_MINUTES, 
  DS1307_REGISTER_HOURS, 
  DS1307_REGISTER_DAY_OF_WEEK, 
  DS1307_REGISTER_DATE, 
  DS1307_REGISTER_MONTH, 
  DS1307_REGISTER_YEAR, 
  DS1307_REGISTER_CONTROL};
  
#define DS1307_I2C_ADDRESS                    0X68

#define CLOCK_RUN                             0X01
#define CLOCK_HALT                            0X00
#define FORCE_RESET                           0X00
#define NO_FORCE_RESET                        0X01
#define DS1307_IS_RUNNING                     0X01
#define DS1307_IS_STOPPED                     0X00
#define OPERATION_DONE                        0X01
#define OPERATION_FAILED                      0X00
#define DS1307_NOT_INITIALIZED                0X00
#define DS1307_INITIALIZED                    0X2C
#define OCCUPIED                              0X01
#define NOT_OCCUPIED                          0X00

#define DS1307_REGISTER_INIT_STATUS           0X08
#define DS1307_REGISTER_SNAP0_VACANCY         0X09
#define DS1307_RAM_START                      0X08
#define DS1307_RAM_END                        0X3F
#define DS1307_BIT_SETTING_CH                 0X07
#define DS1307_BIT_SETTING_RS0                0X00
#define DS1307_BIT_SETTING_RS1                0X01
#define DS1307_BIT_SETTING_SQWE               0X04
#define DS1307_BIT_SETTING_OUT                0X07
#define DS1307_BIT_SETTING_AMPM               0X06
#define DS1307_TIMEKEEPER_REGISTERS_START     0X00
#define DS1307_TIMEKEEPER_REGISTERS_END       0X07
#define DS1307_REGISTER_SECONDS_DEFAULT       0X00
#define DS1307_REGISTER_MINUTES_DEFAULT       0X00
#define DS1307_REGISTER_HOURS_DEFAULT         0X00
#define DS1307_REGISTER_DAY_OF_WEEK_DEFAULT   0X01
#define DS1307_REGISTER_DATE_DEFAULT          0X01
#define DS1307_REGISTER_MONTH_DEFAULT         0X01
#define DS1307_REGISTER_YEAR_DEFAULT          0X00
#define DS1307_REGISTER_CONTROL_DEFAULT       0X00
#define DS1307_RAM_BLOCK_DEFAULT              0x00
#define DS1307_SNAP0_ADDRESS                  0X39

uint8_t DS1307_run(uint8_t run_state);
uint8_t DS1307_run_state(void);
uint8_t DS1307_read(uint8_t registers, uint8_t *data_array);
void DS1307_reset(uint8_t input);
uint8_t DS1307_set(uint8_t registers, uint8_t *data_array);
uint8_t DS1307_init(uint8_t *data_array, uint8_t run_state, uint8_t reset_state);
uint8_t DS1307_init_status_report();
void DS1307_init_status_update();
uint8_t DS1307_square_wave(uint8_t input);
void DS1307_snapshot_save();
void DS1307_snapshot_clear();

void DS1307_I2C_init();
void time_i2c_write_single(uint8_t device_address, uint8_t register_address, uint8_t data_byte);
void time_i2c_write_multi(uint8_t device_address, uint8_t start_register_address, uint8_t *data_array, uint8_t data_length);
void time_i2c_read_single(uint8_t device_address, uint8_t register_address, uint8_t data_byte);
void time_i2c_read_multi(uint8_t device_address, uint8_t start_register_address, uint8_t *data_array, uint8_t data_length);

#endif
