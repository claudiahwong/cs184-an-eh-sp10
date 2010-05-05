#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "nv_math.h"
#include "nv_algebra.h"
#include "Scene.h"

int sizex, sizey;

struct Vertex {
	double pos[3] ;
} ;

struct VertexNormal {
	double pos[3] ;
	double normal[3] ;
} ;

Vertex * vert = 0;
VertexNormal * vertnorm = 0;
int maxverts = 0, maxvertnorms = 0;
int curvert = 0, curvertnorm = 0 ;

int maxdepth = 5 ;
int lightnum = 0 ;

FILE *inputfile ;

int parsed = 0 ;

Scene *mainScene;
Camera *mainCam;

void initialparse (FILE *fp) {
	char line[1000], command[1000] ; // Very bad to prefix array size :-)

	while (1) {
		fgets(line,sizeof(line),fp) ;
		if (feof(fp)) { fprintf(stderr, "Nothing in file\n") ; exit(1); }  ;
		if (line[0] == '#') continue ; // Comment lines    
		int num = sscanf(line, "%s", command) ;
		if (num != 1) continue ; // Blank line etc.
		else break ;
	}

	//......................... SET IMAGE SIZE......................... 
	// The first line should be the size command setting the image size
	assert(!strcmp(command, "size")) ;
	int num = sscanf(line, "%s %d %d", command, &sizex, &sizey) ;
	assert(num == 3) ;
	assert(!strcmp(command, "size")) ;
}

void parsefile (FILE *fp) {
	char line[1000], command[1000] ; // Very bad to prefix array size :-)
	int sizeset = 0 ;  

	while (!feof(fp)) {
		fgets(line,sizeof(line),fp) ;
		if (feof(fp)) break ;
		if (line[0] == '#') continue ; // Comment lines

		int num = sscanf(line, "%s", command) ;
		if (num != 1) continue ; // Blank line etc.

		// Now, we simply parse the file by looking at the first line for the 
		// various commands
		//......................... CAMERA LOCATION ......................... 
		if (!strcmp(command, "camera")) {
			double lookfrom[3], lookat[3], up[3], fov ;
			int num = sscanf(line, "%s %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf", 
				command, lookfrom, lookfrom+1, lookfrom+2, 
				lookat, lookat+1, lookat+2, up, up+1, up+2, &fov) ;
			if (num != 11) {
				fprintf(stderr, "camera from[3] at[3] up[3] fov\n") ;
				exit(1) ;
			}
			assert(!strcmp(command,"camera")) ;

			vec3 e = vec3(*lookfrom, *(lookfrom+1), *(lookfrom+2));
			vec3 c = vec3(*lookat, *(lookat+1), *(lookat+2));
			vec3 u = vec3(*up, *(up+1), *(up+2));
			//Camera *newCamera = new Camera(e, c, u, fov, 1, sizex, sizey);
			mainCam = new Camera(e, c, u, fov, 1, sizex, sizey);
			mainScene->setCamera(mainCam);
		}
		//............................................................

		//......................... GEOMETRY .........................
		else if (!strcmp(command, "sphere")) {
			double radius ; // Syntax is sphere x y z radius 
			double pos[3] ;
			int num = sscanf(line, "%s %lf %lf %lf %lf", command, pos, pos+1, pos+2, &radius) ;
			if (num != 5) {
				fprintf(stderr, "sphere x y z radius\n") ;
				exit(1) ;
			}

			Sphere *newSphere = new Sphere(*pos, *(pos+1), *(pos+2), radius, *mainCam);
			BRDF *sphereBRDF = new BRDF(mainScene->currBRDF.myKd, mainScene->currBRDF.myKs, 
				mainScene->currBRDF.myKa, mainScene->currBRDF.myKr, 
				mainScene->currBRDF.myKe, mainScene->currBRDF.myShininess,
				mainScene->currBRDF.myRIndex, mainScene->currBRDF.myKrefract);
			Material *sphereMat = new Material(sphereBRDF);
			
			mat4 current = mainScene->getCurrentMatrix();
			mat4 inv;
			invert(inv, current);
			Transformation worldToObj = Transformation(inv);
			Transformation objToWorld = Transformation(current);
			
			GeometricPrimitive *spherePrim = new GeometricPrimitive(newSphere, sphereMat, objToWorld, worldToObj);
			mainScene->addPrimitive(spherePrim);
		}

		else if (!strcmp(command, "maxverts")) {
			int num = sscanf(line, "%s %d", command, &maxverts) ;
			assert(num == 2) ; assert(maxverts > 0) ;
			assert(!strcmp(command,"maxverts")) ;
			assert(vert = new Vertex[maxverts]) ;
		}

		else if (!strcmp(command, "maxvertnorms")) {
			int num = sscanf(line, "%s %d", command, &maxvertnorms) ;
			assert(num == 2) ; assert(maxvertnorms > 0) ;
			assert(!strcmp(command,"maxvertnorms")) ;
			assert(vertnorm = new VertexNormal[maxvertnorms]) ;
		}

		else if (!strcmp(command, "vertex")) {  // Add a vertex to the stack
			assert(maxverts) ; assert(curvert < maxverts) ;
			Vertex v ;
			int num = sscanf(line, "%s %lf %lf %lf", command, v.pos, v.pos+1, v.pos+2) ;
			assert(num == 4) ; assert(!strcmp(command,"vertex")) ;
			vert[curvert] = v ;
			++curvert ;
		}

		else if (!strcmp(command, "vertexnormal")) {  
			// Add a vertex to the stack with a normal
			assert(maxvertnorms) ; assert(curvertnorm < maxvertnorms) ;
			VertexNormal vn ;

			int num = sscanf(line, "%s %lf %lf %lf %lf %lf %lf", 
				command, vn.pos, vn.pos+1, vn.pos+2, 
				vn.normal, vn.normal+1, vn.normal+2) ;

			assert(num == 7) ; assert(!strcmp(command,"vertexnormal")) ;
			vertnorm[curvertnorm] = vn ;
			++curvertnorm ;
		}

		else if (!strcmp(command, "tri")) { // Triangle from 3 vertices
			int pts[3] ; 
			int num = sscanf(line, "%s %d %d %d", command, pts, pts+1, pts+2) ;
			assert(num == 4) ; assert(!strcmp(command,"tri")) ;
			int i,j ;
			for (i = 0 ; i < 3 ; i++) {
				assert(pts[i] >= 0 && pts[i] < maxverts) ;
			}
			double vertex[3][3] ;
			double normal[3] ;
			for (i = 0 ; i < 3 ; i++) 
				for (j = 0 ; j < 3 ; j++)
					vertex[i][j] = vert[pts[i]].pos[j] ;


			// Calculate the face normal 
			double vec1[3], vec2[3], vec3[3] ;
			for (i = 0 ; i < 3 ; i++) {
				vec1[i] = vertex[1][i] - vertex[0][i] ;
				vec2[i] = vertex[2][i] - vertex[0][i] ;
			}
			vec3[0] = vec1[1]*vec2[2] - vec1[2]*vec2[1] ;
			vec3[1] = vec1[2]*vec2[0] - vec1[0]*vec2[2] ;
			vec3[2] = vec1[0]*vec2[1] - vec1[1]*vec2[0] ;

			double norm = 0 ;
			for (i = 0 ; i < 3 ; i++) norm += vec3[i] * vec3[i] ;
			norm = sqrt(norm) ;
			if (norm == 0) {normal[0] = 0 ; normal[1] = 0 ; normal[2] = 1.0 ; }
			else {
				for (i = 0 ; i < 3 ; i++) normal[i] = vec3[i] / norm ;
			}

			Triangle *newTriangle = new Triangle(Point(vertex[0][0], vertex[0][1], vertex[0][2]), 
												Point(vertex[1][0], vertex[1][1], vertex[1][2]), 
												Point(vertex[2][0], vertex[2][1], vertex[2][2]));

			mat4 current = mainScene->getCurrentMatrix();
			mat4 inv;
			invert(inv, current);
			Transformation worldToObj = Transformation(inv);
			Transformation objToWorld = Transformation(current);

			BRDF *triBRDF = new BRDF(mainScene->currBRDF.myKd, mainScene->currBRDF.myKs, 
				mainScene->currBRDF.myKa, mainScene->currBRDF.myKr, 
				mainScene->currBRDF.myKe, mainScene->currBRDF.myShininess,
				mainScene->currBRDF.myRIndex, mainScene->currBRDF.myKrefract);
			Material *triMat = new Material(triBRDF);
			GeometricPrimitive *triPrim = new GeometricPrimitive(newTriangle, triMat, objToWorld, worldToObj);
			mainScene->addPrimitive(triPrim);
		}

		else if (!strcmp(command, "trinormal")) {
			int pts[3] ;
			int num = sscanf(line, "%s %d %d %d", command, pts, pts+1, pts+2) ;
			assert(num == 4) ; assert(!strcmp(command,"trinormal")) ;
			int i,j ;
			for (i = 0 ; i < 3 ; i++) {
				assert(pts[i] >= 0 && pts[i] < maxvertnorms) ;
			}

			//Triangle::Triangle(Point vertex1, Point vertex2, Point vertex3, vec3 nor1, vec3 nor2, vec3 nor3);
			Triangle *newTriangle = new Triangle(Point(vertnorm[pts[0]].pos[0], vertnorm[pts[0]].pos[1], vertnorm[pts[0]].pos[2]), 
				Point(vertnorm[pts[1]].pos[0], vertnorm[pts[1]].pos[1], vertnorm[pts[1]].pos[2]), 
				Point(vertnorm[pts[2]].pos[0], vertnorm[pts[2]].pos[1], vertnorm[pts[2]].pos[2]),
				vec3(vertnorm[pts[0]].normal[0], vertnorm[pts[0]].normal[1], vertnorm[pts[0]].normal[2]), 
				vec3(vertnorm[pts[1]].normal[0], vertnorm[pts[1]].normal[1], vertnorm[pts[1]].normal[2]), 
				vec3(vertnorm[pts[2]].normal[0], vertnorm[pts[2]].normal[1], vertnorm[pts[2]].normal[2])
				);

			mat4 current = mainScene->getCurrentMatrix();
			mat4 inv;
			invert(inv, current);
			Transformation worldToObj = Transformation(inv);
			Transformation objToWorld = Transformation(current);

			BRDF *triBRDF = new BRDF(mainScene->currBRDF.myKd, mainScene->currBRDF.myKs, 
				mainScene->currBRDF.myKa, mainScene->currBRDF.myKr, 
				mainScene->currBRDF.myKe, mainScene->currBRDF.myShininess,
				mainScene->currBRDF.myRIndex, mainScene->currBRDF.myKrefract);
			Material *triMat = new Material(triBRDF);
			GeometricPrimitive *triPrim = new GeometricPrimitive(newTriangle, triMat, objToWorld, worldToObj);
			mainScene->addPrimitive(triPrim);
		}
		
		//.........................

		//......................... TRANSFORMATIONS.........................

		else if (!strcmp(command, "translate")) {
			double x,y,z ; // Translate by x y z as in standard OpenGL

			int num = sscanf(line, "%s %lf %lf %lf",command, &x, &y, &z) ;
			if (num != 4) {
				fprintf(stderr, "translate x y z\n") ;
				exit(1) ;
			}
			mainScene->translateMatrix(x, y, z);
		}

		else if (!strcmp(command, "rotate")) {
			double ang, x,y,z ; // Rotate by an angle about axis x y z as in standard OpenGL

			int num = sscanf(line, "%s %lf %lf %lf %lf",command, &x, &y, &z, &ang) ;
			if (num != 5) {
				fprintf(stderr, "rotate angle x y z\n") ;
				exit(1) ;
			}
			mainScene->rotateMatrix(ang, x, y, z);
		}

		else if (!strcmp(command, "scale")) {
			double x,y,z ; // Scale by x y z as in standard OpenGL

			int num = sscanf(line, "%s %lf %lf %lf",command, &x, &y, &z) ;
			if (num != 4) {
				fprintf(stderr, "scale x y z\n") ;
				exit(1) ;
			}
			mainScene->scaleMatrix(x, y, z);
		}

		else if (!strcmp(command, "pushTransform")) {
			// Push the current matrix on the stack as in OpenGL
			mainScene->pushMatrix();
		}

		else if (!strcmp(command, "popTransform")) {
			// Pop the current matrix as in OpenGL
			mainScene->popMatrix();
		}
		
		//..................................................

		//.........................MISCELLANEOUS IGNORED FOR OPENGL.........................

		else if (!strcmp(command, "maxdepth")) {
			int num = sscanf(line, "%s %d", command, &maxdepth) ;
			assert(num == 2) ;
			assert(!strcmp(command, "maxdepth")) ;
			mainScene->myMaxDepth = maxdepth;
			printf("Maxdepth set to %d\n", maxdepth);
		}

		else if (!strcmp(command, "output")) {
			char out[300] ;
			int num = sscanf(line, "%s %s", command, out) ;
			assert(num == 2) ;
			assert(!strcmp(command, "output")) ;
			mainScene->setOutputName(out);
			printf("Output image file set to: %s\n",out) ;
		}

		else if (!strcmp(command, "antialias")) {
			int n;
			int num = sscanf(line, "%s %d", command, &n);
			assert(num == 2);
			assert(!strcmp(command, "antialias"));
			mainScene->n = n;
			printf("%dx Antialiasing Set\n");
		}

		//..................................................

		//.........................LIGHTS.........................

		else if (!strcmp(command, "directional")) {
			float direction[4], color[4] ; color[3] = 1.0 ; direction[3] = 0.0 ;
			int num = sscanf(line, "%s %f %f %f %f %f %f", command, direction, direction+1, direction+2, color, color+1, color+2) ;
			assert(num == 7) ;
			assert(lightnum >= 0) ;
			
			Color *newColor = new Color(*color, *(color+1), *(color+2));
			DirectionalLight *newLight = new DirectionalLight(vec3(*direction, *(direction+1), *(direction+2)), newColor);
			mainScene->addLight(newLight);

			++lightnum ;
		}

		else if (!strcmp(command, "point")) {
			float direction[4], color[4] ; color[3] = 1.0 ; direction[3] = 1.0 ;
			int num = sscanf(line, "%s %f %f %f %f %f %f", command, direction, direction+1, direction+2, color, color+1, color+2) ;
			assert(num == 7) ;
			assert(lightnum >= 0) ;
			
			double att[3];
			att[0] = mainScene->ConstAtt;
			att[1] = mainScene->LinAtt;
			att[2] = mainScene->QuadAtt;
			Color *newColor = new Color(*color, *(color+1), *(color+2));
			PointLight *newLight = new PointLight(direction[0], direction[1], direction[2], *newColor, att);
			mainScene->addLight(newLight);

			++lightnum ;	 
		}

		else if (!strcmp(command, "attenuation")) {
			double attenuation[3] = {1.0, 0.0, 0.0};
			int num = sscanf(line, "%s %lf %lf %lf", command, attenuation, attenuation + 1, attenuation +2) ;
			assert(num == 4) ;
			assert(!strcmp(command, "attenuation")) ;
			mainScene->ConstAtt = attenuation[0];
			mainScene->LinAtt = attenuation[1];
			mainScene->QuadAtt = attenuation[2];

		}

		else if (!strcmp(command, "ambient")) {
			float ambient[4] ; ambient[3] = 1.0 ;
			int num = sscanf(line, "%s %f %f %f", command, ambient, ambient+1, ambient+2) ;
			assert(num == 4) ;
			assert(!strcmp(command, "ambient")) ;
			mainScene->currBRDF.myKa.setEqual(Color(ambient[0], ambient[1], ambient[2])); 
		}

		//..................................................

		//.........................MATERIALS.........................

		else if (!strcmp(command, "diffuse")) {
			float diffuse[4] ; diffuse[3] = 1.0 ;
			int num = sscanf(line, "%s %f %f %f", command, diffuse, diffuse+1, diffuse+2) ;
			assert(num == 4) ; assert (!strcmp(command, "diffuse")) ;
			mainScene->currBRDF.myKd.setEqual(Color(diffuse[0], diffuse[1], diffuse[2]));
		}

		else if (!strcmp(command, "specular")) {
			float specular[4] ; specular[3] = 1.0 ;
			int num = sscanf(line, "%s %f %f %f", command, specular, specular+1, specular+2) ;
			assert(num == 4) ; assert (!strcmp(command, "specular")) ;
			mainScene->currBRDF.myKs.setEqual(Color(specular[0], specular[1], specular[2]));
		}

		else if (!strcmp(command, "shininess")) {
			float shininess ;
			int num = sscanf(line, "%s %f", command, &shininess) ;
			assert(num == 2) ; assert (!strcmp(command, "shininess")) ;
			mainScene->currBRDF.myShininess = shininess;
		}

		else if (!strcmp(command, "emission")) {
			float emission[4] ; emission[3] = 1.0 ;
			int num = sscanf(line, "%s %f %f %f", command, emission, emission+1, emission+2) ;
			assert(num == 4) ; assert (!strcmp(command, "emission")) ;
			mainScene->currBRDF.myKe.setEqual(Color(emission[0], emission[1], emission[2]));
		}

		else if (!strcmp(command, "reflect")) {
			float reflect[4] ; reflect[3] = 1.0 ;
			int num = sscanf(line, "%s %f %f %f", command, reflect, reflect+1, reflect+2) ;
			assert(num == 4) ; assert (!strcmp(command, "reflect")) ;
			mainScene->currBRDF.myKr.setEqual(Color(reflect[0], reflect[1], reflect[2]));
		}

		else if (!strcmp(command, "refract")) {
			float refract[4] ;
			int num = sscanf(line, "%s %f %f %f", command, refract, refract+1, refract+2) ;
			assert(num == 4) ; assert (!strcmp(command, "refract")) ;
			mainScene->currBRDF.myKrefract.setEqual(Color(refract[0], refract[1], refract[2]));
			mainScene->currBRDF.myRIndex = refract[3]; 
		}

		//..................................................

		else {
			fprintf(stderr, "Unimplemented command: %s\n", command) ;
			exit(1) ;
		}
		/*GLenum g = glGetError() ;
		if (g != GL_NO_ERROR) {
			fprintf(stderr, "Error: %d\n",g) ;
			exit(1) ;
		}*/
		//*/
	}
}

int main(int argc,  char ** argv) {
	if (argc != 2) {
		fprintf(stderr, "Usage: openglviewer filename\n") ;
		exit(1) ;
	}
	FILE *fp ; 
	assert (fp = fopen(argv[1], "rt")) ;
	--argc ; ++argv ; 
	inputfile = fp ;
	initialparse(fp) ;
	mainScene = new Scene(sizex, sizey);
	mainScene->loadIdentity();
	parsefile(fp);
	printf("Rendering ... \n");
	mainScene->render();
}