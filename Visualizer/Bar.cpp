#include "Bar.h"


Bar::Bar()
{
}


Bar::~Bar()
{
}

void Bar::tick()
{
	height-=75;
	if (height < 10)
		height = 10;
}
void Bar::addHeight(int h)
{
	if (h > height)
		height = h;
}