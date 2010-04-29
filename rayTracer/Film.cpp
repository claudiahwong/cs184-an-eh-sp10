#include "Film.h"

Film::Film(int scenex, int sceney)
{
	sceneWidth = scenex;
	sceneHeight = sceney;
	currIndex = 0;
	mySamples = new Sample [sceneWidth * sceneHeight];
	myColors = new Color [sceneWidth * sceneHeight];
	myBuff = new unsigned char [3 * sceneWidth * sceneHeight + 1];
}

Film::~Film(void)
{
}

void Film::convertToString() {
	int count = 0;
	for (int i = 0; i < sceneWidth * sceneHeight; i++) {
		myBuff[i*3] = 255 * myColors[i].r;
		myBuff[i*3+1] = 255 * myColors[i].g;
		myBuff[i*3+2] =255 * myColors[i].b;
	}
}

void Film::setColor(Sample &sample, Color &color)
{
	if (currIndex < sceneWidth * sceneHeight) {
		mySamples[currIndex] = Sample(sample.x, sample.y);
		myColors[currIndex] = Color(color.r, color.g, color.b);
		currIndex++;
	}
	// else {
	//	currIndex = 0;
	//}	
}

void Film::writeImage(char filename[])
{
	FILE *out_file = fopen(filename, "wb" );
	fprintf( out_file, "P6\n%d %d\n255\n", sceneWidth, sceneHeight );

	convertToString();

	for (int i = 0; i < sceneWidth * sceneHeight; i++) {
		fwrite(myBuff + 3 * i, 1, 3, out_file);
	}

	fclose(out_file);
}


