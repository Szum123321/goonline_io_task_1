#include <Arduino.h>
#include <Wire.h>

#include "PCA9633.h"
#include "config.h"

struct Color{
	uint8_t red, green, blue, white;
}__attribute__((packed));

void transition_brightness(const int time) {
	int dt = time / 256;
	for(int i = 0; i < 256; i++) {
		Wire.beginTransmission(DEVICE_ADDRESS);
		Wire.write(REG_GRPPWM);
		Wire.write((uint8_t)i);
		Wire.endTransmission();
		delay(dt);
	}
}

void set_color(Color c) {
	Wire.beginTransmission(DEVICE_ADDRESS);
	Wire.write(REG_PWM0);
	Wire.write((uint8_t*)(void*)&c, 4);
	Wire.endTransmission();
}

void setup() {
	Wire.begin();

	//Software reset all devices, look for ack
	Wire.beginTransmission(SWRST_ADDR);
	Wire.write(SWRST_MAGIC1);
	Wire.write(SWRST_MAGIC2);
	if(!Wire.endTransmission()) {
		//Raise Exception!
		while(1);
	}

	//Clear all leds
	Wire.beginTransmission(DEVICE_ADDRESS);
	Wire.write(ADDR_AI2_bm | REG_PWM0);
	for(int i = 0; i < 4; i++); //set all LEDS to 0
		Wire.write(0);
	Wire.write(255); //set group DTC to 100%
	Wire.endTransmission();

//bring up red
	set_color({255, 0, 0, 0});
	transition_brightness(2000);

//green
	set_color({0, 255, 0, 0});
	transition_brightness(2000);

//blue
	set_color({0, 0, 255, 0});
	transition_brightness(2000);

//white
	set_color({0, 0, 0, 255});
	transition_brightness(2000);
}

//indefinitely loop rainbow
void loop() {
	for(uint8_t i = 0; i < 255; i++) {
		set_color({(uint8_t)(255 - i), i, 0, 255});
		delay(1);
	}
	for(uint8_t i = 0; i < 255; i++) {
		set_color({0, (uint8_t)(255 - i), i, 255});
		delay(1);
	}
	for(uint8_t i = 0; i < 255; i++) {
		set_color({i, 0, (uint8_t)(255 - i), 255});
		delay(1);
	}
}