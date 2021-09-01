#include <RF24.h>
#include <Servo.h>



//pin definitions
#define redLED   3
#define greenLED 5
#define blueLED  6

#define rudder_left_pin      A2
#define rudder_right_pin     A3
#define aileron_left_pin     A4
#define aileron_right_pin    A5
#define elevator_left_pin    9
#define elevator_right_pin   10

#define esc_pin              2

Servo esc;
Servo elev_left;
Servo elev_right;
Servo aileron_left;
Servo aileron_right;
Servo rudder_left;
Servo rudder_right;

RF24 radio(7, 8); // CE, CSN

const byte address[6] = "00001";

void setup() {

  Serial.begin(115200);
  
  radio.begin();
  radio.setChannel(75);
  radio.setPALevel(RF24_PA_MIN);
  radio.setDataRate(RF24_2MBPS);
  radio.setAutoAck(1);                     // Ensure autoACK is enabled
  radio.setRetries(2, 15);                 // Optionally, increase the delay between retries & # of retries

  radio.setCRCLength(RF24_CRC_8);
  radio.openReadingPipe(0,address);
 
  radio.startListening();

  Serial.println("Vehicle Ready");

  setRGB(0x10,0x10,0x10);
  randomSeed(millis());

  esc.attach(esc_pin,1000,2000);
  elev_left.attach(elevator_left_pin,800,2200);
  elev_right.attach(elevator_right_pin,800,2200);
  aileron_left.attach(aileron_left_pin,800,2200);
  aileron_right.attach(aileron_right_pin,800,2200);
  rudder_left.attach(rudder_left_pin,800,2200);
  rudder_right.attach(rudder_right_pin,800,2200);
  
}


static char text[32] = "";
static int count = 0;

struct joy_t {
  uint16_t a0 = 0;
  uint16_t a1 = 0;
  uint16_t a2 = 0;
  uint16_t a3 = 0;
  uint8_t  buttons0 = 0;
  uint8_t  buttons1 = 0;
  uint8_t  buttons2 = 0;
  uint8_t  buttons3 = 0;
  uint16_t esc = 0;
}joy;

uint8_t packet_parse() {
  uint8_t r=0;
  if(text[0]=='#' && text[1]=='+') {
    joy.a0 = (text[3]<<8) | (text[2]&0xff);
    joy.a1 = (text[5]<<8) | (text[4]&0xff);
    joy.a2 = (text[7]<<8) | (text[6]&0xff);
    joy.a3 = (text[9]<<8) | (text[8]&0xff);
    joy.buttons0 = text[10];
    joy.buttons1 = text[11];
    joy.buttons2 = text[12];
    joy.buttons3 = text[13];
    joy.esc = (text[15]<<8) | (text[14]&0xff);
    r=1;
  }
  return r;
}
void loop() {
  
  if (radio.available()) {

    radio.read(text,sizeof(text));
    packet_parse();
    //if(!packet_parse())
    //  Serial.println(text);
  
  }
  
  if(count==50){
    uint8_t r = random(0,255);
    uint8_t g = random(0,255);
    uint8_t b = random(0,255);
    setRGB(r,g,b);
    count++;  
  }
  else if(count==100) {
    Serial.print(F("0:"));
    Serial.println(joy.a0,DEC);
    Serial.print(F("1:"));
    Serial.println(joy.a1,DEC);
    Serial.print(F("E:"));
    Serial.println(joy.esc,DEC);
    count = 0;

  }
  else {
    count++;  
  }

  //servo update
  //convert 10-bit to angle
  uint16_t angle = map(joy.esc,0,1023,0,180);
  esc.write(angle);
  
  angle = map(joy.a1,1023,0,0,180);
  elev_left.write(angle);

  angle = map(joy.a1,0,1023,0,180);
  elev_right.write(angle);

  angle = map(joy.a0,0,1023,0,180);
  aileron_left.write(angle);

  angle = map(joy.a0,0,1023,180,0);
  aileron_right.write(angle);

  angle = map(joy.a2,0,1023,0,180);
  rudder_left.write(angle);

  angle = map(joy.a2,0,1023,0,180);
  rudder_right.write(angle);
  
  delay(20);

}

void setRGB(uint8_t r,uint8_t g,uint8_t b) {

  analogWrite(redLED,r);
  analogWrite(greenLED,g);
  analogWrite(blueLED,b);
  
}
