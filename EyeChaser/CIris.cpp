#include "CIris.h"
#include "CKeyMgr.h"

#include "CCore.h"

CIris::CIris()
	: m_ratio(0.f)
{
	m_ratio = CCore::GetInst()->GetRatio();
}

CIris::~CIris()
{
}

//눈동자 위치 지정
void CIris::update()
{
	Vec2 mousePos = MOUSE_POS;

	float dx = mousePos.x - m_CenterPos.x;
	float dy = mousePos.y - m_CenterPos.y + (EYE_ADJUST * m_ratio);
	float dist = sqrt(dx * dx + dy * dy);

	float angle = atan2(dy, dx);
	float maxDistX = ((LEYE_WIDTH * m_ratio) / 2 - (PUPIL_RADIUS * m_ratio)) * cos(angle);
	float maxDistY = ((LEYE_HEIGHT * m_ratio) / 2 - (PUPIL_RADIUS * m_ratio)) * sin(angle);

	if (dist > sqrt(maxDistX * maxDistX + maxDistY * maxDistY)) {
		dx = maxDistX;
		dy = maxDistY;
	}

	float pupilX = m_CenterPos.x + static_cast<int>(dx);
	float pupilY = m_CenterPos.y + (EYE_ADJUST * m_ratio) + static_cast<int>(dy);

	SetPos(Vec2(pupilX, pupilY));
}

void CIris::SetScale()
{
	UINT iWidth = m_pTex->GetWidth();
	UINT iHeight = m_pTex->GetHeight();
	UINT irisScaleX = iWidth / IRIS_TEX_NUM;
	CObject::SetScale(Vec2(irisScaleX, iHeight) * m_ratio);
}

void CIris::SetScale(bool changeScale)
{
	UINT iWidth = m_pTex->GetWidth();
	UINT iHeight = m_pTex->GetHeight();
	UINT irisScaleX = iWidth / IRIS_TEX_NUM;
	CObject::SetScale(Vec2(irisScaleX, iHeight) * EYE_ADJUST * m_ratio);
}

void CIris::render(HDC _dc)
{
	Gdiplus::Graphics graphics(_dc);

	UINT iWidth = m_pTex->GetWidth();
	UINT iHeight = m_pTex->GetHeight();

	UINT irisScaleX = iWidth / IRIS_TEX_NUM;

	// 타일 파일 높이에 따른 최대 열 개수
	UINT iMaxCol = IRIS_TEX_NUM;
	// UINT iMaxCol = (UINT)(iWidth / IRIS_SCALE_X);

	//행 개수(쓰는 이유: 인덱스 초과 예외처리)
	// UINT iMaxRow = (UINT)(iHeight / IRIS_SCALE_Y);

	UINT iCurRow = (UINT)(m_iImgIdx / iMaxCol);
	UINT iCurCol = (UINT)(m_iImgIdx % iMaxCol);

	// 이미지 범위를 넘어선 인덱스
	if (iMaxCol <= iCurCol)
		assert(nullptr);

	Vec2 vPos = GetPos();
	Vec2 vScale = GetScale();

	// 소스 이미지에서 잘라낼 영역을 지정 (가로세로 가로세로)
	Rect srcRect((int)(iCurCol * irisScaleX)
		, (int)(iCurRow * iHeight)
		, (int)(irisScaleX)
		, (int)(iHeight));

	// 대상 위치를 지정
	Rect destRect((int)(vPos.x - vScale.x / 2)
		, (int)(vPos.y - vScale.y / 2)
		, (int)(vScale.x)
		, (int)(vScale.y));

	// 이미지를 잘라서 그리기
	graphics.DrawImage(m_pTex, destRect, srcRect.X, srcRect.Y, srcRect.Width, srcRect.Height, UnitPixel);
}
/*
void CIris::SetScale(Vec2 _vScale)
{
	CObject::SetScale(_vScale * m_ratio);
}*/