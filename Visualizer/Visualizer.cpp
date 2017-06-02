

#include <iostream>
#include <vector>
#include <list>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Bar.h"
#include "FFT.h"
#include "global.h"

//Variables
Variables vars;
//Visual variables
const int rectWidth = 5;
const int rectDist = 3;
const int width = 1000;
const int height = 600;

//Create Bar
sf::RectangleShape createRect(int number, float r_height)
{
	r_height = r_height / 2;
	sf::RectangleShape rectangle(sf::Vector2f(rectWidth, r_height));
	rectangle.setPosition((rectWidth + rectDist) * number, height - r_height);
	rectangle.setFillColor(sf::Color::Green);
	return rectangle;
}

//Create hight of the bar
float newHeight(int num, sf::Vector2f vec)
{
	float hgt;
	float newx = (float)vec.x;
	int newy = vec.y;
	hgt = -powf(num - newx, 2) * vars.give + newy;
	if (hgt < 0) hgt = 0;
	return hgt;
}

//Add a wave tremor
void waveBoop(Bar* bars, float x, float y)
{
	for (int i = 0; vars.barCount > i; i++)
		bars[i].addHeight(newHeight(i, sf::Vector2f(x, y)));
}

int main()
{
	sf::RenderWindow window(sf::VideoMode(width, height), "Bar Visualizer");

	window.setFramerateLimit(60);

	//Initialize Mic Recording
	FFT fft("", 16384);

	//Create Bars
	Bar * bars;
	bars = new Bar[vars.barCount];

	for (int i = 0; vars.barCount > i; i++)
	{
		bars[i] = Bar();
		bars[i].setID(i);
	}

	//Setup Font
	sf::Font font;
	std::cout << "Verdana loaded: " << (font.loadFromFile("Verdana.ttf") ? "True" : "False") << std::endl;
	sf::Text text;
	text.setFont(font);
	text.setCharacterSize(24);
	text.setFillColor(sf::Color::Red);
	text.setStyle(sf::Text::Bold | sf::Text::Underlined);

	while (window.isOpen())
	{
		sf::Event event;
		//Check if window is still open
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
				fft.Close();
			}
		}

		//Update AudioStream Values
		fft.update();

		//Set debug text
		text.setString(fft.dbgp);

		window.clear();
		
		for (int i = 0; i < vars.barCount; i++)
			bars[i].tick();

		//Tremor Bars by audio level
		vector<int> aryBar = fft.getBarValues(vars.barCount);
		for(int i = 0; i < aryBar.size(); i++)
		waveBoop(bars, i, aryBar[i]);

		//cout << "Leds serial print" << endl;
		//Draw the Bars
		for (int i = 0; i < vars.barCount; i++)
		{
			sf::RectangleShape rs = createRect(i, bars[i].height);
			rs.setFillColor(bars[i].getColor());
			window.draw(rs);
			bars[i].drawLeds(window);
			//bars[i].printLedsByte();
		}
		//cout << endl;
		
		window.draw(text);
		window.display();
	}

	return 0;
}
