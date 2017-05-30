#pragma once
#include <SFML/Graphics.hpp>
class Led
{
public:
	Led();
	sf::Color getColor();
	~Led();
	int id;
	int red = 0;
	int green = 0;
	int blue = 0;
	void setID(int ID);
};

