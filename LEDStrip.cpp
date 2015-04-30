/*
	..
*/

#include "Arduino.h"
#include "LEDStrip.h"

LEDStrip::LEDStrip(int redPin, int greenPin, int bluePin)
{
	_redPin = redPin;
	_greenPin = greenPin;
	_bluePin = bluePin;

	pinMode(redPin, OUTPUT);
	pinMode(greenPin, OUTPUT);
	pinMode(bluePin, OUTPUT);

	turnOff();
}

void LEDStrip::setColor(Color color) {
	currentColor = color;

	// sets the rgb values as input for pins
	Color newColor = dimColor(color);
	analogWrite(_redPin, newColor.r);
	analogWrite(_greenPin, newColor.g);
	analogWrite(_bluePin, newColor.b); 
}

Color LEDStrip::dimColor(Color color) {
	// recalculate rgb values based on alpha value
	Color newColor = color;
	newColor.r = (newColor.r * newColor.a) / 255;
	newColor.g = (newColor.g * newColor.a) / 255;
	newColor.b = (newColor.b * newColor.a) / 255;
	return newColor;
}

void LEDStrip::turnOn() {
	masterOn = true;
	setColor(lastManuallySetColor);
}

void LEDStrip::turnOff() {
	masterOn = false;
	setColor(BLACK);
}

void LEDStrip::fadeToColor(Color color) {
	fadeToColor(color, FADE_SPEED_NORMAL);
}

void LEDStrip::fadeToColor(Color color, byte speed) {
	// check if color is already set
	if (isSameColor(color, currentColor)) {
		shouldFadeToColor = false;
		return;
	}

	targetColor = color;
	fadeSpeed = speed;
	shouldFadeToColor = true;

	// actual fadeing will be performed in multiple fadeToColorStep() calls
}

void LEDStrip::fadeToColorStep() {
	Color nextFadeColor = getNextFadeColor();
	setColor(nextFadeColor);

	// check if target color has been reached
	if (isSameColor(targetColor, currentColor)) {
		shouldFadeToColor = false;
		return;
	}
}

Color LEDStrip::getNextFadeColor() {
	return nearColorTo(currentColor, targetColor, fadeSpeed);
}

Color LEDStrip::nearColorTo(Color color1, Color color2, byte fadeSpeed) {
	Color newColor = color1;

	// calculate step size based on the maximum remaining difference
	unsigned char maximumDifference = getMaximumDifference(color1, color2);

	newColor.r = nearValue(color1.r, color2.r, getStepSize(color1.r, color2.r, fadeSpeed, maximumDifference));
	newColor.g = nearValue(color1.g, color2.g, getStepSize(color1.g, color2.g, fadeSpeed, maximumDifference));
	newColor.b = nearValue(color1.b, color2.b, getStepSize(color1.b, color2.b, fadeSpeed, maximumDifference));
	newColor.a = nearValue(color1.a, color2.a, getStepSize(color1.b, color2.b, fadeSpeed, maximumDifference));
	return newColor;
}

byte LEDStrip::getStepSize(unsigned char colorValue1, unsigned char colorValue2, byte fadeSpeed, unsigned char maximumDifference) {
	unsigned char currentDifference = getDifference(colorValue1, colorValue2);
	byte stepSize = round((int)(fadeSpeed * currentDifference) / maximumDifference);
	stepSize = min(fadeSpeed, stepSize);
	stepSize = max(1, stepSize);
	return stepSize;
}

unsigned char LEDStrip::getMaximumDifference(Color color1, Color color2) {
	unsigned char redDifference = getDifference(color1.r, color2.r);
	unsigned char greenDifference = getDifference(color1.g, color2.g);
	unsigned char blueDifference = getDifference(color1.b, color2.b);
	unsigned char alphaDifference = getDifference(color1.a, color2.a);

	unsigned char maximumDifference = max(redDifference, greenDifference);
	maximumDifference = max(maximumDifference, blueDifference);
	maximumDifference = max(maximumDifference, alphaDifference);

	return maximumDifference;
}

unsigned char LEDStrip::getDifference(unsigned char colorValue1, unsigned char colorValue2) {
	int difference = colorValue1 - colorValue2;
	if (difference >= 0) {
		return difference;
	} else {
		return colorValue2 - colorValue1;
	}
}

// checks if the current color is lightning up the room
boolean LEDStrip::isLightningUp() {
	if (shouldFadeToColor) {
		// use the color that will be set
		return willCreateLight(targetColor);
	} else {
		// use the color that is currently set
		return willCreateLight(currentColor);
	}
}

// checks if a given color will produce light or not
boolean LEDStrip::willCreateLight(Color color) {
	unsigned char minimumValue = 20;

	// check if alpha channel dims color
	if (color.a < minimumValue) {
		return false;
	}

	// check if at least one channel is set
	if (color.r > minimumValue || color.g > minimumValue || color.b > minimumValue) {
		return true;
	}

	return false;
}

boolean LEDStrip::isSameColor(Color color1, Color color2) {
	if (color1.r != color2.r) {
		return false;
	}
	if (color1.g != color2.g) {
		return false;
	}
	if (color1.b != color2.b) {
		return false;
	}
	if (color1.a != color2.a) {
		return false;
	}
	return true;
}

byte LEDStrip::nearValue(byte from, byte to, byte steps) {
  int near;
  if (from == to) {
    return to;
  } else if (from < to) {
    near = from + steps;
    if (near > to) {
      return to;
    } else {
      return near;
    }
  } else if (from > to) {
    near = from - steps;
    if (near < to) {
      return to;
    } else {
      return near;
    }
  }
}