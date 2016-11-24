#pragma once
#include <pebble.h>

void tick_handler(struct tm *tick_time, TimeUnits units_changed); //handle ticks
//handle fetching time/date data
void update_time();  
void update_weekday();
void update_date();