#pragma once
#include <pebble.h>

//dimensions & positions

#if defined(PBL_PLATFORM_BASALT)
#define LAYER_DATE_Y 150
#define LAYER_WEEKDAY_Y_TOP 15
#define LAYER_TIME_Y_TOP 37
#define LAYER_WEEKDAY_Y_CENTER 45
#define LAYER_TIME_Y_CENTER 67
#define LAYER_ICON_BT_X 7
#define LAYER_ICON_BT_Y 1
#define LAYER_ICON_QT_X 25
#define LAYER_ICON_QT_Y 1
#endif

#if defined(PBL_PLATFORM_CHALK)
#define LAYER_DATE_Y 120
#define LAYER_WEEKDAY_Y_TOP 25
#define LAYER_TIME_Y_TOP 57
#define LAYER_WEEKDAY_Y_CENTER 55
#define LAYER_TIME_Y_CENTER 77
#define LAYER_ICON_BT_X 74
#define LAYER_ICON_BT_Y 5
#define LAYER_ICON_QT_X 92
#define LAYER_ICON_QT_Y 5
#endif

//layers dimensions
#define LAYER_DATE_X 0
#define LAYER_WEEKDAY_X 0
#define LAYER_TIME_X 0
#define LAYER_WEEKDAY_H 35
#define LAYER_DATE_H 18
#define LAYER_TIME_H 60
#define LAYER_ICON_W 14
#define LAYER_ICON_H 14

#define TIME_LAYERS_SIZE 3
#define WDAY_LAYERS_SIZE 6

#define BT_KEY 12

Window *main_window;                                    
TextLayer *layer_time[TIME_LAYERS_SIZE],*layer_weekday[WDAY_LAYERS_SIZE],*layer_date;     
GFont font_time, font_date_lies,font_date_pixel, font_weekday; 
BitmapLayer *layer_background,*layer_bluetooth, *layer_quiettime;                          
GBitmap *bitmap_background, *bitmap_bluetooth, *bitmap_quiettime;     
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
  bool vibe_on_disconnect;
  Position position; 
  DateFont date_font;
} ClaySettings;

// An instance of the struct
ClaySettings settings;