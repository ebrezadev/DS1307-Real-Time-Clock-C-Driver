#include "rtc_ds1307.h"

/*global variables related to this file*/
static uint8_t register_current_value;        /*used to read current values of ds1307 registers*/
static uint8_t register_new_value;        /*used to write values to ds1307 registers*/
static uint8_t register_default_value[] = {       /*used in reset function*/
  DS1307_REGISTER_SECONDS_DEFAULT,
  DS1307_REGISTER_MINUTES_DEFAULT,
  DS1307_REGISTER_HOURS_DEFAULT,
  DS1307_REGISTER_DAY_OF_WEEK_DEFAULT,
  DS1307_REGISTER_DATE_DEFAULT,
  DS1307_REGISTER_MONTH_DEFAULT,
  DS1307_REGISTER_YEAR_DEFAULT,
  DS1307_REGISTER_CONTROL_DEFAULT
};

/*ds1307_init function accepts 3 inputs, data_array[7] is the new time settings, 
* run_state commands ds1307 to run or halt (CLOCK_RUN and CLOCK_HALT), and reset_state
* could force reset ds1307 (FORCE_RESET) or checks if ds1307 is reset beforehand
* (NO_FORCE_RESET)*/
uint8_t DS1307_init(uint8_t *data_array, uint8_t run_state, uint8_t reset_state)
{
  DS1307_I2C_setting();
  if ((DS1307_init_status_report() == DS1307_NOT_INITIALIZED) || (reset_state == FORCE_RESET))
  {
    DS1307_run(CLOCK_HALT);
    DS1307_reset(RESET_EVERYTHING);       /*everything but the 56 bytes general purpose ram inside ds1307*/
    DS1307_reset(RESET_RAM);        /*fills ds1307 general purpose ram wiith default value*/
    DS1307_set(TIME, data_array);
    DS1307_run(run_state);
    DS1307_init_status_update();        /*now the device is initialized (DS1307_INITIALIZED)*/
    return OPERATION_DONE;
  }
  else
  {
    DS1307_run(run_state);
    return OPERATION_FAILED;
  }
}

/*we use 1 byte of ds1307 ram to preserve the initialization status. this function reads that 1 byte*/
uint8_t DS1307_init_status_report()
{
  time_i2c_read_single(DS1307_I2C_ADDRESS, DS1307_REGISTER_INIT_STATUS, &register_current_value);
  if (register_current_value == DS1307_INITIALIZED)
    return DS1307_INITIALIZED;
  else
    return DS1307_NOT_INITIALIZED;
}

/*this function writes DS1307_INITIALIZED inside DS1307_REGISTER_INIT_STATUS*/
void DS1307_init_status_update()
{
  register_new_value = DS1307_INITIALIZED;
  time_i2c_write_single(DS1307_I2C_ADDRESS, DS1307_REGISTER_INIT_STATUS, &register_new_value);
}

/*function to start or halt the operation of DS1307, using CH control bit in SECONDS register*/
uint8_t DS1307_run(uint8_t run_state)
{
  /*preserving the contents of SECONDS register and changing CH bit*/
  time_i2c_read_single(DS1307_I2C_ADDRESS, DS1307_REGISTER_SECONDS, &register_current_value);
  if (run_state == CLOCK_RUN)
  {
    /*CH=0 runs the clock*/
    register_new_value = register_current_value & (~(1 << DS1307_BIT_SETTING_CH));
  }
  else if (run_state == CLOCK_HALT)
  {
    /*CH=1 halts the clock*/
    register_new_value = register_current_value | (1 << DS1307_BIT_SETTING_CH);
  }
  else
    return OPERATION_FAILED;
  /*write the new value back to SECONDS register*/
  time_i2c_write_single(DS1307_I2C_ADDRESS, DS1307_REGISTER_SECONDS, &register_new_value);
  return OPERATION_DONE;
}

/*polls the ds1307 to see if its running*/
uint8_t DS1307_run_state(void)
{
  time_i2c_read_single(DS1307_I2C_ADDRESS, DS1307_REGISTER_SECONDS, &register_current_value);
  if (register_current_value & (1 << DS1307_BIT_SETTING_CH))
    return DS1307_IS_RUNNING;
  else
    return DS1307_IS_STOPPED;
}

/*resets the desired register(s), without affecting run_state*/
void DS1307_reset(uint8_t input)
{
  switch (input)
  {
    case RESET_SECONDS:
      time_i2c_read_single(DS1307_I2C_ADDRESS, DS1307_REGISTER_SECONDS, &register_current_value);
      register_new_value = (register_current_value & (1 << DS1307_BIT_SETTING_CH)) | DS1307_REGISTER_SECONDS_DEFAULT;
      time_i2c_write_single(DS1307_I2C_ADDRESS, DS1307_REGISTER_SECONDS, &register_new_value);
      break;
    case RESET_MINUTES:
      register_new_value = DS1307_REGISTER_MINUTES_DEFAULT;
      time_i2c_write_single(DS1307_I2C_ADDRESS, DS1307_REGISTER_MINUTES, &register_new_value);
      break;
    case RESET_HOURS:
      register_new_value = DS1307_REGISTER_HOURS_DEFAULT & (~(1 << DS1307_BIT_SETTING_AMPM));
      time_i2c_write_single(DS1307_I2C_ADDRESS, DS1307_REGISTER_HOURS, &register_new_value);
      break;
    case RESET_DAY_OF_WEEK:
      register_new_value = DS1307_REGISTER_DAY_OF_WEEK_DEFAULT;
      time_i2c_write_single(DS1307_I2C_ADDRESS, DS1307_REGISTER_DAY_OF_WEEK, &register_new_value);
      break;
    case RESET_DATE:
      register_new_value = DS1307_REGISTER_DATE_DEFAULT;
      time_i2c_write_single(DS1307_I2C_ADDRESS, DS1307_REGISTER_DATE, &register_new_value);
      break;
    case RESET_MONTH:
      register_new_value = DS1307_REGISTER_MONTH_DEFAULT;
      time_i2c_write_single(DS1307_I2C_ADDRESS, DS1307_REGISTER_MONTH, &register_new_value);
      break;
    case RESET_YEAR:
      register_new_value = DS1307_REGISTER_YEAR_DEFAULT;
      time_i2c_write_single(DS1307_I2C_ADDRESS, DS1307_REGISTER_YEAR, &register_new_value);
      break;
    case RESET_CONTROL:
      register_new_value = DS1307_REGISTER_CONTROL_DEFAULT;
      time_i2c_write_single(DS1307_I2C_ADDRESS, DS1307_REGISTER_CONTROL, &register_new_value);
      break;
    case RESET_TIME:
      time_i2c_read_single(DS1307_I2C_ADDRESS, DS1307_REGISTER_SECONDS, &register_current_value);
      register_new_value = (register_current_value & (1 << DS1307_BIT_SETTING_CH)) | DS1307_REGISTER_SECONDS_DEFAULT;
      time_i2c_write_single(DS1307_I2C_ADDRESS, DS1307_REGISTER_SECONDS, &register_new_value);
      register_default_value[2] &= (~(1 << DS1307_BIT_SETTING_AMPM));
      time_i2c_write_multi(DS1307_I2C_ADDRESS, DS1307_REGISTER_MINUTES, &register_default_value[1], 6);
      break;
    case RESET_EVERYTHING:        /*everything is reset but the general purpose ram*/
      time_i2c_read_single(DS1307_I2C_ADDRESS, DS1307_REGISTER_SECONDS, &register_current_value);
      register_new_value = (register_current_value & (1 << DS1307_BIT_SETTING_CH)) | DS1307_REGISTER_SECONDS_DEFAULT;
      time_i2c_write_single(DS1307_I2C_ADDRESS, DS1307_REGISTER_SECONDS, &register_new_value);
      register_default_value[2] &= (~(1 << DS1307_BIT_SETTING_AMPM));
      time_i2c_write_multi(DS1307_I2C_ADDRESS, DS1307_REGISTER_MINUTES, &register_default_value[1], 7);
      break;
    case RESET_RAM:
      for (uint8_t register_address = DS1307_RAM_START; register_address <= DS1307_RAM_END; register_address++)
      {
        register_new_value = DS1307_RAM_BLOCK_DEFAULT;
        time_i2c_write_single(DS1307_I2C_ADDRESS, register_address, &register_new_value);
      }
      break;
    default:
      break;
  }
}

/**/
uint8_t DS1307_read(uint8_t registers, uint8_t *data_array)
{
  switch (registers)
  {
    case SECONDS:
      time_i2c_read_single(DS1307_I2C_ADDRESS, DS1307_REGISTER_SECONDS, &register_current_value);
      *data_array = register_current_value & (~(1 << DS1307_BIT_SETTING_CH));
      break;
    case MINUTES:
      time_i2c_read_single(DS1307_I2C_ADDRESS, DS1307_REGISTER_MINUTES, &register_current_value);
      *data_array = register_current_value;
      break;
    case HOURS:
      time_i2c_read_single(DS1307_I2C_ADDRESS, DS1307_REGISTER_HOURS, &register_current_value);
      *data_array = register_current_value & (~(1 << DS1307_BIT_SETTING_AMPM));
      break;
    case DAY_OF_WEEK:
      time_i2c_read_single(DS1307_I2C_ADDRESS, DS1307_REGISTER_DAY_OF_WEEK, &register_current_value);
      *data_array = register_current_value;
      break;
    case DATE:
      time_i2c_read_single(DS1307_I2C_ADDRESS, DS1307_REGISTER_DATE, &register_current_value);
      *data_array = register_current_value;
      break;
    case MONTH:
      time_i2c_read_single(DS1307_I2C_ADDRESS, DS1307_REGISTER_MONTH, &register_current_value);
      *data_array = register_current_value;
      break;
    case YEAR:
      time_i2c_read_single(DS1307_I2C_ADDRESS, DS1307_REGISTER_YEAR, &register_current_value);
      *data_array = register_current_value;
      break;
    case CONTROL:
      time_i2c_read_single(DS1307_I2C_ADDRESS, DS1307_REGISTER_CONTROL, &register_current_value);
      *data_array = register_current_value;
      break;
    case TIME:
      time_i2c_read_single(DS1307_I2C_ADDRESS, DS1307_REGISTER_SECONDS, &register_current_value);
      *data_array = register_current_value & (~(1 << DS1307_BIT_SETTING_CH));
      time_i2c_read_multi(DS1307_I2C_ADDRESS, DS1307_REGISTER_MINUTES, ++data_array, 6);
      break;
    case ALL:
      time_i2c_read_single(DS1307_I2C_ADDRESS, DS1307_REGISTER_SECONDS, &register_current_value);
      *data_array = register_current_value & (~(1 << DS1307_BIT_SETTING_CH));
      time_i2c_read_multi(DS1307_I2C_ADDRESS, DS1307_REGISTER_MINUTES, ++data_array, 7);
      break;
    default:
      return OPERATION_FAILED;
  }
  return OPERATION_DONE;
}

/**/
uint8_t DS1307_set(uint8_t registers, uint8_t *data_array)
{
  switch (registers)
  {
    case SECONDS:
      time_i2c_read_single(DS1307_I2C_ADDRESS, DS1307_REGISTER_SECONDS, &register_current_value);
      register_new_value = (register_current_value & (1 << DS1307_BIT_SETTING_CH)) | *data_array;
      time_i2c_write_single(DS1307_I2C_ADDRESS, DS1307_REGISTER_SECONDS, &register_new_value);
      break;
    case MINUTES:
      register_new_value = *data_array;
      time_i2c_write_single(DS1307_I2C_ADDRESS, DS1307_REGISTER_MINUTES, &register_new_value);
      break;
    case HOURS:
      register_new_value = (~(1 << DS1307_BIT_SETTING_AMPM)) & *data_array;
      time_i2c_write_single(DS1307_I2C_ADDRESS, DS1307_REGISTER_HOURS, &register_new_value);
      break;
    case DAY_OF_WEEK:
      register_new_value = *data_array;
      time_i2c_write_single(DS1307_I2C_ADDRESS, DS1307_REGISTER_DAY_OF_WEEK, &register_new_value);
      break;
    case DATE:
      register_new_value = *data_array;
      time_i2c_write_single(DS1307_I2C_ADDRESS, DS1307_REGISTER_DATE, &register_new_value);
      break;
    case MONTH:
      register_new_value = *data_array;
      time_i2c_write_single(DS1307_I2C_ADDRESS, DS1307_REGISTER_MONTH, &register_new_value);
      break;
    case YEAR:
      register_new_value = *data_array;
      time_i2c_write_single(DS1307_I2C_ADDRESS, DS1307_REGISTER_YEAR, &register_new_value);
      break;
    case CONTROL:
      register_new_value = *data_array;
      time_i2c_write_single(DS1307_I2C_ADDRESS, DS1307_REGISTER_CONTROL, &register_new_value);
      break;
    case TIME:
      time_i2c_read_single(DS1307_I2C_ADDRESS, DS1307_REGISTER_SECONDS, &register_current_value);
      register_new_value = (register_current_value & (1 << DS1307_BIT_SETTING_CH)) | data_array[0];
      time_i2c_write_single(DS1307_I2C_ADDRESS, DS1307_REGISTER_SECONDS, &register_current_value);
      data_array[2] &= (~(1 << DS1307_BIT_SETTING_AMPM));
      time_i2c_write_multi(DS1307_I2C_ADDRESS, DS1307_REGISTER_MINUTES, &data_array[1], 6);
      break;
    case ALL:
      time_i2c_read_single(DS1307_I2C_ADDRESS, DS1307_REGISTER_SECONDS, &register_current_value);
      register_new_value = (register_current_value & (1 << DS1307_BIT_SETTING_CH)) | *data_array;
      time_i2c_write_single(DS1307_I2C_ADDRESS, DS1307_REGISTER_SECONDS, &register_current_value);
      data_array[2] &= (~(1 << DS1307_BIT_SETTING_AMPM));
      time_i2c_write_multi(DS1307_I2C_ADDRESS, DS1307_REGISTER_MINUTES, &data_array[1], 7);
      break;
    default:
      return OPERATION_FAILED;
  }
  return OPERATION_DONE;
}
