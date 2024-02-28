#include "Element.h"
class Triangle :
	public Element
{
private:
public:
	Triangle(float x1, float y1, float x2, float y2, float x3, float y3);
	Triangle(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3);
	Triangle(POINT p1, POINT p2, POINT p3);
	Triangle(float* xs, float* ys);
	Triangle(float* xs, float* ys, float* zs);
	Triangle(POINT* ps);

	virtual bool triangleIntersect(const Ray& ray, float* out) const;
	virtual void draw() const;
};
