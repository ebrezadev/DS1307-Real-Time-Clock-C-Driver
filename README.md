# DS1307 C DRIVER
* Version 1.0
* Reza Ebrahimi
## INTRODUCTION
Here is the MCU independent, lightweight and portable C driver for DS1307 Real Time Clock. "The DS1307 serial real-time clock (RTC) is a low-power, full binary-coded decimal (BCD) clock/calendar plus 56 bytes of NV SRAM". The driver is written using standard C straight from Maxim Integrated datasheet and is not influenced by other libraries. CPU intensive tasks are avoided, and mathematical expressions are written using bitwise operators. In order to port this library to any MCU, you have to tweak the low level API file.

## FEATURES
The DS1307 has three main features:

•	Real time BCD clock with stop/start and AM/PM pr 24 hours capabilities.

•	Square wave generator with 4 programmable frequencies: 1 Hz, 4.096 KHz, 8.192 KHz and 32.768 KHz.

•	56 Bytes general purpose RAM.

All of these features are utilized in this library. You can initialize DS1307 using DS1307_init() (with automatic reset, since DS1307 is battery backed and if MCU is reset or out of power, DS1307 continues to keep time so there’s no need to reset the DS1307 everytime our firmware starts). You can start or stop the square wave generator using DS1307_square_wave(). You can also save a snapshot of time inside DS1307 general purpose RAM, for example in the case of an event happening, using DS1307_snapshot_save().

## HOW TO USE
First, you need to tweak the low level file for your MCU (it consists of I2C configurations) which is all you have to do to start using DS1307. In the next step, use DS1307_init(uint8_t *data_array, uint8_t run_state, uint8_t reset_state) to start using DS1307. DS1307_init sets up the I2C connection, sets up the time using your input (data_array[7], in case of a reset), starts or stops the time progression (run_state = CLOCK_START or run_state = CLOCK_HALT) and force resets (or not) the internal registers and RAM of DS1307 (reset_state = FORCE_RESET which resets DS1307 and sets up time from beginning using data_array[], or NO_FORCE_RESET which will automatically not reset DS1307, if it has been set up once and resets DS1307 if it is freshly powered up for the first time).

Now, you can read time by DS1307_read(TIME, time_array); time_array is an array of 7 bytes to read all the time registers inside DS1307. Instead of TIME, you can use these keywords to load your preferred registers from DS1307: SECOND (1 byte), MINUTE (1 byte), HOUR (1 byte), DAY_OF_WEEK, DATE (1 byte), MONTH (1 byte), YEAR (1 byte), CONTROL (1 byte), SNAPSHOT (1 Byte), TIME (7 bytes), ALL (8 bytes).

You can save a snapshot of time using DS1307_snapshot_save() (in case of the happening of an event, for example) inside DS1307 RAM. The handling of save and load are automatic. There’s only one slot of 7 bytes of DS1307’s RAM considered for this purpose and each snapshot takes the place of last snapshot. You can read this snapshot using DS1307_read(SNAPSHOT, snap_array) in which snap_array is an array of 7 bytes. If there was no snapshot saved since last RAM reset, DS1307_read returns an error and snap_array will not be updated.

After initializing, you can use DS1307_reset(ALL) to clear DS1307 to its initial zero values (time settings and RAM contents such as snapshot are lost) or DS1307_reset(SECOND) or any other register to reset them one by one. Then you can set the time registers again, using DS1307_set(TIME, time_set) in which time_set is an array of 7 bytes.

Other useable function is DS1307_run(CLOCK_RUN *or* CLOCK_HALT) to run or halt the clock (please note, resetting or setting the time will not affect run state).

Please note that DS1307 reports time in BCD format. For the ease of use, the driver automatically handles all the conversions between BCD to HEX and HEX to BCD.

The AM/PM or 24 hours capability is set to 24 hours by default and cannot be changed. 

## HOW IT WORKS
Different functions in this library can be categorized into different levels of abstraction from low level functions dealing with I2C hardware, up to higher level functions reporting back time, handling snapshot and etc.

### LEVEL 1:
void DS1307_I2C_init

void time_i2c_write_single

void time_i2c_write_multi

void time_i2c_read_single

void time_i2c_read_multi

### LEVEL 2:
void DS1307_reset

uint8_t DS1307_set

uint8_t DS1307_run

uint8_t DS1307_run_state

uint8_t DS1307_read

uint8_t DS1307_square_wave

uint8_t DS1307_init_status_report

void DS1307_snapshot_save

void DS1307_snapshot_clear

void DS1307_init_status_update

### LEVEL 3:
uint8_t DS1307_init


