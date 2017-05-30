#pragma once
#include "Led.h"
#include <SFML/Graphics.hpp>
#include <iostream>

using namespace sf;
using namespace std;

class Bar
{
public:
	Bar();
	~Bar();
	vector<Led> getLeds();
	void drawLeds(sf::RenderWindow & window);
	void tick();
	void setID(int ID);
	void addHeight(int h);
	int height = 10;
	int id;
	sf::Color getColor();
};
