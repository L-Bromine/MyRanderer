#pragma once
#include "Element.h"
class SkyBox :
    public Element
{
private:
    Texture bkimg;
    float size;
public:
    SkyBox(float size = 0.1);
    virtual void draw() const;
};

