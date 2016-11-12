#include <pebble.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
// Persistent storage key
#define SETTINGS_KEY 1

//globals
static Window *main_window;                                    
static TextLayer *layer_time1, *layer_time2,*layer_time3,*layer_weekday1,
                *layer_weekday2,*layer_weekday3,*layer_weekday4,
                *layer_weekday5,*layer_weekday6,*layer_date;     
static GFont font_time, font_date_lies,font_date_pixel, font_weekday; 
static BitmapLayer *layer_background;                          
static GBitmap *bitmap_background;     


//positions of layers 
static int layer_date_x = 0;
static int layer_weekday_x = 0;
static int layer_time_x = 0;

// y position of date depending on platform
#if defined(PBL_PLATFORM_BASALT)
static int layer_date_y = 150;
#endif
#if defined(PBL_PLATFORM_CHALK)
static int layer_date_y = 120;
#endif 
#if defined(PBL_PLATFORM_EMERY)
static int layer_date_y = 205;
#endif 

//positions of clock,weekday layers
#if defined(PBL_PLATFORM_BASALT)
static int layer_weekday_y_top = 15;
static int layer_time_y_top = 37;
static int layer_weekday_y_center = 45;
static int layer_time_y_center = 67;
#endif
#if defined(PBL_PLATFORM_CHALK)
static int layer_weekday_y_top = 25;
static int layer_time_y_top = 57;
static int layer_weekday_y_center = 55;
static int layer_time_y_center = 77;
#endif 
#if defined(PBL_PLATFORM_EMERY)
static int layer_weekday_y_top = 25;
static int layer_time_y_top = 57;
static int layer_weekday_y_center = 55;
static int layer_time_y_center = 87;
#endif 

//layers dimensions
#if defined(PBL_PLATFORM_BASALT) || (PBL_PLATFORM_CHALK)
static int layer_weekday_height = 35;
static int layer_date_height = 18;
static int layer_time_height = 60;
#endif 
#if defined(PBL_PLATFORM_EMERY)
static int layer_weekday_height = 50;
static int layer_date_height = 30;
static int layer_time_height = 80;
#endif 

static  GRect bounds;


//functions
static void main_window_load(Window *window);
static void main_window_unload(Window *window);
static void init();   //called on start
static void unload(); //called on exit
static void tick_handler(struct tm *tick_time, TimeUnits units_changed); //handle ticks
static void update_time();  //handle fetching time/date data
static void update_weekday();
static void update_date();
static void set_up_text_layer(TextLayer *layer, GColor background, GColor text_color, 
const char * text,GFont font,GTextAlignment alignment); //set up text layer with givent parameters
static void inbox_received_handler(DictionaryIterator *iter, void *context);
static void load_default_settings();
static void save_settings();
static void load_settings();
static void set_weekday_y(int y);
static void set_time_y(int y);
static void main_window_update();


//enums for settings
typedef enum {PIXEL, LIES} DateFont;
typedef enum {TOP, CENTER} Position;

//settings struct
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


//MAIN

int main(void)
{
  init();
  app_event_loop();
  unload();  
}


//INIT
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
        .load = main_window_load,
        .unload = main_window_unload
 });
  
  //add window
  window_stack_push(main_window, true);
    
  //set ticktimeservice
  tick_timer_service_subscribe(MINUTE_UNIT | DAY_UNIT, tick_handler);
  
  // Make sure the time is displayed from the start
  update_time();
  update_weekday();
  if(settings.date_enabled) //update date if enabled in settings
    update_date();
}

//EXIT
static void unload()
{
  //destroy window
  if (main_window) {
    window_destroy(main_window);
 }
}

static void main_window_load(Window *window)
{
  
  //get window layer and bounds
  Layer *window_layer = window_get_root_layer(window);
  bounds = layer_get_bounds(window_layer);
  
  //FONTS  
  
  //load fonts depending on platform
  #if defined(PBL_PLATFORM_BASALT) || (PBL_PLATFORM_CHALK)
  font_time = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_ROBOTO_40));
  font_weekday = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_DISCODUCK_CON_30));  
  font_date_lies = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_TRUELIES_13));
  font_date_pixel = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_PIXELMIX_11));
  #endif
  #if defined(PBL_PLATFORM_EMERY)
  font_time = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_ROBOTO_50));
  font_weekday = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_DISCODUCK_CON_40));  
  font_date_lies = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_TRUELIES_17));
  font_date_pixel = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_PIXELMIX_16));
  #endif 
  
  
  //BACKGROUND
  
  //load bitmap depending on platform
  #if defined(PBL_PLATFORM_BASALT)
  bitmap_background = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BACKGROUND_PT);
  #endif
  #if defined(PBL_PLATFORM_CHALK)
  bitmap_background = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BACKGROUND_PTR);
  #endif
  #if defined(PBL_PLATFORM_EMERY)
  bitmap_background = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BACKGROUND_PT2);
  #endif
  
  //create bg layer and set window bg color
  layer_background = bitmap_layer_create(bounds);
  window_set_background_color(window, settings.background_color);
    
  
  //TEXT
  
  //create text layers 
  //date
  layer_date = text_layer_create(
    GRect(layer_date_x, layer_date_y, bounds.size.w, layer_date_height));
  
  //weekday 
  layer_weekday1 = text_layer_create(
    GRect(layer_weekday_x, layer_weekday_y_top, bounds.size.w, layer_weekday_height));
  layer_weekday2 = text_layer_create(
    GRect(layer_weekday_x+2, layer_weekday_y_top+2, bounds.size.w, layer_weekday_height));
  layer_weekday3 = text_layer_create(
    GRect(layer_weekday_x+5, layer_weekday_y_top+4, bounds.size.w, layer_weekday_height));
  layer_weekday4 = text_layer_create(
    GRect(layer_weekday_x+6, layer_weekday_y_top+6, bounds.size.w, layer_weekday_height));
  layer_weekday5 = text_layer_create(
    GRect(layer_weekday_x+7, layer_weekday_y_top+8, bounds.size.w, layer_weekday_height));
  layer_weekday6 = text_layer_create(
    GRect(layer_weekday_x+8, layer_weekday_y_top+10, bounds.size.w, layer_weekday_height));
  
  
  layer_time1 = text_layer_create(
    GRect(layer_time_x, layer_time_y_top, bounds.size.w, layer_time_height));
  layer_time2 = text_layer_create(
    GRect(layer_time_x+1, layer_time_y_top-1, bounds.size.w, layer_time_height));  
  layer_time3 = text_layer_create(
    GRect(layer_time_x+4, layer_time_y_top+2, bounds.size.w, layer_time_height));  
 
  
  //SET UP LAYERS  
  //set bitmap img to bitmap layer
  bitmap_layer_set_bitmap(layer_background,bitmap_background);    
    
  //set up time layers
  set_up_text_layer(layer_time1, GColorClear, GColorElectricBlue, "00:00", font_time,GTextAlignmentCenter);
  set_up_text_layer(layer_time2, GColorClear, GColorYellow, "00:00", font_time,GTextAlignmentCenter);
  set_up_text_layer(layer_time3, GColorClear, GColorOxfordBlue, "00:00", font_time,GTextAlignmentCenter);
 
  //set up weekday layers
  set_up_text_layer(layer_weekday1, GColorClear, GColorFashionMagenta, "wednesday", font_weekday,GTextAlignmentCenter);
  set_up_text_layer(layer_weekday2, GColorClear, GColorBlack, "wednesday", font_weekday,GTextAlignmentCenter);
  set_up_text_layer(layer_weekday3, GColorClear, GColorImperialPurple, "wednesday", font_weekday,GTextAlignmentCenter);
  set_up_text_layer(layer_weekday4, GColorClear, GColorOxfordBlue, "wednesday", font_weekday,GTextAlignmentCenter);
  set_up_text_layer(layer_weekday5, GColorClear, GColorOrange, "wednesday", font_weekday,GTextAlignmentCenter);
  set_up_text_layer(layer_weekday6, GColorClear, GColorChromeYellow, "wednesday", font_weekday,GTextAlignmentCenter);
  
  //set up date layer
  set_up_text_layer(layer_date, GColorClear, GColorTiffanyBlue, "September 27, 1989", font_date_lies,GTextAlignmentCenter);
  
  
  //add layers    
  layer_add_child(window_layer,bitmap_layer_get_layer(layer_background));
  layer_add_child(window_layer,text_layer_get_layer(layer_weekday6));          //days
  layer_add_child(window_layer,text_layer_get_layer(layer_weekday5));  
  layer_add_child(window_layer,text_layer_get_layer(layer_weekday4));          
  layer_add_child(window_layer,text_layer_get_layer(layer_weekday3));          
  layer_add_child(window_layer,text_layer_get_layer(layer_weekday2)); 
  layer_add_child(window_layer,text_layer_get_layer(layer_weekday1)); 
  layer_add_child(window_layer,text_layer_get_layer(layer_time3));  
  layer_add_child(window_layer,text_layer_get_layer(layer_time2));          //time
  layer_add_child(window_layer,text_layer_get_layer(layer_time1));          
  layer_add_child(window_layer,text_layer_get_layer(layer_date));         //date
  
  //call to update window elements depending on settings
  main_window_update();
}

static void main_window_unload(Window *window)
{
  //destroy text layers
  text_layer_destroy(layer_time1);
  text_layer_destroy(layer_time2);
  text_layer_destroy(layer_time3);
  text_layer_destroy(layer_weekday1);
  text_layer_destroy(layer_weekday2);
  text_layer_destroy(layer_weekday3);
  text_layer_destroy(layer_weekday4);
  text_layer_destroy(layer_weekday5);
  text_layer_destroy(layer_weekday6);
  text_layer_destroy(layer_date);
  
  
  //unload GFonts
  fonts_unload_custom_font(font_time);
  fonts_unload_custom_font(font_weekday);
  fonts_unload_custom_font(font_date_lies);
  fonts_unload_custom_font(font_date_pixel);
  
  
  //destroy gbitmap
  gbitmap_destroy(bitmap_background);
  bitmap_layer_destroy(layer_background);
}

static void main_window_update()
{ 
  //if background image is enabled - show image
  //we change visibility mode of bitmap layer
  if(settings.background_enabled)
  {
     bitmap_layer_set_compositing_mode(layer_background, GCompOpAssign); 
  }
  else
  {
    //set window color based on settings
    window_set_background_color(main_window, settings.background_color);
    //set bitmap transparent
    bitmap_layer_set_compositing_mode(layer_background, GCompOpClear);    
    
  }
  
  //date color
  text_layer_set_text_color(layer_date,  settings.date_color);
  
  //position of clock
  if(settings.position == TOP)
  {
    set_weekday_y(layer_weekday_y_top);
    set_time_y(layer_time_y_top);   
  }
  else
  {
    set_weekday_y(layer_weekday_y_center);
    set_time_y(layer_time_y_center);          
  }
  
  //show date
  if(settings.date_enabled)
  {
    update_date();
  }
  else
  {
    //set layer to show empty string
    text_layer_set_text(layer_date,"");          
  }  
  
  //date font  
  if(settings.date_font == PIXEL)
  {
    text_layer_set_font(layer_date, font_date_pixel);
  }
  else
  {
    text_layer_set_font(layer_date, font_date_lies);
          
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
    update_weekday();
    
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
  text_layer_set_text(layer_time1, s_buffer);
  text_layer_set_text(layer_time2, s_buffer);
  text_layer_set_text(layer_time3, s_buffer);
}

static void update_weekday() 
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
  text_layer_set_text(layer_weekday1, s_buffer);
  text_layer_set_text(layer_weekday2, s_buffer);
  text_layer_set_text(layer_weekday3, s_buffer);
  text_layer_set_text(layer_weekday4, s_buffer);
  text_layer_set_text(layer_weekday5, s_buffer);
  text_layer_set_text(layer_weekday6, s_buffer);
  
  
}

static void update_date() 
{
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);
  
  static char s_buffer[18];
  strftime(s_buffer, sizeof(s_buffer), "%B %d, %G", tick_time);
  
  text_layer_set_text(layer_date, s_buffer);
}

static void set_up_text_layer(TextLayer *layer, GColor background, GColor text_color, const char * text,GFont font,GTextAlignment alignment)
{
  text_layer_set_background_color(layer, background);
  text_layer_set_text_color(layer, text_color);
  text_layer_set_text(layer, text);
  text_layer_set_font(layer, font);
  text_layer_set_text_alignment(layer,alignment); 
}

static void inbox_received_handler(DictionaryIterator *iter, void *context) {
  
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
 
 //save loaded settings
 save_settings();
}

static void save_settings() {
  persist_write_data(SETTINGS_KEY, &settings, sizeof(settings));
  
  //refresh window
  main_window_update();
}

// Read settings from persistent storage
static void load_settings() {
  // Load the default settings
  load_default_settings();
  // Read settings from persistent storage, if they exist
  persist_read_data(SETTINGS_KEY, &settings, sizeof(settings));
}

static void load_default_settings() {
  settings.background_enabled = true;
  settings.background_color = GColorWhite;
  settings.date_color = GColorTiffanyBlue;
  settings.date_enabled = true;
  settings.position=TOP;
  settings.date_font=LIES;
}

static void set_weekday_y(int y)
{
    layer_set_frame(text_layer_get_layer(layer_weekday1),
                    GRect(layer_weekday_x, y, bounds.size.w, layer_weekday_height));
    layer_set_frame(text_layer_get_layer(layer_weekday2),
                    GRect(layer_weekday_x+2, y+2, bounds.size.w, layer_weekday_height));
    layer_set_frame(text_layer_get_layer(layer_weekday3),
                    GRect(layer_weekday_x+5, y+4, bounds.size.w, layer_weekday_height));                 
    layer_set_frame(text_layer_get_layer(layer_weekday4),
                    GRect(layer_weekday_x+6, y+6, bounds.size.w, layer_weekday_height));                 
    layer_set_frame(text_layer_get_layer(layer_weekday5),
                    GRect(layer_weekday_x+7, y+8, bounds.size.w, layer_weekday_height));      
    layer_set_frame(text_layer_get_layer(layer_weekday6),
                    GRect(layer_weekday_x+8, y+10, bounds.size.w, layer_weekday_height)); 
}
static void set_time_y(int y)
{
    layer_set_frame(text_layer_get_layer(layer_time1),
                    GRect(layer_time_x, y, bounds.size.w, layer_time_height));                 
    layer_set_frame(text_layer_get_layer(layer_time2),
                    GRect(layer_time_x+1, y-1, bounds.size.w, layer_time_height));      
    layer_set_frame(text_layer_get_layer(layer_time3),
                    GRect(layer_time_x+4, y+2, bounds.size.w, layer_time_height));  
}