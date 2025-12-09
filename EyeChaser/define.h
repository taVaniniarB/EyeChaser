#pragma once
// 종료창 ……
#define EXIT_WINDOW true
#define CLOSE_DIALOG_TITLE L"SYSTEM"
#define CLOSE_DIALOG L"정말 종료하시겠습니까?"

// 창 텍스트
#define FOCUSED_TEXT L"Lacus Clyne"
#define UNFOCUSED_TEXT L"Mia"

// 눈 이동영역 너비, 높이
// 40 / 35
#define LEYE_WIDTH 38.f
#define LEYE_HEIGHT 32.f

// 값을 올릴 수록 미포커스 시 눈동자 아래로 내려감
#define EYE_ADJUST -1.5f

#define LEYE_CENTERPOS_Y 155.f // 눈 높이
#define EYEOFFSET 37.f // 미간
#define PUPIL_RADIUS 10 // 눈 반지름

// 눈동자 텍스처 변화
#define CHANGE_IRIS false

// 캔버스 사이즈
#define CANVAS_SIZE 300
#define WINDOW_SIZE 200

// 왼쪽 눈 중앙좌표
#define LEYE_CENTERPOS_X (150.f - EYEOFFSET)

// 오른쪽 눈 중앙좌표
#define REYE_CENTERPOS_X (150.f + EYEOFFSET + 1)
#define REYE_CENTERPOS_Y LEYE_CENTERPOS_Y

#define SCALE 1
#define TEXTURE 2
//#define IRIS_ADJUST_SCALE 0.7f
#define IRIS_CHANGE_MODE TEXTURE
#define IRIS_TEX_NUM ((CHANGE_IRIS && (IRIS_CHANGE_MODE == TEXTURE)) ? 2 : 1)

// Singleton 매크로
#define SINGLE(type) public:\
						 static type* GetInst() \
						 {\
							 static type mgr;\
							 return &mgr;\
						 }\
					 private:\
						type();\
						~type();


#define KEY_CHECK(key, state) CKeyMgr::GetInst()->GetKeyState(key) == state
#define KEY_HOLD(key) KEY_CHECK(key, KEY_STATE::HOLD)
#define KEY_TAP(key) KEY_CHECK(key, KEY_STATE::TAP)
#define KEY_AWAY(key) KEY_CHECK(key, KEY_STATE::AWAY)
#define KEY_NONE(key) KEY_CHECK(key, KEY_STATE::NONE)
#define MOUSE_POS CKeyMgr::GetInst()->GetMouseCursor()

#ifndef SW_NOZIZE
#define SW_NOZIZE 0x0008
#endif
