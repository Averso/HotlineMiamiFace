#pragma once
#include <pebble.h>

void window_load(Window *window);
void window_unload(Window *window);
void window_update();
void set_up_text_layer(TextLayer *layer, GColor background, GColor text_color, 
const char * text,GFont font,GTextAlignment alignment); //set up text layer with givent parameters
void set_weekday_y(int y);
void set_time_y(int y);