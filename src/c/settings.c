#include "global.h"
#include "settings.h"
#include "window.h"

void inbox_received_handler(DictionaryIterator *iter, void *context) {
  
  //assign the values from settings to struct
  
  //background enabled
  Tuple *bg_enabled = dict_find(iter, MESSAGE_KEY_enable_background);
  if(bg_enabled) {
   settings.background_enabled = bg_enabled->value->int32 == 1;
  }
  //bg color
  Tuple *bg_color = dict_find(iter, MESSAGE_KEY_background_color);
  if(bg_color) {
   settings.background_color = GColorFromHEX(bg_color->value->int32);
  }
  //date color
  Tuple *date_color = dict_find(iter, MESSAGE_KEY_date_color);
  if(bg_color) {
   settings.date_color = GColorFromHEX(date_color->value->int32);
  }
  //date enabled
  Tuple *show_date = dict_find(iter, MESSAGE_KEY_show_date);
  if(bg_color) {
   settings.date_enabled = show_date->value->int32 == 1;
  }
  //time position
  Tuple *position = dict_find(iter, MESSAGE_KEY_position);
  if(position) {    
    if(strcmp(position->value->cstring, "top")==0)    
       settings.position=TOP;    
    else    
         settings.position=CENTER; 
  }
  //date font
  Tuple *date_font = dict_find(iter, MESSAGE_KEY_date_font);
  if(date_font) {
    if(strcmp(date_font->value->cstring, "pixel")==0)
        settings.date_font=PIXEL;
    else   
         settings.date_font=LIES;    
  }
 //vibration
  Tuple *vibe_enabled = dict_find(iter, MESSAGE_KEY_vibe_disconnect);
  if(vibe_enabled)
    settings.vibe_on_disconnect = vibe_enabled->value->int32 == 1;
  
  
 //save loaded settings
 save_settings();
}

void save_settings() 
{
  persist_write_data(SETTINGS_KEY, &settings, sizeof(settings));
  
  //refresh window
  window_update();
}

void load_settings() 
{
  // Load the default settings
  load_default_settings();
  // Read settings from persistent storage, if they exist
  persist_read_data(SETTINGS_KEY, &settings, sizeof(settings));
}

void load_default_settings() {
  settings.background_enabled = true;
  settings.background_color = GColorWhite;
  settings.date_color = GColorTiffanyBlue;
  settings.date_enabled = true;
  settings.position=TOP;
  settings.date_font=LIES;
  settings.vibe_on_disconnect=false;
}