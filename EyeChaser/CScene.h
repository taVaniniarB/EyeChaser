#pragma once
#include "global.h"

class CTexture;
class CChar;
class CBackground;

class CScene
{
	SINGLE(CScene);

public:
	CBackground* m_pBackground;
	CChar* m_pCharacter;

public:
	virtual void update();
	virtual void render(HDC _dc);

	virtual void Enter();

};