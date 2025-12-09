#include "global.h"
#include "CScene.h"
#include "CObject.h"
#include "CResMgr.h"
#include "CPathMgr.h"
#include "CChar.h"
#include "CBackground.h"
#include "CKeyMgr.h"
#include "CCore.h"

CScene::CScene()
	: m_pCharacter(nullptr)
	, m_pBackground(nullptr)
{
}

CScene::~CScene()
{
	delete m_pCharacter;
	delete m_pBackground;
}

void CScene::update()
{
	m_pCharacter->update();
	if (KEY_TAP(KEY::RIGHT))
	{
		m_pBackground->AddImgIdx();
	}
	else if (KEY_TAP(KEY::LEFT))
	{
		m_pBackground->SubImgIdx();
	}
}

void CScene::render(HDC _dc)
{
	m_pBackground->render(_dc);
	m_pCharacter->render(_dc);
}

void CScene::Enter()
{
	// 캐릭터 객체 등록
	CChar* pChar = new CChar;
	m_pCharacter = pChar;
	m_pCharacter->init();

	// 배경 객체 등록
	CBackground* pBack = new CBackground;
	m_pBackground = pBack;
}
