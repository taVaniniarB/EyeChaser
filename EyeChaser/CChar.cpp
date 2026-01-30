#include "CChar.h"
#include "CCore.h"
#include "CKeyMgr.h"
#include "CResMgr.h"

#include "CSclera.h"
#include "CIris.h"

#include "Images.h"


CChar::CChar()
	: m_pRIris(nullptr)
	, m_pLIris(nullptr)
	, m_pSclera(nullptr)
	, m_iImgIdx(0)
{
	SetTexture(CResMgr::GetInst()->LoadImageFromMemory(charimageData, charimageDataSize));
	SetPos(Vec2(WINDOW_SIZE / 2, WINDOW_SIZE / 2));
	SetScale(Vec2(WINDOW_SIZE, WINDOW_SIZE));
	m_changeFace = (charimageWidth / 300) > 1 ? true : false;
	m_changeSclera = (scleraimageWidth / 300) > 1 ? true : false;
}

CChar::~CChar()
{
	delete m_pSclera;
	delete m_pRIris;
	delete m_pLIris;
}
void CChar::init()
{
	float ratio = CCore::GetInst()->GetRatio();


	// SCLERA
	m_pSclera = new CSclera();
	Image* pImage = CResMgr::GetInst()->LoadImageFromMemory(scleraimageData, scleraimageDataSize);
	m_pSclera->SetTexture(pImage);
	m_pSclera->SetScale(Vec2(WINDOW_SIZE, WINDOW_SIZE));
	m_pSclera->SetPos(Vec2(WINDOW_SIZE / 2, WINDOW_SIZE / 2));


	// RIGHT IRIS
	m_pRIris = new CIris();
	pImage = CResMgr::GetInst()->LoadImageFromMemory(RimageData, RimageDataSize);
	m_pRIris->SetTexture(pImage);
	m_pRIris->SetScale();
	//m_pRIris->SetScale(Vec2(IRIS_SCALE_X, IRIS_SCALE_Y));
	m_pRIris->SetPos(Vec2(REYE_CENTERPOS_X * ratio, REYE_CENTERPOS_Y * ratio));
	m_pRIris->SetCenterPos(Vec2(REYE_CENTERPOS_X * ratio, REYE_CENTERPOS_Y * ratio));



	// LEFT IRIS
	m_pLIris = new CIris();
	pImage = CResMgr::GetInst()->LoadImageFromMemory(LimageData, LimageDataSize);
	m_pLIris->SetTexture(pImage);
	m_pLIris->SetScale();
	//m_pLIris->SetScale(Vec2(IRIS_SCALE_X, IRIS_SCALE_Y));
	m_pLIris->SetPos(Vec2(LEYE_CENTERPOS_X * ratio, LEYE_CENTERPOS_Y * ratio));
	m_pLIris->SetCenterPos(Vec2(LEYE_CENTERPOS_X * ratio, LEYE_CENTERPOS_Y * ratio));


	m_pRIris->SetImgIdx(0);
	m_pLIris->SetImgIdx(0);
}

void CChar::update()
{
	float ratio = CCore::GetInst()->GetRatio();

	// 포커스 O
	if (CCore::GetInst()->GetIsFocused())
	{
		if (m_changeFace)
		{
			m_iImgIdx = 0;
		}

		m_pRIris->SetPos(Vec2(REYE_CENTERPOS_X * ratio, REYE_CENTERPOS_Y * ratio));
		m_pLIris->SetPos(Vec2(LEYE_CENTERPOS_X * ratio, LEYE_CENTERPOS_Y * ratio));

		if (CHANGE_IRIS)
		{
			if (((IRIS_CHANGE_MODE)&SCALE) == SCALE)
			{
				m_pRIris->SetScale();
				m_pLIris->SetScale();
				//m_pRIris->SetScale(Vec2(IRIS_SCALE_X, IRIS_SCALE_Y));
				//m_pLIris->SetScale(Vec2(IRIS_SCALE_X, IRIS_SCALE_Y));
			}
			if (((IRIS_CHANGE_MODE)&TEXTURE) == TEXTURE)
			{
				m_pRIris->SetImgIdx(0);
				m_pLIris->SetImgIdx(0);
			}
		}

		if (m_changeSclera)
		{
			m_pSclera->SetImgIdx(0);
		}
	}
	else
	{
		if (CCore::GetInst()->GetMTMode())
		{
			if (m_changeFace)
			{
				m_iImgIdx = 1;
			}

			m_pRIris->update();
			m_pLIris->update();

			if (CHANGE_IRIS)
			{
				if (((IRIS_CHANGE_MODE)&SCALE) == SCALE)
				{
					m_pRIris->SetScale(true);
					m_pLIris->SetScale(true);
				}
				if (((IRIS_CHANGE_MODE)&TEXTURE) == TEXTURE)
				{
					m_pRIris->SetImgIdx(1);
					m_pLIris->SetImgIdx(1);
				}
			}

			if (m_changeSclera)
			{
				m_pSclera->SetImgIdx(1);
			}
		}
	}


}

void CChar::render(HDC _dc, Gdiplus::Graphics* graphics)
{
	m_pSclera->render(_dc, graphics);
	m_pRIris->render(_dc, graphics);
	m_pLIris->render(_dc, graphics);


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

	// 소스 이미지에서 잘라낼 영역을 지정
	Rect srcRect(iCurCol * CANVAS_SIZE, iCurRow * CANVAS_SIZE, CANVAS_SIZE, CANVAS_SIZE);

	// 대상 위치를 지정
	Rect destRect((int)(vPos.x - vScale.x / 2)
		, (int)(vPos.y - vScale.y / 2)
		, (int)(vScale.x)
		, (int)(vScale.y));

	// 이미지를 잘라서 그리기
	graphics->DrawImage(m_pTex, destRect, srcRect.X, srcRect.Y, srcRect.Width, srcRect.Height, UnitPixel);
}