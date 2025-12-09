#include "CSclera.h"
#include "CResMgr.h"

CSclera::CSclera()
{
	
}

CSclera::~CSclera()
{
}

void CSclera::update()
{
}

void CSclera::render(HDC _dc)
{
	Gdiplus::Graphics graphics(_dc);

	UINT iWidth = m_pTex->GetWidth();
	UINT iHeight = m_pTex->GetHeight();

	// 타일 파일 높이에 따른 최대 열 개수
	UINT iMaxCol = iWidth / CANVAS_SIZE;


	//행 개수(쓰는 이유: 인덱스 초과 예외처리)
	UINT iMaxRow = iHeight / CANVAS_SIZE;

	UINT iCurRow = (UINT)m_iImgIdx / iMaxCol;
	UINT iCurCol = (UINT)m_iImgIdx % iMaxCol;

	// 이미지 범위를 넘어선 인덱스
	if (iMaxCol <= iCurCol)
		assert(nullptr);

	Vec2 vPos = GetPos();
	Vec2 vScale = GetScale();

	// 소스 이미지에서 잘라낼 영역을 지정 (가로세로 가로세로)
	Rect srcRect(iCurCol * CANVAS_SIZE, iCurRow * CANVAS_SIZE, CANVAS_SIZE, CANVAS_SIZE);

	// 대상 위치를 지정
	Rect destRect((int)(vPos.x - vScale.x / 2)
		, (int)(vPos.y - vScale.y / 2)
		, (int)(vScale.x)
		, (int)(vScale.y));

	// 이미지를 잘라서 그리기
	graphics.DrawImage(m_pTex, destRect, srcRect.X, srcRect.Y, srcRect.Width, srcRect.Height, UnitPixel);
}