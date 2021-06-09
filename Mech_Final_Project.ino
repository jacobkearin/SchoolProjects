// this is the working code for a model train system power supply circuit. 
// the power supply is managed by an arduino to control switching of voltages and display load currents and voltages
// the power supply arduino communicates with the main control system via wireless module to determine which voltages are needed

#include <Adafruit_INA219.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
//#include <SPI.h>
//#include <RH_NRF24.h> 

LiquidCrystal_I2C lcd(0x27, 20, 4);   // 4x20 LCD display with I2C module at address 0x27

Adafruit_INA219 ina219_A(0x40);   // INA219_A current sensing module at address 0x40 (no bridging)
Adafruit_INA219 ina219_B(0x41);   // INA219_B current sensing module at address 0x41 (A0 solder bridge on module)
Adafruit_INA219 ina219_C(0x44);   // INA219_C current sensing module at address 0x44 (A1 solder bridge on module)

//RH_NRF24 nrf24;

#define L1 2
#define L2 3
#define L3 4
#define swL1 23
#define swL2 25
#define swL3 27

void L1high(){
  digitalWrite(L1, HIGH);
}

void L2high(){
  digitalWrite(L2, HIGH);
}

void L3high(){
  digitalWrite(L3, HIGH);
}

void L1low(){
  digitalWrite(L1, LOW);
}

void L2low(){
  digitalWrite(L2, LOW);
}

void L3low(){
  digitalWrite(L3, LOW);
}

void setup() {

  Serial.begin(9600);
//  while (!Serial);
//  // wait for serial port to connect. Needed for Leonardo only
//  if (!nrf24.init())
//    Serial.println("init failed");
//  // Defaults after init are 2.402 GHz (channel 2), 2Mbps, 0dBm
//  if (!nrf24.setChannel(1))
//    Serial.println("setChannel failed");
//  if (!nrf24.setRF(RH_NRF24::DataRate2Mbps, RH_NRF24::TransmitPower0dBm))
//    Serial.println("setRF failed");   

//  pinMode(L1, OUTPUT);
//  pinMode(L2, OUTPUT);
//  pinMode(L3, OUTPUT);
//  pinMode(sw1, INPUT);

  
  ina219_A.begin();
  ina219_B.begin();
  ina219_C.begin();

  lcd.init();   // initialize the lcd
  lcd.clear();        // clears display
  lcd.backlight();    // open the backlight 
 
}


void loop() {

//  Serial.println("Sending to nrf24_server");
//  // Send a message to nrf24_server
//  uint8_t data[] = "Hello World!";
//  nrf24.send(data, sizeof(data));
//  
//  nrf24.waitPacketSent();
//  // Now wait for a reply
//  uint8_t buf[RH_NRF24_MAX_MESSAGE_LEN];
//  uint8_t len = sizeof(buf);
//
//  if (nrf24.waitAvailableTimeout(500))
//  { 
//    // Should be a reply message for us now   
//    if (nrf24.recv(buf, &len))
//    {
//      Serial.print("got reply: ");
//      Serial.println((char*)buf);
//    }
//    else
//    {
//      Serial.println("recv failed");
//    }
//  }
//  else
//  {
//    Serial.println("No reply, is nrf24_server running?");
//  }
//

lcd.setCursor(3,3);   
if (digitalRead(swL1) == HIGH) {
  L1high();
  lcd.print("ON ");
} else if (digitalRead(swL1) == LOW) {
  L1low();
  lcd.print("OFF");
}

lcd.setCursor(9,3);  
if (digitalRead(swL2) == HIGH) {
  L2high();
  lcd.print("ON ");
} else if (digitalRead(swL2) == LOW) {
  L2low();
  lcd.print("OFF");
}

lcd.setCursor(15,3);  
if (digitalRead(swL3) == HIGH) {
  L3high();
  lcd.print("ON ");
} else if (digitalRead(swL3) == LOW) {
  L3low();
  lcd.print("OFF");
}


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


  delay(200);   // refresh rate of loop in ms

 
}
