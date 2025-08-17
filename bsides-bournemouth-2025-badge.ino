#include <avr/io.h>-
#include <avr/pgmspace.h>
#include <EEPROM.h>
#include <tinyNeoPixel_Static.h>

// FEEL FREE TO TWEAK

// Wake time
# define WAKE_TIME_MS 300000
// Colours

#define OFF 0,0,0
#define BLUE 0,0,255
#define PINK 6,0,4
#define OLIVE 2,2,0
#define YELLOWGREEN 20,30,20
#define RED 50,0,0
#define ORANGE 50,30,0
#define YELLOW 30,50,0
#define GREEN 0,50,0
#define BLUE 0,0,255
#define PURPLE 10,0,128
#define BB_BLUE 0,100,255
#define BB_SAND 194,178,128

// DONT CHANGE
#define NUMLEDS 6
#define MORSE_CODE_PIN PIN_PA7
// LED positions
#define LT 1
#define LL 0
#define LB 2
#define LR 3
#define RL 4
#define RR 5

byte pixels[NUMLEDS * 3];
tinyNeoPixel strip = tinyNeoPixel(NUMLEDS, PIN_PA3, NEO_GRB, pixels);

uint16_t time_pin_low(uint16_t max_ms)
{
  // blocking for up to max_ms
  if (digitalRead(MORSE_CODE_PIN) == HIGH)
  {
    return(0);
  }
  delay(50); //debounce-
  uint16_t t = 50;
  while(digitalRead(MORSE_CODE_PIN) == LOW)
  {
    delay(5);
    t = t + 5;
    if ( t > max_ms )
      return(max_ms);
  }
  return(t);
}

void setAllPixels(int r, int g, int b, bool show = false)
{
  for (int i = 0; i < NUMLEDS; i++) 
  {
    strip.setPixelColor(i,r,g,b);
  }
  if(show)
    strip.show();
}

int fill(int x, int r, int g, int b)
{
  int step = x % 6;
  switch(step)
  {
    case 0:
      strip.setPixelColor(RR, r,g,b);
      break;
    case 1:
      strip.setPixelColor(RL, r,g,b);
      break;
    case 2:
      strip.setPixelColor(LR, r,g,b);
      break;
    case 3:
      strip.setPixelColor(LB, r,g,b);
      break;
    case 4:
      strip.setPixelColor(LT, r,g,b);
      break;
    case 5:
      strip.setPixelColor(LL, r,g,b);
      break;
    default:
      break;
  }
  strip.show();
  return 100;
}

#include <avr/sleep.h>

void sleep()
{
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  PORTA.PIN7CTRL = PORT_PULLUPEN_bm | PORT_ISC_LEVEL_gc; // enable pullup and interrupt
  digitalWrite(PIN_PA1, LOW); // turn off LED power rail
  sleep_enable();
  sleep_cpu();
  // sleep resumes here
  PORTA.PIN7CTRL = PORT_PULLUPEN_bm; // renable pullup but no interrupt
  digitalWrite(PIN_PA1, HIGH);  // turn on LED power rail
}

ISR(PORTA_PORT_vect) {
  PORTA.INTFLAGS = PORT_INT7_bm; // Clear Pin 7 interrupt flag otherwise keep coming back here
}

uint8_t state;

void setup()
{
  // Power save
  // http://www.technoblogy.com/show?2RA3
  // https://github.com/SpenceKonde/megaTinyCore/blob/master/megaavr/extras/PowerSave.md
  ADC0.CTRLA &= ~ADC_ENABLE_bm; // Disable ADC
  pinMode(PIN_PA1, OUTPUT);
  pinMode(PIN_PA2, OUTPUT);
  pinMode(PIN_PA6, OUTPUT);
  // UPDI does not need setting (PA0)

  // Pin setup
  pinMode(PIN_PA3, OUTPUT);
  pinMode(PIN_PA7, INPUT_PULLUP);
  digitalWrite(PIN_PA1, HIGH);

  
  state = EEPROM.read(0);
  if (state == B11111111)
  {
    EEPROM.update(0, 0);
    state = EEPROM.read(0);
  }
  
}

int circle(int x, int r, int g, int b)
{
  int i = x % 7;
  setAllPixels(OFF);
  switch(i) {
   case 0:
    strip.setPixelColor(LB, OFF);
    strip.setPixelColor(LL, r, g, b);
    break;
   case 1:
    strip.setPixelColor(LL, OFF);
    strip.setPixelColor(LT, r, g, b);
    break;
   case 2:
    strip.setPixelColor(LT, OFF);
    strip.setPixelColor(RL, r, g, b);
    break;
   case 3:
    strip.setPixelColor(RL, OFF);
    strip.setPixelColor(RR, r, g, b);
    break;
   case 4:
    strip.setPixelColor(RR, OFF);
    strip.setPixelColor(RL, r, g, b);
    break;
   case 5:
    strip.setPixelColor(RL, OFF);
    strip.setPixelColor(LR, r, g, b);
    break;
   case 6:
    strip.setPixelColor(LR, OFF);
    strip.setPixelColor(LB, r, g, b);
    break;
  }
  strip.show();
  return 100;
}

int random_cycle(int n)
{
  int i = random(0,6);
  switch(i) {
   case 0:
    setAllPixels(RED,true);
    break;
   case 1:
    setAllPixels(ORANGE,true);
    break;
   case 2:
    setAllPixels(YELLOW,true);
    break;
   case 3:
    setAllPixels(GREEN,true);
    break;
   case 4:
    setAllPixels(BLUE,true);
    break;
   case 5:
    setAllPixels(PURPLE,true);
    break;
  }
  return 150;
}

int cycle(int n)
{
  int i = n % 5;
  switch(i) {
   case 0:
    setAllPixels(RED,true);
    break;
   case 1:
    setAllPixels(GREEN,true);
    break;
   case 2:
    setAllPixels(ORANGE,true);
    break;
   case 3:
    setAllPixels(PURPLE,true);
    break;
   case 4:
    setAllPixels(BLUE,true);
    break;
  }
  return 300;
}

int bournemouth(int n)
{
  int i = n % 9;
  setAllPixels(BB_SAND);

  if (i == 1)
  {
    strip.setPixelColor(LT, BB_BLUE);
  }
    else if (i == 2)
  {
    strip.setPixelColor(LT, BB_BLUE);
    strip.setPixelColor(LL, BB_BLUE);
    strip.setPixelColor(LR, BB_BLUE);
  }
    else if (i == 3 || i == 4)
  {
    strip.setPixelColor(LT, BB_BLUE);
    strip.setPixelColor(LL, BB_BLUE);
    strip.setPixelColor(LR, BB_BLUE);
    strip.setPixelColor(RL, BB_BLUE);
    strip.setPixelColor(LB, BB_BLUE);
  }
      else if (i == 5)
  {
    strip.setPixelColor(LT, BB_BLUE);
    strip.setPixelColor(LL, BB_BLUE);
    strip.setPixelColor(LR, BB_BLUE);
    strip.setPixelColor(RL, BB_BLUE);
  }
  else if (i == 6)
  {
    strip.setPixelColor(LT, BB_BLUE);
    strip.setPixelColor(LL, BB_BLUE);
  }
  else
  {
    strip.setPixelColor(LT, BB_BLUE);
  }
  strip.show();
  return 500;
}

int green(int n)
{
  int i = n % 4;
  setAllPixels(OFF);

  if (i == 0)
  {
    strip.setPixelColor(LT, GREEN);
    strip.setPixelColor(LL, GREEN);
  }
    else if (i == 1)
  {
    strip.setPixelColor(LR, GREEN);
    strip.setPixelColor(LB, GREEN);
  }
    else if (i == 2)
  {
    strip.setPixelColor(RL, GREEN);
  }
      else if (i == 3)
  {
    strip.setPixelColor(RR, GREEN);
  }
  strip.show();
  return 100;
}

int police2(int n)
{
  int i = n % 2;
  setAllPixels(RED);

  if (i == 0)
  {
    strip.setPixelColor(LT, BLUE);
    strip.setPixelColor(LL, BLUE);
    strip.setPixelColor(RL, BLUE);
  }
    else if (i == 1)
  {
    strip.setPixelColor(LR, BLUE);
    strip.setPixelColor(LB, BLUE);
    strip.setPixelColor(RR, BLUE);
  }
  strip.show();
  return 200;
}

int fill_cycle(int x)
{
  int color_step = x % 31;
  switch (color_step)
  {
    case 0 ... 5:
      return fill(color_step,RED);
      break;    
    case 6 ... 11:
      return fill(color_step,GREEN);
      break;
    case 12 ... 17:
      return fill(color_step,ORANGE);
      break;
    case 18 ... 23:
      return fill(color_step,PURPLE);
      break;
    case 24 ... 29:
      return fill(color_step,BLUE);
      break;  
  }
  return 0;
}

int punk(int x)
{
  int color_step = (x % 11) + 1;
  strip.setPixelColor(LL,BLUE);
  strip.setPixelColor(LT,PURPLE);
  strip.setPixelColor(LB,PURPLE);
  strip.setPixelColor(LR,ORANGE);
  strip.setPixelColor(RL,GREEN);
  strip.setPixelColor(RR,RED);
  strip.setBrightness(200 + (48 / color_step));
  strip.show();
  return 10;
}


int fireeye(int x)
{
  setAllPixels(OFF);
  int color_step = (x % 11) + 1;
  strip.setPixelColor(LB,random(0,100), random(0,10), 0);
  strip.setPixelColor(RR,random(0,100), random(0,10), 0);
  strip.show();
  return 10;
}


void loop()
{
  int mode = 0;
  int interval;
  uint16_t button_low_time = 0;
  uint32_t total_interval = 0;
  strip.begin();
  int i = 0;
  while(true)
  {
    strip.setBrightness(255);
    if ( mode == 0 )
    {
      interval = bournemouth(i);
    }
    else if ( mode == 1 )
    {
      interval = green(i);
    }
    else if ( mode == 2 )
    {
      interval = fill_cycle(i);
    }
    else if ( mode == 3 )
    {
      interval = fireeye(i);
    }
    else if ( mode == 4 )
    {
      interval = cycle(i);
    }
    else if ( mode == 5 )
    {
      interval = punk(i);
    }
    else if ( mode == 6 )
    {
      interval = police2(i);
    }
    else if ( mode == 7 )
    {
      interval = circle(i, 255,0,0);
    }
    else
    {
      mode = (mode +1) % 8;
      continue;
    }
    i++;
    // This section breaks down the sleep interval to catch button presses
    total_interval = total_interval + interval;
    while(interval > 0)
    {
      delay(10);
      interval = interval - 10;
      button_low_time = time_pin_low(4000);
      if (button_low_time > 200)
      {
       /*
       * MAIN MENU
       * NO PRESS = CONTINUE
       * SHORT PRESS = CHANGE FLASHY MODE
       * LONG PRESS = SLEEP
       */
        if (button_low_time == 4000)
        {
            digitalWrite(PIN_PA1, LOW);
            while(digitalRead(MORSE_CODE_PIN) == LOW)
              { delay(5); }
          delay(40);
          sleep();
        }
        else
        {
          mode = (mode +1) % 8;
        }
        // reset timer
        i = 0;
        total_interval = 0;
      }

    }
    // At the end of each interval, see if we need to sleep
    if (total_interval > WAKE_TIME_MS)
    {
      total_interval = 0;
      i = 0;
      sleep();
    }
  }
}
