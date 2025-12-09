#pragma once
#include "CObject.h"

class CTexture;

class CIris :
	public CObject
{
private:
	Vec2		m_CenterPos;
	float		m_ratio;



public:
	virtual void update();
	virtual void render(HDC _dc);
	void SetCenterPos(Vec2 _vPos) { m_CenterPos = _vPos; }
	virtual void SetScale();
	virtual void SetScale(bool changeScale);

public:
	CIris();
	~CIris();
};

