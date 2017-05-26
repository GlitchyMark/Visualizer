#include "Bar.h"


float mH = 800;

Bar::Bar()
{
}

Bar::~Bar()
{
}

Led* Bar::getLeds()
{
	Led *led;
	led = new Led[8];
	for (int i = 0; i < 8; i++)
	{
		led[i].red = (int)(((float)height) / mH * 255)*i / 8;
		led[i].green = (int)(255- (((float)height) /mH*255)*i / 8);
	}

}
void Bar::tick()
{
	height-=5;
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
