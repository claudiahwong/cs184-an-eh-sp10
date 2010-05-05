#include "Scene.h"

/*  Constructor */
Scene::Scene(int sizex, int sizey)
{
	mySizex = sizex;
	mySizey = sizey;
	currMatElems.push_back(mat4_id);
	currBRDF.myKa = Color(0.2, 0.2, 0.2);
	currBRDF.myRIndex = 1;
	ConstAtt = 1;
	LinAtt = 0;
	QuadAtt = 0;
	strcpy(output_name, "out.ppm");
	myMaxDepth = 5;

	n = 2; // Default 4x Antialiasing
	srand(time(NULL));
}

Scene::~Scene(void)
{
}

void Scene::setCamera(Camera *mainCam) {
	myCamera.eye = mainCam->eye;
	myCamera.center = mainCam->center;
	myCamera.up = mainCam->up;
	//myCamera.fovx = mainCam->fovx;
	myCamera.fovy = mainCam->fovy;
	myCamera.screenHeight = mainCam->screenHeight;
	myCamera.screenWidth = mainCam->screenWidth;
}    

void Scene::addPrimitive(Primitive* prim) {
	myPrimitivesVector.push_back(prim);
}

void Scene::addLight(Light *light) {
	myLightsVector.push_back(light);
}

void Scene::pushMatrix() {
	myMatrixStack.push_back(mat4(getCurrentMatrix()));
}

void Scene::popMatrix() {
	myMatrixStack.pop_back();
	currMatElems.clear();
	currMatElems.push_back(mat4_id);
}

void Scene::multCurrentMatrix(mat4 M) {
	mat4 current = mat4(myMatrixStack.back());
	myMatrixStack.pop_back();
	mat4 result;
	mult(result, M, current);
	myMatrixStack.push_back(result);
}

mat4 Scene::getCurrentMatrix() {
	composeCurrentMatrix();
	return myMatrixStack.back();
}

void Scene::composeCurrentMatrix() {
	for (vector<mat4>::reverse_iterator m = currMatElems.rbegin(); m != currMatElems.rend(); m++) {
			multCurrentMatrix(*m);
	}
	currMatElems.clear();
	currMatElems.push_back(mat4_id);
}

void Scene::loadIdentity() {
	myMatrixStack.push_back(mat4_id);
}

void Scene::translateMatrix(double x, double y, double z) {
	mat4 T = mat4_id;
	T.set_translation(vec3(x, y, z));
	currMatElems.push_back(T);
}

void Scene::rotateMatrix(double theta, double x, double y, double z) {
	mat4 R = mat4_id;
	R.set_rot(theta*nv_to_rad, vec3(x,y,z));
	currMatElems.push_back(R);
}

void Scene::scaleMatrix(double x, double y, double z) {
	mat4 S = mat4_id;
	vec4 col0 = S.col(0); scale(col0, x); 
	S.set_col(0, col0);
	vec4 col1 = S.col(1); scale(col1, y);
	S.set_col(1, col1);
	vec4 col2 = S.col(2); scale(col2, z);
	S.set_col(2, col2);
	currMatElems.push_back(S);
}


void Scene::setOutputName(char name[]) {
	strcpy(output_name, name);
}

void Scene::render() {
	Sampler *mySampler = new Sampler(mySizex, mySizey);
	Sample *mySample = new Sample();
	Film *myFilm = new Film(mySizex, mySizey);
	Color *testingColor = new Color();
	Color *resultColor = new Color(); // After averaging for antialiasing
	//Color *testBlack = new Color(0, 0, 0);
	
	float totalR, totalG, totalB;
	totalR = 0.0; totalG = 0.0; totalB = 0.0;
	
	Ray *myRay = new Ray();
	double *thit = new double;
	Intersection *in = new Intersection();
	RayTracer *myRayTracer = new RayTracer(this, thit, in, myMaxDepth);
	
	int nSquared = n*n; // n^2 = samples per pixel
	float epsilon;
	
	while(mySampler->getSample(mySample)) {
		resultColor = new Color();
		int p, q;
		if (n > 1) { // This is the antialiasing code
			for (p = 0; p < n; p++)
			{
				for (q = 0; q < n; q++) {

					epsilon = (float)rand()/RAND_MAX;
					mySample->x += ((p+epsilon)/(float)n);
					mySample->y += ((q+epsilon)/(float)n);
					myCamera.generateRay(*mySample, myRay);
					myRayTracer->trace(*myRay, 0, testingColor);
					
					totalR += testingColor->r;
					totalG += testingColor->g;
					totalB += testingColor->b;
					testingColor->r = 0;
					testingColor->g = 0;
					testingColor->b = 0;
				}
			}

			totalR /= (float) nSquared;
			totalG /= (float) nSquared;
			totalB /= (float) nSquared;
			resultColor->r = totalR;
			resultColor->g = totalG;
			resultColor->b = totalB;
			totalR = 0.0; totalG = 0.0; totalB = 0.0;
		} else { // If antialiasing == 1, just do the old code without wasting computation
			myCamera.generateRay(*mySample, myRay);
			myRayTracer->trace(*myRay, 0, resultColor);
		}


		myFilm->setColor(*mySample, *resultColor);
		delete resultColor;
		
	}
	myFilm->writeImage(output_name);
	
	delete mySampler;
	delete mySample;
	delete myFilm;
	delete myRay;
	delete in;
	delete myRayTracer;
}
