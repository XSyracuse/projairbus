#include <RF24.h>
#include <max7219.h>
#include <stdint.h>

#define redpin   3
#define greenpin 5
#define bluepin  6

RF24 radio(7, 8); // CE, CSN

MAX7219 mcp1;
char mcp_txt[9] = "88888888";

const byte address[6] = "00001";

uint8_t packet[32];

static uint8_t ocr_red   = 0x40;
static uint8_t ocr_green = 0x40;
static uint8_t ocr_blue  = 0x40;

static uint8_t directmode = 1;
static uint8_t centermode = 0;

bool debug = true;

static uint8_t expo = 0;
static uint8_t rate = 0;

#define CENTER0 2048
#define CENTER1 2048

struct controls_t {
  uint16_t a0 = CENTER0;
  uint16_t a1 = CENTER1;
  uint16_t a2 = 0;
  uint16_t a3 = 0;
  uint8_t  b0 = 0;
  uint8_t  b1 = 0;
  uint8_t  b2 = 0;
  uint8_t  b3 = 0;
  uint16_t esc = 0;
  uint8_t pitch_mfactor = 1;
  uint8_t pitch_dfactor = 4;
  uint8_t roll_mfactor  = 1;
  uint8_t roll_dfactor  = 4;
} controls;

void get_packet(char *p) {

  p[0] = '#';
  p[1] = '+';
  p[2] = controls.a0 & 0xFF;
  p[3] = controls.a0 >> 8;
  p[4] = controls.a1 & 0xFF;
  p[5] = controls.a1 >> 8;
  p[6] = controls.a2 & 0xFF;
  p[7] = controls.a2 >> 8;
  p[8] = controls.a3 & 0xFF;
  p[9] = controls.a3 >> 8;
  p[10] = controls.b0;
  p[11] = controls.b1;
  p[12] = controls.b2;
  p[13] = controls.b3;
  p[14] = controls.esc & 0xFF;
  p[15] = controls.esc >> 8;

  for (int i = 16; i < 32; i++)
    p[i] = 0;

  uint16_t chksum = 0;
  for (int i = 0; i << 30; i++)
    chksum = chksum + p[i];
  p[30] = chksum & 0xff;
  p[31] = chksum >> 8;
}

void setrgb() {
  analogWrite(redpin,  ocr_red);
  analogWrite(greenpin, ocr_green);
  analogWrite(bluepin, ocr_blue);
}

void setup() {

  Serial.begin(9600);
  //Use Leonardo's 5V reference
  analogReference(DEFAULT);

  setrgb();

  radio.begin();
  radio.setChannel(75);
  radio.setPALevel(RF24_PA_MIN);
  radio.setDataRate(RF24_2MBPS);
  radio.setAutoAck(1);                     // Ensure autoACK is enabled
  radio.setRetries(2, 15);                 // Optionally, increase the delay between retries & # of retries

  radio.setCRCLength(RF24_CRC_8);
  radio.openWritingPipe(address);

  radio.stopListening();

  mcp1.begin();
  mcp1.setDisplayTest(true);
  delay(1000);
  mcp1.setDisplayTest(false);

  mcp1.cls();
  delay(1000);

  for (uint8_t i = 8; i > 0; i--) {
    mcp1.MAX7219_Write(i, 0x06);
    delay(200);
    mcp1.MAX7219_Write(i, 0x36);
    delay(200);
  }
  
  mcp1.cls();
  delay(3000);
  mcp1.DisplayText("radio up", 0);
}

uint16_t bogocount = 0;
void loop() {

  uint16_t x, y, t;

  bool    ok;
  uint8_t p_setting;

  if (Serial.available() > 0) {
    int input = Serial.read();

    switch (input) {
      case 'e':
        if (expo == 5) expo = 0;
        else expo++;
        break;
      case 'D':
        if (rate == 0) rate = 1;
        else rate = 0;
        break;
      case 'L':               
        mcp1.DisplayText("   b 250",0);
        break;
      case 'r':
        ocr_red += 4;
        break;
      case 'R':
        ocr_red -= 4;
        break;
      case 'g':
        ocr_green += 4;
        break;
      case 'G':
        ocr_green -= 4;
        break;
      case 'b':
        ocr_blue += 4;
        break;
      case 'B':
        ocr_blue -= 4;
        break;
      case 'c':
        centermode = (centermode == 0) ? 1 : 0;
        break;
      case 'd':
        directmode = (directmode == 0) ? 1 : 0;
        break;
      case 'm':
        p_setting = get_setting(ok);
        if (ok)
          controls.pitch_mfactor = p_setting;
        break;
      case 'M':
        p_setting = get_setting(ok);
        if (ok)
          controls.roll_mfactor = p_setting;
        break;
      case 'q':
        p_setting = get_setting(ok);
        if (ok)
          controls.pitch_dfactor = p_setting;
        break;
      case 'Q':
        p_setting = get_setting(ok);
        if (ok)
          controls.roll_dfactor = p_setting;
        break;
      case 's':
        Serial.println(F("controls:"));
        Serial.println(controls.pitch_mfactor);
        Serial.println(controls.pitch_dfactor);
        Serial.println(controls.roll_mfactor);
        Serial.println(controls.roll_dfactor);
        break;
      case 'z':
        debug = !debug;
        break;
      default:
        break;
    }

    setrgb();

  }//if input available

  if (bogocount == 20) {
    bogocount++;
  }
  else if (bogocount == 50) {

    if (radio.failureDetected) {
      Serial.println(F("fail"));
      bool tx_ok;
      bool tx_fail;
      bool rx_ready;
      radio.whatHappened(tx_ok, tx_fail, rx_ready);
      Serial.print(F("transmit ok : "));
      Serial.println(tx_ok);
      Serial.print(F("transmit fail : "));
      Serial.println(tx_fail);
      Serial.print(F("receive ready : "));
      Serial.println(rx_ready);
      radio.failureDetected = false;
    }
    else {
      Serial.println(F("ok"));
    }

    Serial.print(F("0: "));
    Serial.println(controls.a0, DEC);
    Serial.print(F("1: "));
    Serial.println(controls.a1, DEC);
    Serial.print(F("E: "));
    Serial.println(controls.esc, DEC);
    if (directmode) Serial.println(F("Direct"));
    if (centermode) Serial.println(F("Centering"));
    if (rate == 1) Serial.println(F("Rate==1"));
    bogocount = 0;
  }
  else {
    bogocount++;
  }

  x = analogRead(A0);
  y = analogRead(A1);
  t = analogRead(A2);

  x=x<<2;
  y=y<<2;
  
  controls.esc = t << 2;

  if (centermode) {
    controls.a0 = CENTER0;
    controls.a1 = CENTER1;
  }
  else if (directmode) {
    controls.a0 = x;
    controls.a1 = y;
  }
  else if (rate == 1) {
    controls.a0 = map(x, 0, 4095, 1024, 3072);
    controls.a1 = map(y, 0, 4095, 1024, 3072);
  }
  else {
    int16_t pitch = (((int16_t)y - CENTER0) * controls.pitch_mfactor) / controls.pitch_dfactor;
    int16_t roll  = (((int16_t)x - CENTER1) * controls.roll_mfactor) / controls.roll_dfactor;

    if (pitch < -2047) pitch = -2047;
    if (roll < -2047)  roll  = -2047;

    controls.a0 = (pitch - roll) + 2048;
    controls.a1 = (pitch + roll) + 2048;

  }

  get_packet(packet);
  radio.write(packet, sizeof(packet));
  delay(20);
}

uint16_t get_setting(bool& ok)
{
  int16_t setting = 0;
  int16_t ch = -1;
  uint16_t endcount = 0;
  ok = false;
  while (!isSpace(ch = Serial.read()) && endcount < 16) {
    endcount++;
    if (debug) {
      Serial.println(F("proc ch"));
      Serial.println(ch, HEX);
    }
    if (ch > -1) {
      setting = (setting * 10) + (ch - '0');
      ok = true;
      if (debug) {
        Serial.println(F("bld s"));
        Serial.println(setting);
      }
    }
    else {
      break;
    }

  }
  if (debug) {
    Serial.println(F("ok  setting"));
    Serial.print(ok, DEC);
    Serial.print(F("  "));
    Serial.println(ch, HEX);
  }
  return setting;
}
