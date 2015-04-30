/*
	..
*/
#ifndef LEDStrip_h
#define LEDStrip_h

#include "Arduino.h"

typedef struct Color {
	unsigned char r; // red
	unsigned char g; // green
	unsigned char b; // blue
	unsigned char a; // alpha
} Color;

class LEDStrip {
	public:
		LEDStrip(int redPin, int greenPin, int bluePin);

		const Color WHITE = {255,255,255,255};
		const Color BLACK = {0,0,0,0};
		const Color RED = {255,0,0,255};
		const Color GREEN = {0,255,0,255};
		const Color BLUE = {0,0,255,255};

		const byte FADE_SPEED_SLOW = 2;
		const byte FADE_SPEED_NORMAL = 5;
		const byte FADE_SPEED_FAST = 10;
		
		Color currentColor = BLACK;					// The color that is currently set
		Color targetColor = BLACK;					// The color that should be faded to
		Color lastManuallySetColor = WHITE;			// The last (non-black) color that the user set
		
		boolean masterOn = false;
		boolean shouldFadeToColor = false;
		byte fadeSpeed = FADE_SPEED_NORMAL;

		void setColor(Color color);
		Color dimColor(Color color);
		void turnOn();
		void turnOff();
		void fadeToColor(Color color);
		void fadeToColor(Color color, byte speed);
		void fadeToColorStep();
		boolean isLightningUp();
		boolean willCreateLight(Color color);
		boolean isSameColor(Color color1, Color color2);
		
	private:
		int _redPin;
		int _greenPin;
		int _bluePin;

		Color getNextFadeColor();
		Color nearColorTo(Color color1, Color color2, byte fadeSpeed);
		byte getStepSize(unsigned char colorValue1, unsigned char colorValue2, byte fadeSpeed, unsigned char maximumDifference);
		byte getMaximumDifference(Color color1, Color color2);
		byte getDifference(unsigned char colorValue1, unsigned char colorValue2);
		byte nearValue(byte from, byte to, byte steps);
};

#endif