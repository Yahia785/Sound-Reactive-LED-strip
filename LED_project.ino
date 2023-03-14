#include <FastLED.h>

#define DATA_PIN 6                //Digital pin for LED strip
#define NUM_LEDS 99               //Number of leds in led strip
#define MIC_PIN A0                //Analog pin for sound sensor
#define MULTIPLYING_CONSTANT 2    //Coonstant value multiplied by reading values from sound sensor to increase range
#define NOISE 6                   //Noise value (difference between readings for a quiet)
#define LOWEST 62                    //Lowest value reading when the room is quiet
#define abs(x) ((x)>0?(x):-(x))   //Getting absolute value function

//Global Variables
CRGB leds[NUM_LEDS];              //Defining array of leds
int mic_data_A = 0;               //Variable that stores analog reading from sound sensor
int count = 0;

typedef enum {BRIGHTNESS_REACTIVE, LINEAR_REACTIVE} LEDstates; //Declares states of FSM
LEDstates state = LINEAR_REACTIVE;                             //Declaring our initial state

void setup()
{
  // put your setup code here, to run once:
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  pinMode(MIC_PIN, INPUT);
  Serial.begin(9600);
}

void loop()
{
  // put your main code here, to run repeatedly:
  //Getting reading from sound sensor and multiplying it by 2
  mic_data_A = analogRead(MIC_PIN) * MULTIPLYING_CONSTANT;
 
  delay(15);
 
 //Getting another reading after a delay of 15 ms 
  int mic_data_B = analogRead(MIC_PIN) * MULTIPLYING_CONSTANT;
 
 //Getting difference between first and second reading
  int reading_difference = abs(mic_data_B - mic_data_A);

 //If the reading difference is greater than noise range enter the switch statement
  if (reading_difference >=   NOISE)
  {
    //Subtracting lowest reading for a quiet room from current reading of sound sensor. 
    int index = mic_data_B - LOWEST;

    Serial.println(index);
    
    switch (state)
    {
      case LINEAR_REACTIVE:
        reactive(index); //Linear reactive animation
        break;

      case BRIGHTNESS_REACTIVE: 
        brightness_reactive(index); //Brightness reactive animation
        break;
    }
  }

}


/* Function that lights up set of leds specified in the color specifdied. It takes
*  position of the first led that needs to be on, the position of last led needs 
*  to be on and the color of leds wanted.
*/ 
void control_leds(int start, int finish, CRGB color)
{
  for (int i = start; i < finish; i++)
  {
    leds[i] = color;
  }
}

/* Sets first 29 leds to red, leds from 40-64 to blue and leds 75-99 to red. 
*  Other LEDs are used for flashing white light. 
*/
void rgb()
{
  control_leds(0, 30, CRGB::Green);
  control_leds(40, 65, CRGB::Blue);
  control_leds(75, 100, CRGB::Red);
  FastLED.show();
}

/* When the sound sensor treshold goes over 20 brightness is increased for leds to flash
*  when the sound sensor treshold goes above 200 white light is flashed on leds 30-39 and
*  65-74
*/
void brightness_reactive(int index)
{
  rgb(); //function that sets the LED strip to three seperatee colors red green and blue.
  if (index >= 30)
  {
    FastLED.setBrightness(240);
  }
  if (index >= 700)
  {
    control_leds(30, 40, CRGB::White);
    control_leds(65, 75, CRGB::White);

  }
  FastLED.show();
  delay(90);
  FastLED.setBrightness(20);
  control_leds(30, 40, CRGB::Black);
  control_leds(65, 75, CRGB::Black);
  FastLED.show();
  
}

/* Linear reactive animation. When the sound sensor's treshold goes between 12 and 40 a set of leds are on 
*  in the color blue. The set of leds that are on depend on the index value. The higher the sound the bigger 
* the value of index therefore more leds light up. If sound sensor treshold goes between 40 and 80 first set
* of leds light up blue and second set lights up green. The number of green leds is then depending on index.
* Lastly if the sound sensor's treshold goes above 80 the first set of leds light blue, second set of leds light green
* and last set lights as red. Number of red leds depend on index here. If treshold is lower than 12 then leds should stay black. 
*/
void reactive(int index)
{
  FastLED.setBrightness(240);
  if (index >= 12 && index < 40)
  {
    blue(index);
  }
  else if (index >= 40 && index < 80)
  {
    green(index);
  }
  else if (index >= 80)
  {
    red(index); 
    
  }
  else if (index < 12)
  {
    control_leds(0, 100, CRGB::Black);
    FastLED.show();
  }
//  Serial.println(index);
}

//Function that is responsible for lighting up first set of led to blue
void blue(int index)
{
  if (index < 12)
  {
    control_leds(0, 100, CRGB::Black);
    FastLED.show();
  }

  if (index >= 14)
  {
    for (int i = 0; i < index; i++)
    {
      control_leds(0, i, CRGB::Green); //Note that the color blue is defined as green by mistake in fastLED library. 
      control_leds(i, NUM_LEDS - i, CRGB::Black);
      FastLED.show();
    }
  }
}

//Function responsible for lighting first set of leds as blue and second set as green
void green(int index)
{
  if (index >= 40)
  {
    for (int i = 40; i < index; i++)
    {
      control_leds(0, 40, CRGB::Green);
      control_leds(40, i, CRGB::Blue); //Note the color green is defined as blue by mistake in fastLED library
      control_leds(i, NUM_LEDS - i, CRGB::Black); 
      FastLED.show();
    }
  }

  if (index >= 60)
  {
    for (int i = 40; i < index; i++)
    {
      control_leds(0, 40, CRGB::Green);
      control_leds(40, i, CRGB::Blue);
      control_leds(i, NUM_LEDS - i, CRGB::Black);
      FastLED.show();
    }
  }

  if (index >= 90)
  {
    for (int i = 40; i < index; i++)
    {
      control_leds(0, 40, CRGB::Green);
      control_leds(40, i, CRGB::Blue);
      control_leds(i, NUM_LEDS - i, CRGB::Black);
      FastLED.show();
    }
  }


  if (index >= 110)
  {
    for (int i = 40; i < index; i++)
    {
      control_leds(0, 40, CRGB::Green);
      control_leds(40, i, CRGB::Blue);
      control_leds(i, NUM_LEDS - i, CRGB::Black);
      FastLED.show();
    }
  }
}

//Function responsible for lighting the first set of leds as blue, second set as green and last set as red
void red(int index)
{
  if (index >= 250)
  {
    for (int i = 80; i < index; i++)
    {
      control_leds(0, 40, CRGB::Green);
      control_leds(40, 80, CRGB::Blue);
      control_leds(80, i, CRGB::Red);
      control_leds(i, NUM_LEDS - i, CRGB::Black);
      FastLED.show();
    }
  }

  if (index >= 400)
  {
    for (int i = 80; i < index; i++)
    {
      control_leds(0, 40, CRGB::Green);
      control_leds(40, 80, CRGB::Blue);
      control_leds(80, i, CRGB::Red);
      control_leds(i, NUM_LEDS - i, CRGB::Black);
      FastLED.show();
    }
  }
}
