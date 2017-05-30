#include "Bar.h"


float mH = 1000;

Bar::Bar()
{
}
Bar::~Bar()
{
}
vector<Led> Bar::getLeds()
{
	vector<Led> led;
	bool done = false;

	for (int i = 0; i < 8; i++)
	{
		led.push_back(Led());
		led[i].setID(i);
	}
	for (int i = 0; i < 8 && !done ; i++)
	{
		//led[i].red = (int)(((float)height) / mH * 255)*i / 8;
		float grn = height / mH;
		grn = grn*255*(8-i);
		if (grn > 255)
			grn = 255;
		else
			done = true;

		//255*8/i wasn't working for some reason...
		float rd = 255;
		rd /= (8);
		rd *= i;
		//std::cout << i << ": " << to_string(rd) << std::endl;
		if (done)
		{
			rd *= (grn/255);
		}
		grn = grn - rd;
		led[i].green = grn;
		led[i].red = rd;
		if (grn < 255)
			led[i].blue = grn;
		else
			led[i].blue = 255;
	}
	return led;

}
/**
*Each led is 8+32 bytes
*Each row is 40*8 bytes
*/
void Bar::printLedsByte()
{
	cout << "Row: " << id << endl;
	vector<Led> leds = getLeds();
	for (int i = 0; leds.size() > i; i++)
	{
		(leds[i].id << leds[i].getColor().toInteger());
	}
}
void Bar::printLeds()
{

	cout << "Row: " << id << endl;
	vector<Led> leds = getLeds();
	for (int i = 0; leds.size() > i; i++)
	{
		cout << "Led: "<< to_string(leds[i].id) << " RGB:" << to_string(leds[i].getColor().toInteger()) << endl;
	}
}

void Bar::drawLeds(sf::RenderWindow &window)
{
	int led_size = 5;
	vector<Led> leds = getLeds();
	for (int i = 0; leds.size() > i; i++)
	{
		sf::RectangleShape rectangle(sf::Vector2f(led_size, led_size));
		rectangle.setPosition((led_size + 10) * id + 0, 200 -(led_size + 10) * i);
		rectangle.setFillColor(leds[i].getColor());
		window.draw(rectangle);
	}
	//std::cout << "Writing for: " << id << std::endl;
}

void Bar::tick()
{
	height-=20;
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
