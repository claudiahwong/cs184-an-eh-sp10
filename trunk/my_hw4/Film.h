#pragma once
#include "Color.h"
#include "Sample.h"
#include <stdio.h>

class Film
{
protected:
	int sceneWidth, sceneHeight, currIndex;
	Sample *mySamples;
	Color *myColors;
	unsigned char *myBuff;

public:
	Film(int scenex, int sceney);
	~Film(void);

	void convertToString();
	void setColor(Sample &sample, Color &color);
	void writeImage(char filename[]);
};
