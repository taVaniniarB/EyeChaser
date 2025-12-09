#include "global.h"
#include "CTexture.h"
#include "CCore.h"

CTexture::CTexture()
	: m_hBit(0)
	, m_dc(0)
	, m_bitInfo{}
{
}

CTexture::~CTexture()
{
	DeleteDC(m_dc);
	DeleteObject(m_hBit);
}

void CTexture::Load(const wstring& _strFilePath)
{
	HDC TextureDC = CCore::GetInst()->GetMainDC();

	Gdiplus::Image* pImage = Gdiplus::Image::FromFile(_strFilePath.c_str());
	m_pImage = pImage;

	Gdiplus::Bitmap* pBitMap = reinterpret_cast<Gdiplus::Bitmap*>(pImage->Clone());

	Gdiplus::Status stat = pBitMap->GetHBITMAP(Gdiplus::Color(0, 0, 0, 0), &m_hBit);

	if (Gdiplus::Status::Ok != stat)
	{
		// 리소스 로드 실패
		assert(nullptr);
	}

	m_dc = CreateCompatibleDC(TextureDC);
	HBITMAP OldBitMap = reinterpret_cast<HBITMAP>(SelectObject(m_dc, m_hBit));
	DeleteObject(OldBitMap);

	// 특정 오브젝트에 대한 정보를 주는 함수
	// 비트맵 정보 멤버에 정보를 저장
	GetObject(m_hBit, sizeof(BITMAP), &m_bitInfo);

}