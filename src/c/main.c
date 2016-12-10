#include <pebble.h>
#include "global.h"
#include "settings.h"
#include "window.h"
#include "time.h"
#include "callbacks.h"

static void init();   //called on start
static void unload(); //called on exit

int main(void){
  init();
  app_event_loop();
  unload();  
}

static void init()
{
 
  //load saved settings
  load_settings();

  // Listen for AppMessages
  app_message_register_inbox_received(inbox_received_handler);
  app_message_open(128, 128);
  
  //create window
  main_window = window_create();  
  //set window handlers
  window_set_window_handlers(main_window,(WindowHandlers) {
        .load = window_load,
        .unload = window_unload
 });
  
  //add window
  window_stack_push(main_window, true);
  
  //register for bluetooth conection updates
  connection_service_subscribe((ConnectionHandlers)
     {
       .pebble_app_connection_handler = bluetooth_callback
     });
    
  //set ticktimeservice
  tick_timer_service_subscribe(MINUTE_UNIT | DAY_UNIT, tick_handler);
  
  
  // Make sure the time is displayed from the start
  update_time();
  update_weekday();
  if(settings.date_enabled) //update date if enabled in settings
    update_date();
}

static void unload()
{
  //destroy window
  if (main_window) {
    window_destroy(main_window);
 }
}



