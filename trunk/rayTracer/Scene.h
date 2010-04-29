#pragma once
#include "Sampler.h"
#include "Sample.h"
#include "Film.h"
#include "Color.h"
#include "Sphere.h"
#include "Triangle.h"
#include "Material.h"
#include "GeometricPrimitive.h"
#include "AggregatePrimitive.h"
#include "Transformation.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include <vector>
#include "RayTracer.h"
#include <string.h>

#define vector std::vector

class RayTracer;

class Scene
{
public:
	int mySizex, mySizey;
	int myMaxDepth;
	char output_name[300];
	vector<Primitive*> myPrimitivesVector;

	vector<Light*> myLightsVector;
	BRDF currBRDF;

	vector<mat4> myMatrixStack;
	vector<mat4> currMatElems; // Contains S,T,R if M = R*T*S
	Camera myCamera;

	void render();
	void setCamera(Camera *mainCam);
	void addPrimitive(Primitive* prim);
	void addLight(Light* light);
	
	void pushMatrix();
	void popMatrix();
	void multCurrentMatrix(mat4 M);
	mat4 getCurrentMatrix();
	void composeCurrentMatrix(); // Right multiply matrices in currMatElems to get M
	void loadIdentity();
	void translateMatrix(double x, double y, double z);
	void rotateMatrix(double theta, double x, double y, double z);
	void scaleMatrix(double x, double y, double z);
	
	void setOutputName(char name[]);

	Scene(int sizex, int sizey);
	~Scene(void);
};
