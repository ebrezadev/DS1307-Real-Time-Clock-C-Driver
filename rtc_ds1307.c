#include "rtc_ds1307.h"

static void BCD_to_HEX(uint8_t *data_array, uint8_t array_length);        /*turns the bcd numbers from ds1307 into hex*/
static void HEX_to_BCD(uint8_t *data_array, uint8_t array_length);        /*turns the hex numbers into bcd*/

static uint8_t register_current_value;        /*used to read current values of ds1307 registers*/
static uint8_t register_new_value;        /*used to write values to ds1307 registers*/
static uint8_t snap0_vacancy;
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
  run_state commands ds1307 to run or halt (CLOCK_RUN and CLOCK_HALT), and reset_state
  could force reset ds1307 (FORCE_RESET) or checks if ds1307 is reset beforehand
  (NO_FORCE_RESET)*/
uint8_t DS1307_init(uint8_t *data_array, uint8_t run_state, uint8_t reset_state)
{
  DS1307_I2C_init();
  if ((DS1307_init_status_report() == DS1307_NOT_INITIALIZED) || (reset_state == FORCE_RESET))
  {
    DS1307_run(CLOCK_HALT);
    DS1307_reset(ALL);       /*everything but the 56 bytes general purpose ram inside ds1307*/
    DS1307_reset(RAM);        /*fills ds1307 general purpose ram wiith default value*/
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
void DS1307_reset(uint8_t option)
{
  switch (option)
  {
    case SECOND:
      time_i2c_read_single(DS1307_I2C_ADDRESS, DS1307_REGISTER_SECONDS, &register_current_value);
      HEX_to_BCD(&register_default_value[0], 1);
      register_new_value = (register_current_value & (1 << DS1307_BIT_SETTING_CH)) | register_default_value[0];
      time_i2c_write_single(DS1307_I2C_ADDRESS, DS1307_REGISTER_SECONDS, &register_new_value);
      break;
    case MINUTE:
      HEX_to_BCD(&register_default_value[1], 1);
      register_new_value = register_default_value[1];
      time_i2c_write_single(DS1307_I2C_ADDRESS, DS1307_REGISTER_MINUTES, &register_new_value);
      break;
    case HOUR:
      HEX_to_BCD(&register_default_value[2], 1);
      register_new_value = register_default_value[2] & (~(1 << DS1307_BIT_SETTING_AMPM));
      time_i2c_write_single(DS1307_I2C_ADDRESS, DS1307_REGISTER_HOURS, &register_new_value);
      break;
    case DAY_OF_WEEK:
      HEX_to_BCD(&register_default_value[3], 1);
      register_new_value = register_default_value[3];
      time_i2c_write_single(DS1307_I2C_ADDRESS, DS1307_REGISTER_DAY_OF_WEEK, &register_new_value);
      break;
    case DATE:
      HEX_to_BCD(&register_default_value[4], 1);
      register_new_value = register_default_value[4];
      time_i2c_write_single(DS1307_I2C_ADDRESS, DS1307_REGISTER_DATE, &register_new_value);
      break;
    case MONTH:
      HEX_to_BCD(&register_default_value[5], 1);
      register_new_value = register_default_value[5];
      time_i2c_write_single(DS1307_I2C_ADDRESS, DS1307_REGISTER_MONTH, &register_new_value);
      break;
    case YEAR:
      HEX_to_BCD(&register_default_value[6], 1);
      register_new_value = register_default_value[6];
      time_i2c_write_single(DS1307_I2C_ADDRESS, DS1307_REGISTER_YEAR, &register_new_value);
      break;
    case CONTROL:
      register_new_value = DS1307_REGISTER_CONTROL_DEFAULT;
      time_i2c_write_single(DS1307_I2C_ADDRESS, DS1307_REGISTER_CONTROL, &register_new_value);
      break;
    case TIME:
      time_i2c_read_single(DS1307_I2C_ADDRESS, DS1307_REGISTER_SECONDS, &register_current_value);
      HEX_to_BCD(&register_default_value[0], 1);
      register_new_value = (register_current_value & (1 << DS1307_BIT_SETTING_CH)) | register_default_value[0];
      time_i2c_write_single(DS1307_I2C_ADDRESS, DS1307_REGISTER_SECONDS, &register_new_value);
      HEX_to_BCD(&register_default_value[2], 1);
      register_default_value[2] &= (~(1 << DS1307_BIT_SETTING_AMPM));
      time_i2c_write_multi(DS1307_I2C_ADDRESS, DS1307_REGISTER_MINUTES, &register_default_value[1], 6);
      break;
    case ALL:        /*everything is reset but the general purpose ram*/
      time_i2c_read_single(DS1307_I2C_ADDRESS, DS1307_REGISTER_SECONDS, &register_current_value);
      HEX_to_BCD(&register_default_value[0], 1);
      register_new_value = (register_current_value & (1 << DS1307_BIT_SETTING_CH)) | register_default_value[0];
      time_i2c_write_single(DS1307_I2C_ADDRESS, DS1307_REGISTER_SECONDS, &register_new_value);
      HEX_to_BCD(&register_default_value[2], 1);
      register_default_value[2] &= (~(1 << DS1307_BIT_SETTING_AMPM));
      time_i2c_write_multi(DS1307_I2C_ADDRESS, DS1307_REGISTER_MINUTES, &register_default_value[1], 7);
      break;
    case RAM:
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

/*function to read internal registers of ds1307, one register at a time or all registers*/
uint8_t DS1307_read(uint8_t option, uint8_t *data_array)
{
  switch (option)
  {
    case SECOND:
      time_i2c_read_single(DS1307_I2C_ADDRESS, DS1307_REGISTER_SECONDS, &register_current_value);
      *data_array = register_current_value & (~(1 << DS1307_BIT_SETTING_CH));
      BCD_to_HEX(data_array, 1);
      break;
    case MINUTE:
      time_i2c_read_single(DS1307_I2C_ADDRESS, DS1307_REGISTER_MINUTES, &register_current_value);
      *data_array = register_current_value;
      BCD_to_HEX(data_array, 1);
      break;
    case HOUR:
      time_i2c_read_single(DS1307_I2C_ADDRESS, DS1307_REGISTER_HOURS, &register_current_value);
      *data_array = register_current_value & (~(1 << DS1307_BIT_SETTING_AMPM));
      BCD_to_HEX(data_array, 1);
      break;
    case DAY_OF_WEEK:
      time_i2c_read_single(DS1307_I2C_ADDRESS, DS1307_REGISTER_DAY_OF_WEEK, &register_current_value);
      *data_array = register_current_value;
      BCD_to_HEX(data_array, 1);
      break;
    case DATE:
      time_i2c_read_single(DS1307_I2C_ADDRESS, DS1307_REGISTER_DATE, &register_current_value);
      *data_array = register_current_value;
      BCD_to_HEX(data_array, 1);
      break;
    case MONTH:
      time_i2c_read_single(DS1307_I2C_ADDRESS, DS1307_REGISTER_MONTH, &register_current_value);
      *data_array = register_current_value;
      BCD_to_HEX(data_array, 1);
      break;
    case YEAR:
      time_i2c_read_single(DS1307_I2C_ADDRESS, DS1307_REGISTER_YEAR, &register_current_value);
      *data_array = register_current_value;
      BCD_to_HEX(data_array, 1);
      break;
    case CONTROL:
      time_i2c_read_single(DS1307_I2C_ADDRESS, DS1307_REGISTER_CONTROL, &register_current_value);
      *data_array = register_current_value;
      break;
    case TIME:
      time_i2c_read_single(DS1307_I2C_ADDRESS, DS1307_REGISTER_SECONDS, &register_current_value);
      data_array[0] = register_current_value & (~(1 << DS1307_BIT_SETTING_CH));
      time_i2c_read_multi(DS1307_I2C_ADDRESS, DS1307_REGISTER_MINUTES, &data_array[1], 6);
      BCD_to_HEX(data_array, 7);
      break;
    case SNAPSHOT:
      /*high level function to load a previously saved snapshot from ds1307 RAM. If there is
         no saved snapshots, returns nothing. needs an array of 7 bytes as input to function*/
      time_i2c_read_single(DS1307_I2C_ADDRESS, DS1307_REGISTER_SNAP0_VACANCY, &snap0_vacancy);
      if (snap0_vacancy == OCCUPIED)
      {
        time_i2c_read_multi(DS1307_I2C_ADDRESS, DS1307_SNAP0_ADDRESS, data_array, 7);
        BCD_to_HEX(data_array, 7);
        break;
      }
      else
        return OPERATION_FAILED;
    case ALL:
      time_i2c_read_single(DS1307_I2C_ADDRESS, DS1307_REGISTER_SECONDS, &register_current_value);
      data_array[0] = register_current_value & (~(1 << DS1307_BIT_SETTING_CH));
      time_i2c_read_multi(DS1307_I2C_ADDRESS, DS1307_REGISTER_MINUTES, &data_array[1], 7);
      BCD_to_HEX(data_array, 7);
      break;
    default:
      return OPERATION_FAILED;
  }
  return OPERATION_DONE;
}

/*function to set internal registers of ds1307, one register at a time or all registers*/
uint8_t DS1307_set(uint8_t option, uint8_t *data_array)
{
  switch (option)
  {
    case SECOND:
      HEX_to_BCD(data_array, 1);
      time_i2c_read_single(DS1307_I2C_ADDRESS, DS1307_REGISTER_SECONDS, &register_current_value);
      register_new_value = (register_current_value & (1 << DS1307_BIT_SETTING_CH)) | *data_array;
      time_i2c_write_single(DS1307_I2C_ADDRESS, DS1307_REGISTER_SECONDS, &register_new_value);
      break;
    case MINUTE:
      HEX_to_BCD(data_array, 1);
      register_new_value = *data_array;
      time_i2c_write_single(DS1307_I2C_ADDRESS, DS1307_REGISTER_MINUTES, &register_new_value);
      break;
    case HOUR:
      HEX_to_BCD(data_array, 1);
      register_new_value = (~(1 << DS1307_BIT_SETTING_AMPM)) & *data_array;
      time_i2c_write_single(DS1307_I2C_ADDRESS, DS1307_REGISTER_HOURS, &register_new_value);
      break;
    case DAY_OF_WEEK:
      HEX_to_BCD(data_array, 1);
      register_new_value = *data_array;
      time_i2c_write_single(DS1307_I2C_ADDRESS, DS1307_REGISTER_DAY_OF_WEEK, &register_new_value);
      break;
    case DATE:
      HEX_to_BCD(data_array, 1);
      register_new_value = *data_array;
      time_i2c_write_single(DS1307_I2C_ADDRESS, DS1307_REGISTER_DATE, &register_new_value);
      break;
    case MONTH:
      HEX_to_BCD(data_array, 1);
      register_new_value = *data_array;
      time_i2c_write_single(DS1307_I2C_ADDRESS, DS1307_REGISTER_MONTH, &register_new_value);
      break;
    case YEAR:
      HEX_to_BCD(data_array, 1);
      register_new_value = *data_array;
      time_i2c_write_single(DS1307_I2C_ADDRESS, DS1307_REGISTER_YEAR, &register_new_value);
      break;
    case CONTROL:
      register_new_value = *data_array;
      time_i2c_write_single(DS1307_I2C_ADDRESS, DS1307_REGISTER_CONTROL, &register_new_value);
      break;
    case TIME:
      HEX_to_BCD(data_array, 7);
      time_i2c_read_single(DS1307_I2C_ADDRESS, DS1307_REGISTER_SECONDS, &register_current_value);
      register_new_value = (register_current_value & (1 << DS1307_BIT_SETTING_CH)) | data_array[0];
      time_i2c_write_single(DS1307_I2C_ADDRESS, DS1307_REGISTER_SECONDS, &register_new_value);
      data_array[2] &= (~(1 << DS1307_BIT_SETTING_AMPM));
      time_i2c_write_multi(DS1307_I2C_ADDRESS, DS1307_REGISTER_MINUTES, &data_array[1], 6);
      break;
    case ALL:
      HEX_to_BCD(data_array, 7);
      time_i2c_read_single(DS1307_I2C_ADDRESS, DS1307_REGISTER_SECONDS, &register_current_value);
      register_new_value = (register_current_value & (1 << DS1307_BIT_SETTING_CH)) | data_array[0];
      time_i2c_write_single(DS1307_I2C_ADDRESS, DS1307_REGISTER_SECONDS, &register_new_value);
      data_array[2] &= (~(1 << DS1307_BIT_SETTING_AMPM));
      time_i2c_write_multi(DS1307_I2C_ADDRESS, DS1307_REGISTER_MINUTES, &data_array[1], 7);
      break;
    default:
      return OPERATION_FAILED;
  }
  return OPERATION_DONE;
}

/*function to utilize the square wave capability of ds1307 i 5 different modes:
   WAVE_OFF, WAVE_1 for 1Hz, WAVE_2 for 4.096KHz, WAVE_3 for 8.192KHz, WAVE_4
   for 32.768 KHz*/
uint8_t DS1307_square_wave(uint8_t input)
{
  switch (input)
  {
    case WAVE_OFF:
      register_new_value = 0X00;
      time_i2c_write_single(DS1307_I2C_ADDRESS, DS1307_REGISTER_CONTROL, &register_new_value);
      return OPERATION_DONE;
    case WAVE_1:
      register_new_value = 0X10;
      time_i2c_write_single(DS1307_I2C_ADDRESS, DS1307_REGISTER_CONTROL, &register_new_value);
      return OPERATION_DONE;
    case WAVE_2:
      register_new_value = 0X11;
      time_i2c_write_single(DS1307_I2C_ADDRESS, DS1307_REGISTER_CONTROL, &register_new_value);
      return OPERATION_DONE;
    case WAVE_3:
      register_new_value = 0X12;
      time_i2c_write_single(DS1307_I2C_ADDRESS, DS1307_REGISTER_CONTROL, &register_new_value);
      return OPERATION_DONE;
    case WAVE_4:
      register_new_value = 0X13;
      time_i2c_write_single(DS1307_I2C_ADDRESS, DS1307_REGISTER_CONTROL, &register_new_value);
      return OPERATION_DONE;
  }
  return OPERATION_FAILED;
}


/*high level function to save a snapshot of all the time and control registers to ds1307 RAM.
  there is only one slot for time snapshot, and a new save clears the last snapshot.*/
void DS1307_snapshot_save()
{
  uint8_t data_array_temporary[7];
  time_i2c_read_multi(DS1307_I2C_ADDRESS, DS1307_REGISTER_SECONDS, data_array_temporary, 7);
  time_i2c_write_multi(DS1307_I2C_ADDRESS, DS1307_SNAP0_ADDRESS, data_array_temporary, 7);
  snap0_vacancy = OCCUPIED;
  time_i2c_write_single(DS1307_I2C_ADDRESS, DS1307_REGISTER_SNAP0_VACANCY, &snap0_vacancy);
}

/*high level function to clear the sapshot slot on ds1307 RAM*/
void DS1307_snapshot_clear()
{
  snap0_vacancy = NOT_OCCUPIED;
  time_i2c_write_single(DS1307_I2C_ADDRESS, DS1307_REGISTER_SNAP0_VACANCY, &snap0_vacancy);
}

static void BCD_to_HEX(uint8_t *data_array, uint8_t array_length)
{
  for (int8_t index = (array_length - 1); index >= 0; index--)
  {
    data_array[index] = ((data_array[index] >> 4) << 1) + ((data_array[index] >> 4) << 3) + (data_array[index] & 0X0F);
  }
}

static void HEX_to_BCD(uint8_t *data_array, uint8_t array_length)
{
  uint8_t temporary_value;
  for (int8_t index = (array_length - 1); index >= 0; index--)
  {
    temporary_value = 0;
    while (((int8_t)data_array[index] - 0X0A) >= 0)
    {
      temporary_value += 0X10;
      data_array[index] -= 0X0A;
    }
    temporary_value += data_array[index];
    data_array[index] = temporary_value;
  }
}
