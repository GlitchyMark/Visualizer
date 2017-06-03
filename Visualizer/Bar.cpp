#include "Bar.h"



Bar::Bar()
{
}
Bar::~Bar()
{
}
vector<Led> Bar::getLeds()
{
	int ledHeight = 6;
	vector<Led> led;
	bool done = false;

	for (int i = 0; i < ledHeight; i++)
	{
		led.push_back(Led());
		led[i].setID(i);
	}
	for (int i = 0; i < ledHeight && !done ; i++)
	{
		//led[i].red = (int)(((float)height) / mH * 255)*i / 8;
		float grn = height / vars.maxHeight;
		grn = grn*255*(ledHeight -i);
		if (grn > 255)
			grn = 255;
		else
			done = true;

		//255*8/i wasn't working for some reason...
		float rd = 255*i/ ledHeight;
		//std::cout << i << ": " << to_string(rd) << std::endl;
		if (done)
		{
			rd *= (grn/255);
		}
		grn = grn - rd;
		led[i].green = grn;
		led[i].red = rd;
		/*if (grn < 255)
			led[i].blue = grn;
		else
			led[i].blue = 255;*/
		led[i].blue = 0;

		//Set led brightness

		led[i].green *= vars.brightness;
		led[i].red *= vars.brightness;
		led[i].blue *= vars.brightness;
	}

	return led;

}
/**
*Each led is 8+32 bytes
*Each row is 40*8 bytes
*/
vector<byte> Bar::getLedsByte()
{
	vector<byte> bytes = vector<byte>();
	vector<Led> leds = getLeds();
	for (int i = 0; leds.size() > i; i++)
	{
		bytes.push_back(leds[i].id);
		bytes.push_back(leds[i].red);
		bytes.push_back(leds[i].green);
		bytes.push_back(leds[i].blue);
	}

	/*byte *data = bytes.data();
	char buffer[16] = 0;
	for (int j = 0; j < 8; j++)
		sprintf(&buffer[2 * j], "%02X", data[j]);*/
	return bytes;
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

void Bar::writeLeds(Communicator com)
{
	vector<byte> bytes = vector<byte>();
	vector<Led> leds = getLeds();
	for (int i = 0; leds.size() > i; i++)
	{
		bytes.push_back(leds[i].id);
		bytes.push_back(leds[i].red);
		bytes.push_back(leds[i].green);
		bytes.push_back(leds[i].blue);
	}
	byte *byts = bytes.data();
	com.writeBytes(byts);
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
	height-=100;
	if (height < minHeight)
		height = minHeight;
}
void Bar::addHeight(int h)
{
	if (h > height)
		height = h;
}

sf::Color Bar::getColor()
{
	sf::Color color(((float)height)/ (float)maxHeight *255, 255- (((float)height) /(float)maxHeight *255), 0);
	return color;
}


void Bar::setID(int ID)
{
	id = ID;
}
