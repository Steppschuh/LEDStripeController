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
	// reaclculate rgb values based on alpha value
	Color newColor = color;
	newColor.r = (newColor.r * newColor.a) / 255;
	newColor.g = (newColor.g * newColor.a) / 255;
	newColor.b = (newColor.b * newColor.a) / 255;
	return newColor;
}

void LEDStrip::turnOn() {
	masterOn = true;
	setColor(WHITE);
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

Color LEDStrip::nearColorTo(Color color1, Color color2, byte steps) {
	Color newColor = color1;
	newColor.r = nearValue(color1.r, color2.r, steps);
	newColor.g = nearValue(color1.g, color2.g, steps);
	newColor.b = nearValue(color1.b, color2.b, steps);
	newColor.a = nearValue(color1.a, color2.a, steps);
	return newColor;
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