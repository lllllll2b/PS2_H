#ifndef PS2_CAR_H
#define PS2_CAR_H

#include <PS2X_lib.h>  //for v1.6
//#define PS2_DAT        13  //14    
//#define PS2_CMD        11  //15
//#define PS2_SEL        10  //16
//#define PS2_CLK        12  //17
/******************************************************************
 * select modes of PS2 controller:
 *   - pressures = analog reading of push-butttons 
 *   - rumble    = motor rumbling
 * uncomment 1 of the lines for each mode selection
 ******************************************************************/
//#define pressures   true
#define pressures   false
//#define rumble      true
#define rumble      false


//right now, the library does NOT support hot pluggable controllers, meaning 
//you must always either restart your Arduino after you connect the controller, 
//or call config_gamepad(pins) again after connecting the controller.


class PS2{
  public:
      void init(int VCC,int PS2_DAT, int PS2_CMD, int PS2_SEL, int PS2_CLK);
      int isPair();
      int getRawButton(int Btn);
      int getX(int numble);
      int getY(int numble);
  };




#endif
