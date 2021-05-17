// this is the working code for a model train system power supply circuit. 
// the power supply is managed by an arduino to control switching of voltages and display load currents and voltages
// the power supply arduino communicates with the main control system via wireless module to determine which voltages are needed


#include <Adafruit_INA219.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);   // 4x20 LCD display with I2C module at address 0x27

Adafruit_INA219 ina219_A(0x40);   // INA219_A current sensing module at address 0x40 (no bridging)
Adafruit_INA219 ina219_B(0x41);   // INA219_B current sensing module at address 0x41 (A0 solder bridge on module)
Adafruit_INA219 ina219_C(0x44);   // INA219_C current sensing module at address 0x44 (A1 solder bridge on module)


void setup() {
  
  ina219_A.begin();
  ina219_B.begin();
  ina219_C.begin();


  lcd.init();   // initialize the lcd
  lcd.clear();        // clears display
  lcd.backlight();    // open the backlight 
 
}


void loop() {

  // obtaining sensor A variables
  float busvoltage_A = 0;
  float current_mA_A = 0;
  busvoltage_A = ina219_A.getBusVoltage_V();
  current_mA_A = ina219_A.getCurrent_mA();

  // obtaining sensor B variables
  float busvoltage_B = 0;
  float current_mA_B = 0;
  busvoltage_B = ina219_B.getBusVoltage_V();
  current_mA_B = ina219_B.getCurrent_mA();

  // obtaining sensor C variables
  float busvoltage_C = 0;
  float current_mA_C = 0;
  busvoltage_C = ina219_C.getBusVoltage_V();
  current_mA_C = ina219_C.getCurrent_mA();


              // display line 0 - power wire labels
  lcd.setCursor(0,0);   
  lcd.print("    L1    L2    L3  ");

              // display line 1 - sensor current ratings
  lcd.setCursor(0,1);  
  lcd.print("mA");
  lcd.setCursor(3,1);
  lcd.print(current_mA_A);
  lcd.setCursor(9,1);
  lcd.print(current_mA_B);
  lcd.setCursor(15,1);
  lcd.print(current_mA_C);

              // display line 2 - voltage ratings
  lcd.setCursor(0,2);   
  lcd.print("V");
  lcd.setCursor(3,2);
  lcd.print(busvoltage_A);
  lcd.setCursor(9,2);
  lcd.print(busvoltage_B);
  lcd.setCursor(15,2);
  lcd.print(busvoltage_C);

              // display line 3 - on/off status of transistors in line
  lcd.setCursor(0,3);   

 
  delay(500);   // refresh rate of display values in ms

 

}