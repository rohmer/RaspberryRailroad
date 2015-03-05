#include "max7219Container.h"

max7219Container::max7219Container()
{
	Clear();
}

void max7219Container::Draw(int row, int col, bool powerMode)
{
	char val;
	if (powerMode)
	{
		val = '1';
	}
	else
	{
		val = '0';
	}
	int pt = (row - 1) * 10 + (col - 1) + 2;
	data[pt] = val;
}

void max7219Container::Clear()
{
	for (int a = 1; a <= 8; a++)
	{
		//Add row beginning
		data[(a - 1) * 10] = '0';
		data[(a - 1) * 10 + 1] = 'b';
		for (int b = 0; b < 8; b++)
		{
			data[(a - 1) * 10 + 2 + b] = '0';
		}			
	}
}
