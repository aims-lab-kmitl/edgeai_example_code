#include <KMITL_BLE.h>

KMITL_BLE kmitlBLE;

void setup(){
  Serial.begin(115200);
  kmitlBLE.begin("RAI_ControlRGB");

  /* RGB Buildin */
  pinMode(LEDR, OUTPUT);
  pinMode(LEDG, OUTPUT);
  pinMode(LEDB, OUTPUT);

  /* Turn off all RGBD*/
  digitalWrite(LEDR, HIGH);
  digitalWrite(LEDG, HIGH);
  digitalWrite(LEDB, HIGH);
}

void loop(){
  BLEDevice central = BLE.central();
  if(central){
    while(central.connect()){
      if(kmitlBLE.checkControl()){
        if(kmitlBLE.getControl() == 1){
          digitalWrite(LEDR, LOW);
          digitalWrite(LEDG, HIGH);
          digitalWrite(LEDB, HIGH);
        }
        else{
          digitalWrite(LEDR, HIGH);
          digitalWrite(LEDG, HIGH);
          digitalWrite(LEDB, HIGH);
        }
      }
    }
  }
}