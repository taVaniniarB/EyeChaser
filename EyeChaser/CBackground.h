#pragma once
#include "CObject.h"

class CTexture;

class CBackground :
	public CObject
{
private:
	int BGCnt = 5;

public:
	void AddImgIdx();
	void SubImgIdx();
	int GetIdx() { return m_iImgIdx; }

public:
	virtual void update();
	virtual void render(HDC _dc, Gdiplus::Graphics* graphics);

public:
	CBackground();
	~CBackground();
};
