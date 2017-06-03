#pragma once
#include "Led.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include "global.h"
#include "Communicator.h"

using namespace sf;
using namespace std;

class Bar
{
public:
	int maxHeight = 700;
	Bar();
	~Bar();
	vector<Led> getLeds();
	vector<byte> getLedsByte();
	void printLeds();
	void writeLeds(Communicator com);
	void drawLeds(sf::RenderWindow & window);
	void tick();
	void setID(int ID);
	void addHeight(int h);
	int minHeight = 0;
	int height = minHeight;
	int id;
	sf::Color getColor();
};

