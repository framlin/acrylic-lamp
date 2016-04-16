#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN 6
#define PIXEL_COUNT 8

#define COLOR_1  strip.Color(255, 255, 0)
#define COLOR_2  strip.Color(255, 0, 255)
#define COLOR_3  strip.Color(0, 0, 255)
#define COLOR_4  strip.Color(255, 0, 0)
#define COLOR_5  strip.Color(0, 255, 0)
#define COLOR_6  strip.Color(0, 255, 255)
#define COLOR_7  strip.Color(255, 255, 255)
#define COLOR_8  strip.Color(255, 128, 64)

#define C_RED Adafruit_NeoPixel::Color(255, 0, 0)
#define C_YELLOW Adafruit_NeoPixel::Color(255, 255, 0)
#define C_WHITE Adafruit_NeoPixel::Color(255, 255, 255)
#define C_BLUE Adafruit_NeoPixel::Color(0, 0, 255)

const int FADE_SPEED = 6;


// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIN, NEO_GRB + NEO_KHZ800);


uint32_t colorMix[8] = {COLOR_1, COLOR_2, COLOR_4, COLOR_4, COLOR_5, COLOR_6, COLOR_7, COLOR_8};
uint32_t colorBuffer[8];





void fullColor(uint32_t color) {
  uint32_t lastColor = 0;
  uint32_t mixedColor = 0;
  
  for (uint16_t i=0; i<strip.numPixels(); i++) {
    lastColor = strip.getPixelColor(i);
    mixedColor = lastColor | color;
    strip.setPixelColor(i, mixedColor);
    strip.show();
    delay(10);
    strip.setPixelColor(i, color);
    strip.show();
    delay(10);
  }

  
}


void mixedColor(uint32_t *color) {
	  uint32_t lastColor = 0;
	  uint32_t mixedColor = 0;

	  for (uint16_t i=0; i<strip.numPixels(); i++) {
/*
	    lastColor = strip.getPixelColor(i);
	    mixedColor = lastColor | color[i];
	    strip.setPixelColor(i, mixedColor);
	    strip.show();
	    delay(10);
*/
		  /*
	    strip.setPixelColor(i, color[i]);
	    strip.show();
	    //delay(10);
	     */

		  fade_to_color(&strip, i, color[i]);

	  }

}

uint32_t* mixColor(uint8_t start) {
	uint8_t i = 0;

	for (i = 0; i < PIXEL_COUNT; i++) {
		colorBuffer[i] = colorMix[(i+start) % PIXEL_COUNT];
	}

	return colorBuffer;
}


void setup() {
  strip.begin();
  strip.setBrightness(255);
  strip.show(); // Initialize all pixels to 'off'
}

uint8_t colorIndex = 0;

void loop() {
	colorIndex = (colorIndex +1) % PIXEL_COUNT;
	mixedColor(mixColor(colorIndex));
	//delay(1000);
}


//===============================================================================
//-------------------------------------------------------------------------------

void fade_to_color(Adafruit_NeoPixel *pxls, int num, uint32_t to_color) {
    uint32_t curr_color = pxls->getPixelColor(num);

        uint8_t
          rf = (uint8_t)(curr_color >> 16),
          gf = (uint8_t)(curr_color >>  8),
          bf = (uint8_t)curr_color;

        uint8_t
          rt = (uint8_t)(to_color >> 16),
          gt = (uint8_t)(to_color >>  8),
          bt = (uint8_t)to_color;

          fade_pixel(pxls, num, rf, gf, bf, rt, gt, bt);

}

void fade_to_colors(Adafruit_NeoPixel *pxls, uint32_t to_color, int leds[], int sze) {
    for (int i = 0; i < sze; i++) {
        fade_to_color(pxls, leds[i], to_color);
    }

}

void show_pixel(Adafruit_NeoPixel *pxls, int num, uint32_t color) {
    pxls->setPixelColor(num, color);
    pxls->setBrightness(255);
    pxls->show();
}


void fade_pixel(Adafruit_NeoPixel *pxls, int num, uint8_t rf, uint8_t gf, uint8_t bf, uint8_t rt, uint8_t gt, uint8_t bt) {
    int rgap = rt - rf,
        ggap = gt - gf,
        bgap = bt - bf,
        r = rf,
        g = gf,
        b = bf;
    int incr = rgap == 0 ? 0 : ((rgap > 0) ? 1 : -1),
        incg = ggap == 0 ? 0 : ((ggap > 0) ? 1 : -1),
        incb = bgap == 0 ? 0 : ((bgap > 0) ? 1 : -1);

        //255,0,0 -> 255,255,0  |||  255,255,0 -> 255,0,0
        //255,255,255 -> 255,255,0
        while ((rgap != 0) || (ggap != 0) || (bgap != 0)) {
            if (rgap != 0) {
                r = r + incr*1;
                rgap = rt - r;
            }

            if (ggap != 0) {
                g = g + incg*1;
                ggap = gt - g;
            }

            if (bgap != 0) {
                b = b + incb*1;
                bgap = bt - b;
            }
            show_pixel(pxls, num, pxls->Color(r, g, b));
            delay(FADE_SPEED);
        }
}
