#define DS1307_I2C_ADDRESS                    0X68

#define OPERATION_DONE                        0X01
#define OPERATION_FAILED                      0X00
#define CLOCK_RUN                             0X01
#define CLOCK_HALT                            0X00
#define DS1307_NOT_INITIALIZED                0X00
#define DS1307_INITIALIZED                    0X2C
#define FORCE_RESET                           0X00
#define NO_FORCE_RESET                        0X01

#define DS1307_REGISTER_SECONDS               0X00
#define DS1307_REGISTER_MINUTES               0X01
#define DS1307_REGISTER_HOURS                 0X02
#define DS1307_REGISTER_DAY_OF_WEEK           0X03
#define DS1307_REGISTER_DATE                  0X04
#define DS1307_REGISTER_MONTH                 0X05
#define DS1307_REGISTER_YEAR                  0X06
#define DS1307_REGISTER_CONTROL               0X07
#define DS1307_REGISTER_INIT_STATUS           0X08
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
#define DS1307_IS_RUNNING                     0X01
#define DS1307_IS_STOPPED                     0X00
#define DS1307_REGISTER_SECONDS_DEFAULT       0X00
#define DS1307_REGISTER_MINUTES_DEFAULT       0X00
#define DS1307_REGISTER_HOURS_DEFAULT         0X00
#define DS1307_REGISTER_DAY_OF_WEEK_DEFAULT   0X01
#define DS1307_REGISTER_DATE_DEFAULT          0X01
#define DS1307_REGISTER_MONTH_DEFAULT         0X01
#define DS1307_REGISTER_YEAR_DEFAULT          0X00
#define DS1307_REGISTER_CONTROL_DEFAULT       0X00
#define DS1307_RAM_BLOCK_DEFAULT              0x00

#define RESET_SECONDS                         0X00
#define RESET_MINUTES                         0X01
#define RESET_HOURS                           0X02
#define RESET_DAY_OF_WEEK                     0X03
#define RESET_DATE                            0X04
#define RESET_MONTH                           0X05
#define RESET_YEAR                            0X06
#define RESET_CONTROL                         0X07
#define RESET_TIME                            0x08
#define RESET_EVERYTHING                      0x09
#define RESET_RAM                             0x0A

#define SECONDS                               0X00
#define MINUTES                               0X01
#define HOURS                                 0X02
#define DAY_OF_WEEK                           0X03
#define DATE                                  0X04
#define MONTH                                 0X05
#define YEAR                                  0X06
#define CONTROL                               0X07
#define TIME                                  0X08
#define ALL                                   0X09

uint8_t DS1307_run(uint8_t run_state);
uint8_t DS1307_run_state(void);
uint8_t DS1307_read(uint8_t registers, uint8_t *data_array);
void DS1307_I2C_setting();
void DS1307_reset(uint8_t input);
uint8_t DS1307_set(uint8_t registers, uint8_t *data_array);
uint8_t DS1307_init(uint8_t *data_array, uint8_t run_state, uint8_t reset_state);
uint8_t DS1307_init_status_report();
void DS1307_init_status_update();

void time_i2c_write_single(uint8_t device_address, uint8_t register_address, uint8_t data_byte);
void time_i2c_write_multi(uint8_t device_address, uint8_t start_register_address, uint8_t *data_array, uint8_t data_length);
void time_i2c_read_single(uint8_t device_address, uint8_t register_address, uint8_t data_byte);
void time_i2c_read_multi(uint8_t device_address, uint8_t start_register_address, uint8_t *data_array, uint8_t data_length);
void time_i2c_goto_address(uint8_t device_address, uint8_t start_register_address);
