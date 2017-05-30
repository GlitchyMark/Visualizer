#include "Led.h"



Led::Led()
{
	red = 0;
	green = 0;
	blue = 0;
}

void Led::setID(int ID)
{
	id = ID;
}

sf::Color Led::getColor()
{
	return sf::Color(red, green, blue);
	//return sf::Color(255, 255, 255);
}

Led::~Led()
{
}
