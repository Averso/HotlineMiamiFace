#pragma once
#include <pebble.h>

//dimensions & positions

#if defined(PBL_PLATFORM_BASALT)
#define LAYER_DATE_Y 150
#define LAYER_WEEKDAY_Y_TOP 15
#define LAYER_TIME_Y_TOP 37
#define LAYER_WEEKDAY_Y_CENTER 45
#define LAYER_TIME_Y_CENTER 67
#endif

#if defined(PBL_PLATFORM_CHALK)
#define LAYER_DATE_Y 120
#define LAYER_WEEKDAY_Y_TOP 25
#define LAYER_TIME_Y_TOP 57
#define LAYER_WEEKDAY_Y_CENTER 55
#define LAYER_TIME_Y_CENTER 77
#endif

//layers dimensions
#define LAYER_DATE_X 0
#define LAYER_WEEKDAY_X 0
#define LAYER_TIME_X 0
#define LAYER_WEEKDAY_H 35
#define LAYER_DATE_H 18
#define LAYER_TIME_H 60



Window *main_window;                                    
TextLayer *layer_time[3],*layer_weekday[6],*layer_date;     
GFont font_time, font_date_lies,font_date_pixel, font_weekday; 
BitmapLayer *layer_background;                          
GBitmap *bitmap_background;     

GRect bounds;


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
ClaySettings settings;