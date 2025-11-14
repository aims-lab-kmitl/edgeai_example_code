#include <Arduino_LSM9DS1.h>
#include <KMITL_BLE.h>

KMITL_BLE kmitlBLE;
AXIS_DataTypedef accel;

void setup(){
  Serial.begin(115200);
  kmitlBLE.begin("RAI");

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }
}

void loop(){
  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(accel.x, accel.y, accel.z);

    Serial.print(accel.x);
    Serial.print('\t');
    Serial.print(accel.y);
    Serial.print('\t');
    Serial.println(accel.z);
  }

  BLEDevice central = BLE.central();
  if(central){
    if (central.connected()) {
      /* Create accelerometer message*/
      kmitlBLE.acclMessage(&accel);

      /* Sending data via Bluetooth*/
      kmitlBLE.sendData();
    }
  }
}