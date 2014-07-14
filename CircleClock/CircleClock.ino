/*

  CircleClock
  
  - Arduino Micro
  - HC-06 Bluetooth
  - RTC DS1307
  - LED Stripe, WS2811 60 LED/m 
  - DCF77 Receiver
  - LDR 
  - PIEZO  
 
  
*/

#include <Wire.h>
#include <Time.h>
#include <DS1307RTC.h>
#include <SoftwareSerial.h>
#include <FastSPI_LED.h>

#define NUM_LEDS            60
#define PIN_LEDS_DATA       10
#define PIN_BLUETOOTH_RX    9
#define PIN_BLUETOOTH_TX    8
const int  PIN_BEEP =  A0;

SoftwareSerial BTSerial( PIN_BLUETOOTH_RX, PIN_BLUETOOTH_TX );
struct CRGB { unsigned char b; unsigned char r; unsigned char g; };
struct CRGB *leds;

void setup() {
  beep_init();
  led_init();
  bluetooth_init();
}


void loop() {
  tmElements_t tm;
  if (RTC.read(tm)) {
    memset(leds, 0, NUM_LEDS * 3);
    leds[ tm.Second ].r = 255;
    leds[ tm.Minute ].g = 255;
    int h;
    h = tm.Hour % 12;
    h = h *5;
    leds[ h  ].b = 255;
    
    FastSPI_LED.show();
    delay(100);    
    
  }
}

void beep_init() {
  pinMode( PIN_BEEP, OUTPUT ); 
  beep(50);
  beep(50);
  beep(50);
}

void beep(unsigned char delayms){
  analogWrite( PIN_BEEP, 20);      // Almost any value can be used except 0 and 255
                           // experiment to get the best tone
  delay(delayms);          // wait for a delayms ms
  analogWrite(9, 0);       // 0 turns it off
  delay(delayms);          // wait for a delayms ms   
}  

void led_init() {
  FastSPI_LED.setLeds( NUM_LEDS );
  FastSPI_LED.setChipset(CFastSPI_LED::SPI_WS2811);
  FastSPI_LED.setPin( PIN_LEDS_DATA );
  FastSPI_LED.init();
  FastSPI_LED.start();
  leds = (struct CRGB*)FastSPI_LED.getRGBData(); 
     
  for( int j = 0; j < 3; j++) { 
    memset(leds, 0, NUM_LEDS * 3);
    for(int i = 0 ; i < NUM_LEDS; i++ ) {
      switch(j) { 
        case 0: leds[i].r = 255; break;
        case 1: leds[i].g = 255; break;
        case 2: leds[i].b = 255; break;
      }
      FastSPI_LED.show();
      delay(10);
    }
  }
  memset(leds, 0, NUM_LEDS * 3);
  FastSPI_LED.show();
}

void bluetooth_init() {
  BTSerial.begin(9600);
  
  // Hier dann noch init BT Module per ATcmds - Name setzen usw

}

