#pragma once
class Led
{
public:
	Led();
	~Led();
	int id;
	int red = 0;
	int green = 0;
	int blue = 0;
	void setID(int ID);
};

