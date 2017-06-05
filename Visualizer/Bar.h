#pragma once
#include "Led.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include "global.h"
#include <vector>

using namespace sf;
using namespace std;

class Bar
{
public:
	int maxHeight = 700;
	Bar();
	~Bar();
	vector<Led> getLeds();
	vector<char> getLedsByte();
	void printLeds();
	void drawLeds(sf::RenderWindow & window);
	void tick();
	void setID(int ID);
	void addHeight(int h);
	int minHeight = 0;
	int height = minHeight;
	int id;
	sf::Color getColor();
};

