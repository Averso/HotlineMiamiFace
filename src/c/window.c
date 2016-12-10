#include <pebble.h>
#include "window.h"
#include "global.h"
#include "time.h"
#include "callbacks.h"

void window_load(Window *window)
{
  
  //get window layer and bounds
  Layer *window_layer = window_get_root_layer(window);
  bounds = layer_get_bounds(window_layer);
  
  window_set_background_color(window, settings.background_color);
  
  load_resources();  
  create_bitmap_layers(bounds);
  create_text_layers();    
  //create bluetooth&quiettime layers
  

  //add layers    
  add_layers_to_window(window_layer);
  
   //turn on transparency
  bitmap_layer_set_compositing_mode(layer_bluetooth, GCompOpSet); 
  bitmap_layer_set_compositing_mode(layer_quiettime, GCompOpSet);   
  
  
  //hide icons
  layer_set_hidden(bitmap_layer_get_layer(layer_bluetooth),true);
  layer_set_hidden(bitmap_layer_get_layer(layer_quiettime),true);
  
  //call to update window elements depending on settings
  window_update();
}

void window_unload(Window *window)
{
  //destroy text layers
  for(int i=0;i<TIME_LAYERS_SIZE;i++)
   text_layer_destroy(layer_time[i]);
  for(int i=0;i<WDAY_LAYERS_SIZE;i++)    
    text_layer_destroy(layer_weekday[i]);
  text_layer_destroy(layer_date);
  
  
  //unload GFonts
  fonts_unload_custom_font(font_time);
  fonts_unload_custom_font(font_weekday);
  fonts_unload_custom_font(font_date_lies);
  fonts_unload_custom_font(font_date_pixel);
  
  
  //destroy gbitmap
  gbitmap_destroy(bitmap_background);
  bitmap_layer_destroy(layer_background);
  gbitmap_destroy(bitmap_bluetooth);
  bitmap_layer_destroy(layer_bluetooth);
  gbitmap_destroy(bitmap_quiettime);
  bitmap_layer_destroy(layer_quiettime);
}

void window_update()
{ 
  //check bt status
  bluetooth_callback(connection_service_peek_pebble_app_connection());  
    
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

void load_resources()
{
  //FONTS
  font_time = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_ROBOTO_40));
  font_weekday = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_DISCODUCK_CON_30));  
  font_date_lies = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_TRUELIES_13));
  font_date_pixel = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_PIXELMIX_11));
  
  //BITMAPS
  #if defined(PBL_PLATFORM_BASALT)
  bitmap_background = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BACKGROUND_PT);
  #endif
  #if defined(PBL_PLATFORM_CHALK)
  bitmap_background = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BACKGROUND_PTR);
  #endif
  
  bitmap_bluetooth = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_ICON_BT);
  bitmap_quiettime = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_ICON_QT);
  
}

void create_bitmap_layers(GRect bounds)
{  
  layer_background = bitmap_layer_create(bounds);   
  bitmap_layer_set_bitmap(layer_background,bitmap_background); 
  
  layer_bluetooth = bitmap_layer_create(GRect(LAYER_ICON_BT_X,LAYER_ICON_BT_Y, LAYER_ICON_W,LAYER_ICON_H));
  bitmap_layer_set_bitmap(layer_bluetooth,bitmap_bluetooth); 
  
  layer_quiettime = bitmap_layer_create(GRect(LAYER_ICON_QT_X,LAYER_ICON_QT_Y, LAYER_ICON_W,LAYER_ICON_H));
  bitmap_layer_set_bitmap(layer_quiettime,bitmap_quiettime); 
}

void create_text_layers()
{
    layer_date = text_layer_create(
    GRect(LAYER_DATE_X,LAYER_DATE_Y, bounds.size.w,LAYER_DATE_H));
  
  //weekday 
  layer_weekday[0] = text_layer_create(
    GRect(LAYER_WEEKDAY_X, LAYER_WEEKDAY_Y_TOP, bounds.size.w, LAYER_WEEKDAY_H));
  layer_weekday[1] = text_layer_create(
    GRect(LAYER_WEEKDAY_X+2, LAYER_WEEKDAY_Y_TOP+2, bounds.size.w, LAYER_WEEKDAY_H));
  layer_weekday[2] = text_layer_create(
    GRect(LAYER_WEEKDAY_X+5, LAYER_WEEKDAY_Y_TOP+4, bounds.size.w, LAYER_WEEKDAY_H));
  layer_weekday[3] = text_layer_create(
    GRect(LAYER_WEEKDAY_X+6, LAYER_WEEKDAY_Y_TOP+6, bounds.size.w, LAYER_WEEKDAY_H));
  layer_weekday[4] = text_layer_create(
    GRect(LAYER_WEEKDAY_X+7, LAYER_WEEKDAY_Y_TOP+8, bounds.size.w, LAYER_WEEKDAY_H));
  layer_weekday[5] = text_layer_create(
    GRect(LAYER_WEEKDAY_X+8, LAYER_WEEKDAY_Y_TOP+10, bounds.size.w, LAYER_WEEKDAY_H));
  
  
  layer_time[0] = text_layer_create(
    GRect(LAYER_TIME_X, LAYER_TIME_Y_TOP, bounds.size.w, LAYER_TIME_H));
  layer_time[1] = text_layer_create(
    GRect(LAYER_TIME_X+1, LAYER_TIME_Y_TOP-1, bounds.size.w, LAYER_TIME_H));  
  layer_time[2] = text_layer_create(
    GRect(LAYER_TIME_X+4, LAYER_TIME_Y_TOP+2, bounds.size.w, LAYER_TIME_H));  
  
    //set up time layers
  set_up_text_layer(layer_time[0], GColorClear, GColorElectricBlue, "00:00", font_time,GTextAlignmentCenter);
  set_up_text_layer(layer_time[1], GColorClear, GColorYellow, "00:00", font_time,GTextAlignmentCenter);
  set_up_text_layer(layer_time[2], GColorClear, GColorOxfordBlue, "00:00", font_time,GTextAlignmentCenter);
 
  //set up weekday layers
  set_up_text_layer(layer_weekday[0], GColorClear, GColorFashionMagenta, "wednesday", font_weekday,GTextAlignmentCenter);
  set_up_text_layer(layer_weekday[1], GColorClear, GColorBlack, "wednesday", font_weekday,GTextAlignmentCenter);
  set_up_text_layer(layer_weekday[2], GColorClear, GColorImperialPurple, "wednesday", font_weekday,GTextAlignmentCenter);
  set_up_text_layer(layer_weekday[3], GColorClear, GColorOxfordBlue, "wednesday", font_weekday,GTextAlignmentCenter);
  set_up_text_layer(layer_weekday[4], GColorClear, GColorOrange, "wednesday", font_weekday,GTextAlignmentCenter);
  set_up_text_layer(layer_weekday[5], GColorClear, GColorChromeYellow, "wednesday", font_weekday,GTextAlignmentCenter);
  
  //set up date layer
  set_up_text_layer(layer_date, GColorClear, GColorTiffanyBlue, "September 27, 1989", font_date_lies,GTextAlignmentCenter);
  
   
}

void add_layers_to_window(Layer *window_layer)
{
   layer_add_child(window_layer,bitmap_layer_get_layer(layer_background));
   for(int i=WDAY_LAYERS_SIZE-1;i>=0;i--)   
      layer_add_child(window_layer,text_layer_get_layer(layer_weekday[i]));          //days
      
   for(int i=TIME_LAYERS_SIZE-1;i>=0;i--)   
      layer_add_child(window_layer,text_layer_get_layer(layer_time[i]));       
    
  
  layer_add_child(window_layer,text_layer_get_layer(layer_date));         //date
  
  layer_add_child(window_layer,bitmap_layer_get_layer(layer_bluetooth));
  layer_add_child(window_layer,bitmap_layer_get_layer(layer_quiettime));
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
    layer_set_frame(text_layer_get_layer(layer_weekday[0]),
                    GRect(LAYER_WEEKDAY_X, y, bounds.size.w, LAYER_WEEKDAY_H));
    layer_set_frame(text_layer_get_layer(layer_weekday[1]),
                    GRect(LAYER_WEEKDAY_X+2, y+2, bounds.size.w, LAYER_WEEKDAY_H));
    layer_set_frame(text_layer_get_layer(layer_weekday[2]),
                    GRect(LAYER_WEEKDAY_X+5, y+4, bounds.size.w, LAYER_WEEKDAY_H));                 
    layer_set_frame(text_layer_get_layer(layer_weekday[3]),
                    GRect(LAYER_WEEKDAY_X+6, y+6, bounds.size.w, LAYER_WEEKDAY_H));                 
    layer_set_frame(text_layer_get_layer(layer_weekday[4]),
                    GRect(LAYER_WEEKDAY_X+7, y+8, bounds.size.w, LAYER_WEEKDAY_H));      
    layer_set_frame(text_layer_get_layer(layer_weekday[5]),
                    GRect(LAYER_WEEKDAY_X+8, y+10, bounds.size.w, LAYER_WEEKDAY_H)); 
}

void set_time_y(int y)
{
    layer_set_frame(text_layer_get_layer(layer_time[0]),
                    GRect(LAYER_TIME_X, y, bounds.size.w, LAYER_TIME_H));                 
    layer_set_frame(text_layer_get_layer(layer_time[1]),
                    GRect(LAYER_TIME_X+1, y-1, bounds.size.w, LAYER_TIME_H));      
    layer_set_frame(text_layer_get_layer(layer_time[2]),
                    GRect(LAYER_TIME_X+4, y+2, bounds.size.w, LAYER_TIME_H));  
}