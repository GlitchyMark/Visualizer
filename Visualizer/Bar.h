#pragma once
#include "Led.h"
#include <SFML/Graphics.hpp>

class Bar
{
public:
	Bar();
	~Bar();
	Led * getLeds();
	void tick();
	void setID(int ID);
	void addHeight(int h);
	int height = 10;
	int id;
	sf::Color getColor();
};

