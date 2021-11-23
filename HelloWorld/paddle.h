#pragma once

#include <iostream>

struct Paddle
{
	//Refers to the centre point of the paddle you create in DrawRect.
	int x = DISPLAY_WIDTH / 2;
	int y = DISPLAY_HEIGHT - 35;
};

Paddle pad;