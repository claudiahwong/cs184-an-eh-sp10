#pragma once
#include "Sample.h"
#include <stdio.h>
#include <stdlib.h>
#include <Math.h>
#include <time.h>

class Sampler
{
public:
	int mySizex, mySizey, totalPixels;
	int curr;
	int tenPercent, toMilestone, level;

	Sampler(int sizex, int sizey);
	~Sampler(void);
	bool getSample(Sample *sample);
	void printProgress();
};
