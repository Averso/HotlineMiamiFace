#include <pebble.h>
#include "window.h"
#include "global.h"
#include "time.h"


void window_load(Window *window)
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
    GRect(LAYER_DATE_X,LAYER_DATE_Y, bounds.size.w,LAYER_DATE_H));
  
  //weekday 
  layer_weekday1 = text_layer_create(
    GRect(LAYER_WEEKDAY_X, LAYER_WEEKDAY_Y_TOP, bounds.size.w, LAYER_WEEKDAY_H));
  layer_weekday2 = text_layer_create(
    GRect(LAYER_WEEKDAY_X+2, LAYER_WEEKDAY_Y_TOP+2, bounds.size.w, LAYER_WEEKDAY_H));
  layer_weekday3 = text_layer_create(
    GRect(LAYER_WEEKDAY_X+5, LAYER_WEEKDAY_Y_TOP+4, bounds.size.w, LAYER_WEEKDAY_H));
  layer_weekday4 = text_layer_create(
    GRect(LAYER_WEEKDAY_X+6, LAYER_WEEKDAY_Y_TOP+6, bounds.size.w, LAYER_WEEKDAY_H));
  layer_weekday5 = text_layer_create(
    GRect(LAYER_WEEKDAY_X+7, LAYER_WEEKDAY_Y_TOP+8, bounds.size.w, LAYER_WEEKDAY_H));
  layer_weekday6 = text_layer_create(
    GRect(LAYER_WEEKDAY_X+8, LAYER_WEEKDAY_Y_TOP+10, bounds.size.w, LAYER_WEEKDAY_H));
  
  
  layer_time1 = text_layer_create(
    GRect(LAYER_TIME_X, LAYER_TIME_Y_TOP, bounds.size.w, LAYER_TIME_H));
  layer_time2 = text_layer_create(
    GRect(LAYER_TIME_X+1, LAYER_TIME_Y_TOP-1, bounds.size.w, LAYER_TIME_H));  
  layer_time3 = text_layer_create(
    GRect(LAYER_TIME_X+4, LAYER_TIME_Y_TOP+2, bounds.size.w, LAYER_TIME_H));  
 
  
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
  window_update();
}

void window_unload(Window *window)
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

void window_update()
{ 
  //if background image is enabled - show image
  //we change visibility mode of bitmap layer
  if(settings.background_enabled)
  {
     //bitmap_layer_set_compositing_mode(layer_background, GCompOpAssign); 
    layer_set_hidden((Layer *)layer_background, false);
  }
  else
  {
    //set window color based on settings
    window_set_background_color(main_window, settings.background_color);
    //set bitmap transparent
   // bitmap_layer_set_compositing_mode(layer_background, GCompOpClear);  
    layer_set_hidden((Layer *)layer_background,true);
    
  }
  
  //date color
  text_layer_set_text_color(layer_date,  settings.date_color);
  
  //position of clock
  if(settings.position == TOP)
  {
    set_weekday_y(LAYER_WEEKDAY_Y_TOP);
    set_time_y(LAYER_TIME_Y_TOP);   
  }
  else
  {
    set_weekday_y(LAYER_WEEKDAY_Y_CENTER);
    set_time_y(LAYER_TIME_Y_CENTER);          
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

void set_up_text_layer(TextLayer *layer, GColor background, GColor text_color, const char * text,GFont font,GTextAlignment alignment)
{
  text_layer_set_background_color(layer, background);
  text_layer_set_text_color(layer, text_color);
  text_layer_set_text(layer, text);
  text_layer_set_font(layer, font);
  text_layer_set_text_alignment(layer,alignment); 
}

void set_weekday_y(int y)
{
    layer_set_frame(text_layer_get_layer(layer_weekday1),
                    GRect(LAYER_WEEKDAY_X, y, bounds.size.w, LAYER_WEEKDAY_H));
    layer_set_frame(text_layer_get_layer(layer_weekday2),
                    GRect(LAYER_WEEKDAY_X+2, y+2, bounds.size.w, LAYER_WEEKDAY_H));
    layer_set_frame(text_layer_get_layer(layer_weekday3),
                    GRect(LAYER_WEEKDAY_X+5, y+4, bounds.size.w, LAYER_WEEKDAY_H));                 
    layer_set_frame(text_layer_get_layer(layer_weekday4),
                    GRect(LAYER_WEEKDAY_X+6, y+6, bounds.size.w, LAYER_WEEKDAY_H));                 
    layer_set_frame(text_layer_get_layer(layer_weekday5),
                    GRect(LAYER_WEEKDAY_X+7, y+8, bounds.size.w, LAYER_WEEKDAY_H));      
    layer_set_frame(text_layer_get_layer(layer_weekday6),
                    GRect(LAYER_WEEKDAY_X+8, y+10, bounds.size.w, LAYER_WEEKDAY_H)); 
}

void set_time_y(int y)
{
    layer_set_frame(text_layer_get_layer(layer_time1),
                    GRect(LAYER_TIME_X, y, bounds.size.w, LAYER_TIME_H));                 
    layer_set_frame(text_layer_get_layer(layer_time2),
                    GRect(LAYER_TIME_X+1, y-1, bounds.size.w, LAYER_TIME_H));      
    layer_set_frame(text_layer_get_layer(layer_time3),
                    GRect(LAYER_TIME_X+4, y+2, bounds.size.w, LAYER_TIME_H));  
}