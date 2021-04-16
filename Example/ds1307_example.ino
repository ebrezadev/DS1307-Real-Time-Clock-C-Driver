#include "rtc_ds1307.h"

uint8_t time_set[7] = {0, 10, 13, 5, 25, 1, 21};
uint8_t time_snap[7];
uint8_t time_current[7];

void setup() {
  Serial.begin(115200);
  DS1307_init(&time_set[0], CLOCK_RUN, NO_FORCE_RESET);
  
  DS1307_square_wave(WAVE_2);
  
  DS1307_snapshot_save();
}

void loop() {
  DS1307_read(SNAPSHOT, time_snap);
  DS1307_read(TIME, time_current);
  
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
