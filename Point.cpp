#include "Point.h"
#include <GL/glut.h>

Point::Point() :Element(1)
{
}

Point::Point(float x, float y) : Element(1)
{
	positions.get(0) = x;
	positions.get(1) = y;
}

Point::Point(float x, float y, float z) :Element(1)
{
	positions.get(0) = x;
	positions.get(1) = y;
	positions.get(2) = z;
}

Point::Point(POINT p) :Element(1)
{
	positions.get(0) = p.x;
	positions.get(1) = p.y;
	positions.get(2) = p.z;
}

void Point::draw()const
{
	glPushMatrix();
	tranform();
	glBegin(GL_POINTS);
	glColor4fv(colors.get());
	glVertex3fv(positions.get());
	glEnd();
	glPopMatrix();
}
