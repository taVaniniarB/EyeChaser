#pragma once
#include "CObject.h"

class CTexture;

class CIris :
	public CObject
{
private:
	Vec2		m_CenterPos;	// 눈 움직이는 타원의 중심 위치
	Vec2		m_vPrevTarget;


public:
	virtual void update();
	virtual void render(HDC _dc, Gdiplus::Graphics* graphics);
	void SetCenterPos(Vec2 _vPos) {
		m_CenterPos = _vPos;
	}
	virtual void SetScale();
	void DrawEllipseBoundary(HDC _dc);

public:
	CIris();
	~CIris();
};

