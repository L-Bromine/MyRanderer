#pragma once
#include <stack>
#include <vector>

#include "Matrix.h"
#include "Texture.h"
#include "Light.h"

#define __TAKEGLOBEVALUE true

class Element;
class Camera;
class Scence
{
private:
	int len;
	float maxView;// 最大视距
	int high, width;
	unsigned char* data;// 帧缓冲
	float* depth;// 深度缓冲
	unsigned char* stencl; // 模板缓冲区
	float nearFace, viewSize; // 光追摄像机参数

	std::stack<Matrix> matrixStack;
	std::vector<Element*> elementSet;

	void setPoint(int x, int y, float z, float r, float g, float b, float a);

	static std::vector<Scence*> scence_list;
public:
	std::vector<Light> lights;

	Texture* texture;	//纹理缓冲区
	Camera* mainCamera = nullptr;

	float Ia[3];

	static int windowHigh;
	static int windowWidth;

	static Scence* __SCENCE;

	Scence();
	~Scence();

	void resize(int w, int h);
	void clear();
	void setRGB(float x, float y, float z, float r, float g, float b);
	void setRGBAd(int x, int y, float z, float r, float g, float b, float a);
	void setRGB(float* ps, float* colors);
	void setRGBA(float x, float y, float z, float r, float g, float b, float a);
	void setRGBA(float* ps, float* colors);
	void setRGBA(float* ps, float* colors, float a);

	void setCamera(float n, float s);
	void pushStack(const Matrix& m);
	void popStack();
	const Matrix& topStack();

	static void toScence(int i);
	static void addScence();
	void registerElement(Element* e);
	void unregisterElement(Element* e);
	void rayTrace();
	void drawElement();
	void show();
	void addLight(const Light& light);
	void setScenceLight(float r, float b, float g);
};

#define scence (*Scence::__SCENCE)

