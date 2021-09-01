// Paint example specifically for the TFTLCD breakout board.
// If using the Arduino shield, use the tftpaint_shield.pde sketch instead!
// DOES NOT CURRENTLY WORK ON ARDUINO LEONARDO
//Technical support:goodtft@163.com

#include <Elegoo_GFX.h>    // Core graphics library
#include <Elegoo_TFTLCD.h> // Hardware-specific library
#include <TouchScreen.h>

#define SD

#ifdef SD
#include <SPI.h>
#include <SD.h>

//SD card objects
Sd2Card card;
SdVolume volume;
SdFile root;

const int chipSelect = 10;

void SD_func() {
  Serial.print(F("\nInitializing SD card..."));

  // we'll use the initialization code from the utility libraries
  // since we're just testing if the card is working!

  if (!card.init(SPI_HALF_SPEED, chipSelect)) {
    Serial.println(F("initialization failed. Things to check:"));
    Serial.println(F("* is a card inserted?"));
    Serial.println(F("* is your wiring correct?"));
    Serial.println(F("* did you change the chipSelect pin to match your shield or module?"));
    while (1);

  } else {
    Serial.println(F("Wiring is correct and a card is present."));
  }

  // print the type of card

  Serial.println();
  Serial.print(F("Card type:         "));

  switch (card.type()) {

    case SD_CARD_TYPE_SD1:
      Serial.println(F("SD1"));
      break;
    case SD_CARD_TYPE_SD2:
      Serial.println(F("SD2"));
      break;
    case SD_CARD_TYPE_SDHC:
      Serial.println(F("SDHC"));
      break;
    default:
      Serial.println(F("Unknown"));

  }

  // Now we will try to open the 'volume'/'partition' - it should be FAT16 or FAT32

  if (!volume.init(card)) {
    Serial.println(F("Could not find FAT16/FAT32 partition.\nMake sure you've formatted the card"));
    while (1);
  }

  Serial.print(F("Clusters:          "));
  Serial.println(volume.clusterCount());
  Serial.print(F("Blocks x Cluster:  "));
  Serial.println(volume.blocksPerCluster());
  Serial.print(F("Total Blocks:      "));
  Serial.println(volume.blocksPerCluster() * volume.clusterCount());
  Serial.println();

  // print the type and size of the first FAT-type volume

  uint32_t volumesize;
  
  Serial.print(F("Volume type is:    FAT"));
  Serial.println(volume.fatType(), DEC);
  
  volumesize = volume.blocksPerCluster();    // clusters are collections of blocks
  volumesize *= volume.clusterCount();       // we'll have a lot of clusters
  volumesize /= 2;                           // SD card blocks are always 512 bytes (2 blocks are 1KB)

  Serial.print(F("Volume size (Kb):  "));
  Serial.println(volumesize);
  
  Serial.print(F("Volume size (Mb):  "));
  volumesize /= 1024;
  Serial.println(volumesize);
  
  Serial.print(F("Volume size (Gb):  "));
  Serial.println((float)volumesize / 1024.0);
  
  Serial.println(F("\nFiles found on the card (name, date and size in bytes): "));
  
  root.openRoot(volume);

  // list all files in the card with date and size
  root.ls(LS_R | LS_DATE | LS_SIZE);
  root.close();
}
#endif

#if defined(__SAM3X8E__)
    #undef __FlashStringHelper::F(string_literal)
    #define F(string_literal) string_literal
#endif

// When using the BREAKOUT BOARD only, use these 8 data lines to the LCD:
// For the Arduino Uno, Duemilanove, Diecimila, etc.:
//   D0 connects to digital pin 8  (Notice these are
//   D1 connects to digital pin 9   NOT in order!)
//   D2 connects to digital pin 2
//   D3 connects to digital pin 3
//   D4 connects to digital pin 4
//   D5 connects to digital pin 5
//   D6 connects to digital pin 6
//   D7 connects to digital pin 7

// For the Arduino Mega, use digital pins 22 through 29
// (on the 2-row header at the end of the board).
//   D0 connects to digital pin 22
//   D1 connects to digital pin 23
//   D2 connects to digital pin 24
//   D3 connects to digital pin 25
//   D4 connects to digital pin 26
//   D5 connects to digital pin 27
//   D6 connects to digital pin 28
//   D7 connects to digital pin 29

// For the Arduino Due, use digital pins 33 through 40
// (on the 2-row header at the end of the board).
//   D0 connects to digital pin 33
//   D1 connects to digital pin 34
//   D2 connects to digital pin 35
//   D3 connects to digital pin 36
//   D4 connects to digital pin 37
//   D5 connects to digital pin 38
//   D6 connects to digital pin 39
//   D7 connects to digital pin 40
/*
#define YP 9  // must be an analog pin, use "An" notation!
#define XM 8  // must be an analog pin, use "An" notation!
#define YM A2   // can be a digital pin
#define XP A3   // can be a digital pin
*/

#define YP A3  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 9   // can be a digital pin
#define XP 8   // can be a digital pin
/*
#define TS_MINX 50
#define TS_MAXX 920

#define TS_MINY 100
#define TS_MAXY 940
*/
//Touch For New ILI9341 TP
#define TS_MINX 120
#define TS_MAXX 900

#define TS_MINY 70
#define TS_MAXY 920

// For better pressure precision, we need to know the resistance
// between X+ and X- Use any multimeter to read it
// For the one we're using, its 300 ohms across the X plate
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 350);

#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
// optional
#define LCD_RESET A4

// Assign human-readable names to some common 16-bit color values:
#define	BLACK   0x0000
#define	BLUE    0x001F
#define	RED     0xF800
#define	GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

#define ILI9341_BLACK   0x0000
#define ILI9341_BLUE    0x001F
#define ILI9341_RED     0xF800
#define ILI9341_GREEN   0x07E0
#define ILI9341_CYAN    0x07FF
#define ILI9341_MAGENTA 0xF81F
#define ILI9341_YELLOW  0xFFE0
#define ILI9341_WHITE   0xFFFF


#define ILI9341_GREY 0x2104 // Dark grey 16 bit colour
// Meter colour schemes
#define RED2RED 0
#define GREEN2GREEN 1
#define BLUE2BLUE 2
#define BLUE2RED 3
#define GREEN2RED 4
#define RED2GREEN 5

Elegoo_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

#define BOXSIZE 40
#define PENRADIUS 3
uint16_t oldcolor, currentcolor;
uint8_t currentrot = 0;
uint16_t x=0,y=0;

void setup(void) {
  Serial.begin(115200);
  Serial.println(F("Paint!"));

  #ifdef SD
  SD_func();
  #endif
  
  tft.reset();
  
  uint16_t identifier = tft.readID();
  if(identifier == 0x9325) {
    Serial.println(F("Found ILI9325 LCD driver"));
  } else if(identifier == 0x9328) {
    Serial.println(F("Found ILI9328 LCD driver"));
  } else if(identifier == 0x4535) {
    Serial.println(F("Found LGDP4535 LCD driver"));
  }else if(identifier == 0x7575) {
    Serial.println(F("Found HX8347G LCD driver"));
  } else if(identifier == 0x9341) {
    Serial.println(F("Found ILI9341 LCD driver"));
  } else if(identifier == 0x8357) {
    Serial.println(F("Found HX8357D LCD driver"));
  } else if(identifier==0x0101)
  {     
      identifier=0x9341;
       Serial.println(F("Found 0x9341 LCD driver"));
  }else {
    Serial.print(F("Unknown LCD driver chip: "));
    Serial.println(identifier, HEX);
    Serial.println(F("If using the Elegoo 2.8\" TFT Arduino shield, the line:"));
    Serial.println(F("  #define USE_Elegoo_SHIELD_PINOUT"));
    Serial.println(F("should appear in the library header (Elegoo_TFT.h)."));
    Serial.println(F("If using the breakout board, it should NOT be #defined!"));
    Serial.println(F("Also if using the breakout, double-check that all wiring"));
    Serial.println(F("matches the tutorial."));
    identifier=0x9341;
   
  }

  tft.begin(identifier);
  tft.setRotation(5);

  tft.fillScreen(BLACK);

  //tft.fillRect(0, 0, BOXSIZE, BOXSIZE, RED);
  //tft.drawRect(0, 0, BOXSIZE, BOXSIZE, WHITE);
  currentcolor = RED;
 
  pinMode(13, OUTPUT);
}

#define MINPRESSURE 10
#define MAXPRESSURE 1000

//instrument constants
#define TEXTSIZE 2
#define TEXTX    (6*TEXTSIZE)
#define TEXTY    (8*TEXTSIZE)
#define FOCUSBOXX (4*TEXTX)
#define FOCUSBOXY (TEXTY+4)

#define AUTOBRKY 128
#define AUTOBRKLABELY  (AUTOBRKY-16)

#define AUTOBRK_XSZ    64
#define AUTOBRK_YSZ    64

#define LOCENTRE_X  (64/2)
#define LOCENTRE_Y  (AUTOBRKY+((3*64)/4)) //ON 
#define LOCENTRE_Y1 (AUTOBRKY+(64/4))     //DECEL

#define MIDCENTRE_X (80 + LOCENTRE_X)

#define MAXCENTRE_X (170 + LOCENTRE_X)

#define LDGGEARY 16
#define LDGCENTRE_Y  (LDGGEARY+((3*64)/4)) //ARROW
#define LDGCENTRE_Y1 (LDGGEARY+(64/4))
#define LEFTCENTRE_X  (64/2)
#define NOSECENTRE_X  (80+LEFTCENTRE_X)
#define RIGHTCENTRE_X (170+LEFTCENTRE_X)

#define xstr(s) str(s)
#define str(s) #s

void ldggear()
{
    tft.drawRect(0,LDGGEARY,64,64,WHITE);
    tft.drawRect(1,LDGGEARY+1,62,62,WHITE);

    tft.drawRect(70+10,LDGGEARY,64,64,WHITE);
    tft.drawRect(70+11,LDGGEARY+1,62,62,WHITE);
      
    tft.drawRect(150+20,LDGGEARY,64,64,WHITE);//apart spaced
    tft.drawRect(150+21,LDGGEARY+1,62,62,WHITE);//apart spaced 

    gearunlock(LEFTCENTRE_X,ILI9341_GREY);
    gearunlock(NOSECENTRE_X,ILI9341_GREY);
    gearunlock(RIGHTCENTRE_X,ILI9341_GREY);

    geardown(LEFTCENTRE_X,ILI9341_GREY);
    geardown(NOSECENTRE_X,ILI9341_GREY);
    geardown(RIGHTCENTRE_X,ILI9341_GREY);
}

void geartransit(){
    
    gearunlock(LEFTCENTRE_X,RED);
    gearunlock(NOSECENTRE_X,RED);
    gearunlock(RIGHTCENTRE_X,RED);
    geardown(LEFTCENTRE_X,ILI9341_GREY);
    geardown(NOSECENTRE_X,ILI9341_GREY);
    geardown(RIGHTCENTRE_X,ILI9341_GREY);
    
}

void geardownlocked(){
  
    gearunlock(LEFTCENTRE_X,ILI9341_GREY);
    gearunlock(NOSECENTRE_X,ILI9341_GREY);
    gearunlock(RIGHTCENTRE_X,ILI9341_GREY);
    geardown(LEFTCENTRE_X,GREEN);
    geardown(NOSECENTRE_X,GREEN);
    geardown(RIGHTCENTRE_X,GREEN);
    
}

void gearuplocked(){
  
    gearunlock(LEFTCENTRE_X,ILI9341_GREY);
    gearunlock(NOSECENTRE_X,ILI9341_GREY);
    gearunlock(RIGHTCENTRE_X,ILI9341_GREY);

    geardown(LEFTCENTRE_X,ILI9341_GREY);
    geardown(NOSECENTRE_X,ILI9341_GREY);
    geardown(RIGHTCENTRE_X,ILI9341_GREY);
    
}

void gearunlock(uint16_t centre_x,uint16_t color)
{
    //tft.setCursor(LEFTCENTRE_X-((TEXTX*4)/2),LDGCENTRE_Y1-(TEXTY/2));
    //tft.setTextColor(RED);
    //tft.println(F("UNLK"));
    tft.setCursor(centre_x-((TEXTX*4)/2),LDGCENTRE_Y1-(TEXTY/2));
    tft.setTextColor(color);
    tft.println(F("UNLK"));
}
void geardown(uint16_t centre_x,uint16_t color)
{
    //gear down
    //tft.drawLine(LEFTCENTRE_X-18,LDGCENTRE_Y-12,LEFTCENTRE_X+18,LDGCENTRE_Y-12,GREEN);
    //tft.drawLine(LEFTCENTRE_X-18,LDGCENTRE_Y-12,LEFTCENTRE_X,LDGCENTRE_Y+8,GREEN);
    //tft.drawLine(LEFTCENTRE_X+18,LDGCENTRE_Y-12,LEFTCENTRE_X,LDGCENTRE_Y+8,GREEN);
    tft.drawLine(centre_x-18,LDGCENTRE_Y-12,centre_x+18,LDGCENTRE_Y-12,color);
    tft.drawLine(centre_x-18,LDGCENTRE_Y-12,centre_x,LDGCENTRE_Y+8,color);
    tft.drawLine(centre_x+18,LDGCENTRE_Y-12,centre_x,LDGCENTRE_Y+8,color);
}
void loop()
{
  if(Serial.available()>0){
    uint16_t c = Serial.read();
    
    if(c=='O'){
       tft.setCursor(LOCENTRE_X-TEXTX,LOCENTRE_Y-(TEXTY/2));
       tft.setTextColor(CYAN);
       tft.println(F("ON"));
       tft.drawRect(LOCENTRE_X-(FOCUSBOXX/2),LOCENTRE_Y-FOCUSBOXY/2,FOCUSBOXX,FOCUSBOXY,CYAN);
    }
    else if(c=='P'){
       tft.setCursor(MIDCENTRE_X-TEXTX,LOCENTRE_Y-(TEXTY/2));
       tft.setTextColor(CYAN);
       tft.println(F("ON"));
       tft.drawRect(MIDCENTRE_X-(FOCUSBOXX/2),LOCENTRE_Y-FOCUSBOXY/2,FOCUSBOXX,FOCUSBOXY,CYAN);
    }
    else if(c=='Q'){
       tft.setCursor(MAXCENTRE_X-TEXTX,LOCENTRE_Y-(TEXTY/2));
       tft.setTextColor(CYAN);
       tft.println(F("ON"));
       tft.drawRect(MAXCENTRE_X-(FOCUSBOXX/2),LOCENTRE_Y-FOCUSBOXY/2,FOCUSBOXX,FOCUSBOXY,CYAN);
    }
    else if(c=='o'){
       tft.setCursor(LOCENTRE_X-TEXTX,LOCENTRE_Y-(TEXTY/2));
       tft.setTextColor(ILI9341_GREY);
       tft.println(F("ON"));
       tft.drawRect(LOCENTRE_X-(FOCUSBOXX/2),LOCENTRE_Y-FOCUSBOXY/2,FOCUSBOXX,FOCUSBOXY,ILI9341_GREY);
    }
    else if(c=='p'){
       tft.setCursor(MIDCENTRE_X-TEXTX,LOCENTRE_Y-(TEXTY/2));
       tft.setTextColor(ILI9341_GREY);
       tft.println(F("ON"));
       tft.drawRect(MIDCENTRE_X-(FOCUSBOXX/2),LOCENTRE_Y-FOCUSBOXY/2,FOCUSBOXX,FOCUSBOXY,ILI9341_GREY);
    }
    else if(c=='q'){
       tft.setCursor(MAXCENTRE_X-TEXTX,LOCENTRE_Y-(TEXTY/2));
       tft.setTextColor(ILI9341_GREY);
       tft.println(F("ON"));
       tft.drawRect(MAXCENTRE_X-(FOCUSBOXX/2),LOCENTRE_Y-FOCUSBOXY/2,FOCUSBOXX,FOCUSBOXY,ILI9341_GREY);
    }
    else if(c=='D'){
       tft.setCursor(LOCENTRE_X-((TEXTX*5)/2),LOCENTRE_Y1-(TEXTY/2));
       tft.setTextColor(GREEN);
       tft.println(F("DECEL"));
    }
    else if(c=='E'){
       tft.setCursor(MIDCENTRE_X-((TEXTX*5)/2),LOCENTRE_Y1-(TEXTY/2));
       tft.setTextColor(GREEN);
       tft.println(F("DECEL"));
    }
    else if(c=='F'){
       tft.setCursor(MAXCENTRE_X-((TEXTX*5)/2),LOCENTRE_Y1-(TEXTY/2));
       tft.setTextColor(GREEN);
       tft.println(F("DECEL"));
    }
    else if(c=='d'){
       tft.setCursor(LOCENTRE_X-((TEXTX*5)/2),LOCENTRE_Y1-(TEXTY/2));
       tft.setTextColor(ILI9341_GREY);
       tft.println(F("DECEL"));
    }
    else if(c=='e'){
       tft.setCursor(MIDCENTRE_X-((TEXTX*5)/2),LOCENTRE_Y1-(TEXTY/2));
       tft.setTextColor(ILI9341_GREY);
       tft.println(F("DECEL"));
    }
    else if(c=='f'){
       tft.setCursor(MAXCENTRE_X-((TEXTX*5)/2),LOCENTRE_Y1-(TEXTY/2));
       tft.setTextColor(ILI9341_GREY);
       tft.println(F("DECEL"));
    }
    else if(c=='G') {
      tft.fillScreen(BLACK);

      tft.drawRect(0,AUTOBRKY,64,64,WHITE);
      tft.drawRect(1,AUTOBRKY+1,62,62,WHITE);
      
      tft.drawRect(70+10,AUTOBRKY,64,64,WHITE);
      tft.drawRect(70+11,AUTOBRKY+1,62,62,WHITE);
      
      tft.drawRect(150+20,AUTOBRKY,64,64,WHITE);//apart spaced
      tft.drawRect(150+21,AUTOBRKY+1,62,62,WHITE);//apart spaced

      tft.setTextColor(WHITE);
      tft.setTextSize(2);
      tft.setCursor(0+64/2-TEXTX,AUTOBRKLABELY);
      tft.println(F("LO"));
      tft.setCursor(70+10+64/2-(3*TEXTX)/2,AUTOBRKLABELY);
      tft.println(F("MED"));
      tft.setCursor(150+20+64/2-(3*TEXTX)/2,AUTOBRKLABELY);
      tft.println(F("MAX"));
      
      ldggear();
      delay(2000);
    }
    else if(c=='a') {
       gearuplocked();
    }
    else if(c=='b') {
       geartransit();
    }
    else if(c=='c') {
       geardownlocked();
    }
    else if(c=='C') {
      tft.fillScreen(BLACK);
    }  
    else if(c=='R') {
      currentrot++;
      if(currentrot>6) currentrot=0; 
      tft.setRotation(currentrot);
      Serial.println(currentrot);
    }
    else if(c=='k'){
      uint16_t number=0;
      /*
      char c;
      do {
        c=Serial.read();
      }while(c<0);
      
      while(isdigit(c)){
        c=c-'0';
        number = number*10+c;
        do{
        c=Serial.read();
        }while(c<0);
      }*/
      String n = Serial.readStringUntil('\n');
      number = n.toInt();
      //Serial.print(number);
      tft.drawPixel(x,y,number);
      x++;
      if(x>319){x=0;y++;}
    }
    else if(c=='Z'){
      if(currentcolor == RED)        currentcolor = GREEN;
      else if(currentcolor == GREEN) currentcolor = BLUE;
      else if(currentcolor == BLUE)  currentcolor = RED;
    }
  }
  digitalWrite(13, HIGH);
  TSPoint p = ts.getPoint();
  digitalWrite(13, LOW);

  // if sharing pins, you'll need to fix the directions of the touchscreen pins
  //pinMode(XP, OUTPUT);
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  //pinMode(YM, OUTPUT);

  // we have some minimum pressure we consider 'valid'
  // pressure of 0 means no pressing!

  if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
    /*
    Serial.print("X = "); Serial.print(p.x);
    Serial.print("\tY = "); Serial.print(p.y);
    Serial.print("\tPressure = "); Serial.println(p.z);
    */
    
    if (p.y < (TS_MINY-5)) {
      Serial.println(F("erase"));
      // press the bottom of the screen to erase 
      tft.fillRect(0, BOXSIZE, tft.width(), tft.height()-BOXSIZE, BLACK);
    }
    // scale from 0->1023 to tft.width
    // works with rotation 6
    p.x = map(p.x, TS_MINX, TS_MAXX, tft.width(), 0);
    p.y = map(p.y, TS_MINY, TS_MAXY, tft.height(), 0);

    //check if within autobrake button row
    if( p.y>( AUTOBRKY ) && p.y<( AUTOBRKY+AUTOBRK_YSZ ) ){
      
       if ( p.x > (LOCENTRE_X-(AUTOBRK_XSZ/2)) && p.x < (LOCENTRE_X+(AUTOBRK_XSZ/2)) )
         Serial.println("LO");
         
       else if ( p.x > (MIDCENTRE_X-(AUTOBRK_XSZ/2)) && p.x < (MIDCENTRE_X+(AUTOBRK_XSZ/2)) )  
         Serial.println("MID");
         
       else if ( p.x > (MAXCENTRE_X-(AUTOBRK_XSZ/2)) && p.x < (MAXCENTRE_X+(AUTOBRK_XSZ/2)) )   
         Serial.println("MAX");
      
    }//autobrake rows

    
    if (((p.y-PENRADIUS) > BOXSIZE) && ((p.y+PENRADIUS) < tft.height())) {
      tft.fillCircle(p.x, p.y, PENRADIUS, currentcolor);
    }
  }
  
}
