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
        digitalWrite(LEDR, HIGH);
        digitalWrite(LEDG, HIGH);
        digitalWrite(LEDB, HIGH);
        switch(kmitlBLE.getControl()){
          case 01:  digitalWrite(LEDR, LOW);
                    break;
          case 02:  digitalWrite(LEDG, LOW);
                    break;
          case 03:  digitalWrite(LEDB, LOW);
                    break;
          default:  digitalWrite(LEDR, HIGH);
                    digitalWrite(LEDG, HIGH);
                    digitalWrite(LEDB, HIGH);
                    break;
        }
      }
    }
  }
}