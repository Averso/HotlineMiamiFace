#include <pebble.h>
#include <ctype.h>
#include "time.h"
#include "global.h"

void tick_handler(struct tm *tick_time, TimeUnits units_changed)
{
  if (units_changed & MINUTE_UNIT)
  {
     update_time();
     
     
  }
  if(units_changed & DAY_UNIT)
  {
    update_weekday();
    
   if(settings.date_enabled)
      update_date();
  }
  
}

void update_time()
{
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);
  
  //get time 
  static char s_buffer[8];
  strftime(s_buffer, sizeof(s_buffer), clock_is_24h_style() ? "%H:%M" : "%I:%M", tick_time);  
  
  //display time on text layer
  for(int i=0; i<TIME_LAYERS_SIZE;i++)
    text_layer_set_text(layer_time[i], s_buffer);
  
  //show/hide quiet time icon depending if it's on
  layer_set_hidden(bitmap_layer_get_layer(layer_quiettime), !quiet_time_is_active());
}

void update_weekday() 
{
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);
  
  //get weekday
  static char s_buffer[10];
  strftime(s_buffer, sizeof(s_buffer),"%A", tick_time);
   
  //change to uppercase
  int i=0;  
  while((unsigned char)s_buffer[i])
  {
    s_buffer[i] = tolower((unsigned char)s_buffer[i]);
    i++;
  }
  //change text on weekday text layers
  for(int i=0; i<WDAY_LAYERS_SIZE;i++)
    text_layer_set_text(layer_weekday[i], s_buffer);
//   text_layer_set_text(layer_weekday1, s_buffer);
//   text_layer_set_text(layer_weekday2, s_buffer);
//   text_layer_set_text(layer_weekday3, s_buffer);
//   text_layer_set_text(layer_weekday4, s_buffer);
//   text_layer_set_text(layer_weekday5, s_buffer);
//   text_layer_set_text(layer_weekday6, s_buffer);
  
  
}

void update_date() 
{
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);
  
  static char s_buffer[18];
  strftime(s_buffer, sizeof(s_buffer), "%B %d, %G", tick_time);
  
  text_layer_set_text(layer_date, s_buffer);
}


