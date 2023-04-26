#include <Arduino.h>
#include <Wire.h>

#include "PCA9633.h"
#include "config.h"

struct Color {
	uint8_t red, green, blue, white;
} __attribute__((packed)); //enforce strict byte packing

void fade_in_global() { //Increment global duty cycle register
	for(int i = 0; i < 256; i++) {
		Wire.beginTransmission(DEVICE_ADDRESS);
		Wire.write(REG_GRPPWM);
		Wire.write((uint8_t)i); 
		Wire.endTransmission();
		delay(DT);
	}
}

//This function sets the color of the LED, by changing PWM register. Global PWM duty cycle register is not altered
//Returned values represent the Bus status. 0 means success
int set_color(Color c) {
	Wire.beginTransmission(DEVICE_ADDRESS);
	Wire.write(ADDR_AI2_bm | REG_PWM0); //auto-increment pointer starting with PWM0 register
	Wire.write((uint8_t*)(void*)&c, 4); //Send four bytes representing colors
	return Wire.endTransmission();
}

void setup() {
	Wire.begin();
	Wire.setClock(1000000); //Use highest available bus frequency
	//Allows for about ~20k set_color calls per second

	//Software reset all devices, look for ack
	Wire.beginTransmission(SWRST_ADDR);
	Wire.write(SWRST_MAGIC1);
	Wire.write(SWRST_MAGIC2);
	if(!Wire.endTransmission()) {
		//Raise Exception, Device didn't respond.
		while(1);
	}

	delay(10); //Let PCA reset

	//Setup devices
	Wire.beginTransmission(DEVICE_ADDRESS);
	Wire.write(ADDR_AI2_bm | REG_MODE1); 
	Wire.write(0); //MODE 1 register: Enable oscillator, disable subaddress, disable ALLCALL address
	Wire.write(MODE2_OUTDRV_bm | MODE2_OUTNE1_bm); //MODE 2 register: Group Control = dimming, Don't invert output, Totem pole driver, High impedance when OE = 1
	Wire.endTransmission();

	delay(10); //Wait for Oscillator to start

	//Clear all leds
	Wire.beginTransmission(DEVICE_ADDRESS);
	Wire.write(ADDR_AI2_bm | REG_PWM0);
	for(int i = 0; i < 4; i++); //set all LEDS to 0
		Wire.write(0);
	Wire.write(255); //set group DTC to 100%
	Wire.endTransmission();

//bring up red
	set_color({255, 0, 0, 0});
	fade_in_global();

//green
	set_color({0, 255, 0, 0});
	fade_in_global();

//blue
	set_color({0, 0, 255, 0});
	fade_in_global();

//white
	set_color({0, 0, 0, 255});
	fade_in_global();
}	

//indefinitely loop rainbow, with white at 50% all the time
void loop() {
	for(uint8_t i = 0; i < 255; i++) { //Red -> Blue
		set_color({(uint8_t)(255 - i), 0, i, 127});
		delay(DT);
	}
	for(uint8_t i = 0; i < 255; i++) { //Blue -> Green
		set_color({0, i, (uint8_t)(255 - i), 127});
		delay(DT);
	}
	for(uint8_t i = 0; i < 255; i++) { //Green -> Red
		set_color({i, 0, (uint8_t)(255 - i), 127});
		delay(DT);
	}
}