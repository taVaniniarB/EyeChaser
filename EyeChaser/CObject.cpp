#include "CObject.h"

void CObject::render(HDC _dc, Gdiplus::Graphics* graphics)
{
	Rectangle(_dc
		, (int)(m_vPos.x - m_vScale.x / 2.f)
		, (int)(m_vPos.y - m_vScale.y / 2.f)
		, (int)(m_vPos.x + m_vScale.x / 2.f)
		, (int)(m_vPos.y + m_vScale.y / 2.f));
}


CObject::CObject()
	: m_pTex(nullptr)
	, m_iImgIdx(0)
{
}

CObject::~CObject()
{
}
