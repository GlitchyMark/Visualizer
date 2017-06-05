#pragma once

//Variables
struct Variables {
	int barCount = 16;
	float ledHeight = 3;
	float maxHeight= 700;
	float give = 100;
	float lowRange = 3 + .8;
	float highRange = 3 + .75;
	float sensivity = 500;
	float divisor = 10000000;
	float brightness = 0.5; //Float value between 1.0f - 0.0f
};
extern Variables vars;