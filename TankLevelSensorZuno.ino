ZUNO_SETUP_CHANNELS(
    ZUNO_SENSOR_MULTILEVEL(
        ZUNO_SENSOR_MULTILEVEL_TYPE_DISTANCE,
        0, // scale is meters
        SENSOR_MULTILEVEL_SIZE_TWO_BYTES,
        2, // two decimals after dot
        getter
    ),
    ZUNO_SENSOR_MULTILEVEL(
      ZUNO_SENSOR_MULTILEVEL_TYPE_TEMPERATURE, 
      SENSOR_MULTILEVEL_SCALE_CELSIUS, 
      SENSOR_MULTILEVEL_SIZE_TWO_BYTES, 
      SENSOR_MULTILEVEL_PRECISION_ONE_DECIMAL, 
      getterTemp)
);

ZUNO_SETUP_ASSOCIATIONS(ZUNO_ASSOCIATION_GROUP_SET_VALUE); // to control other devices
ZUNO_SETUP_SLEEPING_MODE(ZUNO_SLEEPING_MODE_ALWAYS_AWAKE);


#define header_H    0x55 //Header
#define header_L    0xAA //Header
#define device_Addr 0x11 //Address
#define data_Length 0x00 //Data length
#define get_Dis_CMD 0x02 //Command: Read Distance
#define get_Temp_CMD 0x03 //Command: Read Temperature
#define checksum_distance    (header_H+header_L+device_Addr+data_Length+get_Dis_CMD)
#define checksum_temperature    (header_H+header_L+device_Addr+data_Length+get_Temp_CMD)

bool hasDistanceBeenRead = false;
bool hasTemperatureBeenRead = false;
word lastValue = 0;
word lastTempValue = 0;

unsigned char CMD_DISTANCE[6]={
  header_H,header_L,device_Addr,data_Length,get_Dis_CMD,checksum_distance};

unsigned char CMD_TEMP[6]={
  header_H,header_L,device_Addr,data_Length,get_Temp_CMD,checksum_temperature};

unsigned int readDistance(){
  unsigned int distance=0;
  int i = 0;
  unsigned char rx_data[8];
  for(i=0;i<6;i++){
    Serial1.write(CMD_DISTANCE[i]);
  }
  delay(1000);  //Wait for the result
  i=0;
  while (Serial1.available()){  //Read the return data (Note: this demo is only for the reference, no data verification)
    rx_data[i++]=(Serial1.read());
  }
  distance=((rx_data[5]<<8)|rx_data[6]); //Read the distance value
  return distance;
}

int readTemperature(){
  int temperature=0;
  unsigned char i = 0;
  unsigned char rx_data[8];
  for(i=0;i<6;i++){
    Serial1.write(CMD_TEMP[i]);
  }
  delay(50);  //Wait for the result
  i=0;
  while (Serial1.available()){  //Read the return data (Note: this demo is only for the reference, no data verification)
    rx_data[i++]=(Serial1.read());
  }
  temperature=((rx_data[5]<<8)|rx_data[6]); //Read the distance value
  return temperature;
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial1.begin(19200);  //Serial1: Ultrasonic Sensor Communication Serial Port, Buadrate: 19200
  //Serial.begin();
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(100);
  hasDistanceBeenRead = false;
  hasTemperatureBeenRead = false;
  unsigned int distance = readDistance();
  delay(100);
  int temperature = readTemperature();
  
   // Prints the distance on the Serial Monitor
   /*Serial.print("Distance: ");
   Serial.println(distance);
   Serial.println(temperature);
   Serial.print(temperature/10);             //Print Temperature
   Serial.print('.');
   Serial.print(temperature%10);
   Serial.println("C");*/

   // Adjust based on a 0.17% variation per degree
   // 15°C is taken as the basis of 0% variation
   distance = (int)(distance + (temperature - 150)*0.017);

  /* Serial.print("adjusted Distance: ");
   Serial.println(distance);*/
  
   lastValue = word(distance);
   lastTempValue = word(temperature);
   //Serial.println(lastValue);
    
   // send report to controller (Life Line group)
   zunoSendReport(1);
   // this is useful when battery powered
  // while (!hasDistanceBeenRead && !hasTemperatureBeenRead){
  //  delay(1000); // wait before sending device to sleep
  // }
   digitalWrite(LED_BUILTIN, LOW);
   // zunoSendDeviceToSleep();
   delay(60000);
}

word getter() {
  hasDistanceBeenRead = true;
  return lastValue;
}

word getterTemp() {
  hasTemperatureBeenRead = true;
  return lastTempValue;
}
