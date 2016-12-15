module.exports = function() {
  var clayConfig = this;
  

  function toggleBackground()
  { 
    if (this.get()) {
      clayConfig.getItemByMessageKey('background_color').disable();
    } else {
      clayConfig.getItemByMessageKey('background_color').enable();
    }     
  }
  
  function toggleDateFont()
  { 
    if (this.get()) {
      clayConfig.getItemByMessageKey('date_font').enable();
    } else {
      clayConfig.getItemByMessageKey('date_font').disable();
    }
     
  } 
  

  
  //after page loaded
  clayConfig.on(clayConfig.EVENTS.AFTER_BUILD, function() {
    
    //background settings  - set to disable color pick when background image is enabled
    var enableBackgroundToggle = clayConfig.getItemByMessageKey('enable_background');
    toggleBackground.call(enableBackgroundToggle);
    enableBackgroundToggle.on('change', toggleBackground);
    
    //disable date font radiogroup when date is disabled
    var dateToggle = clayConfig.getItemByMessageKey('show_date');
    toggleDateFont.call(dateToggle);
    dateToggle.on('change', toggleDateFont);
   
  });
  

};