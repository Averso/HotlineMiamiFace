#pragma once
#include <pebble.h>

//dimensions & positions

#define LAYER_DATE_X 0
#define LAYER_WEEKDAY_X 0
#define LAYER_TIME_X 0

// y position of date depending on platform
#if defined(PBL_PLATFORM_BASALT)
#define LAYER_DATE_Y 150
#endif
#if defined(PBL_PLATFORM_CHALK)
#define LAYER_DATE_Y 120
#endif 
#if defined(PBL_PLATFORM_EMERY)
#define LAYER_DATE_Y 205
#endif 

//positions of clock,weekday layers
#if defined(PBL_PLATFORM_BASALT)
#define LAYER_WEEKDAY_Y_TOP 15
#define LAYER_TIME_Y_TOP 37
#define LAYER_WEEKDAY_Y_CENTER 45
#define LAYER_TIME_Y_CENTER 67
#endif
#if defined(PBL_PLATFORM_CHALK)
#define LAYER_WEEKDAY_Y_TOP 25
#define LAYER_TIME_Y_TOP 57
#define LAYER_WEEKDAY_Y_CENTER 55
#define LAYER_TIME_Y_CENTER 77
#endif 
#if defined(PBL_PLATFORM_EMERY)
#define LAYER_WEEKDAY_Y_TOP 25
#define LAYER_TIME_Y_TOP 57
#define LAYER_WEEKDAY_Y_CENTER 55
#define LAYER_TIME_Y_CENTER 87
#endif 

//layers dimensions
#if defined(PBL_PLATFORM_BASALT) || (PBL_PLATFORM_CHALK)
#define LAYER_WEEKDAY_H 35
#define LAYER_DATE_H 18
#define LAYER_TIME_H 60
#endif 
#if defined(PBL_PLATFORM_EMERY)
#define LAYER_WEEKDAY_H 50
#define LAYER_DATE_H 30
#define LAYER_TIME_H 80
#endif 


Window *main_window;                                    
TextLayer *layer_time1, *layer_time2,*layer_time3,*layer_weekday1,
                *layer_weekday2,*layer_weekday3,*layer_weekday4,
                *layer_weekday5,*layer_weekday6,*layer_date;     
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