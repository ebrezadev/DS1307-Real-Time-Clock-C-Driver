/*ds1307 rtc showcase, arduino example main code*/
#include "rtc_ds1307.h"

uint8_t time_set[7] = {0, 10, 13, 5, 25, 1, 21};        //array of 7 initial values for seconds (time_set[0]), minutes, hours, day of the week, date, month and year (time_set[6])
uint8_t time_snap[7];       //array of 7 bytes, to read a already saved snapshot from ds1307
uint8_t time_current[7];        //array of 7 bytes to refresh time values from ds1307

void setup() {
  Serial.begin(115200);
  DS1307_init(&time_set[0], CLOCK_RUN, NO_FORCE_RESET);       //&time_set[0] is the starting address of initial values to set the clock for the first time, CLOCK_RUN tells the ds1307 to run after initialization, NO_FORCE_RESET ensures that the mcu will only reset the ds1307 once
  
  DS1307_square_wave(WAVE_2);       //using the square wave capability of ds1307 (4 different waves)
  
  DS1307_snapshot_save();       //saving a snapshot of time, inside ds1307 RAM. theres only one slot for snapshot and each snapshot_save replaces the last one.
}

void loop() {
  DS1307_read(SNAPSHOT, time_snap);       //storing the previously saved snapshot inside ds1307 RAM into an array (time_snap[])
  DS1307_read(TIME, time_current);        //refreshing the current time, read from ds1307 into time_current array
  
  Serial.print("Snapshot: ");       
  timeToSerial(time_snap, 7);
  
  Serial.print("Current time: ");
  timeToSerial(time_current, 7);
  
  delay(1000);
}

void timeToSerial(uint8_t *data_array, uint8_t array_length)
{
   for (int8_t index = array_length - 1; index >= 0; index--)
  {
    Serial.print(data_array[index]);
    if (index != 0)
      Serial.print(":");
  }
  Serial.println("");
}
