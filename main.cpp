using namespace std;
#include <GL/glut.h>
#include "Scence.h"
#include "Point.h"
#include "Line.h"
#include "Triangle.h"
#include "Polygon.h"
#include "Camera.h"
#include "Ray.h"
#include "SkyBox.h"
#include "ObjObject.h"
#include "Particle.h"

void display();
void reshape(int w, int h);
void keyboard(unsigned char key, int x, int y);
void specialkey(int k, int x, int y);
void mouseclick(int button, int state, int x, int y);
void mousemove(int x, int y);
void mousedrag(int x, int y);
void idle();
bool ifload[3] = { true,false,false };

void initScence0()
{
	scence.toScence(0);

	Camera* camera = new Camera();

	//Triangle t1(0, 0, 0.3, 0.3, 0, 0.3, 0, 0.3, 0.3);
	//Triangle t2(0, 0, 0, 0.3, 0, 0, 0, 0, 0.3);
	//Triangle t3(0, 0, 0, 0, 0.3, 0, 0, 0, 0.3);
	//Triangle t4(0, 0.3, 0.3, 0, 0.3, 0, 0, 0, 0.3);
	//Triangle t5(0.3, 0, 0.3, 0.3, 0, 0, 0, 0, 0.3);
	//Triangle t6(0.3, 0.3, 0.3, 0.3, 0, 0.3, 0, 0.3, 0.3);

	float light1[] = {
		1, 1, 1, 1, 1, 1, 0.8, 0.8, 0.8, 100,
		1, 1, 1, 1, 1, 1, 0.8, 0.8, 0.8, 100,
		1, 1, 1, 1, 1, 1, 0.8, 0.8, 0.8, 100,
		1, 1, 1, 1, 1, 1, 0.8, 0.8, 0.8, 100,
	};

	float clp1[] = {
		0  , 0  , 0.3, 1,
		0.3, 0  , 0.3, 1,
		0.3, 0.3, 0.3, 1,
		0  , 0.3, 0.3, 1
	};
	float clp2[] = {
		0.3, 0  , 0  , 1,
		0  , 0  , 0  , 1,
		0  , 0.3, 0  , 1,
		0.3, 0.3, 0  , 1
	};
	float clp3[] = {
		0  , 0  , 0  , 1,
		0.3  , 0  , 0  , 1,
		0.3  , 0  , 0.3  , 1,
		0  , 0  , 0.3  , 1
	};
	float clp4[] = {
		0.3  , 0.3, 0.3  , 1,
		0.3  , 0.3, 0  , 1,
		0  , 0.3, 0  , 1,
		0  , 0.3, 0.3  , 1
	};
	float clp5[] = {
		0  , 0.3  , 0.3  , 1,
		0  , 0.3  , 0  , 1,
		0  , 0  , 0  , 1,
		0  , 0  , 0.3  , 1
	};
	float clp6[] = {
		0.3, 0  , 0  , 1,
		0.3, 0.3  , 0  , 1,
		0.3, 0.3  , 0.3  , 1,
		0.3, 0  , 0.3  , 1
	};

	float nomal1[] = {
		0,0,-1,
		0,0,-1,
		0,0,-1,
		0,0,-1,
	};

	float nomal2[] = {
		0,0,1,
		0,0,1,
		0,0,1,
		0,0,1,
	};

	float nomal3[] = {
		0,1,0,
		0,1,0,
		0,1,0,
		0,1,0,
	};

	float nomal4[] = {
		0,-1,0,
		0,-1,0,
		0,-1,0,
		0,-1,0,
	};

	float nomal5[] = {
		1,0,0,
		1,0,0,
		1,0,0,
		1,0,0,
	};

	float nomal6[] = {
		-1,0,0,
		-1,0,0,
		-1,0,0,
		-1,0,0,
	};

	float lcolors1[12] = { 1,0,0,0.5,
						0,0,1,1,
						0,1,0,1 };
	float lcolors2[12] = { 1,1,0,1,
							1,0,1,1,
							0,1,1,1 };
	float lcolors3[12] = { 1,1,0,1,
							1,0,1,1,
							0,1,1,1 };
	float luv1[6] = { 1,1,1,0,0,1 };
	float luv2[6] = { 0,0,1,0,0,1 };

	float clcolors1[] = {
		1,0,0,0.5,
		0,1,0,0,
		0,0,1,0.5,
		1,1,1,1,
	};
	float clcolors2[] = {
		1,0,1,1,
		1,1,0,1,
		0,1,1,1,
		1,0,1,1,
	};

	float clcolorsf[] = {
		1,1,1,1,
		1,1,1,1,
		1,1,1,1,
		1,1,1,1
	};

	float cluv1[] = { 0,1,0,0,1,0,1,1 };

	Polygon* c1 = new Polygon(4, clp1);
	Polygon* c2 = new Polygon(4, clp2);
	Polygon* c3 = new Polygon(4, clp3);
	Polygon* c4 = new Polygon(4, clp4);
	Polygon* c5 = new Polygon(4, clp5);
	Polygon* c6 = new Polygon(4, clp6);

	SkyBox* sky = new SkyBox(0.4);

	Light bring1;

	ObjObject* obj = new ObjObject("obj", "ball.obj");

	bring1.setIdp(0.5, 0.5, 0.5);
	bring1.setXYZ(1, 0, -1);

	scence.addLight(bring1);

	camera->tranformPosition(0, 0, -2);

	camera->lookdBox(0.1, 10, 0.1);
	scence.mainCamera = camera;

	c1->setColors(clcolors1);
	c2->setColors(clcolors2);
	c3->setColors(clcolorsf);
	c4->setColors(clcolorsf);
	c5->setColors(clcolorsf);
	c6->setColors(clcolorsf);

	c1->setNormals(nomal1);
	c2->setNormals(nomal2);
	c3->setNormals(nomal3);
	c4->setNormals(nomal4);
	c5->setNormals(nomal5);
	c6->setNormals(nomal6);

	c6->setUV(cluv1);

	c6->setTexture(new Texture("texture1.dat"));

	c1->setParm(10, light1);
	c2->setParm(10, light1);
	c3->setParm(10, light1);
	c4->setParm(10, light1);
	c5->setParm(10, light1);
	c6->setParm(10, light1);
	obj->scale(0.15, 0.15, 0.15);
	obj->move(-1.2, 1, 1);
	obj->scale(1.2, 1.2, 1.2);

	scence.registerElement(c1);
	scence.registerElement(c2);
	scence.registerElement(c3);
	scence.registerElement(c4);
	scence.registerElement(c5);
	scence.registerElement(c6);


	scence.registerElement(sky);
	scence.registerElement(obj);

}

void initScence1()
{
	Camera* camera = new Camera();

	scence.toScence(1);

	camera->tranformPosition(0, 0, -2);

	camera->lookdBox(0.1, 10, 0.1);
	scence.mainCamera = camera;
	SkyBox* sky = new SkyBox(0.4);

	Light bring1;
	ObjObject* obj = new ObjObject("obj", "fox.obj");
	bring1.setIdp(0.5, 0.5, 0.5);
	bring1.setXYZ(1, 0, -1);

	obj->move(0, -1, 0);

	scence.addLight(bring1);

	scence.registerElement(obj);
	scence.registerElement(sky);
}

void initScence2()
{
	Camera* camera = new Camera();

	scence.toScence(2);

	Particle* part1 = new Particle(100);
	Particle* part2 = new Particle(100);
	Particle* part3 = new Particle(100);
	Particle* part4 = new Particle(100);
	Particle* part5 = new Particle(100);
	Particle* part6 = new Particle(100);
	Particle* part7 = new Particle(100);

	part1->setColor(1, 1, 1);
	part2->setColor(0, 0, 1);
	part3->setColor(0, 1, 0);
	part4->setColor(1, 0, 0);
	part5->setColor(1, 1, 0);
	part6->setColor(1, 0, 1);
	part7->setColor(0, 1, 1);

	camera->tranformPosition(0, 0, -2);

	camera->lookdBox(0.1, 10, 0.1);
	scence.mainCamera = camera;
	//SkyBox* sky = new SkyBox(0.4);

	Light bring1;
	bring1.setIdp(0.5, 0.5, 0.5);
	bring1.setXYZ(1, 0, -1);

	scence.addLight(bring1);

	part2->move(1, 1, 1);
	part3->move(-1, 1, 1);
	part4->move(1, 0, 1);
	part5->move(1, 1, -1);
	part6->move(1, 0, -1);
	part7->move(-1, 0, 1);

	part2->setOrginV(-5, -5, -5);
	part3->setOrginV(5, -5, -5);
	part4->setOrginV(-5, 5, -5);
	part5->setOrginV(-5, -5, 5);
	part6->setOrginV(-5, 5, 5);
	part7->setOrginV(5, 5, -5);

	//scence.registerElement(sky);
	scence.registerElement(part1);
	scence.registerElement(part2);
	scence.registerElement(part3);
	scence.registerElement(part4);
	scence.registerElement(part5);
	scence.registerElement(part6);
	scence.registerElement(part7);
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glEnable(GL_LINE_STIPPLE);
	glutCreateWindow("renderer");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(specialkey);
	glutMouseFunc(mouseclick);
	glutMotionFunc(mousedrag);
	glutPassiveMotionFunc(mousemove);
	glutIdleFunc(idle);

	scence.addScence();
	scence.addScence();

	scence.resize(500, 500);
	initScence0();
	//initScence1();

	glutMainLoop();
	return 0;
}

void display()
{
	scence.clear();
	scence.drawElement();
	scence.show();
}

void reshape(int w, int h)
{
	scence.resize(w, h);
	scence.drawElement();
	scence.show();
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case '1':
		scence.toScence(0);
		break;
	case '2':
		if (!ifload[1])
		{
			initScence1();
			ifload[1] = true;
		}
		scence.toScence(1);
		break;
	case '3':
		if (!ifload[2])
		{
			initScence2();
			ifload[2] = true;
		}
		scence.toScence(2);
		break;
	default:
		break;
	}
	if (scence.mainCamera == nullptr)return;
	float moveSpeed = 0.1;
	switch (key)
	{
	case 'W':
	case 'w':
		scence.mainCamera->move(0, 0, moveSpeed);
		break;

	case 'S':
	case 's':
		scence.mainCamera->move(0, 0, -moveSpeed);
		break;

	case 'A':
	case 'a':
		scence.mainCamera->move(-moveSpeed, 0, 0);
		break;

	case 'D':
	case 'd':
		scence.mainCamera->move(moveSpeed, 0, 0);
		break;

	case ' ':
		scence.mainCamera->move(0, moveSpeed, 0);
		break;

	case 'Z':
	case 'z':
		scence.mainCamera->move(0, -moveSpeed, 0);
		break;


	default:
		break;
	}

	if (glutGetModifiers() == GLUT_ACTIVE_SHIFT)
	{
		scence.mainCamera->tranformPosition(0, -moveSpeed, 0);
	}

	glutPostRedisplay();
}

void specialkey(int key, int x, int y)
{
	glutPostRedisplay();
}

bool inMidDragFlag = false;
bool lastdown = false;

void mouseclick(int button, int state, int x, int y)
{
	if (state == GLUT_UP)
		lastdown = false;
	switch (button)
	{
	case GLUT_MIDDLE_BUTTON:
		inMidDragFlag = (state == GLUT_DOWN);
		break;
	default:
		break;
	}

}

void mousemove(int x, int y)
{

}

int lastx = 0;
int lasty = 0;

void mousedrag(int x, int y)
{
	if (scence.mainCamera == nullptr)
		return;

	if (!lastdown)
	{
		lastdown = true;
		lastx = x;
		lasty = y;
	}

	int dx, dy;
	dx = x - lastx;
	dy = y - lasty;

	lastx = x;
	lasty = y;

	scence.mainCamera->rotate(dy * 0.01, dx * 0.01, 0);

	glutPostRedisplay();

}

void idle()
{
	scence.clear();
	//scence.rayTrace();
	scence.drawElement();
	scence.show();
}
