#include "Line.h"
#include <gl/glut.h>

Line::Line(float x, float y) :Element(2)
{
	positions.get(0) = x;
	positions.get(1) = y;
}

Line::Line(float x, float y, float z) :Element(2)
{
	positions.get(0) = x;
	positions.get(1) = y;
	positions.get(2) = z;
}

Line::Line(float x1, float y1, float x2, float y2) : Element(2)
{
	positions.get(0, 0) = x1;
	positions.get(1, 0) = y1;
	positions.get(0, 1) = x2;
	positions.get(1, 1) = y2;
}

Line::Line(float x1, float y1, float z1, float x2, float y2, float z2) : Element(2)
{
	positions.get(0, 0) = x1;
	positions.get(1, 0) = y1;
	positions.get(2, 0) = z1;
	positions.get(0, 1) = x2;
	positions.get(1, 1) = y2;
	positions.get(2, 1) = z2;
}

Line::Line(POINT t1, POINT t2) : Element(2)
{
	positions.get(0, 0) = t1.x;
	positions.get(1, 0) = t1.y;
	positions.get(2, 0) = t1.z;
	positions.get(0, 1) = t2.x;
	positions.get(1, 1) = t2.y;
	positions.get(2, 1) = t2.z;
}

void Line::draw()const
{
	glPushMatrix();
	tranform();
	glBegin(GL_LINES);

	glColor4fv(colors.get());
	glVertex2fv(positions.get());

	glColor4fv(colors.get() + 4);
	glVertex2fv(positions.get() + 3);

	glEnd();
	glPopMatrix();
}

