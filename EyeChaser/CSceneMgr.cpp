#include "global.h"
#include "CSceneMgr.h"

#include "Cscene_Start.h"

CSceneMgr::CSceneMgr()
	: m_pCurScene(nullptr)
{}

CSceneMgr::~CSceneMgr()
{
	delete m_pCurScene;
}

void CSceneMgr::init()
{
	// 모든 Scene들 생성해놓고 전환해줄 것
	m_pCurScene = new CScene_Start;

	m_pCurScene->Enter();
}

void CSceneMgr::update()
{
	m_pCurScene->update();
	m_pCurScene->finalUpdate();
}

void CSceneMgr::render(HDC _dc)
{
	m_pCurScene->render(_dc);
}
