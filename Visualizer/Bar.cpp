#include "Bar.h"


float mH = 100000;

Bar::Bar()
{
}

Bar::~Bar()
{
}

vector<Led> Bar::getLeds()
{
	vector<Led> led;
	for (int i = 0; i < 8; i++)
	{
		led.push_back(Led());
		led[i].setID(i);
		led[i].red = (int)(((float)height) / mH * 255)*i / 8;
		led[i].green = (int)(255 - (((float)height) / mH * 255)*i / 8);
	}
	return led;

}

void Bar::tick()
{
	height-=999999;
	if (height < 10)
		height = 10;
}
void Bar::addHeight(int h)
{
	if (h > height)
		height = h;
}

sf::Color Bar::getColor()
{
	sf::Color color(((float)height)/mH*255, 255- (((float)height) /mH*255), 0);
	return color;
}


void Bar::setID(int ID)
{
	id = ID;
}
