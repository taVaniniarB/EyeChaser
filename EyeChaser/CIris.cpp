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

	float targetX = m_CenterPos.x + dx;
	float targetY = m_CenterPos.y + (EYE_ADJUST * m_ratio) + dy;

	// 선형 보간으로 부드럽게 (0.3은 속도 조절값, 0~1 사이)
	Vec2 currentPos = GetPos();
	float smoothX = currentPos.x + (targetX - currentPos.x) * 0.5f;
	float smoothY = currentPos.y + (targetY - currentPos.y) * 0.5f;

	if (abs(targetX - smoothX) < 0.1f) smoothX = targetX;
	if (abs(targetY - smoothY) < 0.1f) smoothY = targetY;

	SetPos(Vec2(smoothX, smoothY));
}

void CIris::SetScale()
{
	float iWidth = static_cast<float>(m_pTex->GetWidth());
	float iHeight = static_cast<float>(m_pTex->GetHeight());
	float irisScaleX = iWidth / static_cast<float>(IRIS_TEX_NUM);
	CObject::SetScale(Vec2(irisScaleX, iHeight) * m_ratio);
}

void CIris::SetScale(bool changeScale)
{
	float iWidth = static_cast<float>(m_pTex->GetWidth());
	float iHeight = static_cast<float>(m_pTex->GetHeight());
	float irisScaleX = iWidth / static_cast<float>(IRIS_TEX_NUM);
	CObject::SetScale(Vec2(irisScaleX, iHeight) * EYE_ADJUST * changeScale);
}

void CIris::render(HDC _dc, Gdiplus::Graphics* graphics)
{
	UINT iWidth = m_pTex->GetWidth();
	UINT iHeight = m_pTex->GetHeight();

	UINT irisScaleX = iWidth / IRIS_TEX_NUM;

	// 타일 파일 높이에 따른 최대 열 개수
	UINT iMaxCol = IRIS_TEX_NUM;

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

	// 대상 위치를 지정
	int renderX = static_cast<int>(vPos.x);
	int renderY = static_cast<int>(vPos.y);
	Rect destRect(renderX - vScale.x / 2, renderY - vScale.y / 2,
		(int)(vScale.x), (int)(vScale.y)); 
	/*Rect destRect((int)(vPos.x - vScale.x / 2)
		, (int)(vPos.y - vScale.y / 2)
		, (int)(vScale.x)
		, (int)(vScale.y));*/

	// 이미지를 잘라서 그리기
	graphics->DrawImage(m_pTex, destRect, srcRect.X, srcRect.Y, srcRect.Width, srcRect.Height, UnitPixel);
}