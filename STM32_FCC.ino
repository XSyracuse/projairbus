#include <Wire.h>
TwoWire i2c(PB9,PB8);

void scani2c()
{
  
  int nDevices = 0;
  byte error, address;
  i2c.begin();
  i2c.setClock(400000);
  Serial.println(F("Scanning..."));
  for(address=1;address<128;address++) {
    i2c.beginTransmission(address);
    error = i2c.endTransmission(true);

    if(error==0){
      Serial.print(F("i2c device found at 0x"));
      if(address<16) Serial.print("0");
      Serial.print(address,HEX);
      Serial.println(" !");
      nDevices++;
    }
    else if(error==4) {
      Serial.print(F("unknown error at 0x"));
      if(address<16) Serial.print("0");
      Serial.print(address,HEX);
      Serial.println(" !");
    }
      
  }//for
  
  if(nDevices==0) Serial.println("no i2c devices found");
  else Serial.println(F("done\n"));
  
}

#define MPU_ADDR 0x68

uint8_t mpu6050_write(uint8_t reg, uint8_t d){
  i2c.beginTransmission(MPU_ADDR);
  i2c.write(reg);
  i2c.write(d);
  uint8_t err = i2c.endTransmission();
  return err;
}

uint8_t mpu6050_read(uint8_t reg,uint8_t n){
  
  i2c.beginTransmission(MPU_ADDR);
  i2c.write(reg);
  i2c.endTransmission(false);
  
  return i2c.requestFrom(MPU_ADDR,n,true);
  
}

uint8_t mpu6050_read8(uint8_t reg,uint8_t& d){

   uint8_t n = mpu6050_read(reg,1);
   
   if(i2c.available()>0) {
     d = i2c.read();
   }

   return n; 
}

void mpu6050_init(){
  uint8_t d;
  
  // reset device
  mpu6050_write(107,0x80);

  delay(1000);
  
  // read whoami
  mpu6050_read8(117,d);
  
  Serial.print("whoami: ");
  Serial.println(d);
  
  // internal 8MHz
  mpu6050_write(107,0x00);
  
  // sample rate divider
  // 8KHz/(smplrt_div+1)
  // therefore 7 = 1000Hz
  mpu6050_write(25,7);
  
  // select interrupt, 50 microsecond pulse
  mpu6050_write(55,0x00);
  
  // select fifo data off
  mpu6050_write(35,0x0);

  //reset fifo and gyro signal paths
  mpu6050_write(106,0x05);
  //enable fifo
  mpu6050_write(106,0x40);
  
  //reset paths
  //mpu6050_write(104,0x07);
  //mpu6050_write(104,0x00);
  
  // enable raw data interrupt
  mpu6050_write(56,0x01);
  
}

int16_t aX,aY,aZ,Temp,gX,gY,gZ;

uint8_t mpu6050_acq(){
  uint8_t n = mpu6050_read(0x3b,14);
  
  aX = (i2c.read() << 8) | i2c.read();
  aY = (i2c.read() << 8) | i2c.read();
  aZ = (i2c.read() << 8) | i2c.read();
  Temp = (i2c.read() << 8) | i2c.read();
  gX = (i2c.read() << 8) | i2c.read();
  gY = (i2c.read() << 8) | i2c.read();
  gZ = (i2c.read() << 8) | i2c.read();

  return n;
}
void setup() {
  
  //blink LED
  pinMode(PC13,OUTPUT);

  //serial
  Serial.begin(9600);

  i2c.begin();
  i2c.setClock(400000);

  // start
  mpu6050_write(107,0x00);
  // enable raw data interrupt
  mpu6050_write(56,0x01);
  
  //mpu6050_init();
}

unsigned char scan_enabled = 0;

void loop() {
  
  // put your main code here, to run repeatedly:
  digitalWrite(PC13,HIGH);
  delay(100);
  digitalWrite(PC13,LOW);
  delay(100);
  
  
  if(Serial.available()>0){
    int c = Serial.read();
    if(c == 's') { 
      if (scan_enabled==1) scan_enabled = 0;
      else scan_enabled = 1;
    }     
  }
  
  if(scan_enabled==1) {
    Serial.println(F("Hej"));
    scani2c();
  }

  uint8_t c = mpu6050_acq();
  
  Serial.print(c);
  
  Serial.print(F(" aX= "));
  Serial.print(aX);
  
  Serial.print(F("  aY= "));
  Serial.print(aY);
  
  Serial.print(F("  aZ= "));
  Serial.print(aZ);

  
  Serial.print(F("  T = "));
  Serial.print(Temp);

  
  Serial.print(F("  gX= "));
  Serial.print(gX);
  
  Serial.print(F("  gY= "));
  Serial.print(gY);
  
  Serial.print(F("  gZ= "));
  Serial.println(gZ);
  
}
