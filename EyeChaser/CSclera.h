#pragma once
#include "CObject.h"

class CTexture;

class CSclera :
    public CObject
{
private:

public:
    virtual void update();
    virtual void render(HDC _dc);

public:
    CSclera();
    ~CSclera();
};

