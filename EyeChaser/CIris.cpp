#include "CIris.h"
#include "CKeyMgr.h"
#include "CCore.h"

CIris::CIris()
{
}

CIris::~CIris()
{
}

//눈동자 위치 지정
void CIris::update()
{
	static const float HALF_LIFE = 0.02f;
	static const float FIXED_DT = 0.033f;
	static const float LAMBDA = log(2.0f) / HALF_LIFE;
	static const float ALPHA = 1.0f - exp(-LAMBDA * FIXED_DT);

	Vec2 mousePos = MOUSE_POS;
	float dx = mousePos.x - m_CenterPos.x;
	float dy = mousePos.y - (m_CenterPos.y + EYE_UNFOCUS_Y_OFFSET); // offset 주의
	float angle = atan2(dy, dx);

	// 눈동자 중심이 움직일 수 있는 최대 반지름
	float a = (LEYE_WIDTH / 2.0f) - IRIS_RADIUS;
	float b = (LEYE_HEIGHT / 2.0f) - IRIS_RADIUS;

	// 1. 현재 마우스 위치가 타원 내부인지 체크 (타원 방정식 이용)
	// (x^2 / a^2) + (y^2 / b^2) <= 1 이면 내부
	float normalizedDist = (dx * dx) / (a * a) + (dy * dy) / (b * b);

	float targetX, targetY;

	if (normalizedDist <= 1.0f) {
		// 타원 내부라면 마우스 위치 그대로 타겟 설정
		targetX = mousePos.x;
		targetY = mousePos.y;
	}
	else {
		// 타원 외부라면 타원 경계상의 좌표를 계산
		targetX = m_CenterPos.x + a * cos(angle);
		targetY = m_CenterPos.y + EYE_UNFOCUS_Y_OFFSET + b * sin(angle);
	}

	Vec2 currentPos = GetPos();

	// 미세한 움직임 무시
	// 이전 프레임의 타겟을 멤버 변수(m_vPrevTarget)로 저장해두고 비교
	if (abs(targetX - m_vPrevTarget.x) < 0.5f && abs(targetY - m_vPrevTarget.y) < 0.5f) {
		targetX = m_vPrevTarget.x;
		targetY = m_vPrevTarget.y;
	}
	m_vPrevTarget = Vec2(targetX, targetY);


	float smoothX = currentPos.x + (targetX - currentPos.x) * ALPHA;
	float smoothY = currentPos.y + (targetY - currentPos.y) * ALPHA;


	// 스냅
	if (abs(targetX - smoothX) < 0.1f) smoothX = targetX;
	if (abs(targetY - smoothY) < 0.1f) smoothY = targetY;

	SetPos(Vec2(smoothX, smoothY));
}


void CIris::SetScale()
{
	float ratio = CCore::GetInst()->GetRatio();
	UINT iWidth = m_pTex->GetWidth();
	UINT iHeight = m_pTex->GetHeight();
	UINT irisScaleX = iWidth / IRIS_TEX_NUM;
	CObject::SetScale(Vec2((float)irisScaleX, (float)iHeight) * ratio);
}

void CIris::render(HDC _dc, Gdiplus::Graphics* graphics)
{
	Gdiplus::REAL iWidth = static_cast<Gdiplus::REAL>(m_pTex->GetWidth());
	Gdiplus::REAL iHeight = static_cast<Gdiplus::REAL>(m_pTex->GetHeight());
	Gdiplus::REAL irisScaleX = iWidth / IRIS_TEX_NUM;

	UINT iMaxCol = IRIS_TEX_NUM;
	UINT iCurRow = (UINT)(m_iImgIdx / iMaxCol);
	UINT iCurCol = (UINT)(m_iImgIdx % iMaxCol);

	Vec2 vPos = GetPos();
	Vec2 vScale = GetScale();

	// 소스 영역 계산
	RectF srcRect(
		static_cast<Gdiplus::REAL>(iCurCol * irisScaleX),
		static_cast<Gdiplus::REAL>(iCurRow * iHeight),
		irisScaleX,
		iHeight
	);

	// 목적지 영역 계산
	RectF destRect(
		(int)vPos.x - (int)(vScale.x / 2.0f),
		vPos.y - (vScale.y / 2.0f),
		(int)vScale.x,
		vScale.y
	);

	graphics->DrawImage(m_pTex, destRect, srcRect.X, srcRect.Y, srcRect.Width, srcRect.Height, UnitPixel);
}

void CIris::DrawEllipseBoundary(HDC _dc)
{
	float ratio = CCore::GetInst()->GetRatio();
	// 타원 파라미터 계산 (update()와 동일)
	float a = LEYE_WIDTH / 2 - IRIS_RADIUS;  // 가로 반지름
	float b = LEYE_HEIGHT / 2 - IRIS_RADIUS; // 세로 반지름

	// 타원 중심점
	float centerX = m_CenterPos.x;
	float centerY = m_CenterPos.y + EYE_UNFOCUS_Y_OFFSET;

	// 펜 설정
	HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0, 255, 255));
	HPEN hOldPen = (HPEN)SelectObject(_dc, hPen);

	// 브러시 제거 (속이 빈 타원)
	HBRUSH hOldBrush = (HBRUSH)SelectObject(_dc, GetStockObject(NULL_BRUSH));

	// 타원 그리기
	Ellipse(_dc,
		(int)(centerX - a),
		(int)(centerY - b),
		(int)(centerX + a),
		(int)(centerY + b));

	// 중심선
	MoveToEx(_dc, (int)(HALF_CANVAS * ratio), 0, NULL);
	LineTo(_dc, (int)(HALF_CANVAS * ratio), WINDOW_SIZE);

	// 현재 Iris 위치 (작은 원)
	Vec2 currentPos = GetPos();
	Ellipse(_dc,
		(int)(currentPos.x - 3),
		(int)(currentPos.y - 3),
		(int)(currentPos.x + 3),
		(int)(currentPos.y + 3));

	// 복원
	SelectObject(_dc, hOldPen);
	SelectObject(_dc, hOldBrush);
	DeleteObject(hPen);
}