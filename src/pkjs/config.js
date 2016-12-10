module.exports = [
  {
    "type": "heading",
    "defaultValue": "Settings"
  },
  {
    "type": "section",
    "items": [
      {
        "type": "heading",
        "defaultValue": "Background"
      },
      {
        "type": "toggle",
        "messageKey": "enable_background",
        "label": "Enable background",
        "defaultValue": true,
        "description": "Enable or disable image background.\nIf disabled, you can pick background color."

      },
      {
        "type": "color",
        "messageKey": "background_color",
        "defaultValue": "0xFFFFFF",
        "label": "Background Color"
        
      }
    ]
  },
  {
    "type": "section",
    "items": [
      {
        "type": "heading",
        "defaultValue": "Clock Settings"
      },
      {
      "type": "radiogroup",
      "messageKey": "position",
      "label": "Date Position",
      "defaultValue":"top",
      "options": [
        { 
          "label": "Top", 
          "value": "top" 
        },
        { 
          "label": "Center", 
          "value": "center" 
        }
      ]
      },
      {
        "type": "toggle",
        "messageKey": "show_date",
        "label": "Show date",
        "defaultValue": true
      },
      {
      "type": "radiogroup",
      "messageKey": "date_font",
      "label": "Date Font",
      "defaultValue":"lies",
      "options": [
        { 
          "label": "Pixel Mix", 
          "value": "pixel" 
        },
        { 
          "label": "True Lies", 
          "value": "lies" 
        }
      ]
     },
     {
        "type": "color",
        "messageKey": "date_color",
        "defaultValue": "0x00AAAA",
        "label": "Date color"        
     }
    ]
  },
  {
    "type": "section",      
    "items": [
      {
      "type": "heading",
      "defaultValue": "Others"
      },
      {
        "type": "toggle",
        "messageKey": "vibe_disconnect",
        "label": "Vibrate on disconnect",
        "defaultValue": false
      }
    ]
  },
  {
    "type": "submit",
    "defaultValue": "Save Settings"
  }
];