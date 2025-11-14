#include <Arduino_APDS9960.h>
#include <Arduino_LPS22HB.h>
#include <Arduino_LSM9DS1.h>
#include <PDM.h>

#include <KMITL_BLE.h>

KMITL_BLE kmitlBLE;
AXIS_DataTypedef accl, gyro;
short audioBuffer[256];
volatile int audioRead, i = 0;

void onPDMdata();

void setup(){
  Serial.begin(115200);
  kmitlBLE.begin("BLE_NodeRed");

  /* Initial Sensor */
  APDS.begin();   // APDS-9960 sensor
  BARO.begin();   // Temperature and Pressure sensor
  IMU.begin();    // IMU sensor

  /* Audio Sensor */
  PDM.onReceive(onPDMdata);
  PDM.begin(1, 16000);

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
    Serial.print("Connected to central: ");
    Serial.println(central.address());
    while (central.connected()) {  
      /* APDS-9960 sensor */
      if (APDS.gestureAvailable()){
        int gesture = APDS.readGesture();
        kmitlBLE.gestMessage(gesture);
      }

      /* IMU Sensor */
      if (IMU.accelerationAvailable()){
        IMU.readAcceleration(accl.x, accl.y, accl.z);
        kmitlBLE.acclMessage(&accl);
      }
      if (IMU.gyroscopeAvailable()){
        IMU.readGyroscope(gyro.x, gyro.y, gyro.z);
        kmitlBLE.gyroMessage(&gyro);
      }

      /* Temperature and Pressure sensor */
      float pressure = BARO.readPressure();
      kmitlBLE.presMessage(pressure);
      float temperature = BARO.readTemperature();
      kmitlBLE.tempMessage(temperature);

      /* Audio*/
      if(i < audioRead) kmitlBLE.audiMessage(audioBuffer[i++]);
      else{
        audioRead = 0;
        i = 0;
      }
      
      /* Sending data via Bluetooth*/
      kmitlBLE.sendData();

      /* Control RGB Buildin */
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

void onPDMdata() {
  int bytesAvailable = PDM.available();
  PDM.read(audioBuffer, bytesAvailable);
  audioRead = bytesAvailable / 2;
}