#include "Bar.h"



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

	for (int i = 0; i < vars.ledHeight; i++)
	{
		led.push_back(Led());
		if (id % 2 == 0)
		{
			led[i].setID(i + vars.ledHeight*id);
		}
		else
		{
			led[i].setID(vars.ledHeight*(id+1)-i-1);
		}
	}
	for (int i = 0; i < vars.ledHeight && !done ; i++)
	{
		//led[i].red = (int)(((float)height) / mH * 255)*i / 8;
		float grn = height / vars.maxHeight;
		grn = grn*255*(vars.ledHeight -i);
		if (grn > 255)
			grn = 255;
		else
			done = true;

		float rd = 255*i/ vars.ledHeight;
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
		if (led[i].red > 254) led[i].red = 254;
		if (led[i].green > 254) led[i].green = 254;
		if (led[i].blue > 254) led[i].blue = 254;
	}

	return led;
}
/**
*Each led is 8+32 bytes
*Each row is 40*8 bytes
*/
vector<char> Bar::getLedsByte()
{
	vector<char> bytes = vector<char>();
	vector<Led> leds = getLeds();
	bytes.clear();
	for (int i = 0; leds.size() > i; i++)
	{
		bytes.push_back(255);
		bytes.push_back(leds[i].id);
		bytes.push_back(leds[i].red);
		bytes.push_back(leds[i].green);
		bytes.push_back(leds[i].blue);
		//bytes.push_back(':');
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
