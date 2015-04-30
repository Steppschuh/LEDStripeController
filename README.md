# LEDStripeController
Control LED Stripes with an Arduino

Setup
-----
To use the LED Stripe library in your own Arduino project, make sure to include it in your .ino file:
```c++
#include "LEDStrip.h"
```

Now create an instance and specify the 3 pins for RGB:
```c++
LEDStrip led(redPin, greenPin, bluePin);
```

To use the LED strip, turn it on in your setup method:
```c++
void setup() {
	led.turnOn();
}
```
This will restore the last manually set (non-black) color, by default white.

Changing colors
-----
You can use the *Color struct* to define RGBA colors (red, green, blue, alpha).
```c++
Color myFancyColor = {255,150,0,255};
```

To actually change the color of the LED strip, use the *setColor(Color newColor)* method:
```c++
led.setColor(myFancyColor);
led.setColor(led.RED); // some colors are predefined
```

You can also invoke a smooth fade on the LED strip, use the *fadeToColor(Color newColor)* method for that:
```c++
led.fadeToColor(led.RED);
led.fadeToColor(led.RED, led.FADE_SPEED_SLOW); // optional speed parameter
```
