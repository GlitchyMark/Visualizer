#pragma once

//Variables
struct Variables {
	int barCount = 32;
	float maxHeight= 700;
	float give = 100;
	float lowRange = 3 + .8;
	float highRange = 3 + .75;
	float sensivity = 500;
	float divisor = 10000000;
};
extern Variables vars;