#include <pebble.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
// Persistent storage key
#define SETTINGS_KEY 1

//globals
static Window *s_main_window;                                     //window
static TextLayer *s_time1_layer, *s_time2_layer,*s_time3_layer,*s_day1_layer,
                *s_day2_layer,*s_day3_layer,*s_day4_layer,
                *s_day5_layer,*s_day6_layer,*s_date_layer;       //text layers
static GFont s_time_font, s_date_font_lies,s_date_font_pixel, s_day_font;                //fonts
static BitmapLayer *s_background_layer;                           //image layers
static GBitmap *s_background_bitmap;                              //bitmaps


//positions of layers 
static int date_x = 0;
// y position of date depending on platform

#if defined(PBL_PLATFORM_BASALT)
static int date_y = 150;
#endif
#if defined(PBL_PLATFORM_CHALK)
static int date_y = 120;
#endif 
#if defined(PBL_PLATFORM_EMERY)
static int date_y = 205;
#endif 

static int day_x = 0;
static int time_x = 0;
//positions of clock,weekday layers - center

//positions of clock,weekday layers - top

#if defined(PBL_PLATFORM_BASALT)
static int dayt_y = 15;
static int timet_y = 37;
static int dayc_y = 45;
static int timec_y = 67;
#endif
#if defined(PBL_PLATFORM_CHALK)
static int dayt_y = 25;
static int timet_y = 57;
static int dayc_y = 55;
static int timec_y = 77;
#endif 
#if defined(PBL_PLATFORM_EMERY)
static int dayt_y = 25;
static int timet_y = 57;
static int dayc_y = 55;
static int timec_y = 87;
#endif 

#if defined(PBL_PLATFORM_BASALT) || (PBL_PLATFORM_CHALK)
static int day_text_height = 35;
static int date_text_height = 18;
static int time_text_height = 60;
#endif 
#if defined(PBL_PLATFORM_EMERY)
static int day_text_height = 50;
static int date_text_height = 30;
static int time_text_height = 80;
#endif 



static  GRect bounds;

static void main_window_load(Window *window);
static void main_window_unload(Window *window);
static void init();
static void unload();
static void tick_handler(struct tm *tick_time, TimeUnits units_changed);
static void update_time();
static void update_day();
static void update_date();
static void set_up_text_layer(TextLayer *layer, GColor background, GColor text_color, const char * text,GFont font,GTextAlignment alignment);
static void prv_inbox_received_handler(DictionaryIterator *iter, void *context);
static void prv_default_settings();
static void prv_save_settings();
static void prv_load_settings();
static void set_weekday_y(int y);
static void set_time_y(int y);
static void main_window_update();


//enums for settings
typedef enum {PIXEL, LIES} DateFont;
typedef enum {TOP, CENTER} Position;

// Define our settings struct
typedef struct ClaySettings {
  GColor background_color;
  GColor date_color;
  bool background_enabled;
  bool date_enabled;
  Position position; 
  DateFont date_font;
} ClaySettings;

// An instance of the struct
static ClaySettings settings;


int main(void)
{
  init();
  app_event_loop();
  unload();
  
}


//INIT
static void init()
{
  APP_LOG(APP_LOG_LEVEL_DEBUG, "INIT Try to load settings");
  //load saved settings
  prv_load_settings();

   APP_LOG(APP_LOG_LEVEL_DEBUG, "Position load setting after load: %d",  settings.position);
  // Listen for AppMessages
  app_message_register_inbox_received(prv_inbox_received_handler);
  app_message_open(128, 128);
  
  
  //create window
  s_main_window = window_create();  
  //set window handlers
  window_set_window_handlers(s_main_window,(WindowHandlers) {
        .load = main_window_load,
        .unload = main_window_unload
 });
  
  //add window
  window_stack_push(s_main_window, true);
    
  //set ticktimeservice
  tick_timer_service_subscribe(MINUTE_UNIT | DAY_UNIT, tick_handler);
  
  // Make sure the time is displayed from the start
  update_time();
 // update_day();
  if(settings.date_enabled)
    update_date();
}

//EXIT
static void unload()
{
  //destroy window
  if (s_main_window) {
    window_destroy(s_main_window);
 }
}

static void main_window_load(Window *window)
{
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Position on beginig of win load: %d",  settings.position);
  //get info about window
  Layer *window_layer = window_get_root_layer(window);
  bounds = layer_get_bounds(window_layer);
  
  //FONTS
  
  #if defined(PBL_PLATFORM_BASALT) || (PBL_PLATFORM_CHALK)
  s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_ROBOTO_40));
  s_day_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_DISCODUCK_CON_30));  
  s_date_font_lies = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_TRUELIES_13));
  s_date_font_pixel = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_PIXELMIX_11));
  #endif
  #if defined(PBL_PLATFORM_EMERY)
  s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_ROBOTO_50));
  s_day_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_DISCODUCK_CON_40));  
  s_date_font_lies = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_TRUELIES_17));
  s_date_font_pixel = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_PIXELMIX_16));
  #endif 
  
  
  //BACKGROUND
  
  //load bitmap depending on platform
  #if defined(PBL_PLATFORM_BASALT)
  s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BACKGROUND_PT);
  #endif
  #if defined(PBL_PLATFORM_CHALK)
  s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BACKGROUND_PTR);
  #endif
  #if defined(PBL_PLATFORM_EMERY)
  s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BACKGROUND_PT2);
  #endif
  s_background_layer=bitmap_layer_create(bounds);
  window_set_background_color(window, settings.background_color);
    
  
  //create text layers 
  //date
  s_date_layer = text_layer_create(
    GRect(date_x, date_y, bounds.size.w, date_text_height));
  
  //days
  s_day1_layer = text_layer_create(
    GRect(day_x, dayt_y, bounds.size.w, day_text_height));
  s_day2_layer = text_layer_create(
    GRect(day_x+2, dayt_y+2, bounds.size.w, day_text_height));
  s_day3_layer = text_layer_create(
    GRect(day_x+5, dayt_y+4, bounds.size.w, day_text_height));
  s_day4_layer = text_layer_create(
    GRect(day_x+6, dayt_y+6, bounds.size.w, day_text_height));
  s_day5_layer = text_layer_create(
    GRect(day_x+7, dayt_y+8, bounds.size.w, day_text_height));
  s_day6_layer = text_layer_create(
    GRect(day_x+8, dayt_y+10, bounds.size.w, day_text_height));
  
  
  s_time1_layer = text_layer_create(
    GRect(time_x, timet_y, bounds.size.w, 60));
  s_time2_layer = text_layer_create(
    GRect(time_x+1, timet_y-1, bounds.size.w, 60));  
  s_time3_layer = text_layer_create(
    GRect(time_x+4, timet_y+2, bounds.size.w, 60));  
 
  
  //SET UP LAYERS  
  //set bitmap onto layer
  bitmap_layer_set_bitmap(s_background_layer,s_background_bitmap);    
    
  //set up time layout
  set_up_text_layer(s_time1_layer, GColorClear, GColorElectricBlue, "00:00", s_time_font,GTextAlignmentCenter);
  set_up_text_layer(s_time2_layer, GColorClear, GColorYellow, "00:00", s_time_font,GTextAlignmentCenter);
  set_up_text_layer(s_time3_layer, GColorClear, GColorOxfordBlue, "00:00", s_time_font,GTextAlignmentCenter);
 
  //set up day layers
  set_up_text_layer(s_day1_layer, GColorClear, GColorFashionMagenta, "wednesday", s_day_font,GTextAlignmentCenter);
  set_up_text_layer(s_day2_layer, GColorClear, GColorBlack, "wednesday", s_day_font,GTextAlignmentCenter);
  set_up_text_layer(s_day3_layer, GColorClear, GColorImperialPurple, "wednesday", s_day_font,GTextAlignmentCenter);
  set_up_text_layer(s_day4_layer, GColorClear, GColorOxfordBlue, "wednesday", s_day_font,GTextAlignmentCenter);
  set_up_text_layer(s_day5_layer, GColorClear, GColorOrange, "wednesday", s_day_font,GTextAlignmentCenter);
  set_up_text_layer(s_day6_layer, GColorClear, GColorChromeYellow, "wednesday", s_day_font,GTextAlignmentCenter);
  
  //set up date layer
  set_up_text_layer(s_date_layer, GColorClear, GColorTiffanyBlue, "September 27, 1989", s_date_font_lies,GTextAlignmentCenter);
  
  
  //add layers    
  layer_add_child(window_layer,bitmap_layer_get_layer(s_background_layer));
  layer_add_child(window_layer,text_layer_get_layer(s_day6_layer));          //days
  layer_add_child(window_layer,text_layer_get_layer(s_day5_layer));  
  layer_add_child(window_layer,text_layer_get_layer(s_day4_layer));          
  layer_add_child(window_layer,text_layer_get_layer(s_day3_layer));          
  layer_add_child(window_layer,text_layer_get_layer(s_day2_layer)); 
  layer_add_child(window_layer,text_layer_get_layer(s_day1_layer)); 
  layer_add_child(window_layer,text_layer_get_layer(s_time3_layer));  
  layer_add_child(window_layer,text_layer_get_layer(s_time2_layer));          //time
  layer_add_child(window_layer,text_layer_get_layer(s_time1_layer));          
  layer_add_child(window_layer,text_layer_get_layer(s_date_layer));         //date
  
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Window load");
  main_window_update();
}

static void main_window_unload(Window *window)
{
  //destroy text layers
  text_layer_destroy(s_time1_layer);
  text_layer_destroy(s_time2_layer);
  text_layer_destroy(s_time3_layer);
  text_layer_destroy(s_day1_layer);
  text_layer_destroy(s_day2_layer);
  text_layer_destroy(s_day3_layer);
  text_layer_destroy(s_day4_layer);
  text_layer_destroy(s_day5_layer);
  text_layer_destroy(s_day6_layer);
  text_layer_destroy(s_date_layer);
  
  
  //unload GFonts
  fonts_unload_custom_font(s_time_font);
  fonts_unload_custom_font(s_day_font);
  fonts_unload_custom_font(s_date_font_lies);
  fonts_unload_custom_font(s_date_font_pixel);
  
  
  //destroy gbitmap
  gbitmap_destroy(s_background_bitmap);
  bitmap_layer_destroy(s_background_layer);
}

static void main_window_update()
{   
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Position window update: %d",  settings.position);
  if(settings.background_enabled)
  {
     //show bitmap
     bitmap_layer_set_compositing_mode(s_background_layer, GCompOpAssign);
  }
  else
  {
    //set window color
    window_set_background_color(s_main_window, settings.background_color);
    //set bitmap transparent
    bitmap_layer_set_compositing_mode(s_background_layer, GCompOpClear);    
    
  }
  //date color
  text_layer_set_text_color(s_date_layer,  settings.date_color);
  
  //position of clock
  APP_LOG(APP_LOG_LEVEL_DEBUG, "window update position: %d", settings.position);
  if(settings.position == TOP)
  {
     APP_LOG(APP_LOG_LEVEL_DEBUG, "Put clock top");
    set_weekday_y(dayt_y);
    set_time_y(timet_y);   
  }
  else
  {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Put clock in center");
    set_weekday_y(dayc_y);
    set_time_y(timec_y);
          
  }
  
  //show date
  if(settings.date_enabled)
  {
    update_date();
  }
  else
  {
    text_layer_set_text(s_date_layer,"");          
  }  
  
  //date font  
  if(settings.date_font == PIXEL)
  {
    text_layer_set_font(s_date_layer, s_date_font_pixel);
  }
  else
  {
    text_layer_set_font(s_date_layer, s_date_font_lies);
          
  }
  
  
}
static void tick_handler(struct tm *tick_time, TimeUnits units_changed)
{
  if (units_changed & MINUTE_UNIT)
  {
     update_time();
  }
  if(units_changed & DAY_UNIT)
  {
  // update_day();
    
   if(settings.date_enabled)
      update_date();
  }
  
}

static void update_time()
{
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);
  
  //get time 
  static char s_buffer[8];
  strftime(s_buffer, sizeof(s_buffer), clock_is_24h_style() ? "%H:%M" : "%I:%M", tick_time);
  
  
  //display time on text layer
  text_layer_set_text(s_time1_layer, s_buffer);
  text_layer_set_text(s_time2_layer, s_buffer);
  text_layer_set_text(s_time3_layer, s_buffer);
}

static void update_day() 
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
  //display time on text layer
  text_layer_set_text(s_day1_layer, s_buffer);
  text_layer_set_text(s_day2_layer, s_buffer);
  text_layer_set_text(s_day3_layer, s_buffer);
  text_layer_set_text(s_day4_layer, s_buffer);
  text_layer_set_text(s_day5_layer, s_buffer);
  text_layer_set_text(s_day6_layer, s_buffer);
  
  
}

static void update_date() 
{
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);
  
  static char s_buffer[18];
  strftime(s_buffer, sizeof(s_buffer), "%B %d, %G", tick_time);
      
  //display time on text layer
  text_layer_set_text(s_date_layer, s_buffer);
}

static void set_up_text_layer(TextLayer *layer, GColor background, GColor text_color, const char * text,GFont font,GTextAlignment alignment)
{
  text_layer_set_background_color(layer, background);
  text_layer_set_text_color(layer, text_color);
  text_layer_set_text(layer, text);
  text_layer_set_font(layer, font);
  text_layer_set_text_alignment(layer,alignment); 
}

static void prv_inbox_received_handler(DictionaryIterator *iter, void *context) {
  
   // Assign the values to our struct
  Tuple *bg_enabled = dict_find(iter, MESSAGE_KEY_enable_background);
  if(bg_enabled) {
   settings.background_enabled = bg_enabled->value->int32 == 1;
  }
  Tuple *bg_color = dict_find(iter, MESSAGE_KEY_background_color);
  if(bg_color) {
   settings.background_color = GColorFromHEX(bg_color->value->int32);
  }
  Tuple *date_color = dict_find(iter, MESSAGE_KEY_date_color);
  if(bg_color) {
   settings.date_color = GColorFromHEX(date_color->value->int32);
  }
  
  Tuple *show_date = dict_find(iter, MESSAGE_KEY_show_date);
  if(bg_color) {
   settings.date_enabled = show_date->value->int32 == 1;
  }

  Tuple *position = dict_find(iter, MESSAGE_KEY_position);
  if(position) {    
    if(strcmp(position->value->cstring, "top")==0)    
       settings.position=TOP;    
    else    
         settings.position=CENTER;
    
  
  }
  Tuple *date_font = dict_find(iter, MESSAGE_KEY_date_font);
  if(date_font) {
    if(strcmp(date_font->value->cstring, "pixel")==0)
        settings.date_font=PIXEL;
    else   
         settings.date_font=LIES;
    
  }
  
 prv_save_settings();
}

static void prv_save_settings() {
  persist_write_data(SETTINGS_KEY, &settings, sizeof(settings));
  
  //refresh window
  main_window_update();
}

// Read settings from persistent storage
static void prv_load_settings() {
  // Load the default settings
  prv_default_settings();
  // Read settings from persistent storage, if they exist
  persist_read_data(SETTINGS_KEY, &settings, sizeof(settings));
}

static void prv_default_settings() {
  settings.background_enabled = true;
  settings.background_color = GColorWhite;
  settings.date_color = GColorTiffanyBlue;
  settings.date_enabled = true;
  settings.position=TOP;
  settings.date_font=LIES;
}

static void set_weekday_y(int y)
{
    layer_set_frame(text_layer_get_layer(s_day1_layer), GRect(day_x, y, bounds.size.w, day_text_height));
    layer_set_frame(text_layer_get_layer(s_day2_layer), GRect(day_x+2, y+2, bounds.size.w, day_text_height));
    layer_set_frame(text_layer_get_layer(s_day3_layer), GRect(day_x+5, y+4, bounds.size.w, day_text_height));                 
    layer_set_frame(text_layer_get_layer(s_day4_layer), GRect(day_x+6, y+6, bounds.size.w, day_text_height));                 
    layer_set_frame(text_layer_get_layer(s_day5_layer), GRect(day_x+7, y+8, bounds.size.w, day_text_height));      
    layer_set_frame(text_layer_get_layer(s_day6_layer), GRect(day_x+8, y+10, bounds.size.w, day_text_height)); 
}
static void set_time_y(int y)
{
    layer_set_frame(text_layer_get_layer(s_time1_layer), GRect(time_x, y, bounds.size.w, time_text_height));                 
    layer_set_frame(text_layer_get_layer(s_time2_layer), GRect(time_x+1, y-1, bounds.size.w, time_text_height));      
    layer_set_frame(text_layer_get_layer(s_time3_layer), GRect(time_x+4, y+2, bounds.size.w, time_text_height));  
}