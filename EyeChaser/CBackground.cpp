#include "CBackground.h"
#include "CTexture.h"
#include "CCore.h"
#include "BackgroundImg.h"
#include "CResMgr.h"


CBackground::CBackground()
{
	SetTexture(CResMgr::GetInst()->LoadImageFromMemory(backgroundimageData, backgroundimageDataSize));
	SetScale(Vec2(WINDOW_SIZE, WINDOW_SIZE));
	SetPos(Vec2(WINDOW_SIZE / 2, WINDOW_SIZE / 2));
	BGCnt = backgroundimageWidth / 250;
}

CBackground::~CBackground()
{
}

void CBackground::AddImgIdx()
{
	++m_iImgIdx;
	if (BGCnt <= m_iImgIdx)
	{
		m_iImgIdx = 0;
	}
}

void CBackground::SubImgIdx()
{
	--m_iImgIdx;
	if (0 > m_iImgIdx)
	{
		m_iImgIdx = BGCnt - 1;
	}
}

void CBackground::update()
{

}

void CBackground::render(HDC _dc, Gdiplus::Graphics* graphics)
{
	UINT iWidth = m_pTex->GetWidth();
	UINT iHeight = m_pTex->GetHeight();

	// 타일 파일 높이에 따른 최대 열 개수
	UINT iMaxCol = iWidth / 250;

	//행 개수(쓰는 이유: 인덱스 초과 예외처리)
	UINT iMaxRow = iHeight / 250;

	UINT iCurRow = (UINT)m_iImgIdx / iMaxCol;
	UINT iCurCol = (UINT)m_iImgIdx % iMaxCol;

	// 이미지 범위를 넘어선 인덱스
	if (iMaxCol <= iCurCol)
		assert(nullptr);

	Vec2 vPos = GetPos();
	Vec2 vScale = GetScale();

	// 소스 이미지에서 잘라낼 영역을 지정
	Rect srcRect(iCurCol * 250, iCurRow * 250, 250, 250);

	// 대상 위치를 지정
	Rect destRect((int)(vPos.x - vScale.x / 2)
		, (int)(vPos.y - vScale.y / 2)
		, (int)(vScale.x)
		, (int)(vScale.y));

	// 이미지를 잘라서 그리기
	graphics->DrawImage(m_pTex, destRect, srcRect.X, srcRect.Y, srcRect.Width, srcRect.Height, UnitPixel);

}