#include <pebble.h>
#include "callbacks.h"
#include "global.h"

static bool is_connected = true;

void bluetooth_callback(bool connected)
{
    if(persist_exists(BT_KEY))
      is_connected = persist_read_bool(BT_KEY);
  
  //show/hide bt icon
  layer_set_hidden(bitmap_layer_get_layer(layer_bluetooth),connected);
  
  if((is_connected && !connected) & settings.vibe_on_disconnect)
    vibes_double_pulse();
  
  is_connected=connected;
  //save current connection state to prevent vibes on every window load
  persist_write_bool(BT_KEY,is_connected); 
}