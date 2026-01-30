#pragma once
#include "global.h"
#include "CTexture.h"

class CObject
{
private:
	wstring		m_strName;
	Vec2		m_vPos;
	Vec2		m_vScale;

public:
	Image* m_pTex;
	int m_iImgIdx;

public:
	Vec2 GetPos() { return m_vPos; }
	Vec2 GetScale() { return m_vScale; }

	void SetPos(Vec2 _vPos) { m_vPos = _vPos; }
	virtual void SetScale(Vec2 _vScale) { m_vScale = _vScale; }

	void SetName(const wstring& _strName) { m_strName = _strName; };
	const wstring& GetName() { return m_strName; }

public:
	void SetImgIdx(int _idx)
	{
		m_iImgIdx = _idx;
	}

	// 순수 가상함수화
	// 모든 Object 파생 클래스들은 자기만의 업데이트를 구현하도록 강제한다
	virtual void update() = 0;
	virtual void render(HDC _dc, Gdiplus::Graphics* graphics);

public:

	void SetTexture(Image* _pTex)
	{
		m_pTex = _pTex;
	}

	Image* GetTexture()
	{
		return m_pTex;
	}

public:
	CObject();
	~CObject();
};

