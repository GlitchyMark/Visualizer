

#include <iostream>
#include <vector>
#include <list>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Bar.h"
#include "FFT.h"

const int led_size = 100;

const int barCount = 8;//4 Sides, 8 Bars each Side - Should be 32, but more for fun
const int rectWidth = 5;
const int rectDist = 3;

const int width = 1200;
const int height = 800;
const int give = 9990;
sf::Keyboard::Key levels[10]{ sf::Keyboard::A,sf::Keyboard::S,sf::Keyboard::D,sf::Keyboard::F,sf::Keyboard::G,sf::Keyboard::H,sf::Keyboard::J,sf::Keyboard::K,sf::Keyboard::L, sf::Keyboard::SemiColon };

//Create Bar
sf::RectangleShape createRect(int number, float r_height)
{
	sf::RectangleShape rectangle(sf::Vector2f(rectWidth, r_height));
	rectangle.setPosition((rectWidth + rectDist) * number, height - r_height);
	rectangle.setFillColor(sf::Color::Green);
	return rectangle;
}



//Create hight of the bar
int newHeight(int num, sf::Vector2f vec)
{
	float hgt;
	float newx = (float)vec.x;
	int newy = vec.y;
	hgt = -powf(num - newx, 2) * give + newy;
	if (hgt < 0) hgt = 0;
	return hgt;
}

int newHeightMouse(int num, sf::Vector2i vec)
{
	float hgt;
	float newx = (float)vec.x / (rectWidth + rectDist) - 1;
	int newy = -vec.y + height;
	hgt = newHeight(num, sf::Vector2f(newx, newy));

	return hgt;
}
//Add a wave tremor
void waveBoop(Bar* bars, float x, float y)
{
	for (int i = 0; barCount > i; i++)
		bars[i].addHeight(newHeight(i, sf::Vector2f(x, y)));
}
//Add a wave tremor for mouse
void waveBoopMouse(Bar* bars, int x, int y)
{
	for (int i = 0; barCount > i; i++)
		bars[i].addHeight(newHeightMouse(i, sf::Vector2i(x, y)));
}
int main()
{
	sf::RenderWindow window(sf::VideoMode(width, height), "Bar Visualizer");

	window.setFramerateLimit(1000);

	//Initialize Mic Recording
	FFT fft("", 16384);

	//Create Bars
	Bar * bars;
	bars = new Bar[barCount];

	for (int i = 0; barCount > i; i++)
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
	int ticks = 0;

	while (window.isOpen())
	{
		sf::Event event;
		//Check if window is still open
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		//Update AudioStream Values
		fft.update();

		//Set debug text
		text.setString(fft.dbgp);

		window.clear();

		for (int i = 0; i < barCount; i++)
			bars[i].tick();
		//Tremor at mouse location
		/*waveBoopMouse(bars, sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);
		for (int i = 0; sizeof(levels) / sizeof(sf::Keyboard::Key) > i; i++)
		{
			if (sf::Keyboard::isKeyPressed(levels[i]))
				waveBoop(bars, i * 8, 500);
		}*/

		//Tremor Bars by audio level
		vector<int> aryBar = fft.getBarValues(barCount);
		for(int i = 0; i < aryBar.size(); i++)
		waveBoop(bars, i, aryBar[i]/100);


		//Draw the Bars
		for (int i = 0; i < barCount; i++)
		{
			sf::RectangleShape rs = createRect(i, bars[i].height);
			rs.setFillColor(bars[i].getColor());
			window.draw(rs);
		}

		window.draw(text);
		window.display();
	}

	return 0;
}
