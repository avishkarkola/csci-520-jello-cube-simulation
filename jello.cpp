//	Avishkar Kolahalu - 6138 4283 68

#include "jello.h"
#include "showCube.h"
#include "input.h"
#include "physics.h"

// Camera parameters
double Theta = pi / 6;
double Phi = pi / 6;
double R = 6;

// Mouse control
int g_iMenuId;
int g_vMousePos[2];
int g_iLeftMouseButton,g_iMiddleMouseButton,g_iRightMouseButton;

// Number of images saved to disk so far
int sprite=0;

// These variables control what is displayed on screen
int shear=0, bend=0, structural=1, pause=0, viewingMode=0, saveScreenToFile=0;

struct world jello;

int windowWidth, windowHeight;

// Inclined Plane Variables
struct point Normal, intersect_list[5];
int total_intersect = 0;

// Plane Intersect Calc.
void planeIntersect()
{
	double T2, above, below, L;
	int i = 0;
	point T1, dir;

	// x:

	// 4 sides:

	T1.x = (-jello.d - 2 * jello.c - 2 * jello.b) / jello.a;
	T1.z = T1.y = 2;
	if (T1.x >= -2 && T1.x <= 2)
		intersect_list[total_intersect++] = T1;

	T1.x = (-jello.d + 2 * jello.c - 2 * jello.b) / jello.a;
	T1.y = 2;
	T1.z = -2;
	if (T1.x >= -2 && T1.x <= 2)
		intersect_list[total_intersect++] = T1;

	T1.x = (-jello.d - 2 * jello.c + 2 * jello.b) / jello.a;
	T1.y = -2;
	T1.z = 2;
	if (T1.x >= -2 && T1.x <= 2)
		intersect_list[total_intersect++] = T1;

	T1.x = (-jello.d + 2 * jello.c + 2 * jello.b) / jello.a;
	T1.z = T1.y = -2;
	if (T1.x >= -2 && T1.x <= 2)
		intersect_list[total_intersect++] = T1;

	// y:

	// 4 sides:

	T1.z = T1.x = 2;
	T1.y = (-jello.d - 2 * jello.c - 2 * jello.a) / jello.b;
	if (T1.y >= -2 && T1.y <= 2)
		intersect_list[total_intersect++] = T1;

	T1.x = 2;
	T1.y = (-jello.d + 2 * jello.c - 2 * jello.a) / jello.b;
	T1.z = -2;
	if (T1.y >= -2 && T1.y <= 2)
		intersect_list[total_intersect++] = T1;

	T1.x = -2;
	T1.y = (-jello.d - 2 * jello.c + 2 * jello.a) / jello.b;
	T1.z = 2;
	if (T1.y >= -2 && T1.y <= 2)
		intersect_list[total_intersect++] = T1;

	T1.z = T1.x = -2;
	T1.y = (-jello.d + 2 * jello.c + 2 * jello.a) / jello.b;
	if (T1.y >= -2 && T1.y <= 2)
		intersect_list[total_intersect++] = T1;

	// z:

	// 4 sides:

	T1.y = T1.x = 2;
	T1.z = (-jello.d - 2 * jello.b - 2 * jello.a) / jello.c;
	if (T1.z >= -2 && T1.z <= 2)
		intersect_list[total_intersect++] = T1;

	T1.x = 2;
	T1.y = -2;
	T1.z = (-jello.d + 2 * jello.b - 2 * jello.a) / jello.c;
	if (T1.z >= -2 && T1.z <= 2)
		intersect_list[total_intersect++] = T1;

	T1.x = -2;
	T1.y = 2;
	T1.z = (-jello.d - 2 * jello.b + 2 * jello.a) / jello.c;
	if (T1.z >= -2 && T1.z <= 2)
		intersect_list[total_intersect++] = T1;

	T1.y = T1.x = -2;
	T1.z = (-jello.d + 2 * jello.b + 2 * jello.a) / jello.c;
	if (T1.z >= -2 && T1.z <= 2)
		intersect_list[total_intersect++] = T1;

	// Calculate Normals:

	// Surfaces:

	below = jello.a*jello.a + jello.b*jello.b + jello.c*jello.c;
	above = jello.a*jello.p[0][0][0].x + jello.b*jello.p[0][0][0].y + jello.c*jello.p[0][0][0].z + jello.d;

	T2 = -(above / below);

	dir.x = jello.p[0][0][0].x - (jello.a*T2 + jello.p[0][0][0].x);
	dir.y = jello.p[0][0][0].y - (jello.b*T2 + jello.p[0][0][0].y);
	dir.z = jello.p[0][0][0].z - (jello.c*T2 + jello.p[0][0][0].z);

	L = sqrt(pow(dir.x,2) + pow(dir.y, 2) + pow(dir.z, 2));

	Normal.x = dir.x / L;
	Normal.y = dir.y / L;
	Normal.z = dir.z / L;

}

void myinit()
{
	// If a plane exists, calculate intersection
	if (jello.incPlanePresent == 1)
		planeIntersect();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90.0,1.0,0.01,1000.0);

	// Set background color to grey
	glClearColor(0.2, 0.2, 0.2, 0.0);

	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	glShadeModel(GL_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);
	glEnable(GL_LINE_SMOOTH);

	return; 
}

void reshape(int w, int h) 
{
	// Prevent a divide by zero, when h is zero
	// You can't make a window of zero height
	if(h == 0)
	h = 1;

	glViewport(0, 0, w, h);

	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Set the perspective
	double aspectRatio = 1.0 * w / h;
	gluPerspective(60.0f, aspectRatio, 0.01f, 1000.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity(); 

	windowWidth = w;
	windowHeight = h;

	glutPostRedisplay();
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Camera parameters are Phi, Theta, R
	gluLookAt(R * cos(Phi) * cos (Theta), R * sin(Phi) * cos (Theta), R * sin (Theta),
			0.0,0.0,0.0, 0.0,0.0,1.0);

	// Lighting:

	// Global ambient light
	GLfloat aGa[] = { 0.0, 0.0, 0.0, 0.0 };
  
	// Light's ambient, diffuse, specular
	GLfloat lKa0[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat lKd0[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat lKs0[] = { 1.0, 1.0, 1.0, 1.0 };

	GLfloat lKa1[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat lKd1[] = { 1.0, 0.0, 0.0, 1.0 };
	GLfloat lKs1[] = { 1.0, 0.0, 0.0, 1.0 };

	GLfloat lKa2[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat lKd2[] = { 1.0, 1.0, 0.0, 1.0 };
	GLfloat lKs2[] = { 1.0, 1.0, 0.0, 1.0 };

	GLfloat lKa3[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat lKd3[] = { 0.0, 1.0, 1.0, 1.0 };
	GLfloat lKs3[] = { 0.0, 1.0, 1.0, 1.0 };

	GLfloat lKa4[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat lKd4[] = { 0.0, 0.0, 1.0, 1.0 };
	GLfloat lKs4[] = { 0.0, 0.0, 1.0, 1.0 };

	GLfloat lKa5[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat lKd5[] = { 1.0, 0.0, 1.0, 1.0 };
	GLfloat lKs5[] = { 1.0, 0.0, 1.0, 1.0 };

	GLfloat lKa6[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat lKd6[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat lKs6[] = { 1.0, 1.0, 1.0, 1.0 };

	GLfloat lKa7[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat lKd7[] = { 0.0, 1.0, 1.0, 1.0 };
	GLfloat lKs7[] = { 0.0, 1.0, 1.0, 1.0 };

	// Light positions and directions
	GLfloat lP0[] = { -1.999, -1.999, -1.999, 1.0 };
	GLfloat lP1[] = { 1.999, -1.999, -1.999, 1.0 };
	GLfloat lP2[] = { 1.999, 1.999, -1.999, 1.0 };
	GLfloat lP3[] = { -1.999, 1.999, -1.999, 1.0 };
	GLfloat lP4[] = { -1.999, -1.999, 1.999, 1.0 };
	GLfloat lP5[] = { 1.999, -1.999, 1.999, 1.0 };
	GLfloat lP6[] = { 1.999, 1.999, 1.999, 1.0 };
	GLfloat lP7[] = { -1.999, 1.999, 1.999, 1.0 };
  
	// Jelly material color

	GLfloat mKa[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat mKd[] = { 0.3, 0.3, 0.3, 1.0 };
	GLfloat mKs[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mKe[] = { 0.0, 0.0, 0.0, 1.0 };

	// Set up lighting
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, aGa);
	glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);

	// Set up cube color
	glMaterialfv(GL_FRONT, GL_AMBIENT, mKa);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mKd);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mKs);
	glMaterialfv(GL_FRONT, GL_EMISSION, mKe);
	glMaterialf(GL_FRONT, GL_SHININESS, 120);
    
	// Macro to set up light i
	#define LIGHTSETUP(i)\
	glLightfv(GL_LIGHT##i, GL_POSITION, lP##i);\
	glLightfv(GL_LIGHT##i, GL_AMBIENT, lKa##i);\
	glLightfv(GL_LIGHT##i, GL_DIFFUSE, lKd##i);\
	glLightfv(GL_LIGHT##i, GL_SPECULAR, lKs##i);\
	glEnable(GL_LIGHT##i)
  
	LIGHTSETUP (0);
	LIGHTSETUP (1);
	LIGHTSETUP (2);
	LIGHTSETUP (3);
	LIGHTSETUP (4);
	LIGHTSETUP (5);
	LIGHTSETUP (6);
	LIGHTSETUP (7);

	// Enable lighting
	glEnable(GL_LIGHTING);    
	glEnable(GL_DEPTH_TEST);

	// Show the cube
	showCube(&jello);

	glDisable(GL_LIGHTING);

	// Show the bounding box
	showBoundingBox();

	// If there exists an inclined plane:

	if (jello.incPlanePresent == 1)
	{
		// Both sides, with different colours, of the plane:

		int i = 0;

		glFrontFace(GL_CW);
		glColor4f(1.0, 0.65, 0.0, 0);

		glBegin(GL_POLYGON);

		glVertex3f(intersect_list[i].x, intersect_list[i].y, intersect_list[i].z);
		glVertex3f(intersect_list[i + 3].x, intersect_list[i + 3].y, intersect_list[i + 3].z);
		glVertex3f(intersect_list[i + 1].x, intersect_list[i + 1].y, intersect_list[i + 1].z);
		glVertex3f(intersect_list[i + 2].x, intersect_list[i + 2].y, intersect_list[i + 2].z);

		glEnd();

		glFrontFace(GL_CCW);
		glColor4f(0.4, 0.0, 0.0, 0);

		glBegin(GL_POLYGON);

		glVertex3f(intersect_list[i].x, intersect_list[i].y, intersect_list[i].z);
		glVertex3f(intersect_list[i + 3].x, intersect_list[i + 3].y, intersect_list[i + 3].z);
		glVertex3f(intersect_list[i + 1].x, intersect_list[i + 1].y, intersect_list[i + 1].z);
		glVertex3f(intersect_list[i + 2].x, intersect_list[i + 2].y, intersect_list[i + 2].z);

		glEnd();

		// Show a black outline for the plane:

		glColor4f(0, 0, 0, 1.0);
		glBegin(GL_LINES);
		glVertex3f(intersect_list[3].x, intersect_list[3].y, intersect_list[3].z);
		glVertex3f(intersect_list[1].x, intersect_list[1].y, intersect_list[1].z);
		glVertex3f(intersect_list[2].x, intersect_list[2].y, intersect_list[2].z);
		glVertex3f(intersect_list[0].x, intersect_list[0].y, intersect_list[0].z);
		glVertex3f(intersect_list[0].x, intersect_list[0].y, intersect_list[0].z);
		glVertex3f(intersect_list[3].x, intersect_list[3].y, intersect_list[3].z);
		glVertex3f(intersect_list[1].x, intersect_list[1].y, intersect_list[1].z);
		glVertex3f(intersect_list[2].x, intersect_list[2].y, intersect_list[2].z);
		glEnd();

	}

	glutSwapBuffers();
}

void doIdle()
{
	char s[20]="picxxxx.ppm";
	int i, delayer = 0;
  
	// save screen to file
	s[3] = 48 + (sprite / 1000);
	s[4] = 48 + (sprite % 1000) / 100;
	s[5] = 48 + (sprite % 100 ) / 10;
	s[6] = 48 + sprite % 10;

	if (saveScreenToFile==1)
	{
		if (delayer == 0)
		{
			saveScreenshot(windowWidth, windowHeight, s);
			sprite++;
			delayer = 1;
		}
		else 
			delayer = 0;

	// saveScreenToFile=0; save only once, change this if you want continuos image generation (i.e. animation)
	}

	// Allow only 300 snapshots
	if (sprite >= 300)
		exit(0);

	if (pause == 0)
	{
		// Appropriate method chosen:

		if (strcmp("Euler", jello.integrator) == 0)
			Euler(&jello);

		if (strcmp("RK4", jello.integrator) == 0)
			RK4(&jello);
	}

	glutPostRedisplay();
}

int main (int argc, char ** argv)
{
	if (argc<2)
	{  
		printf ("Oops! You didn't say the jello world file!\n");
		printf ("Usage: %s [worldfile]\n", argv[0]);
		exit(0);
	}

	readWorld(argv[1],&jello);

	glutInit(&argc,argv);
  
	// double buffered window, use depth testing, 640x480
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  
	windowWidth = 640;
	windowHeight = 480;
	glutInitWindowSize (windowWidth, windowHeight);
	glutInitWindowPosition (0,0);

	glutCreateWindow ("Jello Cube - Avishkar Kolahalu");
	glutDisplayFunc(display);
	glutIdleFunc(doIdle);
	glutMotionFunc(mouseMotionDrag);
	glutReshapeFunc(reshape);
	glutPassiveMotionFunc(mouseMotion);
	glutMouseFunc(mouseButton);
	glutKeyboardFunc(keyboardFunc);
	myinit();
	glutMainLoop();

	return(0);
}