#include "PS2_Car.h"
PS2X ps2x; // create PS2 Controller Class
int error = 0;
byte type = 0;
byte vibrate = 0;
// Reset func
void (* resetFunc) (void) = 0;
void PS2::init(int VCC, int PS2_DAT, int PS2_CMD, int PS2_SEL, int PS2_CLK) {
  pinMode(VCC, OUTPUT);
  digitalWrite(VCC, HIGH);

  //  pinMode(GND, OUTPUT);
  //  digitalWrite(GND, LOW);
  Serial.begin(115200);

  delay(500);  //added delay to give wireless ps2 module some time to startup, before configuring it

  //CHANGES for v1.6 HERE!!! **************PAY ATTENTION*************

  //setup pins and settings: GamePad(clock, command, attention, data, Pressures?, Rumble?) check for error
  error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, pressures, rumble);

  if (error == 0) {
    Serial.print("Found Controller, configured successful ");
    Serial.print("pressures = ");
    if (pressures)
      Serial.println("true ");
    else
      Serial.println("false");
    Serial.print("rumble = ");
    if (rumble)
      Serial.println("true)");
    else
      Serial.println("false");
    Serial.println("Try out all the buttons, X will vibrate the controller, faster as you press harder;");
    Serial.println("holding L1 or R1 will print out the analog stick values.");
    Serial.println("Note: Go to www.billporter.info for updates and to report bugs.");
  }
  else if (error == 1)
    Serial.println("No controller found, check wiring, see readme.txt to enable debug. visit www.billporter.info for troubleshooting tips");

  else if (error == 2)
    Serial.println("Controller found but not accepting commands. see readme.txt to enable debug. Visit www.billporter.info for troubleshooting tips");

  else if (error == 3)
    Serial.println("Controller refusing to enter Pressures mode, may not support it. ");

  type = ps2x.readType();
  switch (type) {
    case 0:
      Serial.println("Unknown Controller type found ");
      break;
    case 1:
      Serial.println("DualShock Controller found ");
      break;
    case 2:
      Serial.println("GuitarHero Controller found ");
      break;
    case 3:
      Serial.println("Wireless Sony DualShock Controller found ");
      break;
  }
}
int PS2::isPair() {
  if (error == 1) { //skip loop if no controller found
    resetFunc();
    return 0;
  }
  return 1;
}
int  PS2::getRawButton(int Btn) {

  if (type != 2) { //DualShock Controller
    ps2x.read_gamepad(false, vibrate); //read controller and set large motor to spin at 'vibrate' speed

    if (ps2x.Button(PSB_START)) {       //will be TRUE as long as button is pressed
      Serial.println("Start is being held and Num = " + String(Btn));

      return 1;
    }
    if (ps2x.Button(PSB_SELECT)) {
      Serial.println("Select is being held");

    }
    if (ps2x.Button(PSB_PAD_UP) && Btn == 1) { //will be TRUE as long as button is pressed
      Serial.print("Up held this hard: ");
      Serial.println(ps2x.Analog(PSAB_PAD_UP), DEC);
      return 1;
    }
    if (ps2x.Button(PSB_PAD_RIGHT) && Btn == 4) {
      Serial.print("Right held this hard: ");
      Serial.println(ps2x.Analog(PSAB_PAD_RIGHT), DEC);
      return 1;
    }
    if (ps2x.Button(PSB_PAD_LEFT) && Btn == 2) {
      Serial.print("LEFT held this hard: ");
      Serial.println(ps2x.Analog(PSAB_PAD_LEFT), DEC);
      return 1;
    }
    if (ps2x.Button(PSB_PAD_DOWN) && Btn == 3) {
      Serial.print("DOWN held this hard: ");
      Serial.println(ps2x.Analog(PSAB_PAD_DOWN), DEC);
      return 1;
    }

    vibrate = ps2x.Analog(PSAB_CROSS);  //this will set the large motor vibrate speed based on how hard you press the blue (X) button
    if (ps2x.NewButtonState()) {        //will be TRUE if any button changes state (on to off, or off to on)
      if (ps2x.Button(PSB_L3))
        Serial.println("L3 pressed");
      if (ps2x.Button(PSB_R3))
        Serial.println("R3 pressed");
      if (ps2x.Button(PSB_L2))
        Serial.println("L2 pressed");
      if (ps2x.Button(PSB_R2))
        Serial.println("R2 pressed");
      //  if (ps2x.Button(PSB_TRIANGLE))
      //    Serial.println("Triangle pressed");
    }
    if (ps2x.Button(PSB_TRIANGLE) && Btn == 5) {
      Serial.println("Triangle pressed");
      return 1;
    }
    if (ps2x.ButtonPressed(PSB_CIRCLE) && Btn == 8) {         //will be TRUE if button was JUST pressed
      Serial.println("Circle just pressed");
      return 1;
    }
    if (ps2x.NewButtonState(PSB_CROSS) && Btn == 7) {         //will be TRUE if button was JUST pressed OR released
      Serial.println("X just changed");
      return 1;
    }
    if (ps2x.ButtonReleased(PSB_SQUARE) && Btn == 5) {        //will be TRUE if button was JUST released
      Serial.println("Square just released");
      return 1;
    }

    /* if (ps2x.Button(PSB_L1) || ps2x.Button(PSB_R1)) { //print stick values if either is TRUE
       Serial.print("Stick Values:");
       Serial.print(ps2x.Analog(PSS_LY), DEC); //Left stick, Y axis. Other options: LX, RY, RX
       Serial.print(",");
       Serial.print(ps2x.Analog(PSS_LX), DEC);
       Serial.print(",");
       Serial.print(ps2x.Analog(PSS_RY), DEC);
       Serial.print(",");
       Serial.println(ps2x.Analog(PSS_RX), DEC);
      }*/
  }
  delay(50);
  return 0;
}
int PS2::getX(int numble) {
  if (type != 2) { //DualShock Controller
    ps2x.read_gamepad(false, vibrate); //read controller and set large motor to spin at 'vibrate' speed
    if (numble == 0) {
      if (ps2x.Button(PSB_L1) || ps2x.Button(PSB_R1)) { //print stick values if either is TRUE
        /*   Serial.print("Stick Values:");
           Serial.print(ps2x.Analog(PSS_LX), DEC);
           Serial.print(",");*/
        return ps2x.Analog(PSS_LX) - 128;
      }
    } else {
      if (ps2x.Button(PSB_L1) || ps2x.Button(PSB_R1)) { //print stick values if either is TRUE
        Serial.print("Stick Values:");
        Serial.println(ps2x.Analog(PSS_RX), DEC);
        return ps2x.Analog(PSS_RX) - 128;
      }
    }
  }
  return 0;
}
int PS2::getY(int numble) {
  if (type != 2) { //DualShock Controller
    if (numble == 0) {
      if (ps2x.Button(PSB_L1) || ps2x.Button(PSB_R1)) { //print stick values if either is TRUE
        //  Serial.print("Stick Values:");
        //   Serial.print(ps2x.Analog(PSS_LY), DEC); //Left stick, Y axis. Other options: LX, RY, RX
        return ps2x.Analog(PSS_LY) - 128;
      }
    } else {
      if (ps2x.Button(PSB_L1) || ps2x.Button(PSB_R1)) { //print stick values if either is TRUE
        //  Serial.print("Stick Values:");
        //    Serial.print(ps2x.Analog(PSS_RY), DEC);
        return ps2x.Analog(PSS_RY) - 128;
      }
    }
  }
  return 0;

}
