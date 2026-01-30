#pragma once
#include "CObject.h"

class CTexture;
class CIris;
class CSclera;

class CChar :
	public CObject
{
public:

private:
	CIris* m_pRIris;
	CIris* m_pLIris;
	CSclera* m_pSclera;
	bool m_changeFace = false;
	bool m_changeSclera = false;
	int			m_iImgIdx;

public:
	void AddImgIdx()
	{
		++m_iImgIdx;
	}

public:
	void init();
	virtual void update();
	virtual void render(HDC _dc, Gdiplus::Graphics* graphics);
	void SetIdx(int _idx) { m_iImgIdx = _idx; }


public:
	CChar();
	~CChar();

};

