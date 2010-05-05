#include "Sampler.h"

Sampler::Sampler(int sizex, int sizey)
{
	mySizex = sizex;
	mySizey = sizey;
	totalPixels = mySizex * mySizey;
	curr = 0;
	tenPercent = totalPixels / 10;
	level = 1;
	toMilestone = 0;
	
}

Sampler::~Sampler(void)
{
}

bool Sampler::getSample(Sample *s) {
	if(curr < totalPixels) {
		s->x = (float) (curr % mySizex);
		s->y = (float) (curr / mySizex);
		
		curr ++;
		printProgress();
		return true;
	} else {
		printf("Done!\n");
	}
	return false;
}

void Sampler::printProgress() {
	if(toMilestone == tenPercent) {
		printf("Progress: %d Percent\n", level*10);
		toMilestone = 0;
		level++;
	}
	toMilestone++;
}